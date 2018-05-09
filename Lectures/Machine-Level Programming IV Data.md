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
    - **Visualization**
        ```
                        -----------------------------------------
        zip_dig cmu :   |1      |5      |2      |1      |3      |
                        -----------------------------------------
                        ^       ^       ^       ^       ^       ^
                        16      20      24      28      32      36
        ```
    - **C code**    
        ```c
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
    ![Row-Major Ordering](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Row-Major%20Ordering.PNG)

- **Visualization**
    ```
    A[0][0]         ...      A[0][C - 1]
        
        .                       .
        .                       .
        .                       .

    A[R - 1][0]     ...     A[R - 1][C - 1] 
    ```
    - **small example but useful**
        ```c
        int A[3][5];
        /*  How to intepret A:
            A is an array of 3 elements
            Each element is an array of 5 ints
        */
        ```
- **Nested Array Example**
    ```c
    #define PCOUNT 4
    zip_dig pgh[PCOUNT] = 
        { {1, 5, 2, 0, 6}
          {1, 5, 2, 1, 3}
          {1, 5, 2, 1, 7}
          {1, 5, 2, 2, 1} };
    ``` 
    
    ![Row-Major Ordering Example](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Row-Major%20Ordering%20Example.PNG)    

    - `zip_dig pgh[4]` is equivalent to `int pgh[4][5]`
        - variable `pgh` is an array of 4 elements, allocated continously
        - Each element is an array of 5 `ints`, allocated continously
    - **Row-Major ordering of all elements in memory**

- **Nested Array Row Access**
    - **Row Vectors**
        - **`A[i]`** is array of _C_ elements, where `C` is number of columns
        - Each element of type `T` requires _K_ bytes
        - Starting Address `A + i * (C * K)`
    - **Array Elements**
        - **`A[i][j]`** is element of type `T`, which requires `K` bytes
        - Address `A + i * (C * K) + j * K = A + (i * C + j) * K` 
            - where `A + i * (C * K)` is the starting address of row `i`
            - where `j * K` is the offset from starting address such that you can reach element on the `jth` column 
    ![Nested Array Element Access](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Nested%20Array%20Element%20Access.PNG)


- **Multi-Level Array Example**
    ```c
    zip_dig cmu = { 1, 5, 2, 1, 3 };
    zip_dig mit = { 0, 2, 1, 3, 9 };
    zip_dig ucb = { 9, 4, 7, 2, 0 };

    #define UCOUNT 3
    int *univ[UCOUNT] = {mit, cmu, ucb};
    ```
    
    ![Multi-Level Array Example](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Multi-Level%20Array%20Example.PNG)
    
    - variable `univ` denotes array of 3 elements
    - Each element is a `pointer` 
    - Each pointer points to an array of `int`

- **Element Access in Multi-Level Array**
    - **C code** 
        ```c 
        int get_univ_digit (size_t index, size_t digit) {
            return univ[index][digit];
        }
        ```
    - **Assembly**
        ```assembly
        salq $2, %rsi               # 4*digit
        addq univ(,%rdi,8), %rsi    # p = univ[index] + 4*digit
        movl (%rsi), %eax           # return *p
        ret
        ```
    - **Computation**
        - Element access **`Mem[Mem[univ + 8 * index] + 4 * digit]`**
        - Must do 2 memory reads
            - First get pointer to row array
            - Then access element within that array

- **Nested Array vs Multi-level Array**
    ![Nested Array vs Multi-level Array](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Nested%20Array%20vs%20Multi-level%20Array.PNG)

    - Nested Array: Only one memory reference
    - Multi-level Array: Two memory references
    - **Even though their `C code` looks exactly same, but the underlying data type are different so the reference is different**


- **N * N Matrix Code**
    - **Fixed dimensions**
        - Know value of `N` at compile time
        - **C code**
            ```c
            #define N 16
            typedef int fix_matrix[N][N];
            /* Get element A[i][j] */
            int fix_ele(fix_matrix A, size_t i, size_t j) {
                return A[i][j];
            }
            ```
    - **Variable dimensions, explicit indexing**
        - Traditionaly way to implement **`dynamic array`**
        - **C code**
            ```c
            #define IDX(n, i, j) ((i)*(n)+(j))
            /* Get element A[i][j] */
            int vec_ele(size_t n, int *A, size_t i, size_t j) {
                return A[IDX(n,i,j)];
            }
            ```
        - **Variable dimensions, implicit indexing**
            - Now support by gcc
            - **C code**
                ```c
                /* Get element a[i][j] */
                int var_ele(size_t n, int A[n][n],
                    size_t i, size_t j) {
                    return A[i][j];
                }
                ```
    - **16 * 16 Matrix Access**
        - **Array Elements**
            - Address `A + i * (C * K) + j * K`
            - C = 16, K = 4
            - **C Code**
                ```c
                /* Get element A[i][j] */
                int fix_ele(fix_matrix A, size_t i, size_t j) {
                    return A[i][j];
                }
                ```
            - **Assembly**
                ```assembly
                                            # A in %rdi, i in %rsi, j in %rdx
                salq $6, %rsi               # 64*i
                addq %rsi, %rdi             # A + 64*i
                movl (%rdi,%rdx,4), %eax    # M[A + 64*i + 4*j]
                ret
                ```
                - `64 * i` stands for `i * C(16) * K(4)`
                - `4 * j` stands for `j * K`

    - **n * n Matrix Access**
        - **Array Elements**
            - Address `A + i * (C * K) + j * K`
            - `C = n`, `K = 4`
            - Must perform integer multiplication
            - **C code**
                ```c
                /* Get element A[i][j] */
                int var_ele(size_t n, int A[n][n], size_t i, size_t j) {
                    return A[i][j];
                }
                ```
            - **Assembly**
                ```
                                            # n in %rdi, A in %rsi, i in %rdx, j in %rcx
                imulq %rdx, %rdi            # n*i
                leaq (%rsi,%rdi,4), %rax    # A + 4*n*i
                movl (%rax,%rcx,4), %eax    # A + 4*n*i + 4*j
                ret
                ```    
                - `4 * n * i` stands for `i * C(n) * K(4)    
                - `4 * j` stands fro `j * K(4)`


