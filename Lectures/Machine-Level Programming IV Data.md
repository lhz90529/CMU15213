## Machine-Level Programming IV: Data
### Arrays
- **Basic Principle**
    - `T A[L]`: Array of data type `T` and length `L`
    - Contiguously allocated region of `L * sizeof(T)` bytes in memory
        - `L`: number of elements within the array
        - `sizeof(T)`: number of bytes to represent each element
    - **Example:**
        ```
                     ----------------------------------------
        int val[5] : |1     |5      |2      |1      |3      |
                     ----------------------------------------
                     ^      ^       ^       ^       ^       ^
                     x      x + 4   x + 8   x + 12  x + 16  x + 20
        ```

        |  Reference  |           Type          |   Value   |
        |:-----------:|:-----------------------:|:---------:|
        |    val[4]   |           int           |     3     |
        |     val     |          int *          |     x     |
        |   val + 1   |          int *          |   x + 4   |
        |   &val[2]   |          int *          |   x + 8   |
        |    val[5]   | Undefined: out of range |           |
        | *(val  + 1) |           int           |     5     |
        |   val + i   |           int*          | x + 4 * i |

- **Array Example**
    ```c
    //A good form of array:
    #define ZLEN 5 // Length
    typedef int zip_dig[ZLEN];
    /*  typedef is a very handy way 
        if you are going to create complex data structures
        HIGHLY RECOMMEND TO USE
    */ 
    zip_dig cmu     = {1, 5, 2, 1, 3};
    zip_dig mit     = {0, 2, 1, 3, 9};
    zip_dig umass   = {0, 1, 0, 0, 2};
    ```
    - declaration of `zip_dig cmu` equivalent to `int cmu[5]`
    - the array is allocated in successive 20 byte blocks

- **Array Example cmu**
    - **C code**
        ```c
        //visualization
                        -----------------------------------------
        zip_dig cmu :   |1      |5      |2      |1      |3      |
                        -----------------------------------------
                        ^       ^       ^       ^       ^       ^
                        16      20      24      28      32      36

        //C code
        int get_digit (zip_dig z, int digit) {
            return z[digit];
        }
        ```
    - **Assembly**
        ```assembly
        # %rdi = z
        # %rsi = digit
        movl (%rdi, %rsi, 4), %eax  # z[digit]
        ```
        - register `%rdi` contains `starting address of array`
        - register `%rsi` contains `array index`
        - desired digit at `%rdi + 4 * %rsi`
        - Use memory reference
            - `(%rdi, %rsi, 4)`

- **Array Loop Example**
    - **C code**
        ```c
        void zincr(zip_dig z) {
            size_t i;
            for (i = 0; i < ZLEN; i++) {
                z[i]++;
            }
        }
        ```
    - **Assembly**
        ```assembly
                                # %rdi = z
        movl $0, %eax           # i = 0
        jmp .L3                 # goto middle
        .L4:                    # loop:
        addl $1, (%rdi,%rax,4)  # z[i]++
        addq $1, %rax           # i++
        .L3:                    # middle
        cmpq $4, %rax           # i:4
        jbe .L4                 # if <=, goto loop
        rep; ret
        ```

#### Multi-dimensional (Nested) Arrays
- **Declaration**
    - `T A[R][C]`: 2D array of type `T`
    - `R`: rows, `C`L columns
    - Type `T` element requires `K` bytes
- **Array size**
    - `R * C * K`, where `R * C` is the number of elements
- **Arrangement**
    - Row-Major Ordering
- **Visualization**
    ```
    A[0][0]         ...      A[0][C - 1]
        
        .                       .
        .                       .
        .                       .

    A[R - 1][0]     ...     A[R - 1][C - 1] 
    ```

#### Multi-level

### Structures 
#### Allocation
#### Access
#### Alignment

### Floating Point