/*!
 * @file GeneratorElfX64.hpp
 * @brief x64 ELF binary generator
 * @author koturn
 */
#ifndef GENERATOR_ELF_X64_HPP
#define GENERATOR_ELF_X64_HPP

#include <iostream>

#include "BinaryGenerator.hpp"
#include "util/elfsubset.h"


/*!
 * @brief x64 ELF binary generator
 */
class GeneratorElfX64 : public BinaryGenerator<GeneratorElfX64>
{
private:
  friend class CodeGenerator<GeneratorElfX64>;
  //! Address of .text section
  static const Elf64_Addr kBaseAddr;
  //! Address of .bss section
  static const Elf64_Addr kBssAddr;
  //! Number of program headers
  static const Elf64_Half kNProgramHeaders;
  //! Number of section headers
  static const Elf64_Half kNSectionHeaders;
  //! Program header size
  static const Elf64_Off kHeaderSize;
  //! Program footer size
  static const Elf64_Off kFooterSize;

public:
  GeneratorElfX64(std::ostream& oStream) :
    BinaryGenerator<GeneratorElfX64>(oStream)
  {}

protected:
  void
  emitHeaderImpl() CODE_GENERATOR_NOEXCEPT
  {
    // skip header
    Elf64_Ehdr ehdr;
    write(ehdr);
    Elf64_Phdr phdr;
    write(phdr);
    write(phdr);

    // - - - - - The start of program body - - - - - //
    // movabs rsi, {kBssAddr}
    u8 opcode1[] = {0x48, 0xbe};
    write(opcode1);
    write(static_cast<u64>(kBssAddr));
    // mov edx, 0x01
    u8 opcode2[] = {0xba};
    write(opcode2);
    write(static_cast<u32>(0x01));
  }

  void
  emitFooterImpl() CODE_GENERATOR_NOEXCEPT
  {
    // Emit newline
    emitAssignImpl('\n');
    emitPutcharImpl();

    // mov eax, 0x3c
    u8 opcode1[] = {0xb8};
    write(opcode1);
    write(static_cast<u32>(0x3c));
    // xor edi, edi
    u8 opcode2[] = {0x31, 0xff};
    write(opcode2);
    // syscall
    u8 opcode3[] = {0x0f, 0x05};
    write(opcode3);
    // - - - - - The end of program body - - - - - //

    std::size_t codeSize = static_cast<std::size_t>(oStreamPtr->tellp()) - kHeaderSize;

    // - - - - - Program footer - - - - - //
    // Section string table (22bytes)
    const char kShStrTbl[] = "\0.text\0.shstrtbl\0.bss";
    write(kShStrTbl);

    // First section header
    Elf64_Shdr shdr;
    shdr.sh_name = 0;
    shdr.sh_type = SHT_NULL;
    shdr.sh_flags = 0x0000000000000000;
    shdr.sh_addr = 0x0000000000000000;
    shdr.sh_offset = 0x0000000000000000;
    shdr.sh_size = 0x0000000000000000;
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x0000000000000000;
    shdr.sh_entsize = 0x0000000000000000;
    write(shdr);

    // Second section header (.shstrtbl)
    shdr.sh_name = 7;
    shdr.sh_type = SHT_STRTAB;
    shdr.sh_flags = 0x0000000000000000;
    shdr.sh_addr = 0x0000000000000000;
    shdr.sh_offset = kHeaderSize + codeSize;
    shdr.sh_size = sizeof(kShStrTbl);
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x0000000000000001;
    shdr.sh_entsize = 0x0000000000000000;
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
    shdr.sh_addralign = 0x0000000000000004;
    shdr.sh_entsize = 0x0000000000000000;
    write(shdr);

    // Fourth section header (.bss)
    shdr.sh_name = 17;
    shdr.sh_type = SHT_NOBITS;
    shdr.sh_flags = SHF_ALLOC | SHF_WRITE;
    shdr.sh_addr = kBssAddr;
    shdr.sh_offset = 0x0000000000001000;
    shdr.sh_size = 0x0000000000010000;  // 65536 cells
    shdr.sh_link = 0x00000000;
    shdr.sh_info = 0x00000000;
    shdr.sh_addralign = 0x0000000000000010;
    shdr.sh_entsize = 0x0000000000000000;
    write(shdr);

    // - - - - - Program header - - - - - //
    oStreamPtr->seekp(0, std::ios_base::beg);
    // ELF header
    Elf64_Ehdr ehdr;
    std::fill_n(ehdr.e_ident, sizeof(ehdr.e_ident), 0x00);
    ehdr.e_ident[EI_MAG0] = ELFMAG0;
    ehdr.e_ident[EI_MAG1] = ELFMAG1;
    ehdr.e_ident[EI_MAG2] = ELFMAG2;
    ehdr.e_ident[EI_MAG3] = ELFMAG3;
    ehdr.e_ident[EI_CLASS] = ELFCLASS64;
    ehdr.e_ident[EI_DATA] = ELFDATA2LSB;
    ehdr.e_ident[EI_VERSION] = EV_CURRENT;
    ehdr.e_ident[EI_OSABI] = ELFOSABI_LINUX;
    ehdr.e_ident[EI_ABIVERSION] = 0x00;
    ehdr.e_ident[EI_PAD] = 0x00;
    ehdr.e_type = ET_EXEC;
    ehdr.e_machine = EM_X86_64;
    ehdr.e_version = EV_CURRENT;
    ehdr.e_entry = kBaseAddr + kHeaderSize;
    ehdr.e_phoff = sizeof(Elf64_Ehdr);
    ehdr.e_shoff = kHeaderSize + sizeof(kShStrTbl) + codeSize;
    ehdr.e_flags = 0x00000000;
    ehdr.e_ehsize = sizeof(Elf64_Ehdr);
    ehdr.e_phentsize = sizeof(Elf64_Phdr);
    ehdr.e_phnum = kNProgramHeaders;
    ehdr.e_shentsize = sizeof(Elf64_Shdr);
    ehdr.e_shnum = kNSectionHeaders;
    ehdr.e_shstrndx = 1;
    write(ehdr);

    // Program header
    Elf64_Phdr phdr;
    phdr.p_type = PT_LOAD;
    phdr.p_flags = PF_R | PF_X;
    phdr.p_offset = 0x0000000000000000;
    phdr.p_vaddr = kBaseAddr;
    phdr.p_paddr = kBaseAddr;
    phdr.p_filesz = kHeaderSize + sizeof(kShStrTbl) + kFooterSize + codeSize;
    phdr.p_memsz = kHeaderSize + sizeof(kShStrTbl) + kFooterSize + codeSize;
    phdr.p_align = 0x0000000000000100;
    write(phdr);

    // Program header for .bss (56 bytes)
    phdr.p_type = PT_LOAD;
    phdr.p_flags = PF_R | PF_W;
    phdr.p_offset = 0x0000000000001000;
    phdr.p_vaddr = kBssAddr;
    phdr.p_paddr = kBssAddr;
    phdr.p_filesz = 0x0000000000000000;
    phdr.p_memsz = 0x0000000000010000;
    phdr.p_align = 0x0000000000200000;
    write(phdr);

    oStreamPtr->seekp(0, std::ios_base::end);
  }

