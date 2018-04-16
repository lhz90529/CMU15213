##[Bits, Bytes and Integers Part2](https://www.cs.cmu.edu/~213/lectures/03-bits-ints-part2.pdf)

#### Addition
##### Unsigned Addition
- Perform regular addition on each bit
- Discard carry if the carry is out of range
![equation](https://latex.codecogs.com/gif.latex?s&space;=&space;UAdd_w(u,v)&space;=&space;(u&space;&plus;&space;v)&space;\mod&space;2^w)
```
example:

    1   1   0   1   13
+   0   1   0   1   5
-------------------------
1   0   0   1   0   2 = 18 mod 16
^
being dropped
```
##### Two's Complement Addition

```
example:

    1   1   0   1   -3   
+   0   1   0   1   5
-------------------------
1   0   0   1   0   2 
^
being dropped

Negative overflow:
    1   1   0   1   -3
    1   0   1   0   -6
-------------------------
1   0   1   1   1   7 ---> we add two negetive numbers, but we got positive
^
being dropped

The reason is that (-3 + -6) = -9 cannot be represented by 4-bit's Two's Complement

Positive overflow:
    0   1   1   1   7
    0   1   0   1   5
-------------------------
    1   1   0   0   -4 ---> we add two positive numbers, we got a negative

This is a positive overflow
```

#### Multiplication
##### Unsigned Multiplication
![equation](https://latex.codecogs.com/gif.latex?UMult_w(u,v)&space;=&space;(u&space;\cdot&space;v)&space;\mod&space;2^w)

```
    0   0   1   1
*   0   1   0   1
--------------------

                0   0   1   1
            0   0   0   0   
        0   0   1   1
+   0   0   0   0
--------------------------------
    0   0   0   1   1   1   1
    ^   ^   ^
    being dropped
final result: 1111 ---> 15 which is good


However, if we multiply 5 * 5
we get: 
    1   0   0   1   1   0   0   1
    ^   ^   ^   ^
    being dropped
final result: 1001 = 9 ---> 25 mod 16
```

##### Two's Complement Multiplication
```
If we do 5 * 4, we get
    0   0   0   1   0   1   0   0 ---> we get 4
If we do 5 * 5, we get
    0   0   0   1   1   0   0   1 ---> we get -7
```
**Key Observation:**
>  If the result number is representable, you get the correct result; otherwise, it cause overflow

##### Power-of-2 Multiply with Shift
> When you multiply a number by 4, the compiler will generate assembly code that shift the number 2 position left.
Historically, multiplication instruction takes longer than shift instruction.

- ![equation](https://latex.codecogs.com/gif.latex?u&space;<<&space;k&space;\text{&space;gives&space;}&space;u&space;*&space;2^k)
- works for both signed and unsigned

##### Power-of-2 Divide with Shift
- ![equation](https://latex.codecogs.com/gif.latex?u&space;>>&space;k&space;\text{&space;gives&space;}&space;\lfloor&space;u&space;*&space;2^k&space;\rfloor)
- use logical shift 


##### Negate
negate(x) = ~x + 1

#### Why Should I Use Unsighed?
##### Don't use without understanding implications
- Easy to make mistakes
    ```c
    unsigned i;
    for (i = n - 1; i >=0; i--) {
        //do something
    }//A infinite loop
    ```
- Can be very subtle
    ```c
    #define DELTA sizeof(int)
    int i;
    for (int i = n - 1; i - DELTA >= 0; i -= DELTA) {
        //do something
    }
    //Also a infinite loop, 
    //since i will be convert into unsigned integer when expression "i - DELTA" is evaluated.
    ```

- Proper way to use unsigned as loop index
    ```c
    unsigned i;
    for (i = n - 1; i < n; i--) {
        //do something
    }
    ```
- See _Robert Seacord, Secure Coding in C and C++_
    `0 - 1 = UMax`
- Even better
    ```c
    size_t i;
    for (i = n - 1; i < n; i--) {
        //do something
    }
    ```

#### Representations in memory, pointers, strings
##### Programs refer to data by address
- Conceptually, envision memory as a very large array of bytes
    - In reality, it's not, but can think of it that way
- An address is like an index into that array
    - and, a pointer variable stores an address

##### System provides private address spaces to each "process"
- Think of a process as a program being executed


#### Machine Words
##### Any given computer has a "Word Size"
- 32-bit machine has word size of 32-bit(4 bytes)
- 64-bit machine has word size of 64-bit(8 bytes)

#### Byte Ordering
> **How are the bytes within a multi-byte word ordered in memory?**
- Big Endian
    > Least significant byte has highest address
- Little Endian
    > Least significant byte has lowest address

```
Example:
variable x: 0x01234567
Address of x: 0x100

0   1   2   3   4   5   6   7   
^   ^                   ^   ^
most significant byte   least significant byte


Big Endian
        0x100   0x101   0x102   0x103   
-------------------------------------------------
|       |   01  |   23  |   45  |   67  |       |
-------------------------------------------------

Little Endian
        0x100   0x101   0x102   0x103   
-------------------------------------------------
|       |   67  |   45  |   23  |   01  |       |
-------------------------------------------------
```

#### Examing Data Representations
**Code to Print Byte Representation of Data**
- Casting pointer to unsigned char* allows treatment as byte array
```c
typedef unsigned char *pointer;
void show_bytes(pointer start, size_t len){
    size_t i;
    for (i = 0; i < len; i++)
    printf(”%p\t0x%.2x\n",start+i, start[i]);
    printf("\n");
}
```

#### Representing Strings
- String in C
    - Represented by array of characters
    - Each character encoded in ASCII format
    - null terminated
        - Final character - 0
