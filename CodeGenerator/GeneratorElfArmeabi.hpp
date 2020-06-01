/*!
 * @file GeneratorElfArmeabi.hpp
 * @brief Arm EABI ELF binary generator
 * @author koturn
 */
#ifndef GENERATOR_ELF_ARMEABI_HPP
#define GENERATOR_ELF_ARMEABI_HPP

#include <iostream>

#include "BinaryGenerator.hpp"
#include "util/elfsubset.h"


/*!
 * @brief Arm EABI ELF binary generator
 */
class GeneratorElfArmeabi : public BinaryGenerator<GeneratorElfArmeabi>
{
private:
  friend class CodeGenerator<GeneratorElfArmeabi>;
  //! Address of .text section
  static const Elf32_Addr kBaseAddr = 0x00010000;
  //! Address of .bss section
  static const Elf32_Addr kBssAddr = 0x00210000;
  //! Number of program headers
  static const Elf32_Half kNProgramHeaders = 2;
  //! Number of section headers
  static const Elf32_Half kNSectionHeaders = 4;
  //! Program header size
  static const Elf32_Off kHeaderSize = static_cast<Elf32_Off>(sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * kNProgramHeaders);
  //! Program footer size
  static const Elf32_Off kFooterSize = static_cast<Elf32_Off>(sizeof(Elf32_Shdr)) * kNSectionHeaders;

public:
  explicit GeneratorElfArmeabi(std::ostream& oStream) CODE_GENERATOR_NOEXCEPT :
    BinaryGenerator<GeneratorElfArmeabi>(oStream)
  {}

private:
  void
  emitHeaderImpl() CODE_GENERATOR_NOEXCEPT
  {
    // skip header
    skip(sizeof(Elf32_Ehdr) + sizeof(Elf32_Phdr) * kNProgramHeaders);

    // - - - - - The start of program body - - - - - //
    // ldr r1,[pc]
    u32 opcode1 = 0xe59f1000;
    write(opcode1);
    // b #0
    u32 opcode2 = 0xea000000;
    write(opcode2);
    // {kBssAddr} (constant)
    write(static_cast<u32>(kBssAddr));
    // mov r2, #1
    u32 opcode3 = 0xe3a02001;
    write(opcode3);
  }

