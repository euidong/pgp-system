#include <stdio.h>
#include <string.h>

#include "lib/md5.h"
#include "lib/rsa.h"
#include "lib/signedMessage.h"
#include "lib/sdes.h"

// 수신단
// 1. Decoding
//  - Base64 Decoding 수행
// 2. Confidentiality
//  - 대칭키 수신단의 private key로 복호화[RSA]
//  - 메세지 복호화 [SDES]
// 3. Decompression
//  - 해당 message 압축 해제
// 4. Authentication
//  - Message와 HMAC 분리
//  - Message Hashing[MD5]
//  - HMAC을 송신단의 public key로 복호화[RSA]
//  - 서로 비교


int main(int n, char *args[n]) {
    if (n != 3) return -1;
    unsigned long int encryptedSessionKey[10];
    unsigned int bits[10000];

    FILE *enfp2 = fopen(args[1], "r");

    fseek(enfp2, 0, SEEK_END);
    int bitSize = ftell(enfp2);
    fseek(enfp2, 0, SEEK_SET);

    bitSize -= (sizeof(long int) * 10);
    bitSize /= sizeof(int);

    fread(encryptedSessionKey, sizeof(long int), 10, enfp2);
    fread(bits, sizeof(int), bitSize, enfp2);
    bits[bitSize] = '\0';
    fclose(enfp2);

    // session key 복호화
    unsigned char decryptedSessionKey[10];
    decryptMsg(decryptedSessionKey, "keyring/sender.txt", PRIVATE_KEY, encryptedSessionKey, 10);

    printf("decryptedSessionKey:    ");
    for (int i = 0; i < 10; i++) {
        decryptedSessionKey[i] -= 16;
        printf ("%d |", decryptedSessionKey[i]);
    }
    printf("\n");

    int sessionKeyInt[10];
    for (int i = 0; i < 10; i++) sessionKeyInt[i] = decryptedSessionKey[i];

    for (int i = 0; i < bitSize / 8; i++) {
        En_De(bits + (i * 8), 1, sessionKeyInt);
    }

    unsigned long int mac2[16];
    unsigned char msg2[1024];
    struct SignedMessage signedMessage2 = { msg2, mac2 };

    getSignedMessageFromBitArray(signedMessage2, bits, bitSize);

    printf("mac:                    ");
    for (int i = 0; i < 16; ++i) {
        printf("%ld |", signedMessage2.mac[i]);
    }
    printf("\n");

    printf("Message:\n%s\n", signedMessage2.message);

    // mac 복호화
    unsigned char decryptedMac[16];
    decryptMsg(decryptedMac, "keyring/receiver.txt", PUBLIC_KEY, signedMessage2.mac, 16);

    printf("decrpyedMac:            ");
    MDPrint(decryptedMac);
    printf("\n");

    // message hash
    unsigned char digest2[1024];
    strcpy(digest2, signedMessage2.message);
    MDString(digest2);
    printf("digest2:                ");
    MDPrint(digest2);

    int flag = 1;
    for (int i = 0; i < 16; i++) {
        if (digest2[i] != decryptedMac[i]) {
            flag = 0;
            break;
        }
    }

    if (flag) {
        printf("\ncorrect!!!");
        FILE *defp = fopen(args[2], "w");
        fputs(signedMessage2.message, defp);

        fclose(defp);
    } else {
        printf("digest is not equal?!");
    }

    return 0;
}