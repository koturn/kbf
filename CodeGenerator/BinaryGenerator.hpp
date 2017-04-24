/*!
 * @file BinaryGenerator.hpp
 * @brief Base class of executable binary generator
 * @author koturn
 */
#ifndef BINARY_GENERATOR_HPP
#define BINARY_GENERATOR_HPP

#include <cstdint>
#include <stack>

#include "CodeGenerator.hpp"


/*!
 * @brief Executable binary generator
 */
template<typename T>
class BinaryGenerator : public CodeGenerator<T>
{
protected:
  friend class CodeGenerator<T>;
  //! Type alias for std::uint8_t
  typedef std::uint8_t u8;
  //! Type alias for std::uint16_t
  typedef std::uint16_t u16;
  //! Type alias for std::uint32_t
  typedef std::uint32_t u32;
  //! Type alias for std::uint64_t
  typedef std::uint64_t u64;

  template<typename U, typename V>
  static inline U
  calcAlignedSize(U size, V alignment) CODE_GENERATOR_NOEXCEPT
  {
    return alignment * ((size + alignment - 1) / alignment);
  }

  //! Loop stack
  std::stack<std::ostream::pos_type> loopStack;

public:
  BinaryGenerator(std::ostream& oStream) :
    CodeGenerator<T>(oStream),
    loopStack()
  {}

protected:
  template<typename U>
  void
  write(const U& data) CODE_GENERATOR_NOEXCEPT
  {
    this->oStreamPtr->write(reinterpret_cast<const char*>(&data), sizeof(data));
  }

  template<typename U>
  void
  write(const U* data, std::size_t size) CODE_GENERATOR_NOEXCEPT
  {
    this->oStreamPtr->write(reinterpret_cast<const char*>(data), size);
  }
};  // class BinaryGenerator


#endif  // BINARY_GENERATOR_HPP
