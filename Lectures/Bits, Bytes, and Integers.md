##[Bits, Bytes and Integers](https://www.cs.cmu.edu/~213/lectures/02-bits-ints-part1.pdf)

#### Representing information as bits
- Each bit is **0** or **1**
- **Byte** = 8 bits
- Binary: ``00000000 to 11111111``
- Decimal: ``0 to 255``
- Hex: `00 to FF`

    | Decimal | Binary | Hex |
    |:-------:|:------:|:---:|
    |    0    |  0000  |  0  |
    |    1    |  0001  |  1  |
    |    2    |  0010  |  2  |
    |    3    |  0011  |  3  |
    |    4    |  0100  |  4  |
    |    5    |  0101  |  5  |
    |    6    |  0110  |  6  |
    |    7    |  0111  |  7  |
    |    8    |  1000  |  8  |
    |    9    |  1001  |  9  |
    |    10   |  1010  |  A  |
    |    11   |  1011  |  B  |
    |    12   |  1100  |  C  |
    |    13   |  1101  |  D  |
    |    14   |  1110  |  E  |
    |    15   |  1111  |  F  |


-
**Data Representations**
> Unit: Byte

|   C Data Type   | Typical 32-bit | Typical 64-bit | x86-64 |
|:---------------:|:--------------:|:--------------:|:------:|
|     **char**    |        1       |        1       |    1   |
|    **short**    |        2       |        2       |    2   |
|     **int**     |        4       |        4       |    4   |
|     **long**    |        4       |        8       |    8   |
|    **float**    |        4       |        4       |    4   |
|    **double**   |        8       |        8       |    8   |
| **long double** |        -       |        -       |  10/16 |
|    _pointer_    |        4       |        8       |    8   |

- When we says that the computer is 64-bit ---> what we really mean is that the address is 64-bit

#### Boolean Algebra
> Developed by George Boole in 19th century
Encode "True" as 1 and "Flase" as 0
- **AND &** is true if both inputs are true
- **OR |** is true if either input is true
- **Not ~** will flip the input bit
- **XOR ^** is true when two inputs are different

**These Operations can be used to manipulate Set**
> When it comes to `Set`,
- **&** ---> intersection
- **|** ---> Union
- **^** ---> Symmetric difference
- **~** ---> Complement

These **Bit-level Operations** are available in C
- Apply to any **integral** data type:
    ```c
    long, int, short, char, unsigned
    ```

#### Programmers are usually mixed up & with &&, | with ||, ~ with !
**&&, ||, !** are Logical Operators
- View 0 as `false`
- View anything nonzero as `true`
- Always return `0` or `1`
- Early termination

```
//Remember we are not doing any bit-level operations, we focus on Logic
!0x41 = 0x00 
!0x00 = 0x01
!!0x41 = 0x01 

0x69 && 0x55 = 0x01
0x69 || 0x55 = 0x01
if (ptr) (avoids null pointer access)
```

#### Shift Operations
**Left Shift:  `x << y`**
- Shift bit-vector **x** left **y** positions
    - get rid os `y` bits on the left
    - fill with 0's on right
**Right Shift: `x >> y`**
- Shift bit-vector **x** right **y** positions
    - **Logic right-shift**
        - get rid of `y` bits on right
        - fill with 0's on left
    - **Arthmetic right-shift**
        - get rid of `y` bits on right
        - fill with **most significant** bits on left
**Undefined Behavior**
- Shift amount < 0 or >= word size


#### Number Representations
- **Unsigned**
<center>
![B2U](https://latex.codecogs.com/gif.latex?$$B2U(X)&space;=&space;\sum_{i=0}^{w&space;-&space;1}&space;x_i&space;\cdot&space;2^i&space;$$)
</center>
