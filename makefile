main: .build/main.o
	gcc .build/main.o -o main

# create main.o, we need main.c
.build/main.o: src/main.c
# compile only, main.c output main.o
	gcc -c src/main.c -o .build/main.o


##main: main.o GUI.o open_save.o
##	gcc main.o GUI.o open_save.o `sdl2-config --libs` -o main

## GUI.o: GUI.c 
## 	gcc -c GUI.c `sdl2-config --cflags` -o GUI.o
## 
## main.o: main.c 
## 	gcc -c main.c `sdl2-config --cflags` -o main.o
## 
## open_save.o: open_save.c 
## 	gcc -c open_save.c -o open_save.o
## 