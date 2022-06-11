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

addr_type_t match_addr_type(char const* addr) {
  regex_t ipv4_pattern;
  if (regcomp(&ipv4_pattern, "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}", 0) != 0) {
    return ADDR_TYPE_IPV4;
  }

  if (regexec(&ipv4_pattern, addr, 0, NULL, 0) == 0) {
    return ADDR_TYPE_IPV4;
  }
  else {
    return ADDR_TYPE_IPV6;
  }
}

int get_port(char const* port) {
  int p = atoi(port);
  if (p == 0) {
    return PORT;
  }
  return p;
}

int read_message(char* buff, int size) {
  char c;
  int pos = 0;
  while (pos < (size - 1) && !feof(stdin) && (c = getchar())) {
    if (c == '\n' || c == '\0') {
      break;
    }
    buff[pos] = c;
    pos++;
  }
  buff[pos] = '\n';
  pos++;
  return pos;
}

void terminate_command_string(char* command) {
  char* char_pos = strchr(command, '\n');
  if (char_pos) {
    *char_pos = '\0';
  }
}

void format_command_string(char* command) {
  int command_end = (int)(strchr(command, '\0') - command);
  command[command_end] = '\n';
}
