#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib/md5.h"
#include "lib/rsa.h"
#include "lib/signedMessage.h"
#include "lib/sdes.h"

// 송신단
// 1. Authentication
//  - Hashing[MD5]
//  - 송신단의 private key로 암호화[RSA] (HMAC생성)
//  - Message와 HMAC 합치기
// 2. Compression
//  - 해당 message 압축
// 3. Confidentiality
//  - 대칭키 생성[random generate]
//  - 암호화 [SDES]
//  - 대칭키를 수신단의 public key로 암호화 [RSA]
// 4. Encoding
//  - Base64 Encoding 수행



// keyring 필요
// 실행 시에 txt 파일 입력 받기
int main(int n, char *args[n]) {
    if (n != 3) return -1;
    FILE *fp;
    fp = fopen(args[1], "r");
    if (fp == NULL) {
        printf("file is not exist.");
        fclose(fp);
        return -1;
    }
    unsigned char *msg;
    int size;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    msg = malloc(size + 1);
    memset(msg, 0, size + 1);

    fseek(fp, 0, SEEK_SET);
    fread(msg, size, 1, fp);
    fclose(fp);

    printf("Message:\n%s\n", msg);

    // message hashing
    unsigned char digest[1024];
    strcpy(digest, msg);
    MDString(digest);

    printf("digest:                 ");
    MDPrint(digest);
    printf("\n");

    // mac 생성
    unsigned long int mac[16];
    encryptMsg(mac, "keyring/receiver.txt", PRIVATE_KEY, digest,16);

    struct SignedMessage signedMessage = { msg, mac };

    unsigned int bits[10000];
    int bitSize = getBitArrayFromSignedMessage(bits, signedMessage);


    printf("mac:                    ");
    for (int i = 0; i < 16; ++i) {
        printf("%ld |", mac[i]);
    }
    printf("\n");

    unsigned char sessionKey[10];
    srand(time(NULL));
    for (int i = 0; i < 10; ++i) {
        sessionKey[i] = (rand() % 2);
    }

    int sessionKeyInt[10];
    for (int i = 0; i < 10; i++) sessionKeyInt[i] = sessionKey[i];
    for (int i = 0; i < bitSize / 8; i++) {
        En_De(bits + (i * 8), 0, sessionKeyInt);
    }


    printf("sessionKey:             ");
    for (int i = 0; i < 10; i++) {
        printf ("%d |", sessionKey[i]);
        sessionKey[i] += 16;
    }
    printf("\n");


    // session key 암호화
    unsigned long int encryptedSessionKey[10];
    encryptMsg(encryptedSessionKey, "keyring/sender.txt", PUBLIC_KEY, sessionKey,10);


    // encryptedSessionKey 암호화 session key
    // bits = 암호화된 text

    FILE *enfp = fopen(args[2], "w");
    fwrite(encryptedSessionKey, sizeof(long int), 10, enfp);
    fwrite(bits, sizeof(int), bitSize, enfp);
    fclose(enfp);

    free(msg);
}

