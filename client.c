#include"common.h"

int main(int argc, char const* argv[]) {
  socket_t serverfd, sockfd;
  sockaddr_in_t serv_addr;
  addr_type_t addr_type;
  socklen_t addr_len = sizeof(serv_addr);
  char req[BUFFSIZE], res[BUFFSIZE];
  char const* host;
  size_t buffsize = BUFFSIZE;
  int port, domain;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
    return 1;
  }

  host = argv[1];
  addr_type = match_addr_type(host);
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
    printf("Connection Failed\n");
    return -1;
  }

  while (!feof(stdin) && strncmp(req, "kill", 5)) {
    int size = read_message(req, buffsize);

    if (size > 1) {
      send(serverfd, req, buffsize, 0);
      terminate_command_string(req);

      read(serverfd, res, buffsize);
      terminate_command_string(res);
      if (strlen(res) > 1) {
        printf("< %s\n", res);
      }
    }
  }

  close(serverfd);
  close(sockfd);

  return 0;
}
