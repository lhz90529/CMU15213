## Machine-Level Programming I: Basics

### Intel x86 Processors
- **Naming:** The first one is called `8086`, they skipped `81` and went to `8286`, `8386` and so forth, ***the one thing in common is `86`*** , so peple call them `x86`

- **CISC:** Complex instruction set computer
- **RISC:** Reduced instruction set computer
[check out here!!!](https://cs.stanford.edu/people/eroberts/courses/soco/projects/risc/risccisc/)

#### Intel x86 Evolution:
|    Name    | Date | Transistor |    MHz    |                                 Description                                 |
|:----------:|:----:|:----------:|:---------:|:---------------------------------------------------------------------------:|
|    8086    | 1978 |     29K    |    5-10   | First 16-bit Intel processor                                                |
|     386    | 1985 |    275K    |   16-33   | First 32-bit Intel processor, referred to as IA32,  capable of running Unix |
| Pentium 4E | 2004 |    125M    | 2800-3800 | First 64-bit Intel processor, referred to as x86-64                         |
|   Core 2   | 2006 |    291M    | 1060-3500 | First multi-core Intel processor                                            |
|   Core i7  | 2008 |    731M    | 1700-3900 | Four cores                                                                  |

#### State of Art (2015)
![2015 state of art](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/2015%20State%20of%20Art.PNG)
- **DDR:**  connection to main memory
- **PCI:**  connection to peripheral device
- **SATA:** connection to different types of disk
- **USB:**  ...
- **Ethernet:** connection to network

#### AMD (Advanced Micro Devices)
- Almost always _No.2_ behind `intel`............

#### Terminology
- **Architecture:** (also ISA: instruction set architecture) The parts of a processor design that one needs to understand for writing assembly/machine code.
- **Microarchitecture:**: Implementation of the architecture
- **Machine code:** The byte-level programs that a processor executes
- **Assembly code:** A text representation of machine code 

#### Assembly/Machine Code View
##### Programmer-visible state
![Assembly/Machine Code View](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Assembly%20Machine%20Code%20View.PNG)
- **PC: program counter**
    - Address of next instruction
    - called "RIP" (x86-64) ---> %rip
- **Register file (a set of registers)**
    - a very small number of memory locations that store the value will be used by processor
- **Condition codes**
    - store status infomation about most recent arthmetic or logical operation
- **Memory**
    - Byte addressable array
    - Code and user data
    - Stack to support procedures    
- **Cache is NOT visible to Programmer**
    - You have NO way to access or manipulate data in `cache`

#### Turn C into Object Code
- Compiling into Assembly
```c
//source code
long plus (long x, long y);

void sumstore(long x, long y, long *dest) {
     long t = plus(x, y);
     *dest = t;
}
```
```bash
Command:
gcc -Og -S sum.c
    -Og: basic optimization
    -S: for assembly
```

```assembly
Coresspoing assembly code
sumstore:
    pushq   %rbx            push sth onto stack
    movq    %rdx, %rbx      move sth from src to dst
    call    plus            call some procedure
    movq    %rax, (%rbx)
    popq    %rbx            counter part to push
    ret                     return out from the particular function
```

#### Assembly Characteristics: Data types
//TODO

#### Assembly Characteristics: Operations
- Perform arithmetic function on register or memory data
- Transfer data between memory and register
    - **load** data from `memory` into `register`
    - **store** `register data` into `memory`
- Transfer control
    - Unconditional jumps to/from procedures
    - Conditional branches
#### Machine Instruction Example
```c
//C
*dest = t;//store value t where designated by "dest"

//Assembly
move %rax, (%rbx)
/*
t: Register %rax
dest: Register %rbx
*dest: Memory M[%rbx]
*/

//Object code 
0x40059e: 48 89 03// 3 byte instruction stored at address 0x40059e
```

#### Disassembling Object Code
```bash 
objdump -d object executable filei or .o file
```
```bash
gdb executable object file
disassemble function that you specify
```

#### x86-64 Integer Register

|      |      |      |       |
|:----:|:----:|:----:|:-----:|
| %rax | %eax |  %r8 |  %r8d |
| %rbx | %ebx |  %r9 |  %r9d |
| %rcx | %ecx | %r10 | %r10d |
| %rdx | %edx | %r11 | %r11d |
| %rsi | %esi | %r12 | %r12d |
| %rdi | %edi | %r13 | %r13d |
| %rsp | %esp | %r14 | %r14d |
| %rbp | %ebp | %r15 | %r15d |

- Named location where you can `store` value and `retrive` value from them

#### IA32 Registers
![IA32-Registers](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/IA32%20Registers.PNG)

#### Moving Data
`movq src dst`
- Operand Types
    - Immediate: Constant integer data prefix with `$` 
        - example: `$0x400`, `$-533`
    - Register: One of the 16 integer register 
        - example: `%rax`, `%r13` and so on
        - `%rsp%` is reserved for special purpose
    - Memory: 8 consecutive bytes of memory at address given by register 
        - example: `(%rax)`
![movq](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/movq.PNG)


#### Simple Memory Addressing Mode
- **(R)**
    - Register R specifies the memory address
    - example: `movq (%rcx) %rax`
- **D(R)**
    - Register R specifies the memory address
    - Constant displacement D specifies the offset
    - example: `movq 8(%rbp), %rdx`

#### Example of Simple Addressing Model
```c
void swap (long *xp, long *yp){
    long t0 = *xp;//mem to reg
    long t1 = *yp;//mem to reg
    *xp = t1;//reg to mem
    *yp = t0;//reg to mem
}

swap:
    movq (%rdi), %rax
    movq (%rsi), %rdx
    movq %rdx, (%rdi)
    movq %rax, (%rsi)
    ret
```

#### Understanding Swap()
```c
void swap (long *xp, long *yp){
    long t0 = *xp;
    long t1 = *yp;
    *xp = t1;
    *yp = t0;
}
```
| Register | Value |
|:--------:|:-----:|
|   %rdi   |   xp  |
|   %rsi   |   yp  |
|   %rax   |   t0  |
|   %rdx   |   t1  |
- `%rdi`: the register for 1st argument
- `%rsi`: the register for 2nd argument
```
swap:
    movq (%rdi), %rax # t0 = *xp
    movq (%rsi), %rdx # t1 = *yp
    movq %rdx, (%rdi) # *xp = t1
    movq %rax, (%rsi) # *yp = t0
    ret
 ```
 > See detailed illustration in slide

 #### Complete Memory Addressing Modes
 - Most General Form **D(Rb, Ri, S)**
    - `D`: constant displacement `1` `2` or `4`
    - `Rb`: Base register: Any of 16 integer registers
    - `Ri`: Any except for `rsp`
    - `S`: Scale: `1`, `2`, `4`, or `8`

- Example: `%rdx = 0xf000`, `%rcx = 0x0100`

| Expression    | Address Computation | Address |
|---------------|---------------------|---------|
| 0x8(%rdx)     | 0xf000 + 0x8        | 0xf008  |
| (%rdx,%rcx)   | 0xf000 + 0x100      | 0xf100  |
| (%rdx,%rcx,4) | 0xf000 + 4 * 0x100  | 0xf400  |
| 0x80(,%rdx,2) | 0xf000 * 2 + 0x80   | 0x1e080 |


### Arithmetic & logical operations

#### `leaq src dst`
//ToDo

####