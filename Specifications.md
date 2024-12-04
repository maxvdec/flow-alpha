# Specifications of the Flow Alpha

The Flow Alpha is intended to be super minimal, thus his specifications are very simple

## Index

- *Memory and addresses*
- *The intructions and structure of FCA (Flow Computer Assembly)*
- *The simulator specifications and debug*
- *The FCP (Flow Control Panel) and coreFlowOS and how to use them*
- *Interrupts and inner circuits*
- *Component list and plans*

## Memory and Addresses

=== RAM -> **512KB** ===

We have 512KB of *Random Access Memory* for program uses. This will erase itself when powered off.
The RAM is divided in the following sections:

- 0x00000 - 0x07FFF -> **Program Memory / PM** (32KB)
- 0x08000 - 0x0FFFF -> **Data Section** (64KB)
    - 0x0FF00 - 0x0FFFF -> **Stack Usage** (256 bytes)
    - 0x02000 - 0x07FFF -> **Heap** (24KB, can grow)
- 0x0F000 - 0x0F7FF -> **Input and Output** (~4KB)

=== ROM -> **64KB** ===

We have also 64KB of *Read-Only Memory* for data. Is permanent.
The ROM is divided in the following sections:

- 0x080000 - 0x0801FF -> **Bootloader** (512 bytes)
- 0x080200 - 0x0807FF -> **BIOS** (1.5KB)
- 0x080800 - 0x080CFF -> **Basic Operating System (coreFlowOS)** (2KB)
- 0x080D00 - 0x0811FF -> **BASIC and FCP** (2KB)
- 0x081200 - 0x08FFFFF -> **User space (Anything)** (~58KB)

## The instructions and structure of Flow Computer Assembly

### Basic instruction charset

- `add r1, r2` -> Adds two numbers and stores the value in `r1`
- `sub r1, r2` -> Subtracts `r2` from `r1`.
- `mul r1, r2` -> Multiplies the values.
- `div r1, r2` -> Divides `r1` by `r2`
- `inc r1` - Increments `r1` by 1. Can also be expressed as `r1++`
- `dec r1` - Decrements `r1` by 1. Can also be expressed as `r1--`
- `switch r1` -> Switches the sign of `r1`
- `and r1, r2` -> Stores the result of the AND logic gate in `r1`
- `or r1, r2` -> Stores the result of the OR logic gate in `r1`
- `xor r1, r2` -> Stores XOR to `r1`
- `not r1` -> Negates the value in `r1`
- `shiftl r1, n` -> Shifts the value `n` bits to the left
- `shiftr r1, n` -> Shifts the value `n` bits to the right
- `mov r1, #VAL` -> Stores `#VAL` to the register `r1`
- `push r1` -> Pushes value `r1` to the pile
- `pop r1` -> Extracts value `r1` from the pile
- `jmp ADDR` -> Jump to an specific address
- `jnz ADDR` -> Jump to an specific address if the past result is not 0
- `in r1, PORT` -> Reads port `PORT` to register `r1`
- `out PORT, r1` -> Writes value `r1` to port `PORT`

### Extended instruction charset

- `jz ADDR` -> Jumps to an specific address if the past result is 0
- `jgr ADDR` -> Jumps to an specific address if the past result is greater than 0
- `jgl ADDR` -> Jumpts to an specific address if the past result is less than 0
- `halt` -> Halts the processor
- `reset` -> Resets the processor
- `nothing` -> Does nothing

### Registers and Flags

- `r0` to `r6` -> General use registers
- `r7` -> (SP) Stack Pile base register
- `ZF` -> Zero Flag. Indicates that the last result is 0
- `CF` -> Indicates if there was carry
- `SF` -> Indicates if the result was negative
- `OF` -> Indicates if there was overflow

### Other language features and syntax

The Flow Computer Assembly Language, also has support for some other features:

