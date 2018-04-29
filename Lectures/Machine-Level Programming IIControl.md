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
|  je |        ZF        |        Equal / Zero       |
| jne |        ~ZF       |     Not Equal/ Not Zero   |
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

#### General "While" Translation #1
- While version
    ```c
    while (Test) {
        Body
    }
    ```
- Goto Version
    ```c
    go to test;
    loop:
        Body
    test:
        if (Test)
            goto loop
        done:
    ```
#### While loop Example #1
- C code 
    ```c
    long pcount_while (unsigned long x) {
        long result = 0;
        while (x) {
            result += x & 0x1;
            x >>= 1;
        }
        return result;
    }
    ```
- Jump to Middle
    ```c
    long pcount_while (unsigned long x) {
        long result = 0;
        goto test;
        loop:
            result += x & 0x1;
            x >>= 1;
        test:
            if (x)
                goto loop;
            return result;
    }
    ```

#### General "While" Translation #2
- Translate `while loop` into `Do-while loop structure`
- While version
    ```c
    while (Test) {
        Body;
    }
    ```
- Do-while version
    ```c
    if (!Test) {
        go to done;
    }
    do {
        Body;
    } while (Test);
    done:
    ```
- Go-to version
    ```c
    if (!Test) {
        goto done;
    }
    loop:
        Body;
        if (Test) {
            go to loop;
        }
        done:
    ```

#### For-loop Form
- General Form
    ```c
    for (Init; Test; Update) {
        Body
    }
    ```

- For-loop version of pcount
    ```c
    //size of unsigned long is 4 bytes
    #define WSIZE 8*sizeof(int)//size of long in unit of bit
    long pcount_for (unsigned long x) {
        size_t i;
        long result = 0;
        for (i = 0; i < WSIZE; i++) {
            unsigned bit = (x >> i) & 0x1;//unsigned is a alias of unsigned int
            result += bit;
        }
        return result;
    }
    ```
    - **Init:** `i = 0`
    - **Test:** `i < WSIZE`
    - **Update:** `i++`
    - **Body:**
        ```c
        {
            unsigned bit = (x >> i) & 0x1;
            result += bit;
        }
        ```
#### For-loop ---> While-loop
```c
for (Init; Test; Update) {
    Body
}

to

while (Test) {
    Body;
    Update;
}
```
- Turn `for-loop pcount` into `while-loop pcount`
    ```c
    //size of unsigned long is 4 bytes
    #define WSIZE 8*sizeof(int)//size of long in unit of bit
    long pcount_for (unsigned long x) {
        size_t i;
        long result = 0;

        size_t i = 0;//Init
        while (i < WSIZE) {//Test
            result += (x >> i) & 0x1;//Body
            i++;//Update
        }
        return result;
    }
    ```

#### `-O1` optimization
```c
long pcount_for_goto_dw (unsigned long x) {
    size_t i;
    long result = 0;
    i = 0;//Initial
    ********************
    if (!(i < WSIZE))//This section will be eliminate by compiler and the reason can be seen below
        goto done;
    ********************
    loop://Body
    {
        unsigned bit =
        (x >> i) & 0x1;
        result += bit;
    }
    i++;//Update
    if (i < WSIZE)//Test
        goto loop;
    done:
        return result;
}
```
- **Reason to eliminate the (unnecessary) section**
    - compiler knows that the initial value of `i` is guarantee to less than the word size `WSIZE = 32`
    - Thus there is no need to do the initial check
    - Thus the section surrounded by "***" will be eliminated


### Switch Statements
#### Switch Statement Example
```c 
long switch_eg (long x, long y, long z) {
    long w = 1;
    switch(x) {
        case 1:
            w = y*z;
            break;
        case 2:
            w = y/z;
            /* Fall Through */
            //if we don't set the "break" statement in "case", after we finish executing the current case, we fall through (move into) the next case
        case 3:
            w += z;
            break;
        case 5://do nothing but fall through to case 6
        case 6:
            w -= z;
            break;
        default://default will be executed if none of these cases is matched 
            w = 2;
    }
    return w; 
}
//Tips from Randy Bryant: when never you use switch statement, pay very close attention to "break"; 
//missing "break" statement could cause bug that you are not even aware of 
```

