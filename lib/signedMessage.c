#include <stdio.h>
#include <stdlib.h>

#include "signedMessage.h"

int getBitArrayFromSignedMessage(unsigned int* bits, struct SignedMessage signedMessage) {
    unsigned long int *mac = signedMessage.mac;
    int k, cursor = 0;
    for (int i = 0 ; i < 16; ++i) {
        for (int c = 31; c >= 0; c--)
        {
            k = mac[i] >> c;
            bits[cursor++] = k & 1;
        }
    }

    unsigned char *message = signedMessage.message;
    int i = 0;
    while(message[i] != '\0') {
        for (int c = 7; c >= 0; c--) {
            k = message[i] >> c;
            bits[cursor++] = k & 1;
        }
        ++i;
    }
    return cursor;
}


void getSignedMessageFromBitArray(struct SignedMessage signedMessage, unsigned int* bits, int size) {
    char longData[32];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 32; j++) {
            longData[j] = bits[i * 32 + j] + '0';
        }
        signedMessage.mac[i] = strtol(longData, 0, 2);
    }
    char charData[9];
    int cursor = 16 * 32;
    int count = 0;
    while (cursor < size) {
        for (int j = 0; j < 8; j++) {
            charData[j] = bits[cursor + j] + '0';
        }
        charData[8] = '\0';
        signedMessage.message[count] = strtol(charData, 0, 2);
        cursor += 8;
        count++;
    }
}