  void
  emitMovePointerImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (op1 > 0) {
      if (op1 > 127) {
        // add rsi, {op1}
        u8 opcode[] = {0x48, 0x81, 0xc6};
        write(opcode);
        write(op1);
      } else if (op1 > 1) {
        // add rsi, {op1}
        u8 opcode[] = {0x48, 0x83, 0xc6};
        write(opcode);
        write(static_cast<u8>(op1));
      } else {
        // inc rsi
        u8 opcode[] = {0x48, 0xff, 0xc6};
        write(opcode);
      }
    } else {
      if (op1 < -127) {
        // sub rsi, {op1}
        u8 opcode[] = {0x48, 0x81, 0xee};
        write(opcode);
        write(-op1);
      } else if (op1 < -1) {
        // sub rsi, {op1}
        u8 opcode[] = {0x48, 0x83, 0xee};
        write(opcode);
        write(static_cast<u8>(-op1));
      } else {
        // dec rsi
        u8 opcode[] = {0x48, 0xff, 0xce};
        write(opcode);
      }
    }
  }

  void
  emitAddImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (op1 > 0) {
      if (op1 > 1) {
        // add byte ptr [rsi], {op1}
        u8 opcode[] = {0x80, 0x06};
        write(opcode);
        write(static_cast<u8>(op1));
      } else {
        // inc byte ptr [rsi]
        u8 opcode[] = {0xfe, 0x06};
        write(opcode);
      }
    } else {
      if (op1 < -1) {
        // sub byte ptr [rsi], {op1}
        u8 opcode[] = {0x80, 0x2e};
        write(opcode);
        write(static_cast<u8>(-op1));
      } else {
        // dec byte ptr [rsi]
        u8 opcode[] = {0xfe, 0x0e};
        write(opcode);
      }
    }
  }

  void
  emitPutcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    // mov rax, 0x01
    u8 opcode1[] = {0x48, 0xc7, 0xc0};
    write(opcode1);
    write(static_cast<u32>(0x01));
    // mov edi, 0x01
    u8 opcode2[] = {0xbf};
    write(opcode2);
    write(static_cast<u32>(0x01));
    // syscall
    u8 opcode3[] = {0x0f, 0x05};
    write(opcode3);
  }

  void
  emitGetcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    // mov rax, 0x00
    u8 opcode1[] = {0x48, 0xc7, 0xc0};
    write(opcode1);
    write(static_cast<u32>(0x00));
    // xor edi, edi
    u8 opcode2[] = {0x31, 0xff};
    write(opcode2);
    // syscall
    u8 opcode3[] = {0x0f, 0x05};
    write(opcode3);
  }

  void
  emitLoopStartImpl() CODE_GENERATOR_NOEXCEPT
  {
    loopStack.push(oStreamPtr->tellp());
    // cmp byte ptr [rsi], 0x00
    u8 opcode1[] = {0x80, 0x3e, 0x00};
    write(opcode1);
    // je 0x********
    u8 opcode2[] = {0x0f, 0x84};
    write(opcode2);
    write(static_cast<u32>(0x00000000));
  }

  void
  emitLoopEndImpl() CODE_GENERATOR_NOEXCEPT
  {
    std::ostream::pos_type pos = loopStack.top();
    int offset = static_cast<int>(pos - oStreamPtr->tellp()) - 1;
    if (offset - static_cast<int>(sizeof(u8)) < -128) {
      // jmp {offset} (near jump)
      u8 opcode = {0xe9};
      write(opcode);
      write(offset - sizeof(u32));
    } else {
      // jmp {offset} (short jump)
      u8 opcode = {0xeb};
      write(opcode);
      write(static_cast<u8>(offset - sizeof(u8)));
    }
    // fill loop start
    std::ostream::pos_type curPos = oStreamPtr->tellp();
    oStreamPtr->seekp(pos + static_cast<std::ostream::pos_type>(5), std::ios_base::beg);
    write(static_cast<u32>(curPos - oStreamPtr->tellp() - sizeof(u32)));
    oStreamPtr->seekp(0, std::ios_base::end);
    loopStack.pop();
  }

  void
  emitEndIfImpl() CODE_GENERATOR_NOEXCEPT
  {
    // fill if jump
    std::ostream::pos_type pos = loopStack.top();
    std::ostream::pos_type curPos = oStreamPtr->tellp();
    oStreamPtr->seekp(pos + static_cast<std::ostream::pos_type>(5), std::ios_base::beg);
    write(static_cast<u32>(curPos - oStreamPtr->tellp() - sizeof(u32)));
    oStreamPtr->seekp(0, std::ios_base::end);
    loopStack.pop();
  }

  void
  emitAssignImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov byte ptr [rsi], {op1}
    u8 opcode[] = {0xc6, 0x06};
    write(opcode);
    write(static_cast<u8>(op1));
  }

  void
  emitAddVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov al, byte ptr [rsi]
    u8 opcode1[] = {0x8a, 0x06};
    write(opcode1);
    // add byte ptr [rsi + {op1}], al
    if (op1 < -128 || 127 < op1) {
      u8 opcode2[] = {0x00, 0x86};
      write(opcode2);
      write(static_cast<u32>(op1));
    } else {
      u8 opcode2[] = {0x00, 0x46};
      write(opcode2);
      write(static_cast<u8>(op1));
    }
  }

  void
  emitSubVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov al, byte ptr [rsi]
    u8 opcode1[] = {0x8a, 0x06};
    write(opcode1);
    // add byte ptr [rsi + {op1}], al
    if (op1 < -128 || 127 < op1) {
      u8 opcode2[] = {0x28, 0x86};
      write(opcode2);
      write(static_cast<u32>(op1));
    } else {
      u8 opcode2[] = {0x28, 0x46};
      write(opcode2);
      write(static_cast<u8>(op1));
    }
  }

  void
  emitAddCMulVarImpl(int op1, int op2) CODE_GENERATOR_NOEXCEPT
  {
    if (op2 > 0) {
      // mov al, {op2}
      u8 opcode1[] = {0xb0};
      write(opcode1);
      write(static_cast<u8>(op2));
      // mul byte ptr [rsi]
      u8 opcode2[] = {0xf6, 0x26};
      write(opcode2);
      // add byte ptr [rsi + {op1}], al
      if (op1 < -128 || 127 < op1) {
        u8 opcode2[] = {0x00, 0x86};
        write(opcode2);
        write(static_cast<u32>(op1));
      } else {
        u8 opcode2[] = {0x00, 0x46};
        write(opcode2);
        write(static_cast<u8>(op1));
      }
    } else {
      // mov al, {-op2}
      u8 opcode1[] = {0xb0};
      write(opcode1);
      write(static_cast<u8>(-op2));
      // mul byte ptr [rsi]
      u8 opcode2[] = {0xf6, 0x26};
      write(opcode2);
      // sub byte ptr [rsi], al
      if (op1 < -128 || 127 < op1) {
        u8 opcode2[] = {0x28, 0x86};
        write(opcode2);
        write(static_cast<u32>(op1));
      } else {
        u8 opcode2[] = {0x28, 0x46};
        write(opcode2);
        write(static_cast<u8>(op1));
      }
    }
  }

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
};  // class GeneratorElfX64


const Elf64_Addr GeneratorElfX64::kBaseAddr = 0x04048000;
const Elf64_Addr GeneratorElfX64::kBssAddr = 0x04248000;
const Elf64_Half GeneratorElfX64::kNProgramHeaders = 2;
const Elf64_Half GeneratorElfX64::kNSectionHeaders = 4;
const Elf64_Off GeneratorElfX64::kHeaderSize = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) * kNProgramHeaders;
const Elf64_Off GeneratorElfX64::kFooterSize = sizeof(Elf64_Shdr) * kNSectionHeaders;


#endif  // GENERATOR_ELF_X64_HPP
