#ifndef WINSUBSET_H
#define WINSUBSET_H


#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#    define WINSUBSET_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#  endif
#  ifndef NOGDICAPMASKS
#    define NOGDICAPMASKS
#    define WINSUBSET_NOGDICAPMASKS_IS_NOT_DEFINED
#  endif
#  ifndef NOVIRTUALKEYCODES
#    define NOVIRTUALKEYCODES
#    define WINSUBSET_NOVIRTUALKEYCODES_IS_NOT_DEFINED
#  endif
#  ifndef NOWINMESSAGES
#    define NOWINMESSAGES
#    define WINSUBSET_NOWINMESSAGES_IS_NOT_DEFINED
#  endif
#  ifndef NOWINSTYLES
#    define NOWINSTYLES
#    define WINSUBSET_NOWINSTYLES_IS_NOT_DEFINED
#  endif
#  ifndef NOSYSMETRICS
#    define NOSYSMETRICS
#    define WINSUBSET_NOSYSMETRICS_IS_NOT_DEFINED
#  endif
#  ifndef NOMENUS
#    define NOMENUS
#    define WINSUBSET_NOMENUS_IS_NOT_DEFINED
#  endif
#  ifndef NOICONS
#    define NOICONS
#    define WINSUBSET_NOICONS_IS_NOT_DEFINED
#  endif
#  ifndef NOKEYSTATES
#    define NOKEYSTATES
#    define WINSUBSET_NOKEYSTATES_IS_NOT_DEFINED
#  endif
#  ifndef NOSYSCOMMANDS
#    define NOSYSCOMMANDS
#    define WINSUBSET_NOSYSCOMMANDS_IS_NOT_DEFINED
#  endif
#  ifndef NORASTEROPS
#    define NORASTEROPS
#    define WINSUBSET_NORASTEROPS_IS_NOT_DEFINED
#  endif
#  ifndef NOSHOWWINDOW
#    define NOSHOWWINDOW
#    define WINSUBSET_NOSHOWWINDOW_IS_NOT_DEFINED
#  endif
#  ifndef OEMRESOURCE
#    define OEMRESOURCE
#    define WINSUBSET_OEMRESOURCE_IS_NOT_DEFINED
#  endif
#  ifndef NOATOM
#    define NOATOM
#    define WINSUBSET_NOATOM_IS_NOT_DEFINED
#  endif
#  ifndef NOCLIPBOARD
#    define NOCLIPBOARD
#    define WINSUBSET_NOCLIPBOARD_IS_NOT_DEFINED
#  endif
#  ifndef NOCOLOR
#    define NOCOLOR
#    define WINSUBSET_NOCOLOR_IS_NOT_DEFINED
#  endif
#  ifndef NOCTLMGR
#    define NOCTLMGR
#    define WINSUBSET_NOCTLMGR_IS_NOT_DEFINED
#  endif
#  ifndef NODRAWTEXT
#    define NODRAWTEXT
#    define WINSUBSET_NODRAWTEXT_IS_NOT_DEFINED
#  endif
#  ifndef NOGDI
#    define NOGDI
#    define WINSUBSET_NOGDI_IS_NOT_DEFINED
#  endif
#  ifndef NOKERNEL
#    define NOKERNEL
#    define WINSUBSET_NOKERNEL_IS_NOT_DEFINED
#  endif
#  ifndef NOUSER
#    define NOUSER
#    define WINSUBSET_NOUSER_IS_NOT_DEFINED
#  endif
#  ifndef NONLS
#    define NONLS
#    define WINSUBSET_NONLS_IS_NOT_DEFINED
#  endif
#  ifndef NOMB
#    define NOMB
#    define WINSUBSET_NOMB_IS_NOT_DEFINED
#  endif
#  ifndef NOMEMMGR
#    define NOMEMMGR
#    define WINSUBSET_NOMEMMGR_IS_NOT_DEFINED
#  endif
#  ifndef NOMETAFILE
#    define NOMETAFILE
#    define WINSUBSET_NOMETAFILE_IS_NOT_DEFINED
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#    define WINSUBSET_NOMINMAX_IS_NOT_DEFINED
#  endif
#  ifndef NOMSG
#    define NOMSG
#    define WINSUBSET_NOMSG_IS_NOT_DEFINED
#  endif
#  ifndef NOOPENFILE
#    define NOOPENFILE
#    define WINSUBSET_NOOPENFILE_IS_NOT_DEFINED
#  endif
#  ifndef NOSCROLL
#    define NOSCROLL
#    define WINSUBSET_NOSCROLL_IS_NOT_DEFINED
#  endif
#  ifndef NOSERVICE
#    define NOSERVICE
#    define WINSUBSET_NOSERVICE_IS_NOT_DEFINED
#  endif
#  ifndef NOSOUND
#    define NOSOUND
#    define WINSUBSET_NOSOUND_IS_NOT_DEFINED
#  endif
#  ifndef NOTEXTMETRIC
#    define NOTEXTMETRIC
#    define WINSUBSET_NOTEXTMETRIC_IS_NOT_DEFINED
#  endif
#  ifndef NOWH
#    define NOWH
#    define WINSUBSET_NOWH_IS_NOT_DEFINED
#  endif
#  ifndef NOWINOFFSETS
#    define NOWINOFFSETS
#    define WINSUBSET_NOWINOFFSETS_IS_NOT_DEFINED
#  endif
#  ifndef NOCOMM
#    define NOCOMM
#    define WINSUBSET_NOCOMM_IS_NOT_DEFINED
#  endif
#  ifndef NOKANJI
#    define NOKANJI
#    define WINSUBSET_NOKANJI_IS_NOT_DEFINED
#  endif
#  ifndef NOHELP
#    define NOHELP
#    define WINSUBSET_NOHELP_IS_NOT_DEFINED
#  endif
#  ifndef NOPROFILER
#    define NOPROFILER
#    define WINSUBSET_NOPROFILER_IS_NOT_DEFINED
#  endif
#  ifndef NODEFERWINDOWPOS
#    define NODEFERWINDOWPOS
#    define WINSUBSET_NODEFERWINDOWPOS_IS_NOT_DEFINED
#  endif
#  ifndef NOMCX
#    define NOMCX
#    define WINSUBSET_NOMCX_IS_NOT_DEFINED
#  endif

