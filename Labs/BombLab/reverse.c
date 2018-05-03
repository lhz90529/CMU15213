
for (int i = 0; i < 6; ++i) {//from 4010f4 to 401151
    if (input[i] < 1 || input[i] > 6) {
        explode();
    } 
    for (int j = i + 1; j < 6; j++) {
        if (input[i] == input[j]) {
            explode();
        }
    }
}

for (int i = 0; i < 6; ++i) {//from 401153 to 40116d
    input[i] = 7 - input[i];
}

int size = 0;
for (int size = 0; size != 24; size = size + 4) {//from 40116f to 4011a9
    /* Table
    size:           curr:
        0                input[0]
        4                input[1]
        8                input[2]
        12               input[3]
        16               input[4]
        20               input[5]
        */   
    int curr = input[size];//0(0), 4(), 8, 
    int* ptr = 0x6032d0;
    if (cur > 1) {
        for (int i = 1; i != curr; ++i) {
            //move one byte of memory stored at the address of %rdx + 8 to %rdx
            //move one byte from *(%rdx + 8)
            update (ptr); // 0x8(%rdx),%rdx
        }
    }
    //move one byte of memory stored in %rdx to 
    Mem at (20 + input + 2 * size) = ptr;// %rdx,0x20(%rsp,%rsi,2)
}
