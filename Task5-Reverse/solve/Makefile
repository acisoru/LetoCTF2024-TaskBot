fullbuild: main.c vm/vm.c
	gcc main.c vm/vm.c -g -o main

fullrun:  fullbuild
	valgrind ./main