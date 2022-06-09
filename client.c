#include"common.h"

int main(int argc, char const* argv[]) {
  socket_t sockfd, serverfd;
  sockaddr_in_t serv_addr;
  addr_type_t addr_type;
  socklen_t addr_len = sizeof(addr_in);
  int port, domain, addr_len = sizeof(serv_addr);

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

  serv_addr.sin_family = domain;
  serv_addr.sin_port = htons(port);

  if ((serverfd = connect(sockfd, &serv_addr, addr_len)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  return 0;
}
