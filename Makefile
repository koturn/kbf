XBYAK_DIR := xbyak/

ifeq ($(DEBUG),true)
    OPT_CFLAGS   := -O0 -g3 -ftrapv -fstack-protector-all -D_FORTIFY_SOURCE=2
    OPT_LDLIBS   := -lssp
ifneq ($(shell echo $$OSTYPE),cygwin)
    OPT_CFLAGS   += -fsanitize=address -fno-omit-frame-pointer
    OPT_LDLIBS   += -fsanitize=address
endif
    OPT_CXXFLAGS := $(OPT_CFLAGS) -D_GLIBCXX_DEBUG
else
ifeq ($(OPT),true)
    OPT_CFLAGS   := -flto -Ofast -march=native -DNDEBUG
    OPT_CXXFLAGS := $(OPT_CFLAGS)
    OPT_LDFLAGS  := -flto -s
else
ifeq ($(LTO),true)
    OPT_CFLAGS   := -flto -DNDEBUG
    OPT_CXXFLAGS := $(OPT_CFLAGS)
    OPT_LDFLAGS  := -flto
else
    OPT_CFLAGS   := -O3 -DNDEBUG
    OPT_CXXFLAGS := $(OPT_CFLAGS)
    OPT_LDFLAGS  := -s
endif
endif
endif

WARNING_COMMON_FLAGS := \
    -Wall \
    -Wextra \
    -Wabi \
    -Wcast-align \
    -Wcast-qual \
    -Wconversion \
    -Wdisabled-optimization \
    -Wdouble-promotion \
    -Wfloat-equal \
    -Wformat=2 \
    -Winit-self \
    -Wlogical-op \
    -Wmissing-declarations \
    -Wno-return-local-addr \
    -Wpointer-arith \
    -Wredundant-decls \
    -Wstrict-aliasing=2 \
    -Wsuggest-attribute=const \
    -Wsuggest-attribute=format \
    -Wsuggest-attribute=noreturn \
    -Wsuggest-attribute=pure \
    -Wsuggest-final-methods \
    -Wsuggest-final-types \
    -Wundef \
    -Wunsafe-loop-optimizations \
    -Wunreachable-code \
    -Wvector-operation-performance \
    -Wwrite-strings \
    -pedantic

WARNING_CFLAGS := \
    $(WARNING_COMMON_FLAGS) \
    -Wc++-compat \
    -Wbad-function-cast \
    -Wjump-misses-init \
    -Wmissing-prototypes \
    -Wunsuffixed-float-constants

WARNING_CXXFLAGS := \
    $(WARNING_COMMON_FLAGS) \
    -Wc++11-compat \
    -Wc++14-compat \
    -Weffc++ \
    -Woverloaded-virtual \
    -Wsign-promo \
    -Wstrict-null-sentinel \
    -Wsuggest-override \
    -Wuseless-cast \
    -Wzero-as-null-pointer-constant

CC            := gcc $(if $(STDC),$(addprefix -std=,$(STDC)),-std=gnu11)
CXX           := g++ $(if $(STDCXX),$(addprefix -std=,$(STDCXX)),-std=gnu++11)
GIT           := git
ECHO          := echo
MKDIR         := mkdir -p
CP            := cp
RM            := rm -rf
CTAGS         := ctags
MACROS        := XBYAK_NO_OP_NAMES
INCDIRS       := xbyak/
CPPFLAGS      := $(addprefix -D,$(MACROS)) $(addprefix -I,$(INCDIRS))
CFLAGS        := -pipe $(WARNING_CFLAGS) $(OPT_CFLAGS)
CXXFLAGS      := -pipe $(WARNING_CXXFLAGS) $(OPT_CXXFLAGS)
LDFLAGS       := -pipe $(OPT_LDFLAGS)
LDLIBS        := $(OPT_LDLIBS)
CTAGSFLAGS    := -R --languages=c,c++
DOXYGEN       := doxygen
DOXYFILE      := Doxyfile
DOXYGENDISTS  := doxygen_sqlite3.db html/ latex/
TARGET        := brainfuck
SRCS          := $(wildcard *.cpp)
VERSION_H     := version.h
OBJS          := $(foreach PAT,%.cpp %.cxx %.cc,$(patsubst $(PAT),%.o,$(filter $(PAT),$(SRCS))))
DEPENDS       := depends.mk
GIT_HEAD_PATH := .git/HEAD
USERNAME      := $(shell $(ECHO) $$USER)
COMMIT_HASH   := $(shell $(GIT) rev-parse HEAD)
CMD_RESULT    := $(shell [ -f $(VERSION_H) ] && : \
    || ($(ECHO) "static const char kUsername[] = \"$(USERNAME)\";" > $(VERSION_H) \
        && [ -f $(GIT_HEAD_PATH) ] \
        && $(ECHO) "static const char kVersion[] = \"$(COMMIT_HASH)\";" >> $(VERSION_H) \
        || $(ECHO) 'static const char kVersion[] = "";') >> $(VERSION_H))

ifeq ($(OS),Windows_NT)
    TARGET := $(addsuffix .exe,$(TARGET))
else
    TARGET := $(addsuffix .out,$(TARGET))
endif
INSTALLED_TARGET := $(if $(PREFIX),$(PREFIX),/usr/local)/bin/$(TARGET)

%.exe:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.cxx %.cc %.o,$^) $(LDLIBS) -o $@
%.out:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.cxx %.cc %.o,$^) $(LDLIBS) -o $@


.PHONY: all test depends syntax ctags doxygen install uninstall clean disclean
all: $(TARGET)
$(TARGET): $(XBYAK_DIR) $(VERSION_H) $(OBJS)

$(foreach SRC,$(SRCS),$(eval $(filter-out \,$(shell $(CXX) -MM $(SRC)))))

$(VERSION_H): $(GIT_HEAD_PATH)
	$(ECHO) "static const char kUsername[] = \"$(USERNAME)\";" > $@ \
		&& [ -f $< ] && $(ECHO) "static const char kVersion[] = \"$(COMMIT_HASH)\";" >> $@ \
		|| $(ECHO) 'static const char kVersion[] = "";' >> $@

$(GIT_HEAD_PATH):

$(XBYAK_DIR):
	$(GIT) submodule update --init

test: $(TARGET)
	$(MAKE) -C t/

depends:
	$(CXX) -MM $(SRCS) > $(DEPENDS)

syntax:
	$(CXX) $(SRCS) -fsyntax-only $(CPPFLAGS) $(WARNING_CXXFLAGS)

ctags:
	$(CTAGS) $(CTAGSFLAGS)

doxygen: $(DOXYFILE)
	$(DOXYGEN) $<

$(DOXYFILE):
	$(DOXYGEN) -g

install: $(INSTALLED_TARGET)
$(INSTALLED_TARGET): $(TARGET)
	@[ ! -d $(@D) ] && $(MKDIR) $(@D) || :
	$(CP) $< $@

uninstall:
	$(RM) $(INSTALLED_TARGET)

clean:
	$(RM) $(OBJS) $(DOXYGENDISTS)

distclean:
	$(RM) $(TARGET) $(VERSION_H) $(OBJS) $(DOXYFILE) $(DOXYGENDISTS)
