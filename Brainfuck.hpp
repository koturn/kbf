/*!
 * @file Brainfuck.hpp
 * @brief Brainfuck processor
 * @author koturn
 */
#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <cassert>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "CodeGenerator/CodeGenerator.hpp"

#if !defined(XBYAK_NO_OP_NAMES) && defined(__GNUC__)
#  define XBYAK_NO_OP_NAMES
#endif

#define BRAINFUCK_GNUC_PREREQ(major, minor)  \
  (defined(__GNUC__) && (__GNUC__ > (major) || __GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))

#if BRAINFUCK_GNUC_PREREQ(4, 6)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Weffc++"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  if __cplusplus >= 201103L
#    pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#    pragma GCC diagnostic ignored "-Wsuggest-final-types"
#    pragma GCC diagnostic ignored "-Wsuggest-override"
#    pragma GCC diagnostic ignored "-Wuseless-cast"
#    pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#  endif
#endif  // BRAINFUCK_GNUC_PREREQ(4, 6)
#include <xbyak/xbyak.h>
#if BRAINFUCK_GNUC_PREREQ(4, 6)
#  pragma GCC diagnostic pop
#endif  // BRAINFUCK_GNUC_PREREQ(4, 6)


#include "BfInst.h"

#if defined(__cplusplus) && __cplusplus >= 201103 \
  || defined(_MSC_VER) && (_MSC_VER > 1800 || (_MSC_VER == 1800 && _MSC_FULL_VER == 180021114))
//! Polyfill macro of @code noexcept @endcode
#  define BRAINFUCK_NOEXCEPT  noexcept
#else
//! Polyfill macro of @code noexcept @endcode
#  define BRAINFUCK_NOEXCEPT  throw()
#endif


static int
getcharWithFlush() BRAINFUCK_NOEXCEPT
{
  std::fflush(stdout);
  return std::getchar();
}


/*!
 * @brief Brainfuck processor
 */
class Brainfuck
{
public:
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1700)
  /*!
   * @brief Compilation type
   */
  enum class CompileType
  {
    kIR, kJit, kUnknown
  };  // enum class CompileType
#else
  /*!
   * @brief Compilation type
   */
  class CompileType
  {
  public:
    /*!
     * @brief Actual enum of compilation type
     */
    enum CompileTypeEnum
    {
      kIR, kJit, kUnknown
    };
    /*!
     * @brief Ctor for implicit conversion: Actual enum to dummy enum class
     * @param [in] value  Actual enum value
     */
    CompileType(CompileTypeEnum value) :
      value(value)
    {}
    /*!
     * @brief operator () for implicit conversion: Dummy enum class to actual
     *        enum
     * @return Actual enum value
     */
    operator CompileTypeEnum() const throw()
    {
      return value;
    }
  private:
    //! Actual enum value
    CompileTypeEnum value;
  };  // class CompileType
#endif  // __cplusplus >= 201103L

#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1700)
  /*!
   * @brief Target format
   */
  enum class Target
  {
    kC, kXbyakC, kWinX86, kWinX64, kElfX86, kElfX64, kElfArmeabi
  };  // enum class Target
#else
  class Target
  {
  public:
    /*!
     * @brief Actual enum of target format
     */
    enum TargetEnum
    {
      kC, kXbyakC, kWinX86, kWinX64, kElfX86, kElfX64, kElfArmeabi
    };
    /*!
     * @brief Ctor for implicit conversion: Actual enum to dummy enum class
     * @param [in] value  Actual enum value
     */
    Target() :
      value()
    {}
    /*!
     * @brief Ctor for implicit conversion: Actual enum to dummy enum class
     * @param [in] value  Actual enum value
     */
    Target(TargetEnum value) :
      value(value)
    {}
    /*!
     * @brief operator () for implicit conversion: Dummy enum class to actual
     *        enum
     * @return Actual enum value
     */
    operator TargetEnum() const throw()
    {
      return value;
    }
  private:
    TargetEnum value;
  };  // class Target
#endif  // __cplusplus >= 201103L

private:
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1700)
  enum class XbyakDirection
  {
    B, F
  };  // enum class XbyakDirection
