make debug:
	gcc -g -std=c99 -pedantic -Wvla -Wall -Wshadow *.c -o pa4

make pa4:
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 *.c -o pa4