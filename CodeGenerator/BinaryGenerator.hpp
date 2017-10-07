/*!
 * @file BinaryGenerator.hpp
 * @brief Base class of executable binary generator
 * @author koturn
 */
#ifndef BINARY_GENERATOR_HPP
#define BINARY_GENERATOR_HPP

#include <cstdint>
#include <algorithm>
#include <stack>
#if __cplusplus >= 201103 || (defined(_MSC_VER) && _MSC_VER >= 1600)
#  include <array>
#  include <memory>
#endif

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
  BinaryGenerator(std::ostream& oStream) CODE_GENERATOR_NOEXCEPT :
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

  template<typename U>
  void
  fill(std::size_t size, const U element) CODE_GENERATOR_NOEXCEPT
  {
#if __cplusplus >= 201103 || (defined(_MSC_VER) && _MSC_VER >= 1600)
#  if __cplusplus >= 201402 || (defined(_MSC_VER) && _MSC_VER >= 1800)
    auto datum = std::make_unique<U[]>(size);
#  else
    std::unique_ptr<U[]> datum(new U[size]);
#  endif  // __cplusplus >= 201402
    std::fill_n(datum.get(), size, element);
    this->oStreamPtr->write(reinterpret_cast<const char*>(datum.get()), sizeof(U) * size);
#else
    U* datum = new U[kSize];
    std::fill_n(datum, kSize, element);
    this->oStreamPtr->write(reinterpret_cast<const char*>(&data), sizeof(data));
    delete datum;
#endif  // __cplusplus >= 201103
  }

  template<std::size_t kSize, typename U>
  void
  fill(const U element) CODE_GENERATOR_NOEXCEPT
  {
#if __cplusplus >= 201103 || (defined(_MSC_VER) && _MSC_VER >= 1600)
    std::array<U, kSize> datum;
    std::fill(std::begin(datum), std::end(datum), element);
    this->oStreamPtr->write(reinterpret_cast<const char*>(datum.data()), sizeof(U) * datum.size());
#else
    U datum[kSize];
    std::fill_n(datum, kSize, element);
    this->oStreamPtr->write(reinterpret_cast<const char*>(&data), sizeof(data));
#endif  // __cplusplus >= 201103
  }
};  // class BinaryGenerator


#endif  // BINARY_GENERATOR_HPP
