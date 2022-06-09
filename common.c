#include"common.h"

addr_type_t get_addr_type(char const* name) {
  if (strncmp(name, "v4", 3) == 0) {
    return ADDR_TYPE_IPV4;
  }
  if (strncmp(name, "v6", 3) == 0) {
    return ADDR_TYPE_IPV6;
  }
  return DEFAULT_ADDR_TYPE;
}

int get_port(char const* port) {
  int p = atoi(port);
  if (p == 0) {
    return PORT;
  }
  return p;
}

int read_message(char* buff, int size){
  char c;
  int pos = 0;
  while(pos < (size - 1) && !feof(stdin) && (c = getchar())){
    if(c == '\n' || c == '\0'){
      break;
    }
    buff[pos] = c;
    pos++;
  }
  buff[pos] = '\n';
  pos++;
  return pos;
}

void terminate_command_string(char* command){
  int command_end = (int)(strchr(command, '\n') - command);
  command[command_end] = '\0';
}