#  include <windows.h>

#  ifdef WINSUBSET_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    undef WINSUBSET_WIN32_LEAN_AND_MEAN_IS_NOT_DEFINED
#    undef WIN32_LEAN_AND_MEAN
#  endif
#  ifdef WINSUBSET_NOGDICAPMASKS_IS_NOT_DEFINED
#    undef WINSUBSET_NOGDICAPMASKS_IS_NOT_DEFINED
#    undef NOGDICAPMASKS
#  endif
#  ifdef WINSUBSET_NOVIRTUALKEYCODES_IS_NOT_DEFINED
#    undef WINSUBSET_NOVIRTUALKEYCODES_IS_NOT_DEFINED
#    undef NOVIRTUALKEYCODES
#  endif
#  ifdef WINSUBSET_NOWINMESSAGES_IS_NOT_DEFINED
#    undef WINSUBSET_NOWINMESSAGES_IS_NOT_DEFINED
#    undef NOWINMESSAGES
#  endif
#  ifdef WINSUBSET_NOWINSTYLES_IS_NOT_DEFINED
#    undef WINSUBSET_NOWINSTYLES_IS_NOT_DEFINED
#    undef NOWINSTYLES
#  endif
#  ifdef WINSUBSET_NOSYSMETRICS_IS_NOT_DEFINED
#    undef WINSUBSET_NOSYSMETRICS_IS_NOT_DEFINED
#    undef NOSYSMETRICS
#  endif
#  ifdef WINSUBSET_NOMENUS_IS_NOT_DEFINED
#    undef WINSUBSET_NOMENUS_IS_NOT_DEFINED
#    undef NOMENUS
#  endif
#  ifdef WINSUBSET_NOICONS_IS_NOT_DEFINED
#    undef WINSUBSET_NOICONS_IS_NOT_DEFINED
#    undef NOICONS
#  endif
#  ifdef WINSUBSET_NOKEYSTATES_IS_NOT_DEFINED
#    undef WINSUBSET_NOKEYSTATES_IS_NOT_DEFINED
#    undef NOKEYSTATES
#  endif
#  ifdef WINSUBSET_NOSYSCOMMANDS_IS_NOT_DEFINED
#    undef WINSUBSET_NOSYSCOMMANDS_IS_NOT_DEFINED
#    undef NOSYSCOMMANDS
#  endif
#  ifdef WINSUBSET_NORASTEROPS_IS_NOT_DEFINED
#    undef WINSUBSET_NORASTEROPS_IS_NOT_DEFINED
#    undef NORASTEROPS
#  endif
#  ifdef WINSUBSET_NOSHOWWINDOW_IS_NOT_DEFINED
#    undef WINSUBSET_NOSHOWWINDOW_IS_NOT_DEFINED
#    undef NOSHOWWINDOW
#  endif
#  ifdef WINSUBSET_OEMRESOURCE_IS_NOT_DEFINED
#    undef WINSUBSET_OEMRESOURCE_IS_NOT_DEFINED
#    undef OEMRESOURCE
#  endif
#  ifdef WINSUBSET_NOATOM_IS_NOT_DEFINED
#    undef WINSUBSET_NOATOM_IS_NOT_DEFINED
#    undef NOATOM
#  endif
#  ifdef WINSUBSET_NOCLIPBOARD_IS_NOT_DEFINED
#    undef WINSUBSET_NOCLIPBOARD_IS_NOT_DEFINED
#    undef NOCLIPBOARD
#  endif
#  ifdef WINSUBSET_NOCOLOR_IS_NOT_DEFINED
#    undef WINSUBSET_NOCOLOR_IS_NOT_DEFINED
#    undef NOCOLOR
#  endif
#  ifdef WINSUBSET_NOCTLMGR_IS_NOT_DEFINED
#    undef WINSUBSET_NOCTLMGR_IS_NOT_DEFINED
#    undef NOCTLMGR
#  endif
#  ifdef WINSUBSET_NODRAWTEXT_IS_NOT_DEFINED
#    undef WINSUBSET_NODRAWTEXT_IS_NOT_DEFINED
#    undef NODRAWTEXT
#  endif
#  ifdef WINSUBSET_NOGDI_IS_NOT_DEFINED
#    undef WINSUBSET_NOGDI_IS_NOT_DEFINED
#    undef NOGDI
#  endif
#  ifdef WINSUBSET_NOKERNEL_IS_NOT_DEFINED
#    undef WINSUBSET_NOKERNEL_IS_NOT_DEFINED
#    undef NOKERNEL
#  endif
#  ifdef WINSUBSET_NOUSER_IS_NOT_DEFINED
#    undef WINSUBSET_NOUSER_IS_NOT_DEFINED
#    undef NOUSER
#  endif
#  ifdef WINSUBSET_NONLS_IS_NOT_DEFINED
#    undef WINSUBSET_NONLS_IS_NOT_DEFINED
#    undef NONLS
#  endif
#  ifdef WINSUBSET_NOMB_IS_NOT_DEFINED
#    undef WINSUBSET_NOMB_IS_NOT_DEFINED
#    undef NOMB
#  endif
#  ifdef WINSUBSET_NOMEMMGR_IS_NOT_DEFINED
#    undef WINSUBSET_NOMEMMGR_IS_NOT_DEFINED
#    undef NOMEMMGR
#  endif
#  ifdef WINSUBSET_NOMETAFILE_IS_NOT_DEFINED
#    undef WINSUBSET_NOMETAFILE_IS_NOT_DEFINED
#    undef NOMETAFILE
#  endif
#  ifdef WINSUBSET_NOMINMAX_IS_NOT_DEFINED
#    undef WINSUBSET_NOMINMAX_IS_NOT_DEFINED
#    undef NOMINMAX
#  endif
#  ifdef WINSUBSET_NOMSG_IS_NOT_DEFINED
#    undef WINSUBSET_NOMSG_IS_NOT_DEFINED
#    undef NOMSG
#  endif
#  ifdef WINSUBSET_NOOPENFILE_IS_NOT_DEFINED
#    undef WINSUBSET_NOOPENFILE_IS_NOT_DEFINED
#    undef NOOPENFILE
#  endif
#  ifdef WINSUBSET_NOSCROLL_IS_NOT_DEFINED
#    undef WINSUBSET_NOSCROLL_IS_NOT_DEFINED
#    undef NOSCROLL
#  endif
#  ifdef WINSUBSET_NOSERVICE_IS_NOT_DEFINED
#    undef WINSUBSET_NOSERVICE_IS_NOT_DEFINED
#    undef NOSERVICE
#  endif
#  ifdef WINSUBSET_NOSOUND_IS_NOT_DEFINED
#    undef WINSUBSET_NOSOUND_IS_NOT_DEFINED
#    undef NOSOUND
#  endif
#  ifdef WINSUBSET_NOTEXTMETRIC_IS_NOT_DEFINED
#    undef WINSUBSET_NOTEXTMETRIC_IS_NOT_DEFINED
#    undef NOTEXTMETRIC
#  endif
#  ifdef WINSUBSET_NOWH_IS_NOT_DEFINED
#    undef WINSUBSET_NOWH_IS_NOT_DEFINED
#    undef NOWH
#  endif
#  ifdef WINSUBSET_NOWINOFFSETS_IS_NOT_DEFINED
#    undef WINSUBSET_NOWINOFFSETS_IS_NOT_DEFINED
#    undef NOWINOFFSETS
#  endif
#  ifdef WINSUBSET_NOCOMM_IS_NOT_DEFINED
#    undef WINSUBSET_NOCOMM_IS_NOT_DEFINED
#    undef NOCOMM
#  endif
#  ifdef WINSUBSET_NOKANJI_IS_NOT_DEFINED
#    undef WINSUBSET_NOKANJI_IS_NOT_DEFINED
#    undef NOKANJI
#  endif
#  ifdef WINSUBSET_NOHELP_IS_NOT_DEFINED
#    undef WINSUBSET_NOHELP_IS_NOT_DEFINED
#    undef NOHELP
#  endif
#  ifdef WINSUBSET_NOPROFILER_IS_NOT_DEFINED
#    undef WINSUBSET_NOPROFILER_IS_NOT_DEFINED
#    undef NOPROFILER
#  endif
#  ifdef WINSUBSET_NODEFERWINDOWPOS_IS_NOT_DEFINED
#    undef WINSUBSET_NODEFERWINDOWPOS_IS_NOT_DEFINED
#    undef NODEFERWINDOWPOS
#  endif
#  ifdef WINSUBSET_NOMCX_IS_NOT_DEFINED
#    undef WINSUBSET_NOMCX_IS_NOT_DEFINED
#    undef NOMCX
#  endif
#else
// Clone from winnt.h in MSVC 2012
#  if __cplusplus >= 201103L
#    include <cstdint>
#  else
#    include <stdint.h>
#  endif
// Clone from WinDef.h in MSVC 2012
typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef uint64_t  ULONGLONG;
typedef int32_t   LONG;


