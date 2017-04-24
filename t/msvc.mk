BRAINFUCK = ..\brainfuck.exe
INPUTS_DIR = inputs
OUTPUTS_DIR = outputs
EXPECTS_DIR = expects
MAKE = nmake /nologo
MKDIR = mkdir
ECHO = echo
DIFF = fc
RM = del /F


# .PHONY: all help version interpreter update_expects interpreter1 interpreter2

all: $(BRAINFUCK) help version $(OUTPUTS_DIR) interpreter

$(BRAINFUCK):
	@$(CD) .. & $(MAKE) /f msvc.mk & cd $(MAKEDIR)

help:
	$(BRAINFUCK) --help

version:
	$(BRAINFUCK) --version

$(OUTPUTS_DIR):
	@$(MKDIR) $@

interpreter: update_expects interpreter1 interpreter2

update_expects:
	@for %f in (*.b) do \
		@set /p MSG=Make expected data: %f ...< NUL & \
		@if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(BRAINFUCK) -O0 %~nf.b < $(INPUTS_DIR)\%~nf.txt > $(EXPECTS_DIR)\%~nf.txt & \
			$(ECHO) Done \
		) else ( \
			$(BRAINFUCK) -O0 %~nf.b > $(EXPECTS_DIR)\%~nf.txt & \
			$(ECHO) Done \
		)

interpreter1:
	@for %f in (*.b) do \
		@set /p MSG=interpreter1 test: %f ...< NUL & \
		if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(BRAINFUCK) -O1 %~nf.b < $(INPUTS_DIR)\%~nf.txt > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		) else ( \
			$(BRAINFUCK) -O1 %~nf.b > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		)

interpreter2:
	@for %f in (*.b) do \
		@set /p MSG=interpreter2 test: %f ...< NUL & \
		if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(BRAINFUCK) -O2 %~nf.b < $(INPUTS_DIR)\%~nf.txt > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		) else ( \
			$(BRAINFUCK) -O2 %~nf.b > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		)
