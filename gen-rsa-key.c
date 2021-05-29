#include<stdlib.h>
#include "lib/rsa.h"

// ./gen-rsa-key.out prime1 prime2 fileName
int main(int n, char* args[n]) {
    generateKey(atol(args[1]), atol(args[2]), args[3]);
}