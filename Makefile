CC	  = g++
CFLAGS  = -g


default: all

all: xor_encrypt

xor_encrypt: xor_encrypt.cpp
	$(CC) $(CFLAGS) -o xor_encrypt xor_encrypt.cpp

clean veryclean:
	rm -f xor_encrypt

be:
	./xor_encrypt B input output keyfile E
	cat output

bd:
	./xor_encrypt B output decrypted keyfile D
	cat decrypted

se:
	./xor_encrypt S input output keyfile E
	cat output

sd:
	./xor_encrypt S output decrypted keyfile D
	cat decrypted