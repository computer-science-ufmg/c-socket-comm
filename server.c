#include"common.h"

#define ADD_SENSORS_PATTERN "add sensor (0[1-4] )+in 0[1-4]"
#define LIST_SENSORS_PATTERN "list sensors in 0[1-4]"
#define REMOVE_SENSORS_PATTERN "remove sensor (0[1-4] )+in 0[1-4]"
#define READ_SENSORS_PATTERN "read (0[1-4] )+in 0[1-4]"

regex_t add_sensors_pattern;
regex_t list_sensors_pattern;
regex_t remove_sensors_pattern;
regex_t read_sensors_pattern;

void init_regex() {
  regcomp(&add_sensors_pattern, ADD_SENSORS_PATTERN, REG_EXTENDED);
  regcomp(&list_sensors_pattern, LIST_SENSORS_PATTERN, REG_EXTENDED);
  regcomp(&remove_sensors_pattern, REMOVE_SENSORS_PATTERN, REG_EXTENDED);
  regcomp(&read_sensors_pattern, READ_SENSORS_PATTERN, REG_EXTENDED);
}

void free_regex() {
  regfree(&add_sensors_pattern);
  regfree(&list_sensors_pattern);
  regfree(&remove_sensors_pattern);
  regfree(&read_sensors_pattern);
}

void add_sensor(char* req, char* res) {
  strcpy(res, "Add sensor");
}

void list_sensor(char* req, char* res) {
  strcpy(res, "List sensor");
}

void remove_sensor(char* req, char* res) {
  strcpy(res, "Remove sensor");
}

void read_sensor(char* req, char* res) {
  strcpy(res, "Read sensor");
}

int run_command(char req[500], char res[500]) {
  if (regexec(&add_sensors_pattern, req, 0, NULL, 0) == 0) {
    add_sensor(req, res);
  }
  else if (regexec(&list_sensors_pattern, req, 0, NULL, 0) == 0) {
    list_sensor(req, res);
  }
  else if (regexec(&remove_sensors_pattern, req, 0, NULL, 0) == 0) {
    remove_sensor(req, res);
  }
  else if (regexec(&read_sensors_pattern, req, 0, NULL, 0) == 0) {
    read_sensor(req, res);
  }
  else {
    strcpy(res, "");
  }
  int size = strlen(res);
  format_command_string(res);
  return size;
}

int main(int argc, char const* argv[]) {
  socket_t sockfd, clientfd;
  addr_type_t addr_type;
  sockaddr_in_t addr_in;
  socklen_t addr_len = sizeof(addr_in);
  char req[BUFFSIZE], res[BUFFSIZE];
  size_t buffsize = BUFFSIZE;
  int port, domain, size;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <addr_type> <port>\n", argv[0]);
    return 1;
  }

  init_regex();

  addr_type = get_addr_type(argv[1]);
  port = get_port(argv[2]);

  domain = addr_type == ADDR_TYPE_IPV4 ? AF_INET : AF_INET6;

  if ((sockfd = socket(domain, SOCK_STREAM, 0)) == 0) {
    perror("socket");
    return 1;
  }

  addr_in.sin_family = domain;
  addr_in.sin_port = htons(port);

  if (bind(sockfd, (sockaddr_t*)&addr_in, addr_len) < 0) {
    perror("bind");
    return 1;
  }

  if (listen(sockfd, 3) < 0) {
    perror("listen");
    exit(1);
  }

  if ((clientfd = accept(sockfd, (sockaddr_t*)&addr_in, &addr_len)) < 0) {
    perror("accept");
    exit(1);
  }

  printf("Connected\n");
  while (strncmp(req, "kill", 5) != 0) {
    read(clientfd, req, buffsize);
    terminate_command_string(req);

    size = run_command(req, res);
    send(clientfd, res, buffsize, 0);
  }

  close(clientfd);
  close(sockfd);

  return 0;
}
