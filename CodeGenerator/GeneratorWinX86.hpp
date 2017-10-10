/*!
 * @file GeneratorWinX86.hpp
 * @brief x86 Windows exe binary generator
 * @author koturn
 */
#ifndef GENERATOR_WIN_X86_HPP
#define GENERATOR_WIN_X86_HPP

#include <cstring>
#include <iostream>

#include "BinaryGenerator.hpp"
#include "util/winsubset.h"


/*!
 * @brief x86 Windows exe binary generator
 */
class GeneratorWinX86 : public BinaryGenerator<GeneratorWinX86>
{
private:
  friend class CodeGenerator<GeneratorWinX86>;
  //! Address of .text section
  static const DWORD kBaseAddr;
  //! Size of PE header with padding
  static const DWORD kPeHeaderSizeWithPadding;
  //! Size of .idata with padding
  static const DWORD kIdataSizeWithPadding;

public:
  explicit GeneratorWinX86(std::ostream& oStream) CODE_GENERATOR_NOEXCEPT :
    BinaryGenerator<GeneratorWinX86>(oStream)
  {}

protected:
  void
  emitHeaderImpl() CODE_GENERATOR_NOEXCEPT
  {
    // skip header
    fill<0x400, u8>(0x00);

    // - - - - - The start of program body - - - - - //
    // mov esi, ds:{0x********}  # putchar() address
    u8 opcode1[] = {0x8b, 0x35};
    write(opcode1);
    write<u32>(0x00000000);  // Fill after
    // mov edi, ds:{0x********}  # getchar() address
    u8 opcode2[] = {0x8b, 0x3d};
    write(opcode2);
    write<u32>(0x00000000);  // Fill after
    // mov ebx, {0x********}  # .bss address
    u8 opcode3[] = {0xbb};
    write(opcode3);
    write<u32>(0x00000000);  // Fill after
  }

  void
  emitFooterImpl() CODE_GENERATOR_NOEXCEPT
  {
    // Emit newline
    emitAssignImpl('\n');
    emitPutcharImpl();

    // mov esi, ds:{0x********}  # exit
    u8 opcode1[] = {0x8b, 0x35};
    write(opcode1);
    std::ostream::pos_type exitAddrPos = oStreamPtr->tellp();
    write<u32>(0x00000000);  // Fill after
    // push 0x00
    u8 opcode3[] = {0x6a};
    write(opcode3);
    write<u8>(0x00);
    // call esi (exit)
    u8 opcode4[] = {0xff, 0xd6};
    write(opcode4);

    // Write padding
    DWORD codeSize = static_cast<DWORD>(oStreamPtr->tellp()) - (kPeHeaderSizeWithPadding + kIdataSizeWithPadding);
    DWORD codeSizeWithPadding = calcAlignedSize(codeSize, 0x1000);
    fill<u8>(codeSizeWithPadding - codeSize, 0x00);
    // - - - - - The end of program body - - - - - //

    // - - - - - Program header - - - - - //
    oStreamPtr->seekp(0, std::ios_base::beg);

    // Write DOS header
    IMAGE_DOS_HEADER idh;
    idh.e_magic = IMAGE_DOS_SIGNATURE;
    idh.e_cblp = 0x0050;
    idh.e_cp = 0x0002;
    idh.e_crlc = 0x0000;
    idh.e_cparhdr = 0x0004;
    idh.e_minalloc = 0x000f;
    idh.e_maxalloc = 0xffff;
    idh.e_ss = 0x0000;
    idh.e_sp = 0x00b8;
    idh.e_csum = 0x0000;
    idh.e_ip = 0x0000;
    idh.e_cs = 0x0000;
    idh.e_lfarlc = 0x0040;
    idh.e_ovno = 0x001a;
    std::fill_n(idh.e_res, sizeof(idh.e_res), 0x0000);
    idh.e_oemid = 0x0000;
    idh.e_oeminfo = 0x0000;
    std::fill_n(idh.e_res2, sizeof(idh.e_res2), 0x0000);
    idh.e_lfanew = 0x00000080;
    write(idh);

    // Write DOS stub
    const u8 kDosStub[] =
      "\xba\x10\x00"  // mov dx, 0x0010 (Offset to message data from here)
      "\x0e"  // push cs
      "\x1f"  // pop ds
      "\xb4\x09"  // mov ah, 0x09 (Argument for int 0x21: print)
      "\xcd\x21"  // int 0x21
      "\xb8\x01\x4c"  // mov ax, 0x4c01 (Argument for int 0x21: exit)
      "\xcd\x21"  // int 0x21
      "\x90"  // nop
      "\x90"  // nop
      "This program cannot be run in DOS mode.\r\n$\x00\x00\x00\x00\x00";
    write(kDosStub);

    // Write image file header
    write<DWORD>(IMAGE_NT_SIGNATURE);
    IMAGE_FILE_HEADER ifh;
    ifh.Machine = IMAGE_FILE_MACHINE_I386;  // 0x014c
    ifh.NumberOfSections = 3;
    ifh.TimeDateStamp = 0;
    ifh.PointerToSymbolTable = 0;
    ifh.NumberOfSymbols = 0;
    ifh.SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32);
    ifh.Characteristics = IMAGE_FILE_RELOCS_STRIPPED
      | IMAGE_FILE_EXECUTABLE_IMAGE
      | IMAGE_FILE_LINE_NUMS_STRIPPED
      | IMAGE_FILE_LOCAL_SYMS_STRIPPED
      | IMAGE_FILE_32BIT_MACHINE
      | IMAGE_FILE_DEBUG_STRIPPED;
    write(ifh);

