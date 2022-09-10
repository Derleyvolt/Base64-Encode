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
    arr->capacity = 10000;
    arr->buf      = (int8*)malloc(sizeof(int8) * 10000);
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
        printf("%c", arr->buf[i]);
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

    for(int i = 0; i < len; i+=4) {
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
    int8 buf[] = "Q29udHJhcnkgdG8gcG9wdWxhciBiZWxpZWYsIExvcmVtIElwc3VtIGlzIG5vdCBzaW1wbHkgcmFuZG9tIHRleHQuIEl0IGhhcyByb290cyBpbiBhIHBpZWNlIG9mIGNsYXNzaWNhbCBMYXRpbiBsaXRlcmF0dXJlIGZyb20gNDUgQkMsIG1ha2luZyBpdCBvdmVyIDIwMDAgeWVhcnMgb2xkLiBSaWNoYXJkIE1jQ2xpbnRvY2ssIGEgTGF0aW4gcHJvZmVzc29yIGF0IEhhbXBkZW4tU3lkbmV5IENvbGxlZ2UgaW4gVmlyZ2luaWEsIGxvb2tlZCB1cCBvbmUgb2YgdGhlIG1vcmUgb2JzY3VyZSBMYXRpbiB3b3JkcywgY29uc2VjdGV0dXIsIGZyb20gYSBMb3JlbSBJcHN1bSBwYXNzYWdlLCBhbmQgZ29pbmcgdGhyb3VnaCB0aGUgY2l0ZXMgb2YgdGhlIHdvcmQgaW4gY2xhc3NpY2FsIGxpdGVyYXR1cmUsIGRpc2NvdmVyZWQgdGhlIHVuZG91YnRhYmxlIHNvdXJjZS4gTG9yZW0gSXBzdW0gY29tZXMgZnJvbSBzZWN0aW9ucyAxLjEwLjMyIGFuZCAxLjEwLjMzIG9mICJkZSBGaW5pYnVzIEJvbm9ydW0gZXQgTWFsb3J1bSIgKFRoZSBFeHRyZW1lcyBvZiBHb29kIGFuZCBFdmlsKSBieSBDaWNlcm8sIHdyaXR0ZW4gaW4gNDUgQkMuIFRoaXMgYm9vayBpcyBhIHRyZWF0aXNlIG9uIHRoZSB0aGVvcnkgb2YgZXRoaWNzLCB2ZXJ5IHBvcHVsYXIgZHVyaW5nIHRoZSBSZW5haXNzYW5jZS4gVGhlIGZpcnN0IGxpbmUgb2YgTG9yZW0gSXBzdW0sICJMb3JlbSBpcHN1bSBkb2xvciBzaXQgYW1ldC4uIiwgY29tZXMgZnJvbSBhIGxpbmUgaW4gc2VjdGlvbiAxLjEwLjMyLgoKVGhlIHN0YW5kYXJkIGNodW5rIG9mIExvcmVtIElwc3VtIHVzZWQgc2luY2UgdGhlIDE1MDBzIGlzIHJlcHJvZHVjZWQgYmVsb3cgZm9yIHRob3NlIGludGVyZXN0ZWQuIFNlY3Rpb25zIDEuMTAuMzIgYW5kIDEuMTAuMzMgZnJvbSAiZGUgRmluaWJ1cyBCb25vcnVtIGV0IE1hbG9ydW0iIGJ5IENpY2VybyBhcmUgYWxzbyByZXByb2R1Y2VkIGluIHRoZWlyIGV4YWN0IG9yaWdpbmFsIGZvcm0sIGFjY29tcGFuaWVkIGJ5IEVuZ2xpc2ggdmVyc2lvbnMgZnJvbSB0aGUgMTkxNCB0cmFuc2xhdGlvbiBieSBILiBSYWNraGFtLg==";

    
    array* out = b64_decode(buf, sizeof(buf)-1);
    print_arr(out);
    return 0;
}
