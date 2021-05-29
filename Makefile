CC: gcc

OBJS = pgp-sender.o pgp-receiver.o gen-rsa-key.o md5.o rsa.o signedMessage.o sdes.o

run: build
	clear
	./gen-rsa-key 37 11 keyring/receiver.txt
	./gen-rsa-key 29 17 keyring/sender.txt
	./pgp-sender Text.txt result/EText.txt
	./pgp-receiver result/EText.txt result/DText.txt

build: gen-rsa-key pgp-sender pgp-receiver
	make gen-rsa-key
	make pgp-sender
	make pgp-receiver
	make clean

gen-rsa-key:	gen-rsa-key.o rsa.o
	gcc -o gen-rsa-key gen-rsa-key.o rsa.o

gen-rsa-key.o:	gen-rsa-key.c
	gcc -c gen-rsa-key.c

pgp-sender: pgp-sender.o md5.o rsa.o signedMessage.o sdes.o
	gcc -o pgp-sender pgp-sender.o md5.o rsa.o signedMessage.o sdes.o

pgp-sender.o: pgp-sender.c
	gcc -c pgp-sender.c

pgp-receiver: pgp-receiver.o md5.o rsa.o signedMessage.o sdes.o
	gcc -o pgp-receiver pgp-receiver.o md5.o rsa.o signedMessage.o sdes.o

pgp-receiver.o: pgp-receiver.c
	gcc -c pgp-receiver.c

rsa.o:	lib
	gcc -c lib/rsa.c

md5.o: lib
	gcc -c lib/md5.c

sdes.o: lib
	gcc -c lib/sdes.c

signedMessage.o: lib
	gcc -c lib/signedMessage.c

clean:
	rm -rf $(OBJS)