#ifndef _MAC
#  define IMAGE_DOS_SIGNATURE     0x5A4D      // MZ
#  define IMAGE_OS2_SIGNATURE     0x454E      // NE
#  define IMAGE_OS2_SIGNATURE_LE  0x454C      // LE
#  define IMAGE_VXD_SIGNATURE     0x454C      // LE
#  define IMAGE_NT_SIGNATURE      0x00004550  // PE00
#else
#  define IMAGE_DOS_SIGNATURE     0x4D5A      // MZ
#  define IMAGE_OS2_SIGNATURE     0x4E45      // NE
#  define IMAGE_OS2_SIGNATURE_LE  0x4C45      // LE
#  define IMAGE_NT_SIGNATURE      0x50450000  // PE00
#endif

typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
  WORD e_magic;                     // Magic number
  WORD e_cblp;                      // Bytes on last page of file
  WORD e_cp;                        // Pages in file
  WORD e_crlc;                      // Relocations
  WORD e_cparhdr;                   // Size of header in paragraphs
  WORD e_minalloc;                  // Minimum extra paragraphs needed
  WORD e_maxalloc;                  // Maximum extra paragraphs needed
  WORD e_ss;                        // Initial (relative) SS value
  WORD e_sp;                        // Initial SP value
  WORD e_csum;                      // Checksum
  WORD e_ip;                        // Initial IP value
  WORD e_cs;                        // Initial (relative) CS value
  WORD e_lfarlc;                    // File address of relocation table
  WORD e_ovno;                      // Overlay number
  WORD e_res[4];                    // Reserved words
  WORD e_oemid;                     // OEM identifier (for e_oeminfo)
  WORD e_oeminfo;                   // OEM information; e_oemid specific
  WORD e_res2[10];                  // Reserved words
  LONG e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;


