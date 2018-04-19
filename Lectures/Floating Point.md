## [Floating Point](https://www.cs.cmu.edu/~213/lectures/04-float.pdf)

#### Fractional binary Numbers
- **Representation**
<p align="center">
    <img src="https://latex.codecogs.com/gif.latex?\sum_{k&space;=&space;-j}^i&space;b_k&space;\times&space;2^k")
</p>

- **Example**
    ```
    5 3/4   --->    101.11  --->    4 + 1 . 1/2 + 1/4
                                        ^
                                        decimal point
    2 7/8   --->    10.111  --->    2 . 1/2 + 1/4 + 1/8 
    1 7/16  --->    1.0111  --->    1 . 1/4 + 1/8 + 1/16




    ```

- **Obeservations**
- shift right ---> divide by 2 (for unsigned case)
- shift left  ---> multiply by 2
- Number of `0.1111111...` are just slightly less than `1.0`
    - 1/2 + 1/4 + 1/8 + ... will give us _almost_ `1.0`
    - Use notation ![](https://latex.codecogs.com/gif.latex?1.0&space;-&space;\epsilon)

- **Limitations**
    - Can only represent numbers of the form x / 2^k
        - other rational number have repeating bit pattern
        - e.g.
            ```
                                                    Repeating Pattern
            1/3     0.01010101...           --->    01
            1/5     0.00110011...           --->    0011
            1/10    0.000110011000011...    --->    0011
            ```
    - Just one setting of binary point within the w bits
        - If we put the binary point too left, we cannot represent a very large number, but we gain more precision.
            ```
            ---------------------------------
            |   Integral    |   Fractional  |
            ---------------------------------
                            ^
                            binary point
            
            ********************************************
            
            If we shift the binary point to the left,
            ---------------------------------
            |Integral|      Fractional      |   small value but with good precision
            ---------------------------------
                        ^
                        binary point
            we cannot represent a very large number, since the Integral part has less bits
            we gain more precision, since we have more bit in Fractional part

            ********************************************
            
            If we shift the binary point to the right,
            ---------------------------------
            |       Integral     |Fractional|   large value but with bad precision
            ---------------------------------
                        ^
                        binary point
            we can represent a very large number, since the Integral part has more bit to use.
            But we lost much precision, since we have less bit in Fractional part
            ```

#### Floating Point
> Before 1985, floating point is a disaster since every computer manufacture has its own representation and you cannot predict what would happen if you move your program to another machine.
It's a very difficult situation

In 1985, IEEE introduce a standard floating point representation called **IEEE Floating Point** which was then adopted by all computer manufactures.

##### Numercial Form
<p align="center">
    <img src="https://latex.codecogs.com/gif.latex?(-1)^s&space;M&space;2^E")
</p>

- **Sign bit s:** whether the number is positive or negative
- **Significand M:** normally a fractional value in range[1.0, 2.0)
- **Exponent E:** weights value by power of two

**These threes value are encoded in 32-bit or 64-bit representation in the following way**
```
-----------------------------------------
| s | exp       | frac                  |
-----------------------------------------
  ^
  most significant ibt

s = most significant ibt
E can be obtained from exp field
M can be obtained from frac field
```

##### Precision options
- **Single Precision**
```
Total: 32-bits
-----------------------------------------
| s | exp       | frac                  |
-----------------------------------------
  1     8-bits            23-bits
```

- **Double Precision**
```
Total: 64-bits
---------------------------------------------------------
| s | exp         | frac                                |
---------------------------------------------------------
  1     11-bits                     52-bits
```

##### Three Kinds of floating point numbers
- Denorm when exp is all 0s `000000...`
- Special when exp is all 1s `111111...`
- Normalized when exp is other case `between 0000... to 1111...`


##### Normalized Value
- when exp is not all 0s and is not all 1s

<p align="center">
    <img src="https://latex.codecogs.com/gif.latex?(-1)^s&space;M&space;2^E")
</p>

- E = Exp - Bias
    - Exp: unsigned value of `exp` field
    - Bias = 2^(k-1) - 1, where k is the number of bit in `exp` field
        - Single precision: Bias is 2^(8-1) - 1 = 127
        - Double precision: Bias is 2^(11 - 1) - 1 = 1023
