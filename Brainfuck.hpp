/*!
 * @file Brainfuck.hpp
 * @brief Brainfuck processor
 * @author koturn
 */
#ifndef BRAINFUCK_HPP
#define BRAINFUCK_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>


#if __cplusplus >= 201103L
#  define BRAINFUCK_NOEXCEPT  noexcept
#else
#  define BRAINFUCK_NOEXCEPT  throw()
#endif  // __cplusplus >= 201103L


/*!
 * @brief Brainfuck processor
 */
class Brainfuck
{
private:
  //! Brainfuck characters: +-><.,[]
  static const std::string kBrainfuckCharacters;
  //! Default eap size
  static const std::size_t kDefaultHeapSize;
  //! Brainfuck source code
  std::string bfSource;

public:
  /*!
   * Empty ctor
   */
  Brainfuck() :
    bfSource("")
  {}

  /*!
   * Copy-ctor
   */
  Brainfuck(const Brainfuck& that) :
    bfSource(that.bfSource)
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
   * @brief Execute brainfuck
   * @param [in] heapSize  Heap size for execution
   */
  void
  execute(std::size_t heapSize=kDefaultHeapSize) BRAINFUCK_NOEXCEPT
  {
    std::unique_ptr<unsigned char[]> heap(new unsigned char[heapSize]);
    std::fill_n(heap.get(), heapSize, 0);
    execute(heap.get());
  }

  /*!
   * @brief Execute brainfuck using given heap
   * @param [in,out] heap Pointer to heap memory
   */
  void
  execute(unsigned char* heap) BRAINFUCK_NOEXCEPT
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
          std::cout.put(heap[hp]);
          break;
        case ',':
          std::cout.flush();
          heap[hp] = static_cast<char>(std::cin.get());
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
