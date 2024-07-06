#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void eat(){
  puts("Yum. =)");
}

typedef struct cookie {
  char* name;
  void* func;
}cookie;

void win(){
  system("/bin/sh");
}

cookie* create_cookie(char* name){
  cookie* new_cookie = (cookie*)malloc(sizeof(cookie));
  int len = strlen(name);
  new_cookie->name = (char*)malloc(len+1);
  memcpy(new_cookie->name,name,len);
  new_cookie->func = (void*)eat;
  return new_cookie;
}

void eat_cookie(cookie* cookie){
  ((void(*)())cookie->func)();
}

void free_cookie(cookie* cookie){
  free(cookie->name);
  free(cookie);
}

int get_int(){
    char buf[10];
    memset(buf,0,10);
    fgets(buf,10,stdin);
    int index = atoi(buf);
    return index;
}

char* get_string(){
  char buf[600]= {0};
  fgets(buf,500,stdin);
  int len = strlen(buf);
  char* str = (char*) malloc(len);
  memcpy(str,buf,len+2);
  return str;
}

void menu(){
  puts("Print 1 to add cookie.");
  puts("Print 2 to eat cookie.");
  puts("Print 3 to exit.");
  puts(">>");
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
  cookie* arr[100];
  while(1){
    menu();
    int option = get_int();
    switch(option){ 
      case 1:
      {
        puts("Enter index.");
        puts(">>");
        int index = get_int()%100;
        puts("Enter name");
        puts(">>");
        char* name = get_string();
        arr[index] = create_cookie(name);
        puts("Successfully created.");
      }
      break;
      case 2:
      {
        puts("Enter index.");
        puts(">>");
        int index = get_int()%100;
        cookie* cookie = arr[index];
        eat_cookie(cookie);
        free_cookie(cookie);
      }
      break;
      case 3:
        return 0;
      break;
      default:
        puts("Invalid option.");
      break;
    }
  }
  return 0;
}