//
// File header format.
//
typedef struct _IMAGE_FILE_HEADER {
  WORD  Machine;
  WORD  NumberOfSections;
  DWORD TimeDateStamp;
  DWORD PointerToSymbolTable;
  DWORD NumberOfSymbols;
  WORD  SizeOfOptionalHeader;
  WORD  Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

#define IMAGE_SIZEOF_FILE_HEADER             20

#define IMAGE_FILE_RELOCS_STRIPPED           0x0001  // Relocation info stripped from file.
#define IMAGE_FILE_EXECUTABLE_IMAGE          0x0002  // File is executable  (i.e. no unresolved externel references).
#define IMAGE_FILE_LINE_NUMS_STRIPPED        0x0004  // Line nunbers stripped from file.
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED       0x0008  // Local symbols stripped from file.
#define IMAGE_FILE_AGGRESIVE_WS_TRIM         0x0010  // Agressively trim working set
#define IMAGE_FILE_LARGE_ADDRESS_AWARE       0x0020  // App can handle >2gb addresses
#define IMAGE_FILE_BYTES_REVERSED_LO         0x0080  // Bytes of machine word are reversed.
#define IMAGE_FILE_32BIT_MACHINE             0x0100  // 32 bit word machine.
#define IMAGE_FILE_DEBUG_STRIPPED            0x0200  // Debugging info stripped from file in .DBG file
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP   0x0400  // If Image is on removable media, copy and run from the swap file.
#define IMAGE_FILE_NET_RUN_FROM_SWAP         0x0800  // If Image is on Net, copy and run from the swap file.
#define IMAGE_FILE_SYSTEM                    0x1000  // System File.
#define IMAGE_FILE_DLL                       0x2000  // File is a DLL.
#define IMAGE_FILE_UP_SYSTEM_ONLY            0x4000  // File should only be run on a UP machine
#define IMAGE_FILE_BYTES_REVERSED_HI         0x8000  // Bytes of machine word are reversed.

#define IMAGE_FILE_MACHINE_UNKNOWN           0
#define IMAGE_FILE_MACHINE_I386              0x014c  // Intel 386.
#define IMAGE_FILE_MACHINE_R3000             0x0162  // MIPS little-endian, 0x160 big-endian
#define IMAGE_FILE_MACHINE_R4000             0x0166  // MIPS little-endian
#define IMAGE_FILE_MACHINE_R10000            0x0168  // MIPS little-endian
#define IMAGE_FILE_MACHINE_WCEMIPSV2         0x0169  // MIPS little-endian WCE v2
#define IMAGE_FILE_MACHINE_ALPHA             0x0184  // Alpha_AXP
#define IMAGE_FILE_MACHINE_SH3               0x01a2  // SH3 little-endian
#define IMAGE_FILE_MACHINE_SH3DSP            0x01a3
#define IMAGE_FILE_MACHINE_SH3E              0x01a4  // SH3E little-endian
#define IMAGE_FILE_MACHINE_SH4               0x01a6  // SH4 little-endian
#define IMAGE_FILE_MACHINE_SH5               0x01a8  // SH5
#define IMAGE_FILE_MACHINE_ARM               0x01c0  // ARM Little-Endian
#define IMAGE_FILE_MACHINE_THUMB             0x01c2
#define IMAGE_FILE_MACHINE_AM33              0x01d3
#define IMAGE_FILE_MACHINE_POWERPC           0x01F0  // IBM PowerPC Little-Endian
#define IMAGE_FILE_MACHINE_POWERPCFP         0x01f1
#define IMAGE_FILE_MACHINE_IA64              0x0200  // Intel 64
#define IMAGE_FILE_MACHINE_MIPS16            0x0266  // MIPS
#define IMAGE_FILE_MACHINE_ALPHA64           0x0284  // ALPHA64
#define IMAGE_FILE_MACHINE_MIPSFPU           0x0366  // MIPS
#define IMAGE_FILE_MACHINE_MIPSFPU16         0x0466  // MIPS
#define IMAGE_FILE_MACHINE_AXP64             IMAGE_FILE_MACHINE_ALPHA64
#define IMAGE_FILE_MACHINE_TRICORE           0x0520  // Infineon
#define IMAGE_FILE_MACHINE_CEF               0x0CEF
#define IMAGE_FILE_MACHINE_EBC               0x0EBC  // EFI Byte Code
#define IMAGE_FILE_MACHINE_AMD64             0x8664  // AMD64 (K8)
#define IMAGE_FILE_MACHINE_M32R              0x9041  // M32R little-endian
#define IMAGE_FILE_MACHINE_CEE               0xC0EE


//
// Directory format.
//
typedef struct _IMAGE_DATA_DIRECTORY {
  DWORD VirtualAddress;
  DWORD Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16


//
// Optional header format.
//
typedef struct _IMAGE_OPTIONAL_HEADER {
  //
  // Standard fields.
  //
  WORD  Magic;
  BYTE  MajorLinkerVersion;
  BYTE  MinorLinkerVersion;
  DWORD SizeOfCode;
  DWORD SizeOfInitializedData;
  DWORD SizeOfUninitializedData;
  DWORD AddressOfEntryPoint;
  DWORD BaseOfCode;
  DWORD BaseOfData;

  //
  // NT additional fields.
  //
  DWORD ImageBase;
  DWORD SectionAlignment;
  DWORD FileAlignment;
  WORD  MajorOperatingSystemVersion;
  WORD  MinorOperatingSystemVersion;
  WORD  MajorImageVersion;
  WORD  MinorImageVersion;
  WORD  MajorSubsystemVersion;
  WORD  MinorSubsystemVersion;
  DWORD Win32VersionValue;
  DWORD SizeOfImage;
  DWORD SizeOfHeaders;
  DWORD CheckSum;
  WORD  Subsystem;
  WORD  DllCharacteristics;
  DWORD SizeOfStackReserve;
  DWORD SizeOfStackCommit;
  DWORD SizeOfHeapReserve;
  DWORD SizeOfHeapCommit;
  DWORD LoaderFlags;
  DWORD NumberOfRvaAndSizes;
  IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
    WORD        Magic;
    BYTE        MajorLinkerVersion;
    BYTE        MinorLinkerVersion;
    DWORD       SizeOfCode;
    DWORD       SizeOfInitializedData;
    DWORD       SizeOfUninitializedData;
    DWORD       AddressOfEntryPoint;
    DWORD       BaseOfCode;
    ULONGLONG   ImageBase;
    DWORD       SectionAlignment;
    DWORD       FileAlignment;
    WORD        MajorOperatingSystemVersion;
    WORD        MinorOperatingSystemVersion;
    WORD        MajorImageVersion;
    WORD        MinorImageVersion;
    WORD        MajorSubsystemVersion;
    WORD        MinorSubsystemVersion;
    DWORD       Win32VersionValue;
    DWORD       SizeOfImage;
    DWORD       SizeOfHeaders;
    DWORD       CheckSum;
    WORD        Subsystem;
    WORD        DllCharacteristics;
    ULONGLONG   SizeOfStackReserve;
    ULONGLONG   SizeOfStackCommit;
    ULONGLONG   SizeOfHeapReserve;
    ULONGLONG   SizeOfHeapCommit;
    DWORD       LoaderFlags;
    DWORD       NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

#define IMAGE_NT_OPTIONAL_HDR32_MAGIC  0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC  0x20b
#define IMAGE_ROM_OPTIONAL_HDR_MAGIC   0x107

// Subsystem Values
#define IMAGE_SUBSYSTEM_UNKNOWN              0   // Unknown subsystem.
#define IMAGE_SUBSYSTEM_NATIVE               1   // Image doesn't require a subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_GUI          2   // Image runs in the Windows GUI subsystem.
#define IMAGE_SUBSYSTEM_WINDOWS_CUI          3   // Image runs in the Windows character subsystem.
#define IMAGE_SUBSYSTEM_OS2_CUI              5   // image runs in the OS/2 character subsystem.
#define IMAGE_SUBSYSTEM_POSIX_CUI            7   // image runs in the Posix character subsystem.
#define IMAGE_SUBSYSTEM_NATIVE_WINDOWS       8   // image is a native Win9x driver.
#define IMAGE_SUBSYSTEM_WINDOWS_CE_GUI       9   // Image runs in the Windows CE subsystem.
#define IMAGE_SUBSYSTEM_EFI_APPLICATION      10  //
#define IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER  11   //
#define IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER   12  //
#define IMAGE_SUBSYSTEM_EFI_ROM              13
#define IMAGE_SUBSYSTEM_XBOX                 14
#define IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16


//
// Section header format.
//
#define IMAGE_SIZEOF_SHORT_NAME  8

typedef struct _IMAGE_SECTION_HEADER {
  BYTE Name[IMAGE_SIZEOF_SHORT_NAME];
  union {
    DWORD PhysicalAddress;
    DWORD VirtualSize;
  } Misc;
  DWORD VirtualAddress;
  DWORD SizeOfRawData;
  DWORD PointerToRawData;
  DWORD PointerToRelocations;
  DWORD PointerToLinenumbers;
  WORD  NumberOfRelocations;
  WORD  NumberOfLinenumbers;
  DWORD Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;


#define IMAGE_SIZEOF_SECTION_HEADER  40

//
// Section characteristics.
//
//      IMAGE_SCN_TYPE_REG                   0x00000000  // Reserved.
//      IMAGE_SCN_TYPE_DSECT                 0x00000001  // Reserved.
//      IMAGE_SCN_TYPE_NOLOAD                0x00000002  // Reserved.
//      IMAGE_SCN_TYPE_GROUP                 0x00000004  // Reserved.
#define IMAGE_SCN_TYPE_NO_PAD                0x00000008  // Reserved.
//      IMAGE_SCN_TYPE_COPY                  0x00000010  // Reserved.

#define IMAGE_SCN_CNT_CODE                   0x00000020  // Section contains code.
#define IMAGE_SCN_CNT_INITIALIZED_DATA       0x00000040  // Section contains initialized data.
#define IMAGE_SCN_CNT_UNINITIALIZED_DATA     0x00000080  // Section contains uninitialized data.

#define IMAGE_SCN_LNK_OTHER                  0x00000100  // Reserved.
#define IMAGE_SCN_LNK_INFO                   0x00000200  // Section contains comments or some other type of information.
//      IMAGE_SCN_TYPE_OVER                  0x00000400  // Reserved.
#define IMAGE_SCN_LNK_REMOVE                 0x00000800  // Section contents will not become part of image.
#define IMAGE_SCN_LNK_COMDAT                 0x00001000  // Section contents comdat.
//                                           0x00002000  // Reserved.
//      IMAGE_SCN_MEM_PROTECTED - Obsolete   0x00004000
#define IMAGE_SCN_NO_DEFER_SPEC_EXC          0x00004000  // Reset speculative exceptions handling bits in the TLB entries for this section.
#define IMAGE_SCN_GPREL                      0x00008000  // Section content can be accessed relative to GP
#define IMAGE_SCN_MEM_FARDATA                0x00008000
//      IMAGE_SCN_MEM_SYSHEAP  - Obsolete    0x00010000
#define IMAGE_SCN_MEM_PURGEABLE              0x00020000
#define IMAGE_SCN_MEM_16BIT                  0x00020000
#define IMAGE_SCN_MEM_LOCKED                 0x00040000
#define IMAGE_SCN_MEM_PRELOAD                0x00080000

#define IMAGE_SCN_ALIGN_1BYTES               0x00100000  //
#define IMAGE_SCN_ALIGN_2BYTES               0x00200000  //
#define IMAGE_SCN_ALIGN_4BYTES               0x00300000  //
#define IMAGE_SCN_ALIGN_8BYTES               0x00400000  //
#define IMAGE_SCN_ALIGN_16BYTES              0x00500000  // Default alignment if no others are specified.
#define IMAGE_SCN_ALIGN_32BYTES              0x00600000  //
#define IMAGE_SCN_ALIGN_64BYTES              0x00700000  //
#define IMAGE_SCN_ALIGN_128BYTES             0x00800000  //
#define IMAGE_SCN_ALIGN_256BYTES             0x00900000  //
#define IMAGE_SCN_ALIGN_512BYTES             0x00A00000  //
#define IMAGE_SCN_ALIGN_1024BYTES            0x00B00000  //
#define IMAGE_SCN_ALIGN_2048BYTES            0x00C00000  //
#define IMAGE_SCN_ALIGN_4096BYTES            0x00D00000  //
#define IMAGE_SCN_ALIGN_8192BYTES            0x00E00000  //
// Unused                                    0x00F00000
#define IMAGE_SCN_ALIGN_MASK                 0x00F00000

#define IMAGE_SCN_LNK_NRELOC_OVFL            0x01000000  // Section contains extended relocations.
#define IMAGE_SCN_MEM_DISCARDABLE            0x02000000  // Section can be discarded.
#define IMAGE_SCN_MEM_NOT_CACHED             0x04000000  // Section is not cachable.
#define IMAGE_SCN_MEM_NOT_PAGED              0x08000000  // Section is not pageable.
#define IMAGE_SCN_MEM_SHARED                 0x10000000  // Section is shareable.
#define IMAGE_SCN_MEM_EXECUTE                0x20000000  // Section is executable.
#define IMAGE_SCN_MEM_READ                   0x40000000  // Section is readable.
#define IMAGE_SCN_MEM_WRITE                  0x80000000  // Section is writeable.


typedef struct _IMAGE_IMPORT_DESCRIPTOR {
  union {
    DWORD Characteristics;     // 0 for terminating null import descriptor
    DWORD OriginalFirstThunk;  // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
  };
  DWORD   TimeDateStamp;       // 0 if not bound,
                               // -1 if bound, and real date\time stamp
                               //     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
                               // O.W. date/time stamp of DLL bound to (Old BIND)
  DWORD   ForwarderChain;      // -1 if no forwarders
  DWORD   Name;
  DWORD   FirstThunk;          // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
typedef IMAGE_IMPORT_DESCRIPTOR *PIMAGE_IMPORT_DESCRIPTOR;


typedef struct _IMAGE_THUNK_DATA64 {
  union {
    ULONGLONG ForwarderString;  // PBYTE
    ULONGLONG Function;         // PDWORD
    ULONGLONG Ordinal;
    ULONGLONG AddressOfData;    // PIMAGE_IMPORT_BY_NAME
  } u1;
} IMAGE_THUNK_DATA64;
typedef IMAGE_THUNK_DATA64 * PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_THUNK_DATA32 {
  union {
    DWORD ForwarderString;      // PBYTE
    DWORD Function;             // PDWORD
    DWORD Ordinal;
    DWORD AddressOfData;        // PIMAGE_IMPORT_BY_NAME
  } u1;
} IMAGE_THUNK_DATA32;
typedef IMAGE_THUNK_DATA32 * PIMAGE_THUNK_DATA32;


#endif  // defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

#endif  // WINSUBSET_H
