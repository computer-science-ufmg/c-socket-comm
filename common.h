#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 51511
#define BUFFSIZE 500

typedef enum addr_type {
  ADDR_TYPE_IPV4,
  ADDR_TYPE_IPV6,
} addr_type_t;

typedef struct sockaddr_in sockaddr_in_t;
typedef struct sockaddr sockaddr_t;
typedef int socket_t;

#define DEFAULT_ADDR_TYPE ADDR_TYPE_IPV4

addr_type_t get_addr_type(char const* name);
int get_port(char const* port);
int read_message(char* buff, int size);
void terminate_command_string(char* command);