  void
  emitFooterImpl() CODE_GENERATOR_NOEXCEPT
  {
    // Emit newline
    emitAssignImpl('\n');
    emitPutcharImpl();

    // mov r7, #1
    u32 opcode1 = 0xe3a07001;
    write(opcode1);
    // mov r0, #0
    u32 opcode2 = 0xe3a00000;
    write(opcode2);
    // svc #0
    u32 opcode3 = 0xef000000;
    write(opcode3);
    // - - - - - The end of program body - - - - - //

    Elf32_Off codeSize = static_cast<Elf32_Off>(oStream.tellp()) - kHeaderSize;

    // - - - - - Program footer - - - - - //
    // Section string table (22bytes)
    const char kShStrTbl[] = "\0.text\0.shstrtbl\0.bss";
    write(kShStrTbl);

    // First section header
    Elf32_Shdr shdr;
    shdr.sh_name = 0;
    shdr.sh_type = SHT_NULL;
    shdr.sh_flags = 0x00000000;
    shdr.sh_addr = 0x00000000;
    shdr.sh_offset = 0x00000000;
    shdr.sh_size = 0x00000000;
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x00000000;
    shdr.sh_entsize = 0x00000000;
    write(shdr);

    // Second section header (.shstrtbl)
    shdr.sh_name = 7;
    shdr.sh_type = SHT_STRTAB;
    shdr.sh_flags = 0x00000000;
    shdr.sh_addr = 0x00000000;
    shdr.sh_offset = kHeaderSize + codeSize;
    shdr.sh_size = sizeof(kShStrTbl);
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x00000001;
    shdr.sh_entsize = 0x00000000;
    write(shdr);

    // Third section header (.text)
    shdr.sh_name = 1;
    shdr.sh_type = SHT_PROGBITS;
    shdr.sh_flags = SHF_EXECINSTR | SHF_ALLOC;
    shdr.sh_addr = kBaseAddr + kHeaderSize;
    shdr.sh_offset = kHeaderSize;
    shdr.sh_size = codeSize;
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x00000004;
    shdr.sh_entsize = 0x00000000;
    write(shdr);

    // Fourth section header (.bss)
    shdr.sh_name = 17;
    shdr.sh_type = SHT_NOBITS;
    shdr.sh_flags = SHF_ALLOC | SHF_WRITE;
    shdr.sh_addr = kBssAddr;
    shdr.sh_offset = 0x00001000;
    shdr.sh_size = 0x00010000;  // 65536 cells
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x00000010;
    shdr.sh_entsize = 0x00000000;
    write(shdr);

    // - - - - - Program header - - - - - //
    oStream.seekp(0, std::ios_base::beg);
    // ELF header
    Elf32_Ehdr ehdr;
    std::fill_n(ehdr.e_ident, sizeof(ehdr.e_ident), 0x00);
    ehdr.e_ident[EI_MAG0] = ELFMAG0;
    ehdr.e_ident[EI_MAG1] = ELFMAG1;
    ehdr.e_ident[EI_MAG2] = ELFMAG2;
    ehdr.e_ident[EI_MAG3] = ELFMAG3;
    ehdr.e_ident[EI_CLASS] = ELFCLASS32;
    ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
    ehdr.e_ident[EI_VERSION] = EV_CURRENT;
    ehdr.e_ident[EI_OSABI] = ELFOSABI_ARM_AEABI;
    ehdr.e_ident[EI_ABIVERSION] = 0x00;
    ehdr.e_ident[EI_PAD] = 0x00;
    ehdr.e_type = ET_EXEC;
    ehdr.e_machine = EM_ARM;
    ehdr.e_version = EV_CURRENT;
    ehdr.e_entry = kBaseAddr + kHeaderSize;
    ehdr.e_phoff = sizeof(Elf32_Ehdr);
    ehdr.e_shoff = static_cast<Elf32_Off>(kHeaderSize + sizeof(kShStrTbl) + codeSize);
    ehdr.e_flags = 0x00000000;
    ehdr.e_ehsize = sizeof(Elf32_Ehdr);
    ehdr.e_phentsize = sizeof(Elf32_Phdr);
    ehdr.e_phnum = kNProgramHeaders;
    ehdr.e_shentsize = sizeof(Elf32_Shdr);
    ehdr.e_shnum = kNSectionHeaders;
    ehdr.e_shstrndx = 1;
    write(ehdr);

    // Program header
    Elf32_Phdr phdr;
    phdr.p_type = PT_LOAD;
    phdr.p_flags = PF_R | PF_X;
    phdr.p_offset = 0x00000000;
    phdr.p_vaddr = kBaseAddr;
    phdr.p_paddr = kBaseAddr;
    phdr.p_filesz = static_cast<Elf32_Word>(kHeaderSize + sizeof(kShStrTbl) + kFooterSize + codeSize);
    phdr.p_memsz = static_cast<Elf32_Word>(kHeaderSize + sizeof(kShStrTbl) + kFooterSize + codeSize);
    phdr.p_align = 0x00000100;
    write(phdr);

    // Program header for .bss (56 bytes)
    phdr.p_type = PT_LOAD;
    phdr.p_flags = PF_R | PF_W;
    phdr.p_offset = 0x00001000;
    phdr.p_vaddr = kBssAddr;
    phdr.p_paddr = kBssAddr;
    phdr.p_filesz = 0x00000000;
    phdr.p_memsz = 0x00010000;
    phdr.p_align = 0x00200000;
    write(phdr);

    oStream.seekp(0, std::ios_base::end);
  }

