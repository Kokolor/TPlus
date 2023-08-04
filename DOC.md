# T+ Language Documentation

I like ducks.

## Table of Contents

- [Introduction](#introduction)
- [Syntax](#syntax)
  - [Definitions](#definitions)
  - [Functions](#functions)
  - [Directives](#directives)
  - [Commands](#commands)
- [Examples](#examples)
- [Contributing](#contributing)

## Introduction

T+ is a high-level abstraction over Assembly designed to simplify OS development. It provides constructs that make the code more readable while still offering the power and flexibility of Assembly.

## Syntax

### Definitions

- **Address Definition**: Define a memory address or constant.
    ```tplus
    ADRESS variable_name = value
    ```

### Functions

- **Function Declaration**: Define a function.
    ```tplus
    FUNC function_name{
        // function body
    }
    ```

### Directives

- **Bit Mode**: Set the bit mode.
    ```tplus
    BITS 32
    ```

- **Section Directives**: Specify the section of the code.
    ```tplus
    CODE  // Equivalent to section .text in NASM
    DATA  // Equivalent to section .data in NASM
    BSS  // Equivalent to section .bss in NASM
    ```

- **Public Directive**: Make a function or variable public.
    ```tplus
    PUBLIC function_or_variable_name
    ```

### Commands

- **HALT**: An infinite loop to halt the processor.
    ```tplus
    HALT
    ```

- **RETURN**: Return from a function.
    ```tplus
    RETURN
    ```

## Examples

A simple example to print "AB" on the screen:

```tplus
BITS 32
CODE

ADRESS video_memory = 0xB8000

PUBLIC _start
FUNC _start{
    video_memory(0) = 'A'
    video_memory(1) = 'B'
    HALT
}
