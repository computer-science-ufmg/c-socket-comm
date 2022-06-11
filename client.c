#include"common.h"

int main(int argc, char const* argv[]) {
  socket_t serverfd, sockfd;
  sockaddr_in_t serv_addr;
  addr_type_t addr_type;
  socklen_t addr_len = sizeof(serv_addr);
  char command[BUFFSIZE], response[BUFFSIZE];
  char const* host;
  size_t buffsize = BUFFSIZE;
  int port, domain;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <addr_type> <port>\n", argv[0]);
    return 1;
  }

  host = argv[1];
  port = get_port(argv[2]);

  domain = AF_INET;

  if ((serverfd = socket(domain, SOCK_STREAM, 0)) == 0) {
    perror("socket");
    return 1;
  }

  serv_addr.sin_family = domain;
  serv_addr.sin_addr.s_addr = inet_addr(host);
  serv_addr.sin_port = htons(port);

  if ((sockfd = connect(serverfd, (sockaddr_t*)&serv_addr, addr_len)) < 0) {
    close(serverfd);
    printf("\nConnection Failed \n");
    return -1;
  }

  while (!feof(stdin) && strncmp(command, "kill", 5)) {
    int size = read_message(command, buffsize);

    if (size > 1) {
      send(serverfd, command, size, 0);
      terminate_command_string(command);
      // printf("%s\n", command);

      // read(serverfd, response, buffsize);
      // terminate_command_string(command);
      // printf("%s\n", response);
    }
  }

  close(serverfd);
  close(sockfd);

  return 0;
}