#### Jump Table Structure
- Switch Form
    ```c
    switch (x) {
        case val_0:
            Block 0;
        case val_1:
            Block 1
        .
        .
        .
        case val_n-1:
            Block n-1;
    }
    ```
- Better Illustration
![switch statement](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Swtich%20Statement.PNG)

#### Swtich Statement Example
- **Source code**
    ```c
    long switch_eg (long x, long y, long z) {
        long w = 1;
        switch(x) {
            case 1:
                w = y*z;
                break;
            case 2:
                w = y/z;
                /* Fall Through */
                //if we don't set the "break" statement in "case", after we finish executing the current case, we fall through (move into) the next case
            case 3:
                w += z;
                break;
            case 5://do nothing but fall through to case 6
            case 6:
                w -= z;
                break;
            default://default will be executed if none of these cases is matched 
                w = 2;
        }
        return w; 
    }
    ```

- **Assembly code**
    ```assembly
    switch_eg:
        movq %rdx, %rcx         # make a copy of z for some reason
        cmpq $6, %rdi # x:6     # compare x with 6 since 6 is the largest number in my cases    
        ja .L8                  # .L8 is the default case, ja means we jump to default case if x > 6 or x < 0, this is a clever trick
        jmp *.L4(,%rdi,8)       #
    ```
- **Registers**
    | Register |      Use     |
    |:--------:|:------------:|
    |   %rdi   |       x      |
    |   %rsi   |       y      |
    |   %rdx   |       z      |
    |   %rax   | Return value |

- **Illustration**
    ![switch statement 2](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Switch%20Statement%202.PNG)


#### Jump Table
![jump table](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Jump%20Table.PNG) 

#### Code Blocks (x == 1)
```c
switch (x) {
    case 1:         //go to .L3
        w = y * z;
        break;
    .
    .
    .
    //other cases
}
```

```assembly
.L3:
    movq    %rsi, %rax      # copy y to return value w
    imulq   %rdx, %rax      # multiply w with z and store the result in w
    ret                     # behaves for "break" statement in source code
```

| Register |      Use     |
|:--------:|:------------:|
|   %rdi   |       x      |
|   %rsi   |       y      |
|   %rdx   |       z      |
|   %rax   | Return value |


#### Handling Fall-through
```c
long w = 1;
switch (x) {
    .
    .
    .
    case 2:
        w = y / z;
        /* Fall through */
    case 3:
        w += z;
        break;
    .
    .
    .
}
```

![Fall through](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Fall%20through.PNG)


| Register |      Use     |
|:--------:|:------------:|
|   %rdi   |       x      |
|   %rsi   |       y      |
|   %rdx   |       z      |
|   %rax   | Return value |

```assembly
.L5:                    # Case 2
    movq    %rsi, %rax
    cqto                # sign extend
    idivq %rcx # y/z
    jmp .L6             # goto merge
.L9:                    # Case 3
    movl $1, %eax       # w = 1
.L6:                    # merge:
    addq %rcx, %rax     # w += z
    ret
```

#### Code Blocks (x == 5, x == 6, default)
```c 
switch(x) {
    .
    .
    .
    case 5:     // .L7
    case 6:     // .L7
        w -= z;
        break;
    default: // .L8
        w = 2;
}
```


| Register |      Use     |
|:--------:|:------------:|
|   %rdi   |       x      |
|   %rsi   |       y      |
|   %rdx   |       z      |
|   %rax   | Return value |

```assembly
.L7:                    # Case 5,6
    movl $1, %eax       # w = 1
    subq %rdx, %rax     # w -= z
    ret
.L8:                    # Default:
    movl $2, %eax       # 2
    ret
```

    