    IMAGE_OPTIONAL_HEADER32 ioh;
    ioh.Magic = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
    ioh.MajorLinkerVersion = 14;
    ioh.MinorLinkerVersion = 0;
    ioh.SizeOfCode = codeSizeWithPadding;
    ioh.SizeOfInitializedData = 0;
    ioh.SizeOfUninitializedData = 65536;
    ioh.AddressOfEntryPoint = 0x1000;
    ioh.BaseOfCode = 0x1000;
    ioh.BaseOfData = ioh.BaseOfCode + codeSizeWithPadding + 0x1000;
    ioh.ImageBase = kBaseAddr;
    ioh.SectionAlignment = 0x1000;
    ioh.FileAlignment = 0x0200;
    ioh.MajorOperatingSystemVersion = 4;
    ioh.MinorOperatingSystemVersion = 0;
    ioh.MajorImageVersion = 0;
    ioh.MinorImageVersion = 0;
    ioh.MajorSubsystemVersion = 4;
    ioh.MinorSubsystemVersion = 0;
    ioh.Win32VersionValue = 0;  // Not used. Always 0
    ioh.SizeOfImage = 0x10000 + codeSizeWithPadding + ioh.SectionAlignment * 2;
    ioh.SizeOfHeaders = kPeHeaderSizeWithPadding;
    ioh.CheckSum = 0;
    ioh.Subsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
    ioh.DllCharacteristics = 0;
    ioh.SizeOfStackReserve = 1024 * 1024;
    ioh.SizeOfStackCommit = 8 * 1024;
    ioh.SizeOfHeapReserve = 1024 * 1024;
    ioh.SizeOfHeapCommit = 4 * 1024;
    ioh.LoaderFlags = 0;
    ioh.NumberOfRvaAndSizes = 16;
    IMAGE_DATA_DIRECTORY iddZero = {0, 0};
    std::fill_n(ioh.DataDirectory, IMAGE_NUMBEROF_DIRECTORY_ENTRIES, iddZero);
    ioh.DataDirectory[1].VirtualAddress = ioh.BaseOfCode + codeSizeWithPadding;  // import table
    ioh.DataDirectory[1].Size = 100;
    write(ioh);