- M is encoded with implied leading 1: M = 1.xxxx...x
    - xxxx...x is the `frac` field
    - M is minimum when frac is all 0s `0000...000`
        - then M  = 1.0
    - M is maximum when frac is all 1s `1111...111`
        - then ![eq](https://latex.codecogs.com/gif.latex?M&space;=&space;2.0&space;-&space;\epsilon) 
- Example

```c
float F = 15213.0;
  s     exp                                  frac
-----------------------------------------------------------------------
| 0 | 0 1 0 0 0 1 0 0 | 1 1 0 1 1 0 1 1 0 1 1 0 1 0 0 0 0 0 0 0 0 0 0 |
----------------------------------------------------------------------- 
  1     8-bits                               23-bits
```

##### Denormalized Value
- when exp is all 0s `00000000`

<p align="center">
    <img src="https://latex.codecogs.com/gif.latex?(-1)^s&space;M&space;2^E")
</p>

- E = 1 - Bias
- M encoded with implied 0, i.e. M = 0.xx..x
    - xx...x is the `frac` field

##### Special Value
- when exp is all 1s `11111111`
- when `exp = 111...1, frac = 000...0`
    - This represent **INFINITY**
    -  will get this number when overflow is occured
        ```
        example:
        1.0 / 0.0 = +Inf
        1.0/-0.0 = -Inf
        ```
- when `exp = 111...1, frac != 000...0`
    - This is Not-a-Number (NaN), i.e. there is no answer 
        ```
        sqrt(-1) = NO answer  
        ```

#### Tiny Floating Point Example

```
Total: 8-bits
---------------------------
| s | exp       | frac    |
---------------------------
1     4-bits      3-bits
```
//TODO TABLE

##### Distribution of Values


#### Special Property of the IEEE Encoding
- Floating point zero is as same as integer 0
    - i.e. all bits are 0
-  Comparision: treat it as unsigned number
    -  Must first compare sign bit
    -  Must consider -0 = 0 ?
    -  NaN is problematic

##### Floating Point Operations: Basic Idea
- x + y = Round(x  + y)
- x * y = Round(x * y);
- **Basic Idea:**
    - first compute exact result
    - Round it to available bit we have
        - possibly overflow if exponent too large
        - possibly round to fit into frac 

##### Rounding Model
- Towards 0
- Round down (towards -Inf) 
- Round up (towards +Inf)
- Nearsest Even (default)
    >  rounds to the nearest value; if the number falls midway it is rounded to the nearest value with an even (zero) least significant bit
    ```
    7.8949999   --->    7.89 since 49999 is less than halfway       
    7.8950001   --->    7.90 since 50001 is larger than halfway
    7.8950000   --->    7.90 since 7.89 is an odd number but 7.90 is an even
    7.8850000   --->    7.80 since 7.80 is even
    if less than halfway,
        round down
    else if greater than halfway
        round up
    else// exactly halfway
        round to nearest even
    ```
##### Rounding Binary Numbers
- Binary Fractional Numbers
    - A binary number is even if the least significant bit is `0`
    - "Half way" is where the right of rounding postion is a 1 followed by all 0s, 
        ```
        x x ... x r 1 0 0 0 ... 0
                ^
                rounding position
        ```
    //Example table TODO

##### Floating Point Multiplication
- s = s1 ^ s2
    - same sign produce 0 ---> positive
    - diff sign produce 1 ---> negative
- Significand M = M1 * M2
- Exponenet E = E1 + E2

- Fixing
- What if M >= 2?
    - shift M to right ---> divide by 2
    - inrement E
- if E out of range
    - overflow to Infinity


##### Floating Point Addition




##### Floating Point in C
- `float` single precision
- `double` double precision
- **Conversion/Casting**
    - Casting between `int`, `float`, and `double` **CHANGES** bit representation
    - `double/float` -> `int`
        - Truncate fractional part
        - Behaveas like rounding towards to 0

    - `int`-> `double`
        - wouldn't lost anything
    - `int` -> `float`
        - will round