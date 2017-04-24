XBYAK_DIR = xbyak

!if "$(DEBUG)" == "true"
DBG_SUFFIX = d
!else
DBG_SUFFIX =
!endif

!if "$(CRTDLL)" == "true"
CRTLIB = /MD$(DBG_SUFFIX)
!else
CRTLIB = /MT$(DBG_SUFFIX)
!endif

!if "$(DEBUG)" == "true"
COPTFLAGS   = /Od /GS /Zi $(CRTLIB)
LDOPTFLAGS  = /Od /GS /Zi $(CRTLIB)
MSVC_MACROS = /D_CRTDBG_MAP_ALLOC /D_USE_MATH_DEFINES /D_SECURE_SCL=1
!else
COPTFLAGS   = /Ox /GL $(CRTLIB)
LDOPTFLAGS  = /Ox /GL $(CRTLIB)
MSVC_MACROS = /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS /D_SECURE_SCL=0 /D_USE_MATH_DEFINES
!endif

CPP      = cl
GIT      = git
ECHO     = echo
MKDIR    = mkdir
RM       = del /F
CTAGS    = ctags
MAKE     = $(MAKE) /nologo
MAKEFILE = msvc.mk
GIT      = git
INCS     = /I$(XBYAK_DIR)
MACROS   = /DXBYAK_NO_OP_NAMES $(MSVC_MACROS)
CPPFLAGS = /nologo $(COPTFLAGS) /EHsc /W4 /c $(INCS) $(MACROS)
LDFLAGS  = /nologo $(LDOPTFLAGS)
LDLIBS   = /link $(GETOPT_LDLIBS)
DOXYGEN      = doxygen
DOXYFILE     = Doxyfile
DOXYGENDISTS = doxygen_sqlite3.db html\\ latex\\
GIT_HEAD_PATH = .git/HEAD

TARGET    = brainfuck.exe
MAIN_SRC  = main.cpp
MAIN_OBJ  = $(MAIN_SRC:.cpp=.obj)
VERSION_H = version.h
HEADERS   = BfInst.h \
    ArgumentParser.hpp \
    Brainfuck.hpp \
    CodeGenerator/CodeGenerator.hpp \
    CodeGenerator/SourceGenerator.hpp \
    CodeGenerator/BinaryGenerator.hpp \
    CodeGenerator/GeneratorC.hpp \
    CodeGenerator/GeneratorElfArmeabi.hpp \
    CodeGenerator/GeneratorElfX64.hpp \
    CodeGenerator/GeneratorElfX86.hpp \
    CodeGenerator/GeneratorWinX64.hpp \
    CodeGenerator/GeneratorWinX86.hpp \
    CodeGenerator/util/elfsubset.h \
    CodeGenerator/util/winsubset.h


.SUFFIXES: .cpp .obj .exe
.obj.exe:
	$(CPP) $(LDFLAGS) $** /Fe$@ $(LDLIBS)
.cpp.obj:
	$(CPP) $(CPPFLAGS) $** /Fo$@


# .PHONY: all test ctags doxygen clean distclean

all: $(TARGET)

$(TARGET): $(MAIN_OBJ)
	$(CPP) $(LDFLAGS) $** /Fe$@ $(LDLIBS)

$(MAIN_OBJ): $(VERSION_H) $(MAIN_SRC) $(HEADERS)

$(VERSION_H): $(GIT_HEAD_PATH)
	@$(ECHO) static const char kUsername[] = "%USERNAME%"; > $@
	@for /F "usebackq" %t in (`$(GIT) rev-parse HEAD`) do $(ECHO) static const char kVersion[] = "%t"; >> $@

$(GIT_HEAD_PATH):

$(XBYAK_DIR):
	$(GIT) submodule update --init

test:
	$(TARGET) -h

ctags:
	$(CTAGS) $(CTAGSFLAGS)

doxygen: $(DOXYFILE)
	$(DOXYGEN) $<

$(DOXYFILE):
	$(DOXYGEN) -g

clean:
	$(RM) $(MAIN_OBJ)
	$(RM) $(DOXYGENDISTS)

distclean:
	$(RM) $(TARGET) $(VERSION_H) $(MAIN_OBJ)
	$(RM) $(DOXYFILE) $(DOXYGENDISTS)
