main: .build/main.o .build/memory.o 
	gcc .build/main.o .build/memory.o -o main

# create main.o, we need main.c
.build/main.o: src/main.c
# compile only, main.c output main.o
	gcc -c src/main.c -o .build/main.o

.build/memory.o: src/memory.c
	gcc -c src/memory.c -o .build/memory.o
