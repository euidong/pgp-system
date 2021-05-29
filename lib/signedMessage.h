struct SignedMessage {
    unsigned char *message;
    unsigned long int *mac;
};

int getBitArrayFromSignedMessage(unsigned int* bits, struct SignedMessage signedMessage);
void getSignedMessageFromBitArray(struct SignedMessage signedMessage, unsigned int* bits, int size);