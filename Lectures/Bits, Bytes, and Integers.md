##[Bits, Bytes and Integers](https://www.cs.cmu.edu/~213/lectures/02-bits-ints-part1.pdf)

#### Representing information as bits
- Byte = 8 bits
- Binary: ``000000000 to 1111111111``
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

- 64-bit ---> the address is 64-bit

#### Boolean Algebra
- **AND &** is true if both inputs are true
- **OR |** is true if either input is true
- **Not ~** is simple to flip the input bit
- **XOR ^** is true when two inputs are different

**These Operations can be used to manipulate Set**
- **&** become intersection
- **|** Union
- **^** Symmetric difference
- **~** Complement

These **Bit-level Operations** are available in C

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

**Right Shift: `x >> y`**
- Shift bit-vector **x** right **y** positions

**Undefined Behavior**
- Shift amount < 0 or >= word size


#### Number Representations
 