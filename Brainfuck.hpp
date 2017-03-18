/*!
 * @file Brainfuck.hpp
 * @brief Brainfuck processor
 * @author koturn
 */
#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <cassert>
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "BfInst.h"


#if __cplusplus >= 201103L
//! Polyfill macro of @code noexcept @endcode
#  define BRAINFUCK_NOEXCEPT  noexcept
#else
//! Polyfill macro of @code noexcept @endcode
#  define BRAINFUCK_NOEXCEPT  throw()
#endif  // __cplusplus >= 201103L


/*!
 * @brief Brainfuck processor
 */
class Brainfuck
{
public:
  #if __cplusplus >= 201103L
  /*!
   * @brief Compilation type
   */
  enum class CompileType
  {
    kIR, kUnknown
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
      kIR, kUnknown
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

private:
  //! Brainfuck characters: +-><.,[]
  static const std::string kBrainfuckCharacters;
  //! Default eap size
  static const std::size_t kDefaultHeapSize;
  //! Brainfuck source code
  std::string bfSource;
  //! IR code
  std::vector<BfInst> ircode;
  //! Internal compile state
  CompileType state;

  /*!
   * @brief Generate an "At" operation
   * @param [in] dInst  Destination instruction
   * @param [in] inst1  Instruction which may represents a value
   * @return Return true if type of inst1 is a value instruction, otherwise false
   */
  static bool
  genOperationAt(BfInst& dInst, const BfInst& inst1) BRAINFUCK_NOEXCEPT
  {
    switch (inst1.type) {
      case BfInst::Type::kInc:
        dInst.type = BfInst::Type::kIncAt;
        return false;
      case BfInst::Type::kDec:
        dInst.type = BfInst::Type::kDecAt;
        return false;
      case BfInst::Type::kAdd:
        dInst.type = BfInst::Type::kAddAt;
        dInst.op2 = inst1.op1;
        return false;
      case BfInst::Type::kSub:
        dInst.type = BfInst::Type::kSubAt;
        dInst.op2 = inst1.op1;
        return false;
      case BfInst::Type::kAssignZero:
        dInst.type = BfInst::Type::kAssignAt;
        return false;
      case BfInst::Type::kAssign:
        dInst.type = BfInst::Type::kAssignAt;
        dInst.op2 = inst1.op1;
        return false;
      default:
        return true;
    }
  }

  /*!
   * @brief Identify whether given instruction type is a pointer movement operation or not
   * @param [in] type  Instruction type
   * @return Return true if given argument is kNext, kNextN, kPrev or kPrevN,
   *         otherwise false.
   */
  static bool
  isPointerOperation(BfInst::Type type) BRAINFUCK_NOEXCEPT
  {
    return type == BfInst::Type::kNext
      || type == BfInst::Type::kNextN
      || type == BfInst::Type::kPrev
      || type == BfInst::Type::kPrevN;
  }

  /*!
   * @brief Identify whether given instruction type is an "At" operation or not
   * @param [in] type  Instruction type
   * @return Return true if given argument is kIncAt, kDecAt, kAddAt or kSubAt,
   *         otherwise false.
   */
  static bool
  isOperationAt(BfInst::Type type) BRAINFUCK_NOEXCEPT
  {
    return type == BfInst::Type::kIncAt
      || type == BfInst::Type::kDecAt
      || type == BfInst::Type::kAddAt
      || type == BfInst::Type::kSubAt;
  }

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


public:
  /*!
   * Empty ctor
   */
  Brainfuck() :
    bfSource(""),
    ircode(),
    state(CompileType::kUnknown)
  {}

  /*!
   * Copy-ctor
   */
  Brainfuck(const Brainfuck& that) :
    bfSource(that.bfSource),
    ircode(that.ircode),
    state(CompileType::kUnknown)
  {}

