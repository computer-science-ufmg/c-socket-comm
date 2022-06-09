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