### Structures 
#### Structure Representation
```c
struct rec {
    int a[4];
    size_t i;
    struct rec *next;
};
```
![visualization](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Structure%20Representation.PNG)

##### Structure represented as block of memory
- **Big enough to hold all of the fields**

##### Fields ordered according to declaration
- **Even if another ordering could yield a more compact representation**

##### Compiler determines overall size + positions of fields
- **Machine-level program has no understanding of the structures in the source code**

#### Generating Pointer to Structure Member
![](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Generating%20Pointer%20to%20Structure%20Member.PNG)
- **C code**
    ```c
    struct rec {
        int a[4];
        size_t i;
        struct rec *next;
    };

    int *get_ap (struct rec *r, size_t idx) {
        return &r->a[idx];
    }
    ```
- **Assembly**
    ```assembly
                                # r in %rdi, idx in %rsi
    leaq (%rdi,%rsi,4), %rax    # %rdi is the base pointer
    ret                         # %rsi is the index
    ```

#### Following Linked List
![](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Following%20Linked%20List.PNG)
- **C code**
    ```c
    struct rec {
        int a[4];
        int i;
        struct rec *next;
    };
    void set_val (struct rec *r, int val) {
        while (r) {
            int i = r->i;
            r->a[i] = val;
            r = r->next;
        }
    }
    ```
- **Assembly**
    ```assembly
    .L11:                       # loop:
    movslq 16(%rdi), %rax       # i = r->i: retrive i from struct
    movl %esi, (%rdi,%rax,4)    # M[r+4*i] = val
    movq 24(%rdi), %rdi         # r = M[r+24] : update pointer r
    testq %rdi, %rdi            # Test r : loop condition
    jne .L11                    # if !=0 goto loop
    ```

#### Structure & Alignment
![](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Structures%20%26%20Alignment.PNG)

#### Alignment Principle
- **Aligned Data**
    - Primitive data type requires `K` bytes
    - Address must be multiple of `K`
    - Required on some machines, advised on `x86-64`
- **Motivation for Aligning Data**
    - Memory accessed by (aligned) chunks of 4 or 8 bytes (system dependent)
        - Inefficient to load or store datum that spans cache lines (64 bytes). Intel states should avoid crossing 16 byte boundaries.     
        - Virtual memory trickier when datum spans 2 pages (4 KB pages) 

#### Specific Cases of Alignment (x86-64)
- **1 byte: char,...**
    - no restrictions on address
- **2 bytes: short, ...**
    - lowest 1 bit of address must be `0`
- **4 bytes: int, float,...**
    - lowest 2 bit of address must be `0`
- **8 bytes: double, long, ptr, ...**
    - lowest 3 bits of address must be 000

#### Satisfying Alignment with Structures
```c
struct S1 {
    char c;
    int i[2];
    double v;
} *p;
```
![Satisfying Alignment with Structures](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Satisfying%20Alignment%20with%20Structures.PNG)
- **Within structure**
    - Must satisfy each element’s alignment requirement
- **Overall structure placement**
    - Each structure has alignment requirement `K`
        - **K = Largest alignment of any element**
    - Initial address & structure length must be multiples of K
- **Example:**
    - `K = 8`, due to `double` element


#### Meeting Overall Alignment Requirement
```c
struct S2 {
    double v;
    int i[2];
    char c;
} *p;
```
![](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Meeting%20Overall%20Alignment%20Requirement.PNG)

- **For largest alignment requirement K**
- **Overall structure must be multiple of K**

#### Array of Structures
- **Overall structure length multiple of K**
- **Satisfy alignment requirement for every element**

    ```c 
    struct S2 {
        double v;
        int i[2];
        char c;
    } a[10];
    ```
    ![Arrays of Structures](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Arrays%20of%20Structures.PNG)

#### Saving Space
![](https://raw.githubusercontent.com/lhz90529/CMU15213/master/pics/Saving%20Space.PNG)
- **declare your `fields` in a way that minimize the amount of wasted space**
- **Can be solved by a Greedy approach**
    - **Put the biggest stuff at the begining, and then successively smaller elements.** 
        - You will minimize the amount of wasted space;



### Floating Point
> way more complex than integer