  void
  emitMovePointerImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (op1 > 0) {
      if (op1 > 4095) {
        // ldr r8, [pc]
        u32 opcode1 = 0xe59f8000;
        write(opcode1);
        // b #0
        u32 opcode2 = 0xea000000;
        write(opcode2);
        // {op1} (const)
        write(static_cast<u32>(op1));
        // add r1, r1, r8
        u32 opcode3 = 0xe0811008;
        write(opcode3);
      } else {
        // add r1, r1, #{op1}
        u32 opcode = (0xe2811000 | (0xff & op1));
        write(opcode);
      }
    } else {
      if (op1 < -4095) {
        // ldr r8, [pc]
        u32 opcode1 = 0xe59f8000;
        write(opcode1);
        // b #0
        u32 opcode2 = 0xea000000;
        write(opcode2);
        // {op1} (const)
        write(static_cast<u32>(-op1));
        // sub r1, r1, r8
        u32 opcode3 = 0xe0411008;
        write(opcode3);
      } else {
        // sub r1, r1, #{op1}
        u32 opcode = (0xe2411000 | (0xff & -op1));
        write(opcode);
      }
    }
  }

  void
  emitAddImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (op1 > 0) {
      // ldrb r8, [r1]
      u32 opcode1 = 0xe5d18000;
      write(opcode1);
      // add r8, r8, {op1}
      u32 opcode2 = (0xe2888000 | (0xff & op1));
      write(opcode2);
      // strb r8, [r1]
      u32 opcode3 = 0xe5c18000;
      write(opcode3);
    } else {
      // ldrb r8, [r1]
      u32 opcode1 = 0xe5d18000;
      write(opcode1);
      // sub r8, r8, {op1}
      u32 opcode2 = (0xe2488000 | (0xff & -op1));
      write(opcode2);
      // strb r8, [r1]
      u32 opcode3 = 0xe5c18000;
      write(opcode3);
    }
  }

  void
  emitPutcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    // mov r7, #4
    u32 opcode1 = 0xe3a07004;
    write(opcode1);
    // mov r0, #1
    u32 opcode2 = 0xe3a00001;
    write(opcode2);
    // svc #0
    u32 opcode3 = 0xef000000;
    write(opcode3);
  }

  void
  emitGetcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    // mov r7, #3
    u32 opcode1 = 0xe3a07003;
    write(opcode1);
    // mov r0, #0
    u32 opcode2 = 0xe3a00000;
    write(opcode2);
    // svc #0
    u32 opcode3 = 0xef000000;
    write(opcode3);
  }

  void
  emitLoopStartImpl() CODE_GENERATOR_NOEXCEPT
  {
    loopStack.push(oStream.tellp());
    // ldrb r8, [r1]
    u32 opcode1 = 0xe5d18000;
    write(opcode1);
    // cmp r8, #0
    u32 opcode2 = 0xe3580000;
    write(opcode2);
    // beq #*
    u32 opcode3 = 0x0a000000;
    write(opcode3);
  }

  void
  emitLoopEndImpl() CODE_GENERATOR_NOEXCEPT
  {
    std::ostream::pos_type pos = loopStack.top();
    std::ostream::pos_type curPos = oStream.tellp();
    int offset = static_cast<int>(curPos - pos) / 4;
    // b #*
    u32 opcode1 = (0xea000000 | (0x00ffffff & -(offset + 2)));
    // beq #*
    u32 opcode2 = (0x0a000000 | (0x00ffffff & (offset - 3)));
    write(opcode1);
    oStream.seekp(pos + static_cast<std::ostream::pos_type>(8), std::ios_base::beg);
    write(opcode2);
    oStream.seekp(0, std::ios_base::end);
    loopStack.pop();
  }

  void
  emitIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitLoopStartImpl();
    // ldrb r9, [r1]
    u32 opcode2 = 0xe5d19000;
    write(opcode2);
  }

  void
  emitEndIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    std::ostream::pos_type pos = loopStack.top();
    std::ostream::pos_type curPos = oStream.tellp();
    int offset = static_cast<int>(curPos - pos) / 4;
    // beq #*
    u32 opcode1 = (0x0a000000 | (0x00ffffff & (offset - 2)));
    oStream.seekp(pos + static_cast<std::ostream::pos_type>(8), std::ios_base::beg);
    write(opcode1);
    oStream.seekp(0, std::ios_base::end);
    loopStack.pop();
  }

  void
  emitAssignImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov r8, #0
    u32 opcode1 = (0xe3a08000 | static_cast<u8>(op1));
    write(opcode1);
    // strb r8, [r1]
    u32 opcode2 = 0xe5c18000;
    write(opcode2);
  }

  void
  emitAddVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (std::abs(op1) > 4095) {
      std::cerr << "Invalie address offset: " << op1 << std::endl;
    }
    // ldrb r8, [r1, #{op1}]
    u32 opcode1 = op1 > 0 ? (0xe5d18000 | (0xff & op1)) : (0xe5518000 | (0xff & -op1));
    write(opcode1);
    // add r8, r8, r9
    u32 opcode3 = 0xe0888009;
    write(opcode3);
    // strb r8, [r1]
    u32 opcode4 = op1 > 0 ? (0xe5c18000 | (0xff & op1)) : (0xe5418000 | (0xff & -op1));
    write(opcode4);
  }

  void
  emitSubVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (std::abs(op1) > 4095) {
      std::cerr << "Invalie address offset: " << op1 << std::endl;
    }
    // ldrb r8, [r1, #{op1}]
    u32 opcode1 = op1 > 0 ? (0xe5d18000 | (0xff & op1)) : (0xe5518000 | (0xff & -op1));
    write(opcode1);
    // sub r8, r8, r9
    u32 opcode3 = 0xe0488009;
    write(opcode3);
    // strb r8, [r1]
    u32 opcode4 = op1 > 0 ? (0xe5c18000 | (0xff & op1)) : (0xe5418000 | (0xff & -op1));
    write(opcode4);
  }

  void
  emitAddCMulVarImpl(int op1, int op2) CODE_GENERATOR_NOEXCEPT
  {
    if (std::abs(op1) > 4095) {
      std::cerr << "Invalie address offset: " << op1 << std::endl;
    }
    // ldrb r8, [r1, #{op1}]
    u32 opcode1 = op1 > 0 ? (0xe5d18000 | (0xff & op1)) : (0xe5518000 | (0xff & -op1));
    write(opcode1);
    if (op2 > 0) {
      // mov r6, #{op2}
      u32 opcode3 = (0xe3a06000 | (0xff & op2));
      write(opcode3);
      // mul r6, r9, r6
      u32 opcode4 = 0xe0060699;
      write(opcode4);
      // add r8, r8, r6
      u32 opcode5 = 0xe0888006;
      write(opcode5);
    } else {
      // mov r6, #{-op2}
      u32 opcode3 = (0xe3a06000 | (0xff & -op2));
      write(opcode3);
      // mul r6, r9, r6
      u32 opcode4 = 0xe0060699;
      write(opcode4);
      // sub r8, r8, r6
      u32 opcode5 = 0xe0488006;
      write(opcode5);
    }
    // strb r8, [r1]
    u32 opcode6 = op1 > 0 ? (0xe5c18000 | (0xff & op1)) : (0xe5418000 | (0xff & -op1));
    write(opcode6);
  }

#if 0
  void
  emitInfLoopImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIfImpl();
    // jmp {offset} (near jump)
    u8 opcode = {0xeb};
    write(opcode);
    write(static_cast<u8>(-2));
    emitEndIfImpl();
  }
#endif

  void
  emitBreakPointImpl() CODE_GENERATOR_NOEXCEPT
  {
    // .inst 0xe7f001f0
    write(0xe7f001f0);
  }
};  // class GeneratorElfArmeabi


#endif  // GENERATOR_ELF_ARMEABI_HPP
