#include <stdio.h>
#include <stdlib.h>
 
const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

typedef unsigned char int8;

typedef struct dynamic_arr {
    int8* buf;
    int len;
    int capacity;
} array;

array* inicialize_array() {
    array* arr    = (array*)malloc(sizeof(array));
    arr->len      = 0;
    arr->capacity = 100;
    arr->buf      = (int8*)malloc(sizeof(int8) * 100);
    return arr;
}

void add(array* arr, int8 val) {
    if(arr->capacity == arr->len) {
        int8* aux = (int8*)malloc(sizeof(int8) * arr->capacity * 2);

        for(int i = 0; i < arr->capacity; i++) {
            aux[i] = arr->buf[i];
        }

        free(arr->buf);

        arr->capacity = arr->capacity * 2;
        arr->buf = aux;
    }

    arr->buf[arr->len] = val;
    arr->len++;
}


// [DEBUG]
//
// void print_bits(int num, int n) {
//     if(n <= 0) {
//         return;
//     }
 
//     print_bits(num >> 1, n-1);
//     printf("%d", num&1);
// }

void mapping(array* arr, int pad) {
    for(int i = 0; i < arr->len; i++) {
        arr->buf[i] = i+pad >= arr->len ? '=' : b64[arr->buf[i]];
    }
}

void print_arr(array* arr) {
    for(int i = 0; i < arr->len; i++) {
        printf("%c", arr->buf[i]);
    }
    //printf("\n");
}

array* b64_encode(int8* buf, int len) {
    int pad = 0;

    if(len % 3 > 0) {
        pad       = (3 - len % 3);
        int8* aux = (int8*)malloc(sizeof(int8) * len + (3 - len % 3));

        for(int i = 0; i < len + (3 - len%3); i++) {
            aux[i] = i < len ? buf[i] : 0;
        }

        buf = aux;
        len = len + (3 - len%3);
    }

    array* arr  = inicialize_array();

    int byte;

    for(int i = 0; i < len; i+=3) {
        byte = 0;

        for(int j = 0; j < 3; j++) {
            byte = byte | (buf[i+j]<<(16-(j*8)));
        }

        for(int j = 0; j < 4; j++) {
            add(arr, (byte & (0x3F << (18-(j*6)))) >> 18-(j*6));
        }
    }
    
    mapping(arr, pad);
    return arr;
}

int main() {
    int8 bytes[] = "Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of \"de Finibus Bonorum et Malorum\" (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, \"Lorem ipsum dolor sit amet..\", comes from a line in section 1.10.32.\
\
\n\nThe standard chunk of Lorem Ipsum used since the 1500s is reproduced below for those interested. Sections 1.10.32 and 1.10.33 from \"de Finibus Bonorum et Malorum\" by Cicero are also reproduced in their exact original form, accompanied by English versions from the 1914 translation by H. Rackham.";
 
    array* arr = b64_encode(bytes, sizeof(bytes)-1);
    print_arr(arr);
    return 0;
}
