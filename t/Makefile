ifeq ($(OS),Windows_NT)
    BIN_SUFFIX := .exe
else
    BIN_SUFFIX := .out
endif

BRAINFUCK := $(addsuffix $(BIN_SUFFIX),../brainfuck)
TESTS := $(basename $(sort $(wildcard *.b)))
INPUTS_DIR := inputs
EXPECTS_DIR := expects
MAKE := make
MKDIR := mkdir
ECHO := echo
DIFF := diff -Z --strip-trailing-cr
RM := rm -f


define generate-interpreter-test
$1:
	@$(ECHO) -n "Interpreter$3 test: $2.b ... "
	@([ -f $(INPUTS_DIR)/$2.txt ] \
		&& $(BRAINFUCK) -O$3 $2.b < $(INPUTS_DIR)/$2.txt || $(BRAINFUCK) -O$3 $2.b) \
		| $(DIFF) - $(EXPECTS_DIR)/$2.txt > /dev/null
	@$(ECHO) 'Success'
endef


.PHONY: all interpreter clean $(TESTS)

all: $(BRAINFUCK) interpreter

$(BRAINFUCK):
	$(MAKE) -C ../

interpreter: $(foreach OPT_LEVEL,0 1,$(foreach TEST,$(TESTS),interpreter$(OPT_LEVEL)_$(TEST)))

$(foreach OPT_LEVEL,0 1,$(foreach TEST,$(TESTS),$(eval $(call generate-interpreter-test,interpreter$(OPT_LEVEL)_$(TEST),$(TEST),$(OPT_LEVEL)))))
