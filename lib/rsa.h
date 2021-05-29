typedef enum _KeyType {
    PUBLIC_KEY,
    PRIVATE_KEY
} KeyType;

void generateKey(long int p, long int q, char *fileName);
void decryptMsg(unsigned char* dest, char* keyFile, KeyType keyType, unsigned long int* encodedMessage, int size);
void encryptMsg(unsigned long int * dest, char* keyFile, KeyType keyType, unsigned char* message, int size);