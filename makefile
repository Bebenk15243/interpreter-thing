main: .build/main.o .build/lexer.o
	gcc $^ -o $@

# create main.o, we need main.c
.build/main.o: src/main.c
# compile only, main.c output main.o
	gcc -c src/main.c -o $@

.build/memory.o: src/memory.c
	gcc -c src/memory.c -o $@

.build/lexer.o: src/lexer.c
	gcc -c src/lexer.c -o $@