- **Labels**
Labels are indicated with the syntax `name:` and usually contain code. They run until the keyword `end`.
- **Macros**
Macros are defined with the `@macro(args)` syntax and they expand to custom instructions.
- **Inclusions**
You can include other files that will be compiled with the current file with the `!include` syntax.
- **Constants**
Constants are useful beacuse they express constant values that cannot be changed. They are defined with `CONSTANT! something`.
- **Metadata**
Metadata is the one that defined some information about the targeting program. They begin with `!`:
    - **Language metadata**
    They mean the current langauge that we are using (the platform): `!lang flow-alpha`
    - **Origin metadata**
    They indicate the point where the program is going to be stored: `!origin ADDR`

### Examples


```
% This program subtracts from 10 to 0 and they powers a LED up

!lang flow-alpha
!origin 0x081200

% Define a macro that turns a led on
@turn_led(port)

% Store 1 to our register
mov r1, #1

% Pass that value to port 0xF200
out 0xF200, r1

@end

start:
    mov r0, #10
loop:
    r0--

    ; If the last expression didn't output '0' then we continue
    jnz loop
    
    turn_led 0x0F200

    halt
```

### Turning FCA to FCH

Flow Computer Hexadecimal is the format (`.fch`) with the one that code is runned in the processor. It represents code with opcodes:
| Instruction  | OpCode | Charset              |
|--------------|--------|----------------------|
| `add`        | 0x10   | Basic                |
| `sub`        | 0x11   | Basic                |
| `mul`        | 0x12   | Basic                |
| `div`        | 0x13   | Basic                |
| `inc`        | 0x14   | Basic                |
| `dec`        | 0x15   | Basic                |
| `switch`     | 0x16   | Basic                |
| `and`        | 0x17   | Basic                |
| `or`         | 0x18   | Basic                |
| `xor`        | 0x19   | Basic                |
| `not`        | 0x1A   | Basic                |
| `shiftl`     | 0x1B   | Basic                |
| `shiftr`     | 0x1C   | Basic                |
| `mov`        | 0x1D   | Basic                |
| `push`       | 0x1E   | Basic                |
| `pop`        | 0x1F   | Basic                |
| `jmp`        | 0x20   | Basic                |
| `jnz`        | 0x21   | Basic                |
| `in`         | 0x22   | Basic                |
| `out`        | 0x23   | Basic                |
| `jz`         | 0x24   | Extended             |
| `jgr`        | 0x25   | Extended             |
| `jgl`        | 0x26   | Extended             |
| `halt`       | 0x27   | Extended             |
| `reset`      | 0x28   | Extended             |
| `nothing`    | 0x29   | Extended             |
| `!origin`    | 0x2A   | Metadata             |
| Inmediat Number Marker | 0xFD  | Special     |
| Inmediat Register Marker | 0xFE| Special     |


For instance, the code we saw will be translated to:
```fch
0x2A 0x08 0x12 0x0 ; [!origin] [0x081200]
0x1D 0x00 0xFD 0xA ; [mov] [r0] [INM]
; loop is at address 0x081208
0x15 0x00 ; [dec] [r0]
0x21 0x08 0x12 0x08 ; [jnz] [0x081208]
0x15 0x01 0xFD 0x01 ; [mov] [r1] [1]
0x23 0xF2 0x00 0xFE 0x01 ; [out] [0xF200] [r1]
0x27 ; [halt]
```

### How to use the FCA compiler

First make the code in the `flow-computer-assembly/compiler` folder by going in and typing `make`.
You can find some examples in `flow-computer/examples`. It will generate two tools in the `flow-computer/examples/bin` folder (uses CMake).

- `flowcmp some_file.fca` will compile the assembly file and **all the inclusions**. So if you include all the files of the project you don't need to clear them. NOTE THAT **THEY WILL BE ADDED IN THE BOTTOM OF THE FILE** so be sure to have your entry point in the same file or at the top of the inclusions file (if there is no code in the file).

Example:
```fca
% afile.fca
!lang flow-alpha

!include "somefile.fca"

some_function:
    mov r1, #20
```
Will turn to:
```fca
% afile.fca
!lang flow-alpha

some_function:
    mov r1, #20
somefile_function:
    r1--
```
**BEWARE OF THAT**

- `flowinspect some_file.fca` will show you the final code that `flowcmp` will compile when doing all the inclusions and macro substitution
- `flowreverse some_hex.fch` will reverse and output `some_hex.fca`. Note that macros and separate files in inclusions will not be created