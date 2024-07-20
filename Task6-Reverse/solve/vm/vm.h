#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#define TRUE 1
#define FALSE 0
//

#define READABLE 0x1
#define WRITEABLE 0x2
#define EXECUTABLE 0x3 


//flags
#define ZERO 0x1
#define GREATER 0x2
//commands
#define MOVRR 49
#define MOVRM 50
#define MOVMR 51

#define IN 52
#define OUT 53

#define ADDRV 54
#define SUBRV 55
#define XORRV 56


#define ADDRR 57
#define SUBRR 58
#define XORRR 59


#define JMPV 60
#define JZV 61
#define JGZV 62

#define CMPRR 63
#define CMPRV 64

//

typedef struct Command{
    signed char opcode;
    signed char op1;
    signed char op2;
    signed char padding;  //4 byte command
}Command;

typedef struct Memory{
    int flags; //READABLE || WRITEABLE || EXECUTABLE
    void* mem_ptr;
    size_t size;
}Memory;

typedef struct Registers{
    size_t regs[3];
}Registers;

typedef struct Vm{
    int eflags;
    Memory data;
    Memory code;
    void* ip;
    Registers regs;
}Vm;

Vm init_vm(void* code,void* data,size_t code_size,size_t data_size);
Command* read_command(Vm* vm);
int execute_command(Vm* vm,Command* cmd);






