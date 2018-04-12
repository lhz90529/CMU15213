## [Overview](https://www.cs.cmu.edu/~213/lectures/01-overview.pdf)
### Useful Outcome
Become more **effective** programmers
- Able to find and eliminate bugs efficiently
- Able to understand and tune for program performance

#### Great Reality #1:
**Int are not integers, Floats are not reals**
Example1: 
$$x^2 \geq 0 \space?$$
- Float: Yes
- Int: 
    - 40000 * 40000 -> 1600000000
    - 50000 * 50000 -> -1794967296 (that is not what we want, it is a very strange number)
    - 300 * 400 * 500 * 600 -> 1640261632 (same thing happens again)
```
Do this with lldb
print 40000 * 40000
(int) $0 = 160000000
print 50000 * 50000
(int) $1 = -1794967296
print 300 * 400 * 500 * 600
(int) $2 = 1640261632
print 400 * 500 * 600 * 300 
(int) $3 = 1640261632 ---> Communitive property is hold
```
Example2:
$$ (x + y) + z == x + (y + z) \space?$$
This is **Associative property**
- Int: Yes
- Float:
    - Yes, (1e20 + -1e20) + 3.14 -> 3.14
    - No, 1e20 + (-1e20 + 3.14) -> 0
    > since 3.14 is so insigificant to -1e20, thus -1e20 + 3.14 = -1e20, then you add 1e20 + -1e20, you get 0

_The examples above are **overflow** of integer and float_

#### Great Reality #2:
**You've Got to Know Assembly**
- **You will never write program in assembly**
    - Compilers are much better & more patient than you are
- **But understanding assembly is key to machine-level execution model**

#### Great Reality #3:
**Memory Matters**
Random Access Memory Is an Unphysical Abstraction
- **Memory is not unbounded**
    - It must be allocated and managed
- **Memory referencing bugs especially pernicious**
    - Effects are distant in both time and space
- **Memory performance is not uniform**
    - Cache and virtual memory effects can greatly affect program performance
    - Adapting program to characteristics of memory system can lead to major speed improvements

```c
//Example
#include <stdio.h>

typedef struct {
    int a[2];
    double d;
} struct_t;

double fun(int i) {
    
    volatile struct_t s;
    s.d = 3.14;
    s.a[i] = 1073741824; /* Possibly out of bounds */
    return s.d;
}

int main() {
    int i;
    for (i = 0; i < 7; ++i) {
        fun(i);
    }
}
-------------------------------------------------------
fun(0) --> 3.14
fun(1) --> 3.14
fun(2) --> 3.1399998664856
fun(3) --> 2.00000061035156
fun(4) --> 3.14
fun(6) --> Segmentation fault
//C program doesn't have bound-checking
//The valid input for fun() should be either 0 or 1 
//since the array in our struct is space of 2
//as the argument i increase, we will modify the internal state of function fun()
//which cause stack overflow
```
**Memory Referencing Errors**
- **C and C++ do not provide any memory protection**
    - Out of bounds array references
    - Invalid pointer values
    - Abuses of malloc/free

#### Great Reality #4:
**There’s more to performance than asymptotic complexity**
- **Constant factors matter too!**
- **And even exact op count does not predict performance**
- **Must understand system to optimize performance**
```c++
//copy element row by row from src to dst
void copyij(int src[2048][2048], int dst[2048][2048]) {
    int i,j;
    for (i = 0; i < 2048; i++)
    for (j = 0; j < 2048; j++)
    dst[i][j] = src[i][j];
}

//copy element col by col from src to dst
void copyji(int src[2048][2048], int dst[2048][2048]) {
    int i,j;
    for (j = 0; j < 2048; j++)
    for (i = 0; i < 2048; i++)
    dst[i][j] = src[i][j];
}
/*Runtime:
Even though the 2 functions do exactly the same thing, 
their performance vary significantly
row-by-row: 15.6ms
col-by-col: 58.4ms
*/
```
![copyij](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/copyij.PNG)

#### Great Reality #5:
**Computers do more than execute programs**
- I/O
- Communicate over the network

#### Course perspective: Programmer-centric

#### Textbook
- _Computer Systems: A Programmer’s Perspective, Third Edition (CS:APP3e), Pearson, 2016_
- _The C Programming Language, Second Edition, Prentice Hall, 1988_