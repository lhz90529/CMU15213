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
- **Stack** (if you have more than 6 arguments)
    - Only allocate stack space when needed



#### Managing local data
### Illustration of Recursion 