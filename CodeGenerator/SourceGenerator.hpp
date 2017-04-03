/*!
 * @file SourceGenerator.hpp
 * @brief Base class of source code generator
 * @author koturn
 */
#ifndef SOURCE_GENERATOR_HPP
#define SOURCE_GENERATOR_HPP

#include <iostream>

#include "CodeGenerator.hpp"


/*!
 * @brief Source code generator
 */
template<typename T>
class SourceGenerator : public CodeGenerator<T>
{
private:
  friend class CodeGenerator<T>;

protected:
  //! Indent string
  std::string indent;
  //! indent level
  int indentLevel;

public:
  SourceGenerator(std::ostream& oStream, const std::string& indent_="  ") :
    CodeGenerator<T>(oStream),
    indent(indent_),
    indentLevel(0)
  {}

  void
  emitIndent() CODE_GENERATOR_NOEXCEPT
  {
    for (int i = 0; i < indentLevel; i++) {
      (*(this->oStreamPtr)) << indent;
    }
  }
};  // class SourceGenerator


#endif  // SOURCE_GENERATOR_HPP