#else
  class XbyakDirection
  {
  public:
    enum XbyakDirectionEnum
    {
      B, F
    };
    XbyakDirection(XbyakDirectionEnum value) :
      value(value)
    {}
    operator XbyakDirectionEnum() const throw()
    {
      return value;
    }
  private:
    XbyakDirectionEnum value;
  };  // class XbyakDirection
#endif  // __cplusplus >= 201103L


  //! Brainfuck characters: +-><.,[]
  static const std::string kBrainfuckCharacters;
  //! Default eap size
  static const std::size_t kDefaultHeapSize;
  //! Default code generator size
  static const std::size_t kDefaultXbyakCodeGeneratorSize;
  //! Brainfuck source code
  std::string bfSource;
  //! IR code
  std::vector<BfInst> ircode;
  //! Native code generator
  Xbyak::CodeGenerator cg;
  //! Internal compile state
  CompileType state;

  /*!
   * @brief Compress value or pointer movement operation
   * @param [in] pc  A program counter
   * @param [in] c1  Pair 1
   * @param [in] c2  Pair 2
   * @return Compressed size
   */
  int
  compressInstruction(std::size_t& pc, char c1, char c2) BRAINFUCK_NOEXCEPT
  {
    int value = 0;
    for (; pc < bfSource.length(); pc++) {
      if (bfSource[pc] == c1) {
        value++;
      } else if (bfSource[pc] == c2) {
        value--;
      } else {
        pc--;
        break;
      }
    }
    return value;
  }

  void
  reduceIrCode(int n, const BfInst& inst) BRAINFUCK_NOEXCEPT
  {
    for (int i = 0; i < n; i++) {
      ircode.pop_back();
    }
    ircode.emplace_back(inst);
  }

  /*!
   * @brief Convert label to string
   * @param [in] labelNo  Label Number
   * @param [in] dir      Direction (Backword or Forward)
   * @return Label in string format
   */
  inline static std::string
  toXbyakLabelString(int labelNo, XbyakDirection dir)
  {
    return Xbyak::Label::toStr(labelNo) + (dir == XbyakDirection::B ? 'B' : 'F');
  }


public:
  /*!
   * Empty ctor
   */
  Brainfuck() BRAINFUCK_NOEXCEPT :
    bfSource(""),
    ircode(),
    cg(kDefaultXbyakCodeGeneratorSize),
    state(CompileType::kUnknown)
  {}

  /*!
   * Copy-ctor
   */
  Brainfuck(const Brainfuck& that) BRAINFUCK_NOEXCEPT :
    bfSource(that.bfSource),
    ircode(that.ircode),
    cg(kDefaultXbyakCodeGeneratorSize),
    state(CompileType::kUnknown)
  {}

  /*!
   * @brief Copy operator @code operator= @endcode
   * @param [in] that  Another Brainfuck object
   * @return This Brainfuck object
   */
  Brainfuck&
  operator=(const Brainfuck& that) BRAINFUCK_NOEXCEPT
  {
    bfSource = that.bfSource;
    ircode = that.ircode;
    state = that.state;
    return *this;
  }


#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1700)
  // move-ctor
  Brainfuck(Brainfuck&&) = default;
  // move-operator=
  Brainfuck&
  operator=(Brainfuck&&) = default;