    // .text section
    IMAGE_SECTION_HEADER ishText;
    std::memcpy(reinterpret_cast<void*>(ishText.Name), ".text\0\0", sizeof(ishText.Name));
    ishText.Misc.VirtualSize = codeSizeWithPadding;
    ishText.VirtualAddress = ioh.BaseOfCode;
    ishText.SizeOfRawData = codeSizeWithPadding;
    ishText.PointerToRawData = kPeHeaderSizeWithPadding + kIdataSizeWithPadding;
    ishText.PointerToRelocations = 0x00000000;
    ishText.PointerToLinenumbers = 0x00000000;
    ishText.NumberOfRelocations = 0x0000;
    ishText.NumberOfLinenumbers = 0x0000;
    ishText.Characteristics = IMAGE_SCN_CNT_CODE
      | IMAGE_SCN_CNT_INITIALIZED_DATA
      | IMAGE_SCN_ALIGN_16BYTES
      | IMAGE_SCN_MEM_EXECUTE
      | IMAGE_SCN_MEM_READ;
    write(ishText);

    // .idata section
    IMAGE_SECTION_HEADER ishIdata;
    std::memcpy(reinterpret_cast<void*>(ishIdata.Name), ".idata\0", sizeof(ishIdata.Name));
    ishIdata.Misc.VirtualSize = 100;
    ishIdata.VirtualAddress = ishText.VirtualAddress + codeSizeWithPadding;
    ishIdata.SizeOfRawData = 512;
    ishIdata.PointerToRawData = kPeHeaderSizeWithPadding;
    ishIdata.PointerToRelocations = 0x00000000;
    ishIdata.PointerToLinenumbers = 0x00000000;
    ishIdata.NumberOfRelocations = 0x0000;
    ishIdata.NumberOfLinenumbers = 0x0000;
    ishIdata.Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA
      | IMAGE_SCN_ALIGN_4BYTES
      | IMAGE_SCN_MEM_READ
      | IMAGE_SCN_MEM_WRITE;
    write(ishIdata);

    // .bss section
    IMAGE_SECTION_HEADER ishBss;
    std::memcpy(reinterpret_cast<void*>(ishBss.Name), ".bss\0\0\0", sizeof(ishBss.Name));
    ishBss.Misc.VirtualSize = 65536;
    ishBss.VirtualAddress = ishIdata.VirtualAddress + ioh.SectionAlignment;
    ishBss.SizeOfRawData = 0;
    ishBss.PointerToRawData = 0;
    ishBss.PointerToRelocations = 0x00000000;
    ishBss.PointerToLinenumbers = 0x00000000;
    ishBss.NumberOfRelocations = 0x0000;
    ishBss.NumberOfLinenumbers = 0x0000;
    ishBss.Characteristics = IMAGE_SCN_CNT_UNINITIALIZED_DATA
      | IMAGE_SCN_ALIGN_8BYTES
      | IMAGE_SCN_MEM_READ
      | IMAGE_SCN_MEM_WRITE;
    write(ishBss);

    oStreamPtr->seekp(kPeHeaderSizeWithPadding, std::ios_base::beg);

    const char kDllName[] = "msvcrt.dll\0\0\0\0\0";
    const char kPutcharName[] = "putchar";
    const char kGetcharName[] = "getchar";
    const char kExitName[] = "exit\0\0\0";

    IMAGE_IMPORT_DESCRIPTOR iids[2];
    IMAGE_THUNK_DATA32 itdInts[4];

    iids[0].OriginalFirstThunk = static_cast<DWORD>(ishIdata.VirtualAddress + sizeof(iids));  // int
    iids[0].TimeDateStamp = 0x00000000;
    iids[0].ForwarderChain = 0x00000000;
    iids[0].Name = static_cast<DWORD>(iids[0].OriginalFirstThunk + sizeof(itdInts));  // msvcrt.dll
    iids[0].FirstThunk = iids[0].Name + 16;  // iat
    iids[1].Characteristics = 0x00000000;
    iids[1].TimeDateStamp = 0x00000000;
    iids[1].ForwarderChain = 0x00000000;
    iids[1].Name = 0x00000000;
    iids[1].FirstThunk = 0x00000000;
    write(iids);

