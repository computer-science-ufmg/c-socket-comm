#include"common.h"

int main(int argc, char const* argv[]) {
  socket_t sockfd, clientfd;
  addr_type_t addr_type;
  sockaddr_in_t addr_in;
  socklen_t addr_len = sizeof(addr_in);
  int port, domain;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <addr_type> <port>\n", argv[0]);
    return 1;
  }

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

  return 0;
}