#endif  // __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1700)

  /*!
   * @brief Load brainfuck source from a file
   * @param [in] filename  Brainfuck source file name
   */
  void
  load(const std::string& filename)
  {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
      throw std::runtime_error("Failed to open: " + filename);
    }
    load(ifs);
  }

  /*!
   * @brief Load brainfuck source from a std::istream
   * @param [in,out] is  std::istream of brainfuck source code
   */
  void
  load(std::istream& is) BRAINFUCK_NOEXCEPT
  {
    bfSource = std::string((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
    state = CompileType::kUnknown;
  }

  /*!
   * @brief Load brainfuck source from a std::string
   * @param [in] bfSource_  Brainfuck source code
   */
  void
  loadSource(const std::string& bfSource_) BRAINFUCK_NOEXCEPT
  {
    bfSource = bfSource_;
    state = CompileType::kUnknown;
  }

  /*!
   * @brief Remove extra character from the source code
   */
  void
  trim() BRAINFUCK_NOEXCEPT
  {
    bfSource.erase(
        std::remove_if(
          bfSource.begin(),
          bfSource.end(),
          [&](const decltype(bfSource)::value_type& ch) {
            return kBrainfuckCharacters.find_first_of(ch) == std::string::npos;
          }),
        bfSource.end());
  }

  /*!
   * @brief Compile brainfuck source code
   * @param [in] ct  Compilation type
   */
  void
  compile(CompileType ct=CompileType::kIR) BRAINFUCK_NOEXCEPT
  {
    switch (ct) {
      case CompileType::kIR:
        compileToIR();
        break;
      case CompileType::kJit:
        compileToIR();
        compileToNative();
        break;
      default:
        assert(false);
    }
    state = ct;
  }

  /*!
   * @brief Compile brainfuck source code to IR code
   */
  void
  compileToIR() BRAINFUCK_NOEXCEPT
  {
    std::stack<int> loopStack;
    ircode.clear();
    for (std::string::size_type pc = 0; pc < bfSource.size(); pc++) {
      switch (bfSource[pc]) {
        case '>':
        case '<':
          {
            int offset = (bfSource[pc] == '>' ? 1 : -1);
            pc++;
            offset += compressInstruction(pc, '>', '<');
            if (offset != 0) {
              ircode.emplace_back(BfInst(BfInst::Type::kMovePointer, offset));
            }
          }
          break;
        case '+':
        case '-':
          {
            int offset = (bfSource[pc] == '+' ? 1 : -1);
            pc++;
            offset += compressInstruction(pc, '+', '-');
            if (offset != 0) {
              if (ircode.size() > 0 && ircode[ircode.size() - 1].type == BfInst::Type::kAssign && ircode[ircode.size() - 1].op1 == 0) {
                ircode[ircode.size() - 1].op1 = offset;
              } else {
                ircode.emplace_back(BfInst(BfInst::Type::kAdd, offset));
              }
            }
          }
          break;
        case '.':
          ircode.emplace_back(BfInst(BfInst::Type::kPutchar));
          break;
        case ',':
          ircode.emplace_back(BfInst(BfInst::Type::kGetchar));
          break;
        case '[':
          loopStack.push(static_cast<int>(ircode.size()));
          ircode.emplace_back(BfInst(BfInst::Type::kLoopStart));
          break;
        case ']':
          {
            bool isReduced = false;
            std::vector<BfInst>::size_type size = ircode.size();
            if (size > 0 && ircode[size - 1].type == BfInst::Type::kLoopStart) {
              reduceIrCode(1, BfInst(BfInst::Type::kInfLoop));
              isReduced = true;
            } else if (size > 1 && ircode[size - 2].type == BfInst::Type::kLoopStart) {
              const BfInst& prevInst1 = ircode[size - 1];
              if (prevInst1.type == BfInst::Type::kAdd && std::abs(prevInst1.op1) == 1) {
                reduceIrCode(2, BfInst(BfInst::Type::kAssign, 0));
                isReduced = true;
              } else if (prevInst1.type == BfInst::Type::kMovePointer) {
                reduceIrCode(2, BfInst(BfInst::Type::kSearchZero, prevInst1.op1));
                isReduced = true;
              }
            } else if (size > 2) {
              int base = loopStack.top();
              const BfInst& prevInst1 = ircode[size - 1];
              const BfInst& prevInst2 = ircode[size - 2];
              const BfInst& fromInst1 = ircode[base + 1];
              std::vector<BfInst>::size_type startIdx = -1, endIdx = -1;
              int rollbackMove = -1;
              if (prevInst1.type == BfInst::Type::kAdd && prevInst1.op1 == -1) {
                startIdx = base + 1;
                endIdx = size - 2;
                rollbackMove = prevInst2.op1;
              } else if (fromInst1.type == BfInst::Type::kAdd && fromInst1.op1 == -1) {
                startIdx = base + 2;
                endIdx = size - 1;
                rollbackMove = prevInst1.op1;
              }
              if (startIdx != static_cast<std::vector<BfInst>::size_type>(-1)) {
                std::queue<BfInst> reduceQueue;
                int sumMove = 0;
                for (std::vector<BfInst>::size_type i = startIdx; i < endIdx; i += 2) {
                  const BfInst& inst1 = ircode[i];
                  const BfInst& inst2 = ircode[i + 1];
                  if (inst1.type == BfInst::Type::kMovePointer && inst2.type == BfInst::Type::kAdd) {
                    sumMove += inst1.op1;
                    if (inst2.op1 == 1) {
                      reduceQueue.push(BfInst(BfInst::Type::kAddVar, sumMove));
                    } else if (inst2.op1 == -1) {
                      reduceQueue.push(BfInst(BfInst::Type::kSubVar, sumMove));
                    } else {
                      reduceQueue.push(BfInst(BfInst::Type::kAddCMulVar, sumMove, inst2.op1));
                    }
                  } else {
                    sumMove = 0x7fffffff;
                    break;
                  }
                }
                if (sumMove + rollbackMove == 0) {
                  for (std::queue<BfInst>::size_type i = 0, im = reduceQueue.size() + 2; i < im; i++) {
                    ircode.pop_back();
                  }
                  for (int i = 1; !reduceQueue.empty(); i++) {
                    ircode[base + i] = reduceQueue.front();
                    reduceQueue.pop();
                  }
                  ircode.emplace_back(BfInst(BfInst::Type::kAssign, 0));
                  ircode[base] = BfInst(BfInst::Type::kIf, static_cast<int>(ircode.size()));
                  ircode.emplace_back(BfInst(BfInst::Type::kEndIf, base));
                  isReduced = true;
                }
              }
            }
            if (!isReduced) {
              ircode[static_cast<std::size_t>(loopStack.top())].op1 = static_cast<int>(ircode.size());
              ircode.emplace_back(BfInst(BfInst::Type::kLoopEnd, loopStack.top()));
            }
            loopStack.pop();
          }
          break;
      }
    }
  }

  void
  compileToNative() BRAINFUCK_NOEXCEPT
  {
    cg.reset();
#ifdef XBYAK32
    const Xbyak::Reg32& pPutchar(cg.esi);
    const Xbyak::Reg32& pGetchar(cg.edi);
    const Xbyak::Reg32& stack(cg.ebp);
    const Xbyak::Address cur = Xbyak::util::byte[stack];
    cg.push(cg.ebp);  // stack
    cg.push(cg.esi);
    cg.push(cg.edi);
    const int P_ = 4 * 3;
    cg.mov(pPutchar, Xbyak::util::ptr[cg.esp + P_ + 4]);  // putchar
    cg.mov(pGetchar, Xbyak::util::ptr[cg.esp + P_ + 8]);  // getchar
    cg.mov(stack, Xbyak::util::ptr[cg.esp + P_ + 12]);  // stack
#elif defined(XBYAK64_WIN)
    const Xbyak::Reg64& pPutchar(cg.rsi);
    const Xbyak::Reg64& pGetchar(cg.rdi);
    const Xbyak::Reg64& stack(cg.rbp);  // stack
    const Xbyak::Address cur = Xbyak::util::byte[stack];
    cg.push(cg.rsi);
    cg.push(cg.rdi);
    cg.push(cg.rbp);
    cg.mov(pPutchar, cg.rcx);  // putchar
    cg.mov(pGetchar, cg.rdx);  // getchar
    cg.mov(stack, cg.r8);  // stack
#else
    const Xbyak::Reg64& pPutchar(cg.rbx);
    const Xbyak::Reg64& pGetchar(cg.rbp);
    const Xbyak::Reg64& stack(cg.r12);  // stack
    const Xbyak::Address cur = Xbyak::util::byte[stack];
    cg.push(cg.rbx);
    cg.push(cg.rbp);
    cg.push(cg.r12);
    cg.mov(pPutchar, cg.rdi);  // putchar
    cg.mov(pGetchar, cg.rsi);  // getchar
    cg.mov(stack, cg.rdx);  // stack
#endif  // XBYAK32
    int labelNo = 0;
    std::stack<int> keepLabelNo;
    for (const auto& inst : ircode) {
      switch (inst.type) {
        case BfInst::Type::kMovePointer:
          if (inst.op1 > 0) {
            if (inst.op1 == 1) {
              cg.inc(stack);
            } else {
              cg.add(stack, inst.op1);
            }
          } else if (inst.op1 < 0) {
            if (inst.op1 == -1) {
              cg.dec(stack);
            } else {
              cg.sub(stack, -inst.op1);
            }
          }
          break;
        case BfInst::Type::kAdd:
          if (inst.op1 > 0) {
            if (inst.op1 == 1) {
              cg.inc(cur);
            } else {
              cg.add(cur, inst.op1);
            }
          } else if (inst.op1 < 0) {
            if (inst.op1 == -1) {
              cg.dec(cur);
            } else {
              cg.sub(cur, -inst.op1);
            }
          }
          break;
        case BfInst::Type::kPutchar:
#ifdef XBYAK32
          cg.push(cur);
          cg.call(pPutchar);
          cg.pop(cg.eax);
#elif defined(XBYAK64_WIN)
          cg.mov(cg.rcx, cur);
          cg.sub(cg.rsp, 32);
          cg.call(pPutchar);
          cg.add(cg.rsp, 32);
#else
          cg.mov(cg.rdi, cur);
          cg.call(pPutchar);
#endif  // XBYAK32
          break;
        case BfInst::Type::kGetchar:
#ifdef XBYAK64_WIN
          cg.sub(cg.rsp, 32);
#endif  // XBYAK64_WIN
          cg.call(pGetchar);
#ifdef XBYAK64_WIN
          cg.add(cg.rsp, 32);
#endif  // XBYAK64_WIN
          cg.mov(cur, cg.al);
          break;
        case BfInst::Type::kLoopStart:
        case BfInst::Type::kIf:
          cg.L(toXbyakLabelString(labelNo, XbyakDirection::B));
          cg.mov(cg.al, cur);
          cg.test(cg.al, cg.al);
          cg.jz(toXbyakLabelString(labelNo, XbyakDirection::F), Xbyak::CodeGenerator::T_NEAR);
          keepLabelNo.push(labelNo++);
          break;
        case BfInst::Type::kLoopEnd:
          {
            int no = keepLabelNo.top();
            keepLabelNo.pop();
            cg.jmp(toXbyakLabelString(no, XbyakDirection::B));
            cg.L(toXbyakLabelString(no, XbyakDirection::F));
          }
          break;
        case BfInst::Type::kEndIf:
          {
            int no = keepLabelNo.top();
            keepLabelNo.pop();
            cg.L(toXbyakLabelString(no, XbyakDirection::F));
          }
          break;
        case BfInst::Type::kAssign:
          cg.mov(cur, inst.op1);
          break;
        case BfInst::Type::kSearchZero:
          // kLoopStart
          cg.L(toXbyakLabelString(labelNo, XbyakDirection::B));
          cg.mov(cg.al, cur);
          cg.test(cg.al, cg.al);
          cg.jz(toXbyakLabelString(labelNo, XbyakDirection::F), Xbyak::CodeGenerator::T_NEAR);
          // kNextN / kPrevN
          if (inst.op1 > 0) {
            if (inst.op1 == 1) {
              cg.inc(stack);
            } else {
              cg.add(stack, inst.op1);
            }
          } else if (inst.op1 < 0) {
            if (inst.op1 == -1) {
              cg.dec(stack);
            } else {
              cg.sub(stack, -inst.op1);
            }
          }
          // kLoopEnd
          cg.jmp(toXbyakLabelString(labelNo, XbyakDirection::B));
          cg.L(toXbyakLabelString(labelNo, XbyakDirection::F));
          labelNo++;
          break;
        case BfInst::Type::kAddVar:
          cg.mov(cg.al, cur);
          cg.add(Xbyak::util::byte[stack + inst.op1], cg.al);
          break;
        case BfInst::Type::kSubVar:
          cg.mov(cg.al, cur);
          cg.sub(Xbyak::util::byte[stack + inst.op1], cg.al);
          break;
        case BfInst::Type::kAddCMulVar:
          cg.mov(cg.al, inst.op2);
          cg.mul(cur);
          cg.add(Xbyak::util::byte[stack + inst.op1], cg.al);
          break;
        case BfInst::Type::kInfLoop:
          // if (cur != 0)
          cg.mov(cg.al, cur);
          cg.test(cg.al, cg.al);
          cg.jz(toXbyakLabelString(labelNo, XbyakDirection::F), Xbyak::CodeGenerator::T_NEAR);
          // infinite loop
          cg.L(toXbyakLabelString(labelNo, XbyakDirection::B));
          cg.jmp(toXbyakLabelString(labelNo, XbyakDirection::B));
          // endif
          cg.L(toXbyakLabelString(labelNo, XbyakDirection::F));
          labelNo++;
          break;
        default:
          assert(false);
      }
    }
#ifdef XBYAK32
    cg.push('\n');
    cg.call(pPutchar);
    cg.pop(cg.eax);
    cg.pop(cg.edi);
    cg.pop(cg.esi);
    cg.pop(cg.ebp);
#elif defined(XBYAK64_WIN)
    cg.mov(cg.rcx, '\n');
    cg.sub(cg.rsp, 32);
    cg.call(pPutchar);
    cg.add(cg.rsp, 32);
    cg.pop(cg.rbp);
    cg.pop(cg.rdi);
    cg.pop(cg.rsi);
#else
    cg.mov(cg.rdi, '\n');
    cg.call(pPutchar);
    cg.pop(cg.r12);
    cg.pop(cg.rbp);
    cg.pop(cg.rbx);
#endif  // XBYAK32
    cg.ret();
  }

  /*!
   * @brief Execute brainfuck
   * @param [in] heapSize  Heap size for execution
   */
  void
  execute(std::size_t heapSize=kDefaultHeapSize) const BRAINFUCK_NOEXCEPT
  {
    std::unique_ptr<unsigned char[]> heap(new unsigned char[heapSize]);
    std::fill_n(heap.get(), heapSize, 0);
    switch (state) {
      case CompileType::kIR:
        executeIR(heap.get());
        break;
      case CompileType::kJit:
        executeJit(heap.get());
        break;
      case CompileType::kUnknown:
        execute(heap.get());
        break;
    }
  }

  /*!
   * @brief Execute brainfuck using given heap
   * @param [in,out] heap  Pointer to heap memory
   */
  void
  execute(unsigned char* heap) const BRAINFUCK_NOEXCEPT
  {
    std::size_t hp = 0;
    for (std::string::size_type pc = 0; pc < bfSource.size(); pc++) {
      switch (bfSource[pc]) {
        case '+':
          heap[hp]++;
          break;
        case '-':
          heap[hp]--;
          break;
        case '>':
          hp++;
          break;
        case '<':
          hp--;
          break;
        case '.':
          std::cout.put(static_cast<char>(heap[hp]));
          break;
        case ',':
          std::cout.flush();
          heap[hp] = static_cast<unsigned char>(std::cin.get());
          break;
        case '[':
          if (heap[hp] == 0) {
            int depth = 1;
            for (pc++; depth > 0; pc++) {
              switch (bfSource[pc]) {
                case '[':
                  depth++;
                  break;
                case ']':
                  depth--;
                  break;
              }
            }
            pc--;
          }
          break;
        case ']':
          if (heap[hp] != 0) {
            int depth = 1;
            for (pc--; depth > 0; pc--) {
              switch (bfSource[pc]) {
                case '[':
                  depth--;
                  break;
                case ']':
                  depth++;
                  break;
              }
            }
            pc++;
          }
          break;
      }
    }
    std::cout.put('\n');
    std::cout.flush();
  }

  /*!
   * @brief Execute IR code
   * @param [in,out] heap  Pointer to heap memory
   */
  void
  executeIR(unsigned char* heap) const BRAINFUCK_NOEXCEPT
  {
    std::size_t hp = 0;
    for (std::vector<BfInst>::size_type pc = 0, size = ircode.size(); pc < size; pc++) {
      switch (ircode[pc].type) {
        case BfInst::Type::kMovePointer:
          hp = hp + static_cast<std::size_t>(ircode[pc].op1);
          break;
        case BfInst::Type::kAdd:
          heap[hp] = static_cast<unsigned char>(heap[hp] + ircode[pc].op1);
          break;
        case BfInst::Type::kPutchar:
          std::cout.put(static_cast<char>(heap[hp]));
          break;
        case BfInst::Type::kGetchar:
          std::cout.flush();
          heap[hp] = static_cast<unsigned char>(std::cin.get());
          break;
        case BfInst::Type::kLoopStart:
          if (heap[hp] == 0) {
            pc = static_cast<std::size_t>(ircode[pc].op1);
          }
          break;
        case BfInst::Type::kLoopEnd:
          if (heap[hp] != 0) {
            pc = static_cast<std::size_t>(ircode[pc].op1);
          }
          break;
        case BfInst::Type::kIf:
          if (heap[hp] == 0) {
            pc = static_cast<std::size_t>(ircode[pc].op1);
          }
          break;
        case BfInst::Type::kEndIf:
          // Do nothing
          break;
        case BfInst::Type::kAssign:
          heap[hp] = static_cast<unsigned char>(ircode[pc].op1);
          break;
        case BfInst::Type::kSearchZero:
          {
            std::size_t offset = static_cast<std::size_t>(ircode[pc].op1);
            while (heap[hp]) {
              hp += offset;
            }
          }
          break;
        case BfInst::Type::kAddVar:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] + heap[hp]);
          break;
        case BfInst::Type::kSubVar:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] - heap[hp]);
          break;
        case BfInst::Type::kAddCMulVar:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] + heap[hp] * ircode[pc].op2);
          break;
        case BfInst::Type::kInfLoop:
          if (heap[hp]) {
            for (;;);
          }
          break;
        case BfInst::Type::kUnknown:
          break;
      }
    }
    std::cout.put('\n');
    std::cout.flush();
  }

  void
  executeJit(unsigned char* heap) const BRAINFUCK_NOEXCEPT
  {
    cg.getCode<void (*)(int (*)(int), int (*)(), unsigned char*)>()
      (std::putchar, getcharWithFlush, heap);
    std::fflush(stdout);
  }

  /*!
   * @brief Dump IR code (Debug function)
   */
  void
  dumpIR() const BRAINFUCK_NOEXCEPT
  {
    for (std::vector<BfInst>::size_type pc = 0, size = ircode.size(); pc < size; pc++) {
      switch (ircode[pc].type) {
        case BfInst::Type::kMovePointer:
          std::cout << "kMovePointer: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kAdd:
          std::cout << "kAdd: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kPutchar:
          std::cout << "kPutchar" << std::endl;
          break;
        case BfInst::Type::kGetchar:
          std::cout << "kGetchar" << std::endl;
          break;
        case BfInst::Type::kLoopStart:
          std::cout << "kLoopStart: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kLoopEnd:
          std::cout << "kLoopEnd: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kIf:
          std::cout << "kIf: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kEndIf:
          std::cout << "kEndIf" << std::endl;
          break;
        case BfInst::Type::kAssign:
          std::cout << "kAssign: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kSearchZero:
          std::cout << "kSearchZero: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kAddVar:
          std::cout << "kAddVar: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kSubVar:
          std::cout << "kSubVar: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kAddCMulVar:
          std::cout << "kAddCMulVar: " << ircode[pc].op1 << ", " << ircode[pc].op2 << std::endl;
          break;
        case BfInst::Type::kInfLoop:
          std::cout << "kInfLoop" << std::endl;
          break;
        case BfInst::Type::kUnknown:
          std::cout << "kUnknown" << std::endl;
          break;
      }
    }
  }

  void
  emit(std::ostream& os, Target target) BRAINFUCK_NOEXCEPT
  {
    switch (target) {
      case Target::kC:
        GeneratorC(os).emit(ircode);
        break;
      case Target::kXbyakC:
        dumpXbyak(os);
        break;
      case Target::kWinX86:
        GeneratorWinX86(os).emit(ircode);
        break;
      case Target::kWinX64:
        GeneratorWinX64(os).emit(ircode);
        break;
      case Target::kElfX86:
        GeneratorElfX86(os).emit(ircode);
        break;
      case Target::kElfX64:
        GeneratorElfX64(os).emit(ircode);
        break;
      case Target::kElfArmeabi:
        GeneratorElfArmeabi(os).emit(ircode);
        break;
      default:
        assert(false);
    }
  }

  void
  dumpXbyak(std::ostream& os) BRAINFUCK_NOEXCEPT
  {
    std::size_t size = cg.getSize();
    if (size == 0) {
      compile(CompileType::kJit);
      size = cg.getSize();
    }
    const Xbyak::uint8* code = cg.getCode();

    os << "#include <stdio.h>\n"
                 "#include <stdlib.h>\n"
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
                 "#ifndef WIN32_LEAN_AND_MEAN\n"
                 "#  define WIN32_LEAN_AND_MEAN\n"
                 "#  define WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
                 "#endif\n"
                 "#include <windows.h>\n"
                 "#ifdef WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
                 "#  undef WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED\n"
                 "#  undef WIN32_LEAN_AND_MEAN\n"
                 "#endif\n"
#elif defined(__linux__)
                 "#include <unistd.h>\n"
                 "#include <sys/mman.h>\n"
#endif  // defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
                 "\n"
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
                 "#define GNUC_PREREQ(major, minor) \\\n"
                 "  (defined(__GNUC__) && (__GNUC__ > (major) || __GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))\n\n"
                 "#if defined(_MSC_VER)\n"
                 "#  pragma warning(push)\n"
                 "#  pragma warning(disable: 4055)\n"
                 "#elif GNUC_PREREQ(4, 6)\n"
                 "#  pragma GCC diagnostic push\n"
                 "#  pragma GCC diagnostic ignored \"-Wcast-qual\"\n"
                 "#  pragma GCC diagnostic ignored \"-Wpedantic\"\n"
#elif defined(__linux__)
                 "#if __GNUC_PREREQ(4, 6)\n"
                 "#  pragma GCC diagnostic push\n"
                 "#  pragma GCC diagnostic ignored \"-Wcast-qual\"\n"
                 "#  pragma GCC diagnostic ignored \"-Wpedantic\"\n"
#endif  // defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
                 "#endif\n\n"
                 "static unsigned char stack[65536];\n"
                 "/* code size: " << size << " bytes */\n"
                 "static const unsigned char code[] = {\n"
              << std::hex << " ";
    for (std::size_t i = 0; i < size; i++) {
      os << " 0x" << std::setfill('0') << std::setw(2) << static_cast<unsigned int>(code[i]) << ",";
      if (i % 16 == 15) {
        os << "\n ";
      }
    }
    os << std::dec
       << "\n};\n\n\n"
          "int\n"
          "main(void)\n"
          "{\n"
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
          "  DWORD old_protect;\n"
          "  VirtualProtect((LPVOID) code, sizeof(code), PAGE_EXECUTE_READWRITE, &old_protect);\n"
#elif defined(__linux__)
          "  unsigned long page_size = (unsigned long) (sysconf(_SC_PAGESIZE) - 1);\n"
          "  mprotect((void *) code, (sizeof(code) + page_size) & ~page_size, PROT_READ | PROT_EXEC);\n"
#endif  // defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
          "  ((void (*)(int (*)(int), int (*)(), unsigned char *)) (unsigned char *) code)(putchar, getchar, stack);\n"
          "  return EXIT_SUCCESS;\n"
          "}\n\n\n"
#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
          "#if defined(_MSC_VER)\n"
          "#  pragma warning(pop)\n"
          "#elif GNUC_PREREQ(4, 6)\n"
          "#  pragma GCC diagnostic pop\n"
#elif defined(__linux__)
          "#if __GNUC_PREREQ(4, 6)\n"
          "#  pragma GCC diagnostic pop\n"
#endif  // defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && defined(__x86_64__))
          "#endif"
       << std::endl;
  }

  std::string
  getSource() const BRAINFUCK_NOEXCEPT
  {
    return bfSource;
  }

  /*!
   * @brief Operator overload for output stream
   *
   * Output loadded brainfuck source code to given stream.
   *
   * @tparam CharT   Character type
   * @tparam Traits  Type traits
   * @param [in] os     Output stream
   * @param [in] this_  Refrence to this ArgumentParser object
   * @return Output stream
   */
  template<typename CharT, typename Traits>
  friend std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const Brainfuck& this_)
  {
    os << this_.bfSource;
    return os;
  }

  /*!
   * @brief Operator overload for input stream
   *
   * Load brainfuck source code from given stream.
   *
   * @tparam CharT   Character type
   * @tparam Traits  Type traits
   * @param [in] is     Input stream
   * @param [in] this_  Refrence to this ArgumentParser object
   * @return Input stream
   */
  template<typename CharT, typename Traits>
  friend std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, Brainfuck& this_)
  {
    is >> this_.bfSource;
    return is;
  }
};  // class Brainfuck


const std::string Brainfuck::kBrainfuckCharacters = "+-><.,[]";
const std::size_t Brainfuck::kDefaultHeapSize = 65536;
const std::size_t Brainfuck::kDefaultXbyakCodeGeneratorSize = 1048576;


#endif  // BRAINFUCK_HPP
