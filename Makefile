
all: client
 
client: client.o bignumber.o
    gcc -o client client.o bignumber.o
 
client.o: client.c bignumber.h
    gcc -o client.o client.c -c -std=c99 -Wall -Wextra -Wvla -g

bignumber.o: bignumber.c bignumber.h
    gcc -o bignumber.o bignumber.c -c -std=c99 -Wall -Wextra -Wvla -g