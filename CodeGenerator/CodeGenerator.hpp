/*!
 * @file CodeGenerator.hpp
 * @brief Base class of brainfuck translator
 * @author koturn
 */
#ifndef CODE_GENERATOR_HPP
#define CODE_GENERATOR_HPP

#include <cassert>
#include <iostream>
#include <vector>

#include "../BfInst.h"

#if defined(__cplusplus) && __cplusplus >= 201103 \
  || defined(_MSC_VER) && (_MSC_VER > 1800 || (_MSC_VER == 1800 && _MSC_FULL_VER == 180021114))
#  define CODE_GENERATOR_NOEXCEPT  noexcept
#else
#  define CODE_GENERATOR_NOEXCEPT  throw()
#endif


/*!
 * @brief Base class of code generator
 */
template<typename T>
class CodeGenerator
{
protected:
  //! Output stream pointer
  std::ostream* oStreamPtr;

public:
  explicit CodeGenerator(std::ostream& oStream) CODE_GENERATOR_NOEXCEPT :
    oStreamPtr(&oStream)
  {}

#if __cplusplus >= 201103 && (defined(_MSC_VER) && _MSC_VER >= 1800)
  CodeGenerator(const CodeGenerator&) = delete;

  CodeGenerator&
  operator=(const CodeGenerator&) = delete;
#else
private:
  CodeGenerator(const CodeGenerator&);

  CodeGenerator&
  operator=(const CodeGenerator&);
public:
#endif  // __cplusplus >= 201103L && (defined(_MSC_VER) && _MSC_VER >= 1800)

  void
  setOutputStream(std::ostream& oStream) CODE_GENERATOR_NOEXCEPT
  {
    oStreamPtr = &oStream;
  }

  void
  emit(const std::vector<BfInst>& ircode) CODE_GENERATOR_NOEXCEPT
  {
    emitHeader();
    for (std::vector<BfInst>::size_type pc = 0, size = ircode.size(); pc < size; pc++) {
      switch (ircode[pc].type) {
        case BfInst::Type::kMovePointer:
          emitMovePointer(ircode[pc].op1);
          break;
        case BfInst::Type::kAdd:
          emitAdd(ircode[pc].op1);
          break;
        case BfInst::Type::kPutchar:
          emitPutchar();
          break;
        case BfInst::Type::kGetchar:
          emitGetchar();
          break;
        case BfInst::Type::kLoopStart:
          emitLoopStart();
          break;
        case BfInst::Type::kLoopEnd:
          emitLoopEnd();
          break;
        case BfInst::Type::kIf:
          emitIf();
          break;
        case BfInst::Type::kEndIf:
          emitEndIf();
          break;
        case BfInst::Type::kAssign:
          emitAssign(ircode[pc].op1);
          break;
        case BfInst::Type::kSearchZero:
          emitSearchZero(ircode[pc].op1);
          break;
        case BfInst::Type::kAddVar:
          emitAddVar(ircode[pc].op1);
          break;
        case BfInst::Type::kSubVar:
          emitSubVar(ircode[pc].op1);
          break;
        case BfInst::Type::kAddCMulVar:
          emitAddCMulVar(ircode[pc].op1, ircode[pc].op2);
          break;
        case BfInst::Type::kInfLoop:
          emitInfLoop();
          break;
        default:
          assert(false);
      }
    }
    emitFooter();
    oStreamPtr->flush();
  }

private:
  void
  emitHeader() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitHeaderImpl();
  }

  void
  emitFooter() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitFooterImpl();
  }

  void
  emitMovePointer(int op1) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitMovePointerImpl(op1);
  }

  void
  emitAdd(int op1) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitAddImpl(op1);
  }

  void
  emitPutchar() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitPutcharImpl();
  }

  void
  emitGetchar() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitGetcharImpl();
  }

  void
  emitLoopStart() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitLoopStartImpl();
  }

  void
  emitLoopEnd() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitLoopEndImpl();
  }

  void
  emitIf() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitIfImpl();
  }

  void
  emitEndIf() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitEndIfImpl();
  }

  void
  emitAssign(int op1) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitAssignImpl(op1);
  }

  void
  emitSearchZero(int op1) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitSearchZeroImpl(op1);
  }

  void
  emitAddVar(int op1) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitAddVarImpl(op1);
  }

  void
  emitSubVar(int op1) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitSubVarImpl(op1);
  }

  void
  emitAddCMulVar(int op1, int op2) CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitAddCMulVarImpl(op1, op2);
  }

  void
  emitInfLoop() CODE_GENERATOR_NOEXCEPT
  {
    static_cast<T*>(this)->emitInfLoopImpl();
  }

  // - - - Default implementations - - -
protected:
  void
  emitIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitLoopStart();
  }

  void
  emitEndIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitLoopEnd();
  }

  void
  emitAssignImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitLoopStart();
    emitAdd(-1);
    emitLoopEnd();
    emitAdd(op1);
  }

  void
  emitSearchZeroImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitLoopStart();
    emitMovePointer(op1);
    emitLoopEnd();
  }

  void
  emitAddVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitMovePointer(op1);
    emitAdd(1);
    emitMovePointer(-op1);
  }

  void
  emitSubVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitMovePointer(op1);
    emitAdd(-1);
    emitMovePointer(-op1);
  }

  void
  emitAddCMulVarImpl(int op1, int op2) CODE_GENERATOR_NOEXCEPT
  {
    emitMovePointer(op1);
    emitAdd(op2);
    emitMovePointer(-op1);
  }

  void
  emitInfLoopImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitLoopStart();
    emitLoopEnd();
  }
};  // class CodeGenerator


#include "GeneratorC.hpp"
#include "GeneratorWinX86.hpp"
#include "GeneratorWinX64.hpp"
#include "GeneratorElfX86.hpp"
#include "GeneratorElfX64.hpp"
#include "GeneratorElfArmeabi.hpp"


#endif  // CODE_GENERATOR_HPP
