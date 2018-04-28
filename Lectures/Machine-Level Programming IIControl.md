## Machine-Level Programming II: Control
### Control: Condition codes
#### Processor State (x86-64, Partial)
##### Information about currently executing program
- Temporary data: `%rax`, ...
- Location of run-time stack: `%rsp` ---> **`Top of current stack`**
- Location of current code control point: `%rip`---> **`instruction pointer`**, ...
    - Naming: `ip stands for instruction pointer`
    - `%rip stores the address of the currently executing instrcution`
- Status of recent tests: **`CF`, `ZF`, `SF`, `OF`** ---> **`all of these are conditional code`**

#### Condition Codes (Implicit Setting)
##### Single-bit registers
- **CF:** Carry Flag (for unsigned)
- **SF:** Sign Flag (for signed)
- **ZF:** Zero Flag
- **OF:** Overflow Flag (for signed)

##### Impilicitly set by arthmetic operations
Example: `addq src, dst i.e. t = a + b`
- **`CF`** set if carry out from most significant bit (unsigned overflow)
- **`ZF`** set if `t == 0`
- **`SF`** set if `t < 0`
- **`OF`** set if 
    - `a > 0 && b > 0 && t < 0` positive overflow
    - `a < 0 && b < 0 && t > 0` negative overflow

#### Note set by `leaq` instruction

#### Condition Codes (Explicit setting: Compare)

##### Explict Setting by Compare constrcution
- `cmpq src2, src1`
- `cmpq b, a` like computing `a-b` without setting destination
- **`CF`** set if if carry out from most significant bit (unsigned overflow)
- **`ZF`** set if `a == b`
- **`SF`** set if `a - b < 0`
- **`OF`** set if 
    - `a > 0 && b < 0 && a - b < 0`
    - `a < 0 && b > 0 && a - b > 0`

#### Condition Codes (Explicit setting: Test)
##### Explicit Setting by Test instruction
- `testq src2, src1`
    - `testq a, b` like computing `a&b`without setting destinatio
- **`ZF`** when `a&b ==0`
- **`SF`** when `a&b < 0`
- **Very Often:** `testq %rax, %rax` 
    - give you `%rax` itself

#### Reading Condition Codes
##### SetX Instructions
- What `setx` does is that it set single byte of a single register to either `1` or `0`
- ***Set single byte baed on combination of condition codes***
- Set low-order byte of destination to 0 or 1 based on combinations of condition codes
- Doesn't alter remaining 7 bytes 

|  SetX |   Condition  |        Description        |
|:-----:|:------------:|:-------------------------:|
|  sete |      ZF      |         Equal/Zero        |
| setne |      ~ZF     |    Note Equal/ Not Zero   |
|  sets |      SF      |          Negative         |
| setns |      SF      |        Non-negative       |
|  setg | ~(SF^OF)&~ZF |      Greater (signed)     |
| setge |   ~(SF^OF)   | Greater or Equal (signed) |
|  setl |     SF^OF    |       Less (signed)       |
| setle | (SF^OF) | ZF |   Less or Equal (signed)  |
|  seta |    ~CF&~ZF   |      Above (unsigned)     |
|  setb |      CF      |      Below (unsigned)     |

![x86-64 registers](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/x86-64%20lowerByte.PNG)
- For every one of the 16 registers, you can directly set the lowest order byte of them to either `0` or `1`
- Naming: The `l` in `%al` means `low`

- **Example**

| Registers |      Use     |
|:---------:|:------------:|
|    %rdi   |  Argument x  |
|    %rsi   |  Argument y  |
|    %rax   | Return value |


```
illustration of %rax:  
-----------------------------
|%rax       |%eax       |%al|
-----------------------------
```

```c
//function gt() takes two arguments, compare them and return 1 if x is greater than y; 0 otherwise
int gt (long x, long y) {
    return  x > y;
}
```
 
```assembly
cmpq    %rsi, %rdi      # Compare x:y
setg    %al             # Set when >
movzbl  %al, %eax       # Zero rest of %rax
ret
```


### Conditional Branches
#### Jumping
##### jX instructions
- Jump to different part of code depending on condition codes

