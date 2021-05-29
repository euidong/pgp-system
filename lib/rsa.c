#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <termios.h>
#include "rsa.h"

#define MAX_SIZE 1024

int prime(long int);
long int cd(long int, long int);


int prime(long int pr)
{
    int i, j;
    j=sqrt(pr);
    for(i=2;i<=j;i++)
    {
        if(pr%i==0)
            return 0;
    }
    return 1;
}

long int cd(long int x, long int t)
{
    long int k=1;
    while(1)
    {
        k=k+t;
        if(k%x==0)
            return(k/x);
    }
}

void writeKey(char *fileName, long int N, long int E, long int D) {
    FILE* fp;
    fp = fopen(fileName, "w");
    char content[MAX_SIZE];
    sprintf(content, "public: %ld %ld\nprivate: %ld %ld", N, E, N, D);
    fwrite(content, 1, strlen(content), fp);
    fclose(fp);
}

void generateKey(long int p, long int q, char *fileName) {
    if (prime(p) == 0 || prime(q) == 0 || p==q) {
        printf("WRONG INPUT\n");
        return;
    }
    long int N =p*q;
    long int T =(p-1)*(q-1);
    long int E[100], D[100];
    int k;
    k=0;
    for(int i=2;i<T;i++)
    {
        if(T%i==0)
            continue;
        int flag=prime(i);
        if(flag==1&&i!=p&&i!=q)
        {
            E[k]=i;
            flag=cd(E[k], T);
            if(flag>0)
            {
                D[k]=flag;
                k++;
            }
            if(k==99)
                break;
        }
    }
    printf("\nPOSSIBLE VALUES OF e AND d ARE\n");
    int j = sqrt(q);
    for(int i=0;i<j-1;i++)
        printf("\n%ld\t%ld",E[i],D[i]);
    printf("\n");
    writeKey(fileName, N, E[0], D[0]);
}

unsigned long int* getKeyFromFile(char* keyFile, KeyType keyType) {
    FILE *fp = fopen(keyFile, "r");
    char line[MAX_SIZE];
    char *keyStrAry[2];
    fgets(line, MAX_SIZE, fp);
    if (keyType == PUBLIC_KEY) {
        keyStrAry[0] = strtok(line, " ");
        keyStrAry[0] = strtok(NULL, " ");
        keyStrAry[1] = strtok(NULL, " ");
    } else {
        fgets(line, MAX_SIZE, fp);
        keyStrAry[0] = strtok(line, " ");
        keyStrAry[0] = strtok(NULL, " ");
        keyStrAry[1] = strtok(NULL, " ");
    }
    unsigned long int key[2] = { atol(keyStrAry[0]), atol(keyStrAry[1])};
    return key;
}

void encryptMsg(unsigned long int* dest, char* keyFile, KeyType keyType, unsigned char* message, int size) {
    unsigned long int *key = getKeyFromFile(keyFile, keyType);
    unsigned long int pt, ct, k;
    int cursor = 0;
    int len=size;

    while(cursor != len)
    {
        pt = message[cursor];
        k = 1;

        for(int l = 0; l < key[1]; l++)
        {
            k = k * pt;
            k = k % key[0];
        }
        ct = k;
        dest[cursor] = ct;
        cursor++;
    }
}

void decryptMsg(unsigned char* dest, char* keyFile, KeyType keyType, unsigned long int* encodedMessage, int size) {
    unsigned long int *key = getKeyFromFile(keyFile, keyType);
    unsigned long int pt, ct, k;
    int cursor = 0;

    while(cursor != size)
    {
        ct = encodedMessage[cursor];
        k = 1;
        for(int j = 0; j < key[1]; j++)
        {
            k = k * ct;
            k = k % key[0];
        }
        pt = k;
        dest[cursor] = pt;
        cursor++;
    }
}