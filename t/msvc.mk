BRAINFUCK = ..\brainfuck.exe
INPUTS_DIR = inputs
OUTPUTS_DIR = outputs
EXPECTS_DIR = expects
MAKE = nmake /nologo
MKDIR = mkdir
ECHO = echo
DIFF = fc
RM = del /F
RMDIR = rmdir /Q /S

CC = cl /nologo
CFLAGS = /O2 /W4 /DNDEBUG /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_WARNINGS /D_SECURE_SCL=0


# .PHONY: all help version interpreter update_expects interpreter1 interpreter2 compile transpile clean distclean

all: $(BRAINFUCK) help version $(OUTPUTS_DIR) interpreter compile transpile

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
		@set /p MSG=Interpreter1 test: %f ...< NUL & \
		@if exist $(INPUTS_DIR)\%~nf.txt ( \
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
		@set /p MSG=Interpreter2 test: %f ...< NUL & \
		@if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(BRAINFUCK) -O2 %~nf.b < $(INPUTS_DIR)\%~nf.txt > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		) else ( \
			$(BRAINFUCK) -O2 %~nf.b > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		)

!if "$(PROCESSOR_ARCHITECTURE)" == "x86"
!if "$(PROCESSOR_ARCHITEW6432)" == "AMD64"
compile: compile-winx64 compile-winx86
!else
compile: compile-winx86
!endif
!else
compile: compile-winx64 compile-winx86
!endif

compile-winx64:
	@if not exist $(OUTPUTS_DIR)\winx64 ( \
		$(MKDIR) $(OUTPUTS_DIR)\winx64 \
	)
	@for %f in (*.b) do \
		@set /p MSG=Compile to winx64 test: %f ...< NUL & \
		@$(BRAINFUCK) --target=winx64 %~nf.b -o $(OUTPUTS_DIR)\winx64\%~nf.exe & \
		@if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(OUTPUTS_DIR)\winx64\%~nf.exe < $(INPUTS_DIR)\%~nf.txt > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		) else ( \
			$(OUTPUTS_DIR)\winx64\%~nf.exe > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		)

compile-winx86:
	@if not exist $(OUTPUTS_DIR)\winx86 ( \
		$(MKDIR) $(OUTPUTS_DIR)\winx86 \
	)
	@for %f in (*.b) do \
		@set /p MSG=Compile to winx86 test: %f ...< NUL & \
		@$(BRAINFUCK) --target=winx86 %~nf.b -o $(OUTPUTS_DIR)\winx86\%~nf.exe & \
		@if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(OUTPUTS_DIR)\winx86\%~nf.exe < $(INPUTS_DIR)\%~nf.txt > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		) else ( \
			$(OUTPUTS_DIR)\winx86\%~nf.exe > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		)

transpile: transpile-c

transpile-c:
	@if not exist $(OUTPUTS_DIR)\c ( \
		$(MKDIR) $(OUTPUTS_DIR)\c \
	)
	@for %f in (*.b) do \
		@set /p MSG=Compile to c test: %f ...< NUL & \
		@$(BRAINFUCK) --target=c %~nf.b -o $(OUTPUTS_DIR)\c\%~nf.c & \
		@$(CC) $(CFLAGS) $(OUTPUTS_DIR)\c\%~nf.c /Fe$(OUTPUTS_DIR)\c\%~nf.exe > NUL & \
		@if exist $(INPUTS_DIR)\%~nf.txt ( \
			$(OUTPUTS_DIR)\c\%~nf.exe < $(INPUTS_DIR)\%~nf.txt > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		) else ( \
			$(OUTPUTS_DIR)\c\%~nf.exe > $(OUTPUTS_DIR)\%~nf.txt & \
			$(DIFF) $(OUTPUTS_DIR)\%~nf.txt $(EXPECTS_DIR)\%~nf.txt > NUL && \
			$(ECHO) Success || $(ECHO) Failed \
		)

clean:
distclean:
	$(RMDIR) $(OUTPUTS_DIR)
