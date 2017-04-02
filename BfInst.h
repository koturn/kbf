/*!
 * @file BfInst.h
 * @brief Brainfuck IR instructions
 * @author koturn
 */
#ifndef BF_INST_H
#define BF_INST_H


/*!
 * @brief One brainfuck IR instruction
 */
struct BfInst {
//! Brainfuck instructions
#define BF_INST_INSTRUCTIONS \
  kMovePointer, kAdd, \
  kPutchar, kGetchar, \
  kLoopStart, kLoopEnd, kIf, kEndIf, \
  kAssign, kSearchZero, \
  kAddVar, kSubVar, kAddCMulVar, \
  kInfLoop, \
  kUnknown

#if __cplusplus >= 201103L
  /*!
   * @brief Instruction type
   */
  enum class Type
  {
    //! Brainfuck instructions
    BF_INST_INSTRUCTIONS
  };  // enum class Type
#else
  /*!
   * @brief Instruction type
   */
  class Type
  {
  public:
    /*!
     * @brief Actual enum of instruction type
     */
    enum TypeEnum
    {
      BF_INST_INSTRUCTIONS
    };
    /*!
     * @brief Ctor for implicit conversion: Actual enum to dummy enum class
     * @param [in] value  Actual enum value
     */
    Type(TypeEnum value) :
      value(value)
    {}
    /*!
     * @brief operator () for implicit conversion: Dummy enum class to actual
     *        enum
     * @return Actual enum value
     */
    operator TypeEnum() const throw()
    {
      return value;
    }
  private:
    //! Actual enum value
    TypeEnum value;
  };  // class Type
#endif  // __cplusplus >= 201103L

#undef BF_INST_INSTRUCTIONS

  //! Instruction type
  Type type;
  //! Operand 1
  int op1;
  //! Operand 2
  int op2;

  /*!
   * @brief Ctor
   * @param [in] type
   * @param [in] op1
   * @param [in] op2
   */
  BfInst(Type type=Type::kUnknown, int op1=0, int op2=0) :
    type(type),
    op1(op1),
    op2(op2)
  {}
};  // struct BfInst


#endif  // BF_INST_H
