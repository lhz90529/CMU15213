## Machine-level Programming III:Procedures

### Mechanisms in Procudures

```c
        P(...) {
            .
            .           //go to procudure Q with arg
            y = Q(x); ---------------------------
----------> print(y);                           |
|          .                                    |
|      }                                        |   
|                                               |        
|      int Q(int i) {                           |    
|          int t = 3 * i; <----------------------
|          int v[10];//Memory allocation       
|          .
|          .
----------- return v[t];
//Back to 
        }
/*  x in P() is passed to Q() as an argument
    Q() return v[t] to y in P()
*/
```
#### Passing control
- To **begining** of procedure code
- Back to **return point**

#### Passing data
- Procedure **arguments**
- **Return value**

#### Memory Management
- **Allocate** during procedure execution
- **Deallocate** upon return

#### Mechanisms all implemented with machine instructions

#### x86-64 implementation of a procedure uses only those mechanisms required

### Stack Structure

#### x86-64 Stack
![x86-64](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/x86-64%20stack.PNG)
##### Region of memory managed with stack discipline
- Memory viewed as array of bytes
- Different regions have different purpose
- (Like `ABI`, a policy decision)

##### Grows toward lower addresses

##### Register `%rsp` contains lowest stack address
- i.e. address of `top element`


#### x86-64 Stack: `Push`

##### `pushq Src`
- Fetch operand at `Src`
- Decrement `%rsp` by 8
- Write operand at address given by `%rsp`

#### x86-64 Stack: `Pop`

##### `popq Dest`
- Read valude **at address** given by `%rsp`
- increment `%rsp` by 8
- Store value at `Dest`, and the `Dest` must be a `register`

### Call Conventions

#### Passing control

##### Code Example

```c
long mult2(long a, long b) {
    long s = a * b;
    return s;
}

void multstore(long x, long y, long* dest) {
    long t = mult2(x, y);
    *dest = t;
}
```
```assembly
0000000000400550 <mult2>:
    400550: mov %rdi,%rax # a
    400553: imul %rsi,%rax # a * b
    400557: retq # Return

0000000000400540 <multstore>:
    400540: push %rbx # Save %rbx
    400541: mov %rdx,%rbx # Save dest
    400544: callq 400550 <mult2> # mult2(x,y)
    400549: mov %rax,(%rbx) # Save at dest
    40054c: pop %rbx # Restore %rbx
    40054d: retq
```
#### Procedure Control Flow

##### Use stack to support procedure call and return
##### Procedure call: `call lable`
- `push` return address on stack
- Jump to `lable`

##### Return address:
- Address of the next instruction **right after call**
- Example from assembly

##### Procedure return: `ret`
- `pop` address from stack
- Jump to address

#### Passing data
##### procedure Data Flow
- **Registers**
    - **first 6 arguments**
        1.  `%rdi`
        2.  `%rsi`
        3.  `%rdx`
        4.  `%rcx`
        5.  `%r8`
        6.  `%r9`
    - **return value**
        - `%rax`
- **Stack** (if you have more than 6 arguments though not very ususal)
    - those extra arguments will be pushed onto stack
    - Back into the bad old day of `IA32`, all the arguments got passed onto the stack
        - But now, for the most part, you pass arguments by the `registers`
        - The reason is **`register access is way faster than memory access`**
    - Only allocate stack space when needed

![Data Flow](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Data%20Flow.PNG)


#### Managing local data
##### Stack Based Language
- **Languages that support recursion**
    - e.g. `C`, `Pascal`, `Java`
    - Code must be `reentrant`
        - Multiple simultaneous instantiations of a single procedure
    - Need some place to store state of each instantiation
        - Arguments
        - Local variables
        - Return pointer
- **Stack discipline**
    - State for given procedure needed for limited time
        - From `when called` to `when return`
    - Callee returns before caller does
- **Stack allocated in `Frames`**
    - state of single procedure `instantiation`
    - i.e. state for a particular instance of procedure

##### Stack Frames
- **Contents**
    - Return information
    - Local storage (if needed)
    - Temporary space (if needed)
- **Management**
    - Space allocated when enter procedure
        - Set-up code
        - Includes `push` by `call` instruction
    - Deallocated when return 
        - "Finish" code
        - Includes `pop` by `ret` instruction
![Stack Frame](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Stack%20Frame.PNG)