|  jX |     Condition    |        Description        |
|:---:|:----------------:|:-------------------------:|
| jmp |         1        |       Unconditional       |
|  je |        ZF        |           Equal           |
| jne |        ~ZF       |            Not            |
|  js |        SF        |          Negative         |
| jns |        ~SF       |        Nonnegative        |
|  jg | ~(SF ^ OF) & ~ZF |      Greater (signed)     |
| jge |    ~(SF ^ OF)    | Greater or Equal (signed) |
|  jl |     (SF ^ OF)    |       Less (signed)       |
| jle | (SF ^ OF)  |  ZF |   Less or Equal (signed)  |
|  ja |    ~CF & ~ZF     |      Above (unsigned)     |
|  jb |        CF        |      Below (unsigned)     |


- **Conditional Branch Example(Old Style)**

| Registers |      Use     |
|:---------:|:------------:|
|    %rdi   |  Argument x  |
|    %rsi   |  Argument y  |
|    %rax   | Return value |

```c
long absdiff (long x, long y) {
    long result;
    if (x > y)
        result = x-y;
    else
        result = y-x;
    return result;
}
```

```assembly
absdiff:
    cmpq    %rsi, %rdi # x:y
    jle     .L4
    movq    %rdi, %rax
    subq    %rsi, %rax
    ret
.L4:        # x <= y
    movq    %rsi, %rax
    subq    %rdi, %rax
    ret
```

#### Expressing with Goto Code
- C allows **`goto`** statement
- Jump to position designated by label

```c
long absdiff_j (long x, long y) {
    long result;
    int ntest = x <= y;
    if (ntest) 
        goto Else;
    result = x-y;
    goto Done;

    Else:
        result = y-x;
    Done:
        return result;
}
```

#### General Conditional Expression Translation (Using Branches)
- C Code
```c
val = Test ? Then_Expr : Else_Expr;
val = x > y ? x - y : y - x;
```

#### Using Conditional Moves
- Basic idea: compute the result for both condition `true` and `false`, evaluate condition and assign one of the value to result 
- Conditional Move Instructions:
    if (Test) Dest <--- Src

```c
Goto Version
result = Then_Expr;//get the value if condition is true
eval = Else_Expr;//get the value if condition is false
nt = !Test;
if (nt)//evaluate the condition and assign appropriate value
    result = eval;
return result;
```

```c
long absdiff (long x, long y) {
    long result;
    if (x > y)
        result = x-y;
    else
        result = y-x;
    return result;
}
```
| Registers |      Use     |
|:---------:|:------------:|
|    %rdi   |  Argument x  |
|    %rsi   |  Argument y  |
|    %rax   | Return value |

```assembly
absdiff:
    movq    %rdi, %rax # x
    subq    %rsi, %rax # result = x-y
    movq    %rsi, %rdx
    subq    %rdi, %rdx # eval = y-x
    cmpq    %rsi, %rdi # x:y
    cmovle  %rdx, %rax # if <=, result = eval
    ret
```


#### Bad Cases for Conditional Move
1. Expensive Computations:
    `val = Test(x) ? Hard1(x) : Hard2(2)`
    - If `Hard1(x)` and `Hard2(x)` requires really expensive computations, it is bad idea to use conditional move
    - only make sense when computations are **very simple**
2. Risky Computations:
    `val = p ? *p : 0;`
    - Think about in this way, if the pointer p is `null`, you cannot compute `*p` which is required by `conditional move`
    - May have undesirable effects
3. Computation with side effects
    `val = x > 0 ? x*=7 :x+=3`
    - When you compute `x*=7`, you have already altered `x` such that the other value `x+=3` will be contaminated;
    - Must be side-effect free 

### Loops

#### Do-While Loop Example
- C code
    ```c
    //Count number of 1’s in argument x
    long pcount_do (unsigned long x) {
        long result = 0;
        do {
            result += x & 0x1;
            x >>= 1;
        } while (x);
        return result;
    }
    ```
- Goto version:
    ```c
    long pcount_goto (unsigned long x) {
    long result = 0;
    loop:
        result += x & 0x1;
        x >>= 1;
        if(x) 
            goto loop;
        return result;
    }
    ```
- Assembly:
    ```assembly
        movl $0, %eax   # result = 0
    .L2:                # loop:
        movq %rdi, %rdx
        andl $1, %edx   # t = x & 0x1
        addq %rdx, %rax # result += t
        shrq %rdi       # x >>= 1
        jne .L2         # if (x) goto loop
        rep; ret
    ```

- Use `condition branch` to either continue looping or to exit loop


#### General "Do-while" Translation
- C code:
    ```c
    do 
        Body
    while (Test);
    ```

- Goto version:
    ```c
    loop:
        Body
        if (Test)
            go to loop
    ```