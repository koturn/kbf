ifeq ($(OS),Windows_NT)
    BIN_SUFFIX := .exe
else
    BIN_SUFFIX := .out
endif

BRAINFUCK := $(addsuffix $(BIN_SUFFIX),../brainfuck)
TESTS := $(basename $(sort $(wildcard *.b)))
OPT_LEVELS := 0 1 2
INPUTS_DIR := inputs
EXPECTS_DIR := expects
MAKE := make
MKDIR := mkdir
ECHO := echo
DIFF := diff -Z --strip-trailing-cr
RM := rm -f


define generate-interpreter-test-parent
$1: $(foreach TEST,$(TESTS),$1_$(TEST))
endef

define generate-interpreter-test-child
$1:
	@$(ECHO) -n "Interpreter$3 test: $2.b ... "
	@([ -f $(INPUTS_DIR)/$2.txt ] \
		&& $(BRAINFUCK) -O$3 $2.b < $(INPUTS_DIR)/$2.txt || $(BRAINFUCK) -O$3 $2.b) \
		| $(DIFF) - $(EXPECTS_DIR)/$2.txt > /dev/null
	@$(ECHO) 'Success'
endef


.PHONY: all help warning interpreter clean $(TESTS)

all: $(BRAINFUCK) help version interpreter

$(BRAINFUCK):
	$(MAKE) -C ../

help:
	$(BRAINFUCK) --help

version:
	$(BRAINFUCK) --version

interpreter: $(foreach OPT_LEVEL,$(OPT_LEVELS), interpreter$(OPT_LEVEL))

$(foreach OPT_LEVEL,$(OPT_LEVELS),$(eval $(call generate-interpreter-test-parent,interpreter$(OPT_LEVEL))))

$(foreach OPT_LEVEL,$(OPT_LEVELS),$(foreach TEST,$(TESTS),$(eval $(call generate-interpreter-test-child,interpreter$(OPT_LEVEL)_$(TEST),$(TEST),$(OPT_LEVEL)))))