##### x86-64/Linux Stack Frame
- **Current Stack Frame (Top to Bottom)**
    - Argument build:
        - Parameters for function about to call
    - Local variables
        - if can't keep in registers
    - Saved register context
    - Old frame pointer (optional)
- **Caller Stack Frame**
    - Return address
        - Pushed by `call` instruction
    - Arguments for this call

- **Example**

```c
//This function takes 2 arguments
//one is a pointer, another is an integer value
//return the original value of the pointer 
long incr(long *p, long val) {
    long x = *p;
    long y = x + val;
    *p = y;
    return x;
}
```

```assembly
incr:
    movq (%rdi), %rax   # x = *p
    addq %rax, %rsi     # y = x + val
    movq %rsi, (%rdi)   # *p = y
    ret                 # return x
```

| Register |       Use       |
|:--------:|:---------------:|
|   %rdi   |    Argument p   |
|   %rsi   | Argument val, y |
|   %rax   | x, Return value |

- Example: Calling incr#1
![incr#1](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Stack%20Frame%20incr%231.PNG)
 - `subq $16 %rsp`: allocate 16 bytes on the stack
 - `movq $15213, 8(%rsp)`: store 15213 at offset 8 from the stack pointer

- Example: Calling incr#2
![incr#2](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Stack%20Frame%20incr%232.PNG)
  - `movl $3000, %esi`: store the second argument to function incr()
  - `leaq 8(%rsp), %rdi`: store the first argument to function incr()

##### Register Saving Conventions
- **When procedure `yoo` calls `who`:**
    - **`yoo`** is the **`caller`**
    - **`who`** is the **`callee`**
- **Can register be used for temporary storage?**
    ![](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Register%20Saving%20Conventions.PNG)

    - Contents of register `%rdx` overwritten by `who`
    - This could be trouble 

- **Conventions**
    - **Caller Saved**
        - Caller saves temporary values in its frame before that call
    - **Callee Saved**
        - Callee saves temporary value in its frame before using 
        - Callee restore them before returning to caller

##### x86-64 Linux Register Usage
![linux register](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Linux%20Register.PNG)
- **`%rax`**
    - Return value
    - Also caller-saved
    - Can be modified by procedure
- **`%rdi,...,%r9`**
    - Arguments
    - Also caller-saved
    - Can be modified by procedure
- **`%r10, %r11`**
    - Caller-saved
    - Can be modified by procedure
- **`%rbx`, `%r12`,`%r13`,`%r14`**
    - Callee-saved
    - Callee must save & restore
- **`rbp`**
    - Callee-saved
    - Callee must save & restore
    - May be used as frame pointer
    - Can mix & match
- **`%rsp`**
    - Special form of callee save
    - restored to original value upon exit from procedure


#### Illustration of Recursion
\
```c
/* Recursive popcount */
long pcount_r(unsigned long x) {
    if (x == 0)
        return 0;
    else
        return (x & 1) + pcount_r(x >> 1);
}
```

```assembly
pcount_r:
    movl $0, %eax   
    testq %rdi, %rdi    # test if the input argument is 0
    je .L6              # if it is, jump to .L6
    pushq %rbx          # caller save %rbx
    movq %rdi, %rbx     # such that we can use %rbx freely right now
    andl $1, %ebx       # x & 1
    shrq %rdi           # x >> 1
    call pcount_r       #   
    addq %rbx, %rax     # %rbx is (x & 1), %rax is the return of p_count
    popq %rbx           # restore %rbx
.L6:
    rep; ret            # you can ignores the "rep;"

 ```

 ##### Observations About Recursion
 - **Handled Without Special Consideration**
    - Stack frames mean that each function call has ***private storage***
        - `saved registers & local variables`
        - `saved return pointer`
    - Register saving conventions prevent one function call from corrupting another's data
        - Unless the C code explicitly does so (e.g. buffer overflow)
    - Stack discipline follows call/return pattern
        - If P calls Q, then Q returns before P
        - Last-in, First-Out

#### x86-64 Procedure Summary
- **Important Points**
    - Stack is the right data structure for procedure call/return 
        - If P calls Q, then Q returns before P
- **Recursion(&mutual recursion) handled by normal calling conventions**
    - Can safely store values in local stack frame and in callee-saved registers
    - Put function arguments at top of stack
    - Result return in `%rax`
- **Pointers are address of values**
    - On stack or global
