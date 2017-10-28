koturn's brainfuck processor
============================

[![Test status](https://travis-ci.org/koturn/kbf.png)](https://travis-ci.org/koturn/kbf)
[![Test status](https://ci.appveyor.com/api/projects/status/q9gh5jkn06iq707o?svg=true)](https://ci.appveyor.com/project/koturn/cppbrainfuck)

A brainfuck interpretor/compiler.

## Features

- Interpreter, Bytecode interpretor, JIT-compiled code interpretor
- Transpiler to C
- Compiler to executable binary
    - x86 for Windows
    - x64 for Windows
    - x86 for Linux
    - x64 for Linux
    - Armeabi (WIP)

## Usage

### Execute brainfuck

You can execute brainfuck as following.

```shell
$ ./kbf hello.b
```

The command is equivalent to following command, with option `-O1` which means that compile branfuck code to IR-code and execute it.

```shell
$ ./kbf hello.b -O1
```

If you want to execute brainfuck code directory, try following command (very slow).

```shell
$ ./kbf hello.b -O1
```

You can also execute branfuck code with JIT compile (very fast).
But currently, this option is available in x86 or x64 environment.

```shell
$ ./kbf hello.b -O2
```

### Transpile to C code

You can transpile brainfuck code to C code as following.

```shell
$ ./kbf hello.b --target=c -o hello.c
```

Ofcourse, generated C source file can be compile with C-compiler.

```shell
$ gcc hello.b -Wall -Wextra -O2 -o hello
$ ./hello
```

### Compile to native binary

#### ELF binary

Following example shows that compile brainfuck code to a x64 ELF binary and execute it in x64-Linux environment.

```shell
$ ./kbf hello.b --target=elfx64 -o hello
$ ./hello
```

Generated ELF binary is glibc less binary, use system call only.


### EXE binary

Following example shows that compile brainfuck code to a x64 Windows binary and execute it in x64-Windows environment.

```shell
$ kbf hello.b --target=winx64 -o hello.exe
$ hello.exe
```

## Future perspective

- Execute with JIT-compile by LLVM
- Generate LLVM-IR
- Fix ARM binary generation
- Use buffer for output characters in Linux ELF binary

## LICENSE

This software is released under the MIT License, see [LICENSE](LICENSE).
