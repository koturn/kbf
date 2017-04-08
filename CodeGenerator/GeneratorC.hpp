/*!
 * @file GeneratorC.hpp
 * @brief C-source generator
 * @author koturn
 */
#ifndef GENERATOR_C_HPP
#define GENERATOR_C_HPP

#include <iostream>

#include "SourceGenerator.hpp"


/*!
 * @brief C-source generator
 */
class GeneratorC : public SourceGenerator<GeneratorC>
{
private:
  friend class CodeGenerator<GeneratorC>;

public:
  GeneratorC(std::ostream& oStream, const std::string& indent_="  ") :
    SourceGenerator<GeneratorC>(oStream, indent_)
  {}

protected:
  void
  emitHeaderImpl() CODE_GENERATOR_NOEXCEPT
  {
    (*oStreamPtr) << "#include <stdio.h>\n"
                     "#include <stdlib.h>\n"
                     "#include <string.h>\n\n"
                     "#define MEMORY_SIZE 65536\n\n"
                     "#if defined(_MSC_VER) || defined(__INTEL_COMPILER)\n"
                     "#  define INLINE  __forceinline\n"
                     "#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L || defined(__cplusplus)\n"
                     "#  define INLINE  inline\n"
                     "#else\n"
                     "#  define INLINE\n"
                     "#endif\n"
                     "#ifndef __GNUC__\n"
                     "INLINE static void *\n"
                     "memrchr(const void *s, int c, size_t n);\n"
                     "#endif\n\n\n"
                     "int\n"
                     "main(void)\n"
                     "{\n"
                  << indent << "static unsigned char memory[MEMORY_SIZE] = {0};\n"
                  << indent << "unsigned char *p = memory;\n\n";
    indentLevel++;
  }

  void
  emitFooterImpl() CODE_GENERATOR_NOEXCEPT
  {
    (*oStreamPtr) << "putchar('\\n');\n\n"
                  << indent << "return EXIT_SUCCESS;\n"
                     "}\n\n\n"
                     "#ifndef __GNUC__\n"
                     "INLINE static void *\n"
                     "memrchr(const void *s, int c, size_t n)\n"
                     "{\n"
                  << indent << "const char *_s = (const char *) s;\n"
                  << indent << "for (; *((int *) _s) != c; _s--);\n"
                  << indent << "return (void *) _s;\n"
                     "}\n"
                     "#endif"
                  << std::endl;
  }

  void
  emitMovePointerImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    if (op1 > 0) {
      if (op1 == 1) {
        *oStreamPtr << "p++;\n";
      } else {
        *oStreamPtr << "p += " << op1 << ";\n";
      }
    } else {
      if (op1 == -1) {
        *oStreamPtr << "p--;\n";
      } else {
        *oStreamPtr << "p -= " << -op1 << ";\n";
      }
    }
  }

  void
  emitAddImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    if (op1 > 0) {
      if (op1 == 1) {
        *oStreamPtr << "(*p)++;\n";
      } else {
        *oStreamPtr << "*p += " << op1 << ";\n";
      }
    } else {
      if (op1 == -1) {
        *oStreamPtr << "(*p)--;\n";
      } else {
        *oStreamPtr << "*p -= " << -op1 << ";\n";
      }
    }
  }

  void
  emitPutcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    *oStreamPtr << "putchar(*p);\n";
  }

  void
  emitGetcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    *oStreamPtr << "*p = (unsigned char) getchar();\n";
  }

  void
  emitLoopStartImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    *oStreamPtr << "while (*p) {\n";
    indentLevel++;
  }

  void
  emitLoopEndImpl() CODE_GENERATOR_NOEXCEPT
  {
    indentLevel--;
    emitIndent();
    *oStreamPtr << "}\n";
  }

  void
  emitIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    *oStreamPtr << "if (*p) {\n";
    indentLevel++;
  }

  void
  emitEndIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitLoopEndImpl();
  }

  void
  emitAssignImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    *oStreamPtr << "*p = " << op1 << ";\n";
  }

  void
  emitSearchZeroImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    if (op1 > 0) {
      if (op1 == 1) {
        *oStreamPtr << "p = memchr(p, 0, sizeof(memory));\n";
      } else {
        *oStreamPtr << "for (; *p; p += " << op1 << ");\n";
      }
    } else {
      if (op1 == -1) {
        *oStreamPtr << "p = memrchr(p, 0, sizeof(memory));\n";
      } else {
        *oStreamPtr << "for (; *p; p -= " << -op1 << ");\n";
      }
    }
  }

  void
  emitAddVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    if (op1 > 0) {
      *oStreamPtr << "*(p + " << op1;
    } else {
      *oStreamPtr << "*(p - " << -op1;
    }
    *oStreamPtr << ") += *p;\n";
  }

  void
  emitSubVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    if (op1 > 0) {
      *oStreamPtr << "*(p + " << op1;
    } else {
      *oStreamPtr << "*(p - " << -op1;
    }
    *oStreamPtr << ") -= *p;\n";
  }

  void
  emitAddCMulVarImpl(int op1, int op2) CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    if (op1 > 0) {
      *oStreamPtr << "*(p + " << op1;
    } else {
      *oStreamPtr << "*(p - " << -op1;
    }
    *oStreamPtr << ") += *p * " << op2 << ";\n";
  }

  void
  emitInfLoopImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIndent();
    *oStreamPtr << "if (*ptr) {\n";
    emitIndent();
    *oStreamPtr << indent << "for (;;);\n";
    emitIndent();
    *oStreamPtr << "}\n";
  }
};  // class GeneratorC


#endif  // GENERATOR_C_HPP