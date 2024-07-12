#include "./vm/vm.h"
#include <stdlib.h>
#include <string.h>
char memory[0x1000*2]={0};

//letoctf{v1rtual_mach1ne_1s_a_c00l_th1ng_f0r_ctf}

int main(){
    setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

    // Инициализация виртуалки
    Vm vm = init_vm(memory+0x1000, memory, 0x1000, 0x1000);
    puts("Welcome to the virtual iasher machine (vim)!");
    puts("Vim is executing your code");

    // Считывание флага
    FILE *file = fopen("data.txt", "r");
    char number;
    for (int i = 0; i < 48; i++) {
        fscanf(file, "%c", &number);
        memory[i] = number;
        printf("%c", memory[i]);
    }
    printf("\n");
    fclose(file);

    // Считывание программы для виртуалки
    // Начало программы в memory[0x1000]
    char p[48 * 4 * 4];
    FILE *f = fopen("prog.txt", "r");
    for (int i = 0; i < 5 * 48 * 4; i += 4) {
        int opcode, op1, op2, s;
        fscanf(f, "%d %d %d %d", &opcode, &op1, &op2, &s);
        memory[0x1000 + i] = opcode;
        memory[0x1000 + i + 1] = op1;
        memory[0x1000 + i + 2] = op2;
        memory[0x1000 + i + 3] = s;
    }
    fclose(f);

    // Выполнение кода
    while(1) {
        Command* cmd = read_command(&vm);
        int result = execute_command(&vm, cmd);
        if(!result)
            break;
    }

    FILE *res = fopen("output.txt", "w");
    for (int i = 0; i < 48; i++) {
        fprintf(res, "%d", memory[i]);
        if (i != 47) {
            fprintf(res, " ");
        }
    }
    fclose(res);
}