  /*!
   * Dtor
   */
  ~Brainfuck() = default;

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
    return *this;
  }


#if __cplusplus >= 201103L
  // move-ctor
  Brainfuck(Brainfuck&& that) = default;
  // move-operator=
  Brainfuck&
  operator=(Brainfuck&& that) = default;
#endif  // __cplusplus >= 201103L

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
  }

  /*!
   * @brief Load brainfuck source from a std::string
   * @param [in] bfSource  Brainfuck source code
   */
  void
  loadSource(const std::string& bfSource) BRAINFUCK_NOEXCEPT
  {
    this->bfSource = bfSource;
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
    compileToIR();
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
          {
            bool isNormalNext = true;
            pc++;
            int value = compressInstruction(pc, '>', '<') + 1;
            BfInst bi;
            if (ircode.size() > 1) {
              BfInst inst1 = ircode[ircode.size() - 1];  // (+*|-*) ?
              BfInst inst2 = ircode[ircode.size() - 2];  // <* ?
              if (value == 1 && inst2.type == BfInst::Type::kPrev) {
                bi.op1 = -1;
                isNormalNext = genOperationAt(bi, inst1);
              } else if (value == -1 && inst2.type == BfInst::Type::kNext) {
                bi.op1 = 1;
                isNormalNext = genOperationAt(bi, inst1);
              } else if (value > 1 && inst2.type == BfInst::Type::kPrevN && value == inst2.op1) {
                bi.op1 = -inst2.op1;
                isNormalNext = genOperationAt(bi, inst1);
              } else if (value < 1 && inst2.type == BfInst::Type::kNextN && -value == inst2.op1) {
                bi.op1 = inst2.op1;
                isNormalNext = genOperationAt(bi, inst1);
              }
            }
            if (isNormalNext) {
              if (value > 0) {
                ircode.emplace_back(value == 1 ? BfInst(BfInst::Type::kNext) : BfInst(BfInst::Type::kNextN, value));
              } else if (value < 0) {
                ircode.emplace_back(value == -1 ? BfInst(BfInst::Type::kPrev) : BfInst(BfInst::Type::kPrevN, -value));
              } else {
                continue;
              }
            } else {
              ircode.pop_back(); ircode.pop_back();
              ircode.emplace_back(bi);
            }
          }
          break;
        case '<':
          {
            bool isNormalPrev = true;
            pc++;
            int value = compressInstruction(pc, '<', '>') + 1;
            BfInst bi;
            if (ircode.size() > 1) {
              BfInst inst1 = ircode[ircode.size() - 1];  // (+*|-*) ?
              BfInst inst2 = ircode[ircode.size() - 2];  // >* ?
              if (value == 1 && inst2.type == BfInst::Type::kNext) {
                bi.op1 = 1;
                isNormalPrev = genOperationAt(bi, inst1);
              } else if (value == -1 && inst2.type == BfInst::Type::kPrev) {
                bi.op1 = -1;
                isNormalPrev = genOperationAt(bi, inst1);
              } else if (value > 0 && inst2.type == BfInst::Type::kNextN && value == inst2.op1) {
                bi.op1 = inst2.op1;
                isNormalPrev = genOperationAt(bi, inst1);
              } else if (value < 0 && inst2.type == BfInst::Type::kPrevN && -value == inst2.op1) {
                bi.op1 = -inst2.op1;
                isNormalPrev = genOperationAt(bi, inst1);
              }
            }
            if (isNormalPrev) {
              if (value > 0) {
                ircode.emplace_back(value == 1 ? BfInst(BfInst::Type::kPrev) : BfInst(BfInst::Type::kPrevN, value));
              } else if (value < 0) {
                ircode.emplace_back(value == -1 ? BfInst(BfInst::Type::kNext) : BfInst(BfInst::Type::kNextN, -value));
              } else {
                continue;
              }
            } else {
              ircode.pop_back(); ircode.pop_back();
              ircode.emplace_back(bi);
            }
          }
          break;
        case '+':
          {
            pc++;
            int value = compressInstruction(pc, '+', '-') + 1;
            if (ircode.size() > 0 && ircode[ircode.size() - 1].type == BfInst::Type::kAssignZero) {
              ircode.pop_back();  // remove kAssignZero
              ircode.emplace_back(BfInst(BfInst::Type::kAssign, value));
            } else {
              if (value > 0) {
                ircode.emplace_back(value == 1 ? BfInst(BfInst::Type::kInc) : BfInst(BfInst::Type::kAdd, value));
              } else if (value < 0) {
                ircode.emplace_back(value == -1 ? BfInst(BfInst::Type::kDec) : BfInst(BfInst::Type::kSub, -value));
              }
            }
          }
          break;
        case '-':
          {
            pc++;
            int value = compressInstruction(pc, '-', '+') + 1;
            if (ircode.size() > 0 && ircode[ircode.size() - 1].type == BfInst::Type::kAssignZero) {
              while (value < 0) {
                value += 256;
              }
              ircode.pop_back();  // remove kAssignZero
              ircode.emplace_back(BfInst(BfInst::Type::kAssign, value));
            } else {
              if (value > 0) {
                ircode.emplace_back(value == 1 ? BfInst(BfInst::Type::kDec) : BfInst(BfInst::Type::kSub, value));
              } else if (value < 0) {
                ircode.emplace_back(value == -1 ? BfInst(BfInst::Type::kInc) : BfInst(BfInst::Type::kAdd, -value));
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
            bool isNormalLoopEnd = true;
            std::vector<BfInst>::size_type size = ircode.size();
            if (size > 0 && ircode[size - 1].type == BfInst::Type::kLoopStart) {
              ircode.pop_back();
              ircode.emplace_back(BfInst(BfInst::Type::kInfLoop));
              isNormalLoopEnd = false;
            }
            if (size > 1 && ircode[size - 2].type == BfInst::Type::kLoopStart) {
              BfInst inst = ircode[size - 1];
              if (inst.type == BfInst::Type::kInc || inst.type == BfInst::Type::kDec) {
                ircode.pop_back(); ircode.pop_back();
                ircode.emplace_back(BfInst(BfInst::Type::kAssignZero));
              } else if (isPointerOperation(inst.type)) {
                ircode.pop_back(); ircode.pop_back();
                switch (inst.type) {
                  case BfInst::Type::kNext:
                    ircode.emplace_back(BfInst(BfInst::Type::kSearchZero, 1));
                    break;
                  case BfInst::Type::kPrev:
                    ircode.emplace_back(BfInst(BfInst::Type::kSearchZero, -1));
                    break;
                  case BfInst::Type::kNextN:
                    ircode.emplace_back(BfInst(BfInst::Type::kSearchZero, inst.op1));
                    break;
                  case BfInst::Type::kPrevN:
                    ircode.emplace_back(BfInst(BfInst::Type::kSearchZero, -inst.op1));
                    break;
                  default:
                    assert(false);  // never reach here
                }
              }
              isNormalLoopEnd = false;
            }
            if (size > 2) {  // [->+<]
              BfInst inst1 = ircode[size - 1];  // >*(+|-)*<* ? or - ?
              BfInst inst2 = ircode[size - 2];  // - ? or >*(+|-)*<* ?
              BfInst inst3 = ircode[size - 3];  // [ ?
              if (inst3.type == BfInst::Type::kLoopStart &&
                  ((inst2.type == BfInst::Type::kDec && isOperationAt(inst1.type)) ||
                   (isOperationAt(inst2.type) && inst1.type == BfInst::Type::kDec))) {
                ircode.pop_back(); ircode.pop_back(); ircode.pop_back();
                const BfInst& c = (inst2.type == BfInst::Type::kDec) ? inst1 : inst2;
                switch (c.type) {
                  case BfInst::Type::kIncAt:
                    ircode.emplace_back(BfInst(BfInst::Type::kAddVar, c.op1));
                    isNormalLoopEnd = false;
                    break;
                  case BfInst::Type::kDecAt:
                    ircode.emplace_back(BfInst(BfInst::Type::kSubVar, c.op1));
                    isNormalLoopEnd = false;
                    break;
                  case BfInst::Type::kAddAt:
                    ircode.emplace_back(BfInst(BfInst::Type::kCMulVar, c.op1, c.op2));
                    isNormalLoopEnd = false;
                    break;
                  default:
                    break;
                }
              }
            }
            if (isNormalLoopEnd) {
              ircode[static_cast<std::size_t>(loopStack.top())].op1 = static_cast<int>(ircode.size());
              ircode.emplace_back(BfInst(BfInst::Type::kLoopEnd, loopStack.top()));
            }
            loopStack.pop();
          }
          break;
      }
    }
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
        case BfInst::Type::kNext:
          hp++;
          break;
        case BfInst::Type::kPrev:
          hp--;
          break;
        case BfInst::Type::kNextN:
          hp = hp + static_cast<std::size_t>(ircode[pc].op1);
          break;
        case BfInst::Type::kPrevN:
          hp = hp - static_cast<std::size_t>(ircode[pc].op1);
          break;
        case BfInst::Type::kInc:
          heap[hp]++;
          break;
        case BfInst::Type::kDec:
          heap[hp]--;
          break;
        case BfInst::Type::kAdd:
          heap[hp] = static_cast<unsigned char>(heap[hp] + ircode[pc].op1);
          break;
        case BfInst::Type::kSub:
          heap[hp] = static_cast<unsigned char>(heap[hp] - ircode[pc].op1);
          break;
        case BfInst::Type::kIncAt:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)]++;
          break;
        case BfInst::Type::kDecAt:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)]--;
          break;
        case BfInst::Type::kAddAt:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] + ircode[pc].op2);
          break;
        case BfInst::Type::kSubAt:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] - ircode[pc].op2);
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
        case BfInst::Type::kAssignZero:
          heap[hp] = 0;
          break;
        case BfInst::Type::kAssign:
          heap[hp] = static_cast<unsigned char>(ircode[pc].op1);
          break;
        case BfInst::Type::kAssignAt:
          heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(ircode[pc].op2);
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
          if (heap[hp]) {
            heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] + heap[hp]);
            heap[hp] = 0;
          }
          break;
        case BfInst::Type::kSubVar:
          if (heap[hp]) {
            heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] - heap[hp]);
            heap[hp] = 0;
          }
          break;
        case BfInst::Type::kCMulVar:
          if (heap[hp]) {
            heap[hp + static_cast<std::size_t>(ircode[pc].op1)] = static_cast<unsigned char>(heap[hp + static_cast<std::size_t>(ircode[pc].op1)] + heap[hp] * ircode[pc].op2);
            heap[hp] = 0;
          }
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

  /*!
   * @brief Dump IR code (Debug function)
   */
  void
  dumpIR() const BRAINFUCK_NOEXCEPT
  {
    for (std::vector<BfInst>::size_type pc = 0, size = ircode.size(); pc < size; pc++) {
      switch (ircode[pc].type) {
        case BfInst::Type::kNext:
          std::cout << "kNext" << std::endl;
          break;
        case BfInst::Type::kPrev:
          std::cout << "kPrev" << std::endl;
          break;
        case BfInst::Type::kNextN:
          std::cout << "kNextN: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kPrevN:
          std::cout << "kPrevN: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kInc:
          std::cout << "kInc" << std::endl;
          break;
        case BfInst::Type::kDec:
          std::cout << "kDec" << std::endl;
          break;
        case BfInst::Type::kAdd:
          std::cout << "kAddN: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kSub:
          std::cout << "kSubN: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kIncAt:
          std::cout << "kIncAt: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kDecAt:
          std::cout << "kDecAt: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kAddAt:
          std::cout << "kAddAt: " << ircode[pc].op1 << ", " << ircode[pc].op2 << std::endl;
          break;
        case BfInst::Type::kSubAt:
          std::cout << "kSubAt: " << ircode[pc].op1 << ", " << ircode[pc].op2 << std::endl;
          break;
        case BfInst::Type::kPutchar:
          std::cout << "kPutchar" << std::endl;
          break;
        case BfInst::Type::kGetchar:
          std::cout << "kGetchar" << std::endl;
          break;
        case BfInst::Type::kLoopStart:
          std::cout << "kLoopStart" << std::endl;
          break;
        case BfInst::Type::kLoopEnd:
          std::cout << "kLoopEnd" << std::endl;
          break;
        case BfInst::Type::kAssignZero:
          std::cout << "kAssignZero" << std::endl;
          break;
        case BfInst::Type::kAssign:
          std::cout << "kAssign: " << ircode[pc].op1 << std::endl;
          break;
        case BfInst::Type::kAssignAt:
          std::cout << "kAssignAt: " << ircode[pc].op1 << ", " << ircode[pc].op2 << std::endl;
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
        case BfInst::Type::kCMulVar:
          std::cout << "kCMulVar: " << ircode[pc].op1 << ", " << ircode[pc].op2 << std::endl;
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


#endif  // BRAINFUCK_HPP
