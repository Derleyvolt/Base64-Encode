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

int8 unmapping(int8 c) {
    return c >= 'A' && c <= 'Z' ? c - 'A' : c >= 'a' && c <= 'z' ? c + 26 - 'a' : c >= '0' && c <= '9' ? c + 52 - '0' : c == '+' ? 62 : c == '/' ? 63 : 0; 
}

void print_arr(array* arr) {
    for(int i = 0; i < arr->len; i++) {
        printf("%d ", arr->buf[i]);
    }
    printf("\n");
}

array* b64_decode(int8* buf, int len) {
    int byte;

    array* arr = inicialize_array();

    int ignore = 0;

    for(int i = len-1; i >= 0 && buf[i] == '='; i--) {
        ignore++;
    }

    for(int i = 0; i < len; i++) {
        buf[i] = unmapping(buf[i]);
    }

    for(int i = 0; i < len*6/8; i+=4) {
        byte = 0;
        for(int j = 0; j < 4; j++) {
            byte = byte | (buf[i+j] << (18-(j*6)));
        }

        for(int k = 0; k < 3; k++) {
            add(arr, (byte & (0xFF << (16-(k*8)))) >> (16-(k*8)));
        }
    }

    arr->len = arr->len-ignore;
    return arr;
}

int main() {
    int8 buf[] = { 'A', 'A', 'A', 'B', 'E', 'Q', 'A', 'A', 'A', 'Q', 'A', 'A' };
    array* out = b64_decode(buf, 12);
    print_arr(out);
    return 0;
}