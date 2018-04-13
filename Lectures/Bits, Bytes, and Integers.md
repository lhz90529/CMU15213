## [Bits, Bytes and Integers](https://www.cs.cmu.edu/~213/lectures/02-bits-ints-part1.pdf)

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
**&&, ||, !** are _Logical Operators_
- View 0 as `false`
- View anything nonzero as `true`
- Always return `0` or `1`
- Early termination

```
//Remember we are not doing any bit-level operations, the focus right now is Logic
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
##### Unsigned

![B2U](https://latex.codecogs.com/gif.latex?$$B2U(X)&space;=&space;\sum_{i=0}^{w&space;-&space;1}&space;x_i&space;\cdot&space;2^i&space;$$)

What the equation above saying is that add up all 
- digits * its corresponding weight

##### Two's Complement

![B2T](https://latex.codecogs.com/gif.latex?B2T(X)&space;=&space;-x_{w-1}&space;\cdot&space;2^{w-1}&space;&plus;&space;\sum&space;_{i&space;=&space;0}^{w-2}x_i&space;\cdot&space;2^i,&space;\text{&space;where&space;}&space;-x_{w-1}&space;\text{&space;the&space;\textbf{sign&space;bit}})

```
Example
      -16  8   4   2   1
10  =   0  1   0   1   0       8 + 2 = 10

      -16   8   4   2   1
-10 =   1   0   1   1   0      -16 + 4 + 2 = -10
```

#### Numeric Ranges
##### Unsigned Values
- UMin = 0
    > all bits are 0, ---> 000...0
- Umax = 2^w - 1
    > all bits are 1, ---> 111...1

##### Two's Complement Values
- TMin = -2^(w-1)
    > The first bit is 1, the rest are 0s, ----> 100...0
    Only the weight of the first bit (most significatn bit) has negative value, the rest are positive;
    Thus we want to maximize the negative contribtuion as well as minimize the positive contributions
- TMax = 2^(w-1) - 1
    > The first bit is 0, the rest are all 1s, ----> 011...1
    Conversely, we want to minimize the negative contribution and maximize the positive contributions
- `Special value -1`: All bits are 1, ---> 111...1

##### Observations
- |TMin| = TMax + 1
- UMax = 2 * TMax + 1


|                  |   X  | B2U(X) | B2T(X) |
|:----------------:|:----:|:------:|:------:|
|       UMin       | 0000 |    0   |    0   |
|                  | 0001 |    1   |    1   |
|                  | 0010 |    2   |    2   |
|                  | 0011 |    3   |    3   |
|                  | 0100 |    4   |    4   |
|                  | 0101 |    5   |    5   |
|                  | 0110 |    6   |    6   |
|       TMax       | 0111 |    7   |    7   |
|       TMin       | 1000 |    8   |   -8   |
|                  | 1001 |    9   |   -7   |
|                  | 1010 |   10   |   -6   |
|                  | 1011 |   11   |   -5   |
|                  | 1100 |   12   |   -4   |
|                  | 1101 |   13   |   -3   |
|                  | 1110 |   14   |   -2   |
| UMax, Special -1 | 1111 |   15   |   -1   |

> The Maximum number in unsigned representation will become -1 in Two's Complement

unsigned = Two's complement + 2^(w), where w is # of bits


#### Signed vs. Unsigned in C
##### Casting
- Explicit casting
    ```C
    int tx, ty;
    unsigned int ux, uy;
    tx = (int) ux;
    uy = (unsigned) ty;
    ```
- Implicit casting
    ```c
    tx = xy;
    uy = ty;
    ```

##### Casting Surprises
- If there is a mix of unsigned and signed in single expression, 
    - **signed value implicitly cast to unsigned value**
        - if either of the arguments is unsigned, then convert the other one into unsigned

```c
//This is actually a infinit loop, 
//since when i is 0, i.e. 000....0
//You try to minus 1 on it
//You get Umax, i.e. 111...1
//Thus goes forever
unsigned int i;
for (i = n - 1; i >= 0; i--) {
    //do something
}

//another infinite loop
//i - sizeof(char) is a expression of mix of signed integer with unsigned integer 
//Thus i will be implicitly convert to unsigned integer
int i;
for (i = n - 1; i - sizeof(char) >= 0; i--) {
    //do somthing
}
```

#### Sign Extension
> Given a w-bit signed integer, try to convert it into a larger size of w + k bits signed integer without changing its value
example: 8-bits signed integer ---> 16-bits signed integer

- Make k copies of sign bit
- append the rest at the right
```
Example:
Given a 4-bits signed integer 0110 ---> 2 + 4 = 6
Convert it to 5-bits signed integer
        0   1   1   0
    0   0   1   1   0   = 2 + 4 = 6 
    ^
    copy of sign bit 0
---------------------------------------------------------------------------------------
What if the given 4-bits signed integer is a negative number: 1110 ---> 2 + 4 + -8 = -2

        1   1   1   0
    1   1   1   1   0   = 2 + 4 + 8 + -16 = -2;
    ^
    copy of sign bit 1
```

#### Truncate unsigned integer



#### Summary
- **Expanding**
    - Unsigned: zeros added
    - Signed: copy of most sigificant bit
    - Both yield expected result
- **Truncating**
    - Unsinge / signed: bits are truncated
    - Result will be reinterpreted
    - Unsigned: behaves like the original number is mod by some number
    - Signed: similar to mod
    - will cause unexpected result