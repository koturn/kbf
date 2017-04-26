ifeq ($(OS),Windows_NT)
    BIN_SUFFIX := .exe
    ifeq ($(shell echo $$PROCESSOR_ARCHITECTURE),x86)
        ifeq ($(shell echo $$PROCESSOR_ARCHITEW6432),AMD64)
            BINTYPE := winx64
            TARGET_ARCHS := $(BINTYPE) winx86
        else
            BINTYPE := winx86
            TARGET_ARCHS := $(BINTYPE)
        endif
    else
        BINTYPE := winx64
        TARGET_ARCHS := $(BINTYPE) winx86
    endif
else
    BIN_SUFFIX := .out
    ifeq ($(shell getconf LONG_BIT),64)
        BINTYPE := elfx64
        TARGET_ARCHS := $(BINTYPE)
    else
        BINTYPE := elfx86
        TARGET_ARCHS := $(BINTYPE)
    endif
endif

BRAINFUCK := $(addsuffix $(BIN_SUFFIX),../brainfuck)
TESTS := $(basename $(sort $(wildcard *.b)))
OPT_LEVELS := 0 1 2
INPUTS_DIR := inputs
OUTPUTS_DIR := outputs
EXPECTS_DIR := expects
MAKE := make
MKDIR := mkdir
ECHO := echo
DIFF := diff -Z --strip-trailing-cr
RM := rm -f

CC := gcc
CFLAGS := -Wall -Wextra -O2
CHMOD := chmod
MODE := 755


define generate-interpreter-test
interpreter$1: $(foreach TEST,$(TESTS),interpreter$1-$(TEST))

$(foreach TEST,$(TESTS),$(eval $(call generate-interpreter-test-child,$(TEST),$1)))
endef

define generate-interpreter-test-child
interpreter$2-$1:
	@$(ECHO) -n "Interpreter$2 test: $1.b ... "
	@([ -f $(INPUTS_DIR)/$1.txt ] \
		&& $(BRAINFUCK) -O$2 $1.b < $(INPUTS_DIR)/$1.txt || $(BRAINFUCK) -O$2 $1.b) \
		| $(DIFF) - $(EXPECTS_DIR)/$1.txt > /dev/null
	@$(ECHO) 'Success'
endef


define generate-compile-test
compile-$1: $(foreach TEST,$(TESTS),compile-$1-$(TEST))

$(foreach TEST,$(TESTS),$(eval $(call generate-compile-test-child,$(TEST),$1)))
endef

define generate-compile-test-child
compile-$2-$1: compile-$2-$1-prev $(OUTPUTS_DIR)/$2/$1$(BIN_SUFFIX)
	@([ -f $(INPUTS_DIR)/$1.txt ] \
		&& $(OUTPUTS_DIR)/$2/$1$(BIN_SUFFIX) < $(INPUTS_DIR)/$1.txt || $(OUTPUTS_DIR)/$2/$1$(BIN_SUFFIX)) \
		| $(DIFF) - $(EXPECTS_DIR)/$1.txt > /dev/null
	@$(ECHO) 'Success'

compile-$2-$1-prev:
	@$(ECHO) -n "Compile to $2 test: $1.b ... "

$(OUTPUTS_DIR)/$2/$1$(BIN_SUFFIX):
	@[ ! -d $$(@D) ] && $(MKDIR) -p $$(@D) || :
	@$(BRAINFUCK) --target=$2 $1.b -o $(OUTPUTS_DIR)/$2/$1$(BIN_SUFFIX)
	@$(CHMOD) $(MODE) $$@
endef


define generate-transpile-c-test
transpile-c-$1: transpile-c-$1-prev $(OUTPUTS_DIR)/c/$1$(BIN_SUFFIX)
	@([ -f $(INPUTS_DIR)/$1.txt ] \
		&& $(OUTPUTS_DIR)/c/$1$(BIN_SUFFIX) < $(INPUTS_DIR)/$1.txt || $(OUTPUTS_DIR)/c/$1$(BIN_SUFFIX)) \
		| $(DIFF) - $(EXPECTS_DIR)/$1.txt > /dev/null
	@$(ECHO) 'Success'

transpile-c-$1-prev:
	@$(ECHO) -n "Compile to c test: $1.b ... "

$(OUTPUTS_DIR)/c/$1$(BIN_SUFFIX): $(OUTPUTS_DIR)/c/$1.c
	@[ ! -d $$(@D) ] && $(MKDIR) -p $$(@D) || :
	@$(CC) $(CFLAGS) $$< -o $$@

$(OUTPUTS_DIR)/c/$1.c: .FORCE
	@[ ! -d $$(@D) ] && $(MKDIR) -p $$(@D) || :
	@$(BRAINFUCK) --target=c $1.b -o $$@
endef


.PHONY: all help warning interpreter compile transpile clean distclean $(TESTS)

.FORCE:

all: $(BRAINFUCK) help version interpreter compile transpile

$(BRAINFUCK):
	$(MAKE) -C ../

help:
	$(BRAINFUCK) --help

version:
	$(BRAINFUCK) --version

$(OUTPUTS_DIR):
	$(MKDIR) $(OUTPUTS_DIR)

interpreter: $(foreach OPT_LEVEL,$(OPT_LEVELS), interpreter$(OPT_LEVEL))

$(foreach OPT_LEVEL,$(OPT_LEVELS),$(eval $(call generate-interpreter-test,$(OPT_LEVEL))))

compile: $(foreach TARGET_ARCH,$(TARGET_ARCHS),compile-$(TARGET_ARCH))

$(foreach TARGET_ARCH,$(TARGET_ARCHS),$(eval $(call generate-compile-test,$(TARGET_ARCH))))

transpile: transpile-c

transpile-c: $(foreach TEST,$(TESTS),transpile-c-$(TEST))

$(foreach TEST,$(TESTS),$(eval $(call generate-transpile-c-test,$(TEST))))

clean:
distclean:
	$(RM) $(OUTPUTS_DIR)