    itdInts[0].u1.AddressOfData = ishIdata.VirtualAddress + static_cast<DWORD>(sizeof(iids) + sizeof(kDllName) + sizeof(itdInts) * 2);  // putchar
    itdInts[1].u1.AddressOfData = itdInts[0].u1.AddressOfData + static_cast<DWORD>(sizeof(WORD) + sizeof(kPutcharName));  // getchar
    itdInts[2].u1.AddressOfData = itdInts[1].u1.AddressOfData + static_cast<DWORD>(sizeof(WORD) + sizeof(kGetcharName));  // exit
    itdInts[3].u1.AddressOfData = 0x00000000;
    write(itdInts);  // write INT (import Name Table)
    write(kDllName);
    write(itdInts);  // IAT (Import Address Table) is same as INT

    write<WORD>(0x0000);
    write(kPutcharName);
    write<WORD>(0x0000);
    write(kGetcharName);
    write<WORD>(0x0000);
    write(kExitName);

    oStreamPtr->seekp(ishText.PointerToRawData + 0x02, std::ios_base::beg);
    write(ioh.ImageBase + iids[0].FirstThunk);  // Fill putchar() address
    oStreamPtr->seekp(ishText.PointerToRawData + 0x08, std::ios_base::beg);
    write(static_cast<u32>(ioh.ImageBase + iids[0].FirstThunk + sizeof(DWORD)));  // Fill getchar() address
    oStreamPtr->seekp(exitAddrPos, std::ios_base::beg);
    write(static_cast<u32>(ioh.ImageBase + iids[0].FirstThunk + sizeof(DWORD) * 2));  // Fill exit() address
    oStreamPtr->seekp(ishText.PointerToRawData + 0x0d, std::ios_base::beg);
    write(ioh.ImageBase + ishBss.VirtualAddress);  // Fill .bss address

