#include "vm.h"
#include <stdlib.h>

Vm init_vm(void* code,void* data,size_t code_size,size_t data_size){
        Vm new_vm;
        new_vm.code.size=code_size;
        new_vm.data.size=data_size;
        new_vm.code.mem_ptr = code;
        new_vm.data.mem_ptr = data;
        new_vm.ip=code;
        new_vm.code.flags=READABLE|EXECUTABLE; //real rwx
        new_vm.data.flags=READABLE|WRITEABLE; //real rwx
        return new_vm;
}; //init vm start state

Command* read_command(Vm* vm){
    Command* cmd = (Command*)vm->ip;
    return cmd;
}

void Mov(Vm* vm,Command* cmd){
    switch(cmd->opcode){
    case MOVRR:
    if(cmd->op1 >=3 || cmd->op2>=3)
        exit(128);
    vm->regs.regs[cmd->op1]=vm->regs.regs[cmd->op2];
    break;
    case MOVRM:
    if(cmd->op1 >=3)
        exit(128);
    vm->regs.regs[cmd->op1]=*(size_t*)((long)vm->data.mem_ptr+cmd->op2);
    break;
    case MOVMR:
    if(cmd->op2>=3)
        exit(128);
    *(size_t*)((long)vm->data.mem_ptr+cmd->op1)=vm->regs.regs[cmd->op2];
    break;
    }
}

void In(Vm* vm,Command* cmd){
    char* buffer = (char*)vm->data.mem_ptr+cmd->op1;
    fgets(buffer,cmd->op2,stdin);
}

void Out(Vm* vm,Command* cmd){
    char* buffer = (char*)vm->data.mem_ptr+cmd->op1;
    printf("%s\n",buffer);
}

void Jmp(Vm* vm,Command* cmd){
    switch(cmd->opcode){
    case JMPV:
        vm->ip = (char*)vm->code.mem_ptr+cmd->op1;
    break;
    case JZV:
    if(vm->eflags & ZERO)
        vm->ip = (char*)vm->code.mem_ptr+cmd->op1;
    break;
    case JGZV:
    if(vm->eflags & ZERO || vm->eflags & GREATER)
        vm->ip = (char*)vm->code.mem_ptr+cmd->op1;
    break;
    }
    execute_command(vm,cmd);
}

void Cmp(Vm* vm,Command* cmd){
    vm->eflags = 0;

    if(cmd->op1>=3 || cmd->op2 >=3)
        exit(128);

    size_t r1 =vm->regs.regs[cmd->op1];
    size_t r2 = vm->regs.regs[cmd->op2];
    if(r1-r2==0)
        vm->eflags|=ZERO;
    else if(r1-r2>0)
        vm->eflags|=GREATER;
}

void Add(Vm* vm,Command* cmd){
    switch(cmd->opcode){
    case ADDRV:
    if(cmd->op1>=3)
        exit(128);
    vm->regs.regs[cmd->op1]+=cmd->op2;
    break;
    case ADDRR:
    if(cmd->op1>=3 || cmd->op2>=3)
        exit(128);
    vm->regs.regs[cmd->op1]+=vm->regs.regs[cmd->op2];
    break;
    }
}

void Xor(Vm* vm,Command* cmd){
    switch(cmd->opcode){
    case XORRV:
    if(cmd->op1>=3)
        exit(128);
    vm->regs.regs[cmd->op1]^=cmd->op2;
    break;
    case XORRR:
    if(cmd->op1>=3 || cmd->op2>=3)
        exit(128);
    vm->regs.regs[cmd->op1]^=vm->regs.regs[cmd->op2];
    break;
    }
}

void Sub(Vm* vm,Command* cmd){
    switch(cmd->opcode){
    case SUBRV:
    if(cmd->op1>=3){
        puts("Register oob in sub :(");
        exit(128);
    }
    vm->regs.regs[cmd->op1]-=cmd->op2;
    break;
    case SUBRR:
    if(cmd->op1>=3 || cmd->op2>=3){
        puts("Register oob in sub :(");
        exit(128);
    }
    vm->regs.regs[cmd->op1]-=vm->regs.regs[cmd->op2];
    break;
    }

}


int execute_command(Vm* vm,Command* cmd){
    switch(cmd->opcode){
    case MOVRR:
    case MOVRM:
    case MOVMR:
        Mov(vm,cmd);
        break;
    case IN:
        In(vm,cmd);
        break;
    case OUT:
        Out(vm,cmd);
        break;
    case SUBRV:
    case SUBRR:
        Sub(vm,cmd);
    case XORRV:
    case XORRR:
        Xor(vm,cmd);
        break;
    case ADDRR:
    case ADDRV:
        Add(vm,cmd);
        break;
    case JMPV:
    case JZV:
    case JGZV:
        Jmp(vm,cmd);
        break;
    case CMPRR:
        Cmp(vm,cmd);
        break;
    case 0:
    return FALSE;
    break;
    default:
        puts("Unknown opcode :(");
        exit(127);
    }

    vm->ip=(void*)((long)vm->ip+sizeof(Command));
    return TRUE;
}