    oStreamPtr->seekp(0, std::ios_base::end);
  }

  void
  emitMovePointerImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (op1 > 0) {
      if (op1 > 127) {
        // add ebx, {op1}
        u8 opcode[] = {0x81, 0xc3};
        write(opcode);
        write(op1);
      } else if (op1 > 1) {
        // add ebx, {op1}
        u8 opcode[] = {0x83, 0xc3};
        write(opcode);
        write(static_cast<u8>(op1));
      } else {
        // inc ebx
        u8 opcode[] = {0x43};
        write(opcode);
      }
    } else {
      if (op1 < -127) {
        // sub ebx, {-op1}
        u8 opcode[] = {0x81, 0xeb};
        write(opcode);
        write(-op1);
      } else if (op1 < -1) {
        // sub ebx, {-op1}
        u8 opcode[] = {0x83, 0xeb};
        write(opcode);
        write(static_cast<u8>(-op1));
      } else {
        // dec ebx
        u8 opcode[] = {0x4b};
        write(opcode);
      }
    }
  }

  void
  emitAddImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    if (op1 > 0) {
      if (op1 > 1) {
        // add byte ptr [ebx], {op1}
        u8 opcode[] = {0x80, 0x03};
        write(opcode);
        write(static_cast<u8>(op1));
      } else {
        // inc byte ptr [ebx]
        u8 opcode[] = {0xfe, 0x03};
        write(opcode);
      }
    } else {
      if (op1 < -1) {
        // sub byte ptr [ebx], {op1}
        u8 opcode[] = {0x80, 0x2b};
        write(opcode);
        write(static_cast<u8>(-op1));
      } else {
        // dec byte ptr [ebx]
        u8 opcode[] = {0xfe, 0x0b};
        write(opcode);
      }
    }
  }

  void
  emitPutcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    // push byte ptr [ebx]
    u8 opcode1[] = {0xff, 0x33};
    write(opcode1);
    // call esi (putchar)
    u8 opcode2[] = {0xff, 0xd6};
    write(opcode2);
    // pop eax
    u8 opcode3[] = {0x58};
    write(opcode3);
  }

  void
  emitGetcharImpl() CODE_GENERATOR_NOEXCEPT
  {
    // call edi (getchar)
    u8 opcode1[] = {0xff, 0xd7};
    write(opcode1);
    // mov byte ptr [ebx], al
    u8 opcode2[] = {0x88, 0x03};
    write(opcode2);
  }

  void
  emitLoopStartImpl() CODE_GENERATOR_NOEXCEPT
  {
    loopStack.push(oStreamPtr->tellp());
    // cmp byte ptr [ebx], 0x00
    u8 opcode1[] = {0x80, 0x3b};
    write(opcode1);
    write(static_cast<u8>(0x00));
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
      write(static_cast<u32>(offset - sizeof(u32)));
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
    oStreamPtr->seekp(curPos, std::ios_base::beg);
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
    oStreamPtr->seekp(curPos, std::ios_base::beg);
    loopStack.pop();
  }

  void
  emitAssignImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov byte ptr [ebx], {op1}
    u8 opcode[] = {0xc6, 0x03};
    write(opcode);
    write(static_cast<u8>(op1));
  }

  void
  emitAddVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov al, byte ptr [ebx]
    u8 opcode1[] = {0x8a, 0x03};
    write(opcode1);
    // add byte ptr [ebx + {op1}], al
    if (op1 < -128 || 127 < op1) {
      u8 opcode2[] = {0x00, 0x83};
      write(opcode2);
      write(static_cast<u32>(op1));
    } else {
      u8 opcode2[] = {0x00, 0x43};
      write(opcode2);
      write(static_cast<u8>(op1));
    }
  }

  void
  emitSubVarImpl(int op1) CODE_GENERATOR_NOEXCEPT
  {
    // mov al, byte ptr [ebx]
    u8 opcode1[] = {0x8a, 0x03};
    write(opcode1);
    // sub byte ptr [rbx + {op1}], al
    if (op1 < -128 || 127 < op1) {
      u8 opcode2[] = {0x28, 0x83};
      write(opcode2);
      write(static_cast<u32>(op1));
    } else {
      u8 opcode2[] = {0x28, 0x43};
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
      // mul byte ptr [ebx]
      u8 opcode2[] = {0xf6, 0x23};
      write(opcode2);
      // add byte ptr [ebx + {op1}], al
      if (op1 < -128 || 127 < op1) {
        u8 opcode3[] = {0x00, 0x83};
        write(opcode3);
        write(static_cast<u32>(op1));
      } else {
        u8 opcode3[] = {0x00, 0x43};
        write(opcode3);
        write(static_cast<u8>(op1));
      }
    } else {
      // mov al, {-op2}
      u8 opcode1[] = {0xb0};
      write(opcode1);
      write(static_cast<u8>(-op2));
      // mul byte ptr [ebx]
      u8 opcode2[] = {0xf6, 0x23};
      write(opcode2);
      // sub byte ptr [ebx], al
      if (op1 < -128 || 127 < op1) {
        u8 opcode3[] = {0x28, 0x83};
        write(opcode3);
        write(static_cast<u32>(op1));
      } else {
        u8 opcode3[] = {0x28, 0x43};
        write(opcode3);
        write(static_cast<u8>(op1));
      }
    }
  }

  void
  emitInfLoopImpl() CODE_GENERATOR_NOEXCEPT
  {
    emitIfImpl();
    // jmp {offset} (near jump)
    u8 opcode[] = {0xeb};
    write(opcode);
    write(static_cast<u8>(-2));
    emitEndIfImpl();
  }
};  // class GeneratorWinX86


const DWORD GeneratorWinX86::kBaseAddr = 0x00400000;
const DWORD GeneratorWinX86::kPeHeaderSizeWithPadding = 0x0200;
const DWORD GeneratorWinX86::kIdataSizeWithPadding = 0x0200;


#endif  // GENERATOR_WIN_X86_HPP
