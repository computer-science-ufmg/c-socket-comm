#include<time.h>

#include"common.h"

#define DEVICES_NUM 4
#define SENSORS_NUM 4

#define ADD_SENSORS_PATTERN "add sensor ((0[1-4] )+)in (0[1-4])"
#define LIST_SENSORS_PATTERN "list sensors in (0[1-4])"
#define REMOVE_SENSORS_PATTERN "remove sensor ((0[1-4] )+)in (0[1-4])"
#define READ_SENSORS_PATTERN "read ((0[1-4] )+)in (0[1-4])"

#define MAX_GROUPS 32

//==================== Types ====================//

typedef int devices_t[DEVICES_NUM][SENSORS_NUM];
typedef int device_t;
typedef int sensor_t;

devices_t devices;

typedef struct args_t {
  char* argv[MAX_GROUPS];
  int argc;
} args_t;

typedef struct add_sensor_args_t {
  int sensor_count;
  device_t device;
  sensor_t* sensors;
} add_sensor_args_t;

typedef struct list_sensor_args_t {
  device_t device;
} list_sensor_args_t;

typedef struct remove_sensor_args_t {
  int sensor_count;
  device_t device;
  sensor_t* sensors;
} remove_sensor_args_t;

typedef struct read_sensor_args_t {
  int sensor_count;
  device_t device;
  sensor_t* sensors;
} read_sensor_args_t;

regex_t add_sensors_pattern;
regex_t list_sensors_pattern;
regex_t remove_sensors_pattern;
regex_t read_sensors_pattern;


//==================== Utils ====================//

void init_regex() {
  regcomp(&add_sensors_pattern, ADD_SENSORS_PATTERN, REG_EXTENDED);
  regcomp(&list_sensors_pattern, LIST_SENSORS_PATTERN, REG_EXTENDED);
  regcomp(&remove_sensors_pattern, REMOVE_SENSORS_PATTERN, REG_EXTENDED);
  regcomp(&read_sensors_pattern, READ_SENSORS_PATTERN, REG_EXTENDED);
}

void init_devices() {
  for (device_t device = 0; device < DEVICES_NUM; device++) {
    for (sensor_t sensor = 0; sensor < SENSORS_NUM; sensor++) {
      devices[device][sensor] = 0;
    }
  }
}

void free_regex() {
  regfree(&add_sensors_pattern);
  regfree(&list_sensors_pattern);
  regfree(&remove_sensors_pattern);
  regfree(&read_sensors_pattern);
}

args_t* new_args() {
  args_t* args;
  args = (args_t*)malloc(sizeof(args_t));
  args->argc = 0;
  return args;
}

add_sensor_args_t* new_add_sensor_args(int sensor_count) {
  add_sensor_args_t* args;
  args = (add_sensor_args_t*)malloc(sizeof(add_sensor_args_t));
  args->sensor_count = sensor_count;
  args->sensors = (int*)malloc(sensor_count * sizeof(int));
  return args;
}

list_sensor_args_t* new_list_sensor_args() {
  list_sensor_args_t* args;
  args = (list_sensor_args_t*)malloc(sizeof(list_sensor_args_t));
  return args;
}

remove_sensor_args_t* new_remove_sensor_args(int sensor_count) {
  remove_sensor_args_t* args = (remove_sensor_args_t*)malloc(sizeof(remove_sensor_args_t));
  args->sensor_count = sensor_count;
  args->sensors = (sensor_t*)malloc(args->sensor_count * sizeof(sensor_t));
  args->device = 0;
  return args;
}

read_sensor_args_t* new_read_sensor_args(int sensor_count) {
  read_sensor_args_t* args = (read_sensor_args_t*)malloc(sizeof(read_sensor_args_t));
  args->sensor_count = sensor_count;
  args->sensors = (sensor_t*)malloc(args->sensor_count * sizeof(sensor_t));
  args->device = 0;
  return args;
}

int count_sensors() {
  int count = 0;
  for (device_t device = 0; device < DEVICES_NUM; device++) {
    for (sensor_t sensor = 0; sensor < SENSORS_NUM; sensor++) {
      if (devices[device][sensor] == 1) {
        count++;
      }
    }
  }
  return count;
}

float get_random_number(int max) {
  int d = rand();
  float r = (float)d / (float)RAND_MAX;
  return max * r;
}

//=================== Parsers ===================//

args_t* parse_regex_args(regex_t* pattern, char* req) {
  char req_copy[strlen(req) + 1];
  args_t* args = new_args();
  regmatch_t group_array[MAX_GROUPS];

  strcpy(req_copy, req);
  if (regexec(pattern, req, MAX_GROUPS, group_array, 0) == 0) {
    for (unsigned i = 0; i < MAX_GROUPS; i++) {
      if (group_array[i].rm_so == (size_t)-1) {
        break;
      }

      args->argv[i] = (req_copy + group_array[i].rm_so);
      args->argc++;
    }
  }

  printf("argc: %d\n", args->argc);
  printf("argv: ");
  for (int i = 0; i < args->argc; i++) {
    printf("%s, ", args->argv[i]);
  }
  printf("\b\b\n");

  return args;
}

add_sensor_args_t* parse_add_sensor(char* req) {
  args_t* args = parse_regex_args(&add_sensors_pattern, req);
  char* sensors_str = args->argv[1];
  int sensor_count = ((strlen(sensors_str) + 1) / 3) - 2;

  add_sensor_args_t* add_args = new_add_sensor_args(sensor_count);

  for (unsigned i = 0; i < (strlen(sensors_str) - 5); i += 3) {
    add_args->sensors[(int)i / 3] = sensors_str[i + 1] - '0';
  }
  add_args->device = atoi(args->argv[args->argc - 1]);

  printf("sensors: %d\n", add_args->sensor_count);
  printf("device: %d\n", add_args->device);
  free(args);
  return add_args;
}

list_sensor_args_t* parse_list_sensor(char* req) {
  args_t* args = parse_regex_args(&list_sensors_pattern, req);

  list_sensor_args_t* list_args = new_list_sensor_args();
  list_args->device = atoi(args->argv[args->argc - 1]);

  printf("device: %d\n", list_args->device);
  free(args);
  return list_args;
}

remove_sensor_args_t* parse_remove_sensor(char* req) {
  args_t* args = parse_regex_args(&remove_sensors_pattern, req);

  char* sensors_str = args->argv[1];
  int sensor_count = ((strlen(sensors_str) + 1) / 3) - 2;

  remove_sensor_args_t* remove_args = new_remove_sensor_args(sensor_count);

  for (unsigned i = 0; i < (strlen(sensors_str) - 5); i += 3) {
    remove_args->sensors[(int)i / 3] = sensors_str[i + 1] - '0';
  }
  remove_args->device = atoi(args->argv[args->argc - 1]);

  printf("sensors: %d\n", remove_args->sensor_count);
  printf("device: %d\n", remove_args->device);
  free(args);
  return remove_args;
}

read_sensor_args_t* parse_read_sensor(char* req) {
  args_t* args = parse_regex_args(&read_sensors_pattern, req);

  char* sensors_str = args->argv[1];
  int sensor_count = ((strlen(sensors_str) + 1) / 3) - 2;

  read_sensor_args_t* read_args = new_read_sensor_args(sensor_count);

  for (unsigned i = 0; i < (strlen(sensors_str) - 5); i += 3) {
    read_args->sensors[(int)i / 3] = sensors_str[i + 1] - '0';
  }
  read_args->device = atoi(args->argv[args->argc - 1]);

  printf("sensors: %d\n", read_args->sensor_count);
  printf("device: %d\n", read_args->device);
  free(args);
  return read_args;
}

//=================== Commands ===================//

void add_sensor(add_sensor_args_t* args, char* res) {
  char error[BUFFSIZE] = "sensor";

  for (int i = 0; i < args->sensor_count; i++) {
    if (devices[args->device - 1][args->sensors[i] - 1] == 1) {
      sprintf(error, "%s 0%d", error, args->sensors[i]);
    }
  }
  if (strlen(error) > strlen("sensor")) {
    sprintf(res, "%s already existis in 0%d", error, args->device);
    return;
  }


  if (count_sensors() + args->sensor_count > 15) {
    strcpy(res, "limit exceeded");
    return;
  }

  strcpy(res, "sensor");
  for (int i = 0; i < args->sensor_count; i++) {
    devices[args->device - 1][args->sensors[i] - 1] = 1;
    sprintf(res, "%s 0%d", res, args->sensors[i]);
  }
  strcat(res, " added");
}

void list_sensor(list_sensor_args_t* args, char* res) {
  int count = 0;

  strcpy(res, "");
  for (sensor_t sensor = 0; sensor < SENSORS_NUM; sensor++) {
    if (devices[args->device - 1][sensor - 1] == 1) {
      sprintf(res, "%s%s0%d", res, count == 0 ? "" : " ", sensor);
      count++;
    }
  }
}

void remove_sensor(remove_sensor_args_t* args, char* res) {
  char error[BUFFSIZE] = "sensor";

  for (int i = 0; i < args->sensor_count; i++) {
    if (devices[args->device - 1][args->sensors[i] - 1] == 0) {
      sprintf(error, "%s 0%d", error, args->sensors[i]);
    }
  }
  if (strlen(error) > strlen("sensor")) {
    sprintf(res, "%s does not exist in 0%d", error, args->device);
    return;
  }

  strcpy(res, "sensor");
  for (int i = 0; i < args->sensor_count; i++) {
    devices[args->device - 1][args->sensors[i] - 1] = 0;
    sprintf(res, "%s 0%d", res, args->sensors[i]);
  }
  strcat(res, " removed");
}

void read_sensor(read_sensor_args_t* args, char* res) {
  char error[BUFFSIZE] = "sensor(s)";
  int count = 0;

  for (int i = 0; i < args->sensor_count; i++) {
    if (devices[args->device - 1][args->sensors[i] - 1] == 0) {
      sprintf(error, "%s 0%d", error, args->sensors[i]);
    }
  }
  if (strlen(error) > strlen("sensor(s)")) {
    sprintf(res, "%s not installed", error);
    return;
  }

  strcpy(res, "");
  for (int i = 0; i < args->sensor_count; i++) {
    float value = get_random_number(10);
    sprintf(res, "%s%s%.2f", res, count == 0 ? "" : " ", value);
    count++;
  }
}

int run_command(char req[500], char res[500]) {
  printf("\nReceived: %s\n", req);
  if (regexec(&add_sensors_pattern, req, 0, NULL, 0) == 0) {
    add_sensor_args_t* args = parse_add_sensor(req);
    add_sensor(args, res);
    free(args->sensors);
    free(args);
  }
  else if (regexec(&list_sensors_pattern, req, 0, NULL, 0) == 0) {
    list_sensor_args_t* args = parse_list_sensor(req);
    list_sensor(args, res);
    free(args);
  }
  else if (regexec(&remove_sensors_pattern, req, 0, NULL, 0) == 0) {
    remove_sensor_args_t* args = parse_remove_sensor(req);
    remove_sensor(args, res);
    free(args->sensors);
    free(args);
  }
  else if (regexec(&read_sensors_pattern, req, 0, NULL, 0) == 0) {
    read_sensor_args_t* args = parse_read_sensor(req);
    read_sensor(args, res);
    free(args->sensors);
    free(args);
  }
  else {
    strcpy(res, "");
  }
  int size = strlen(res);
  printf("Response: %s\n", res);
  format_command_string(res);
  return size;
}

//==================== Main ====================//

int main(int argc, char const* argv[]) {
  socket_t sockfd, clientfd;
  addr_type_t addr_type;
  sockaddr_in_t addr_in;
  socklen_t addr_len = sizeof(addr_in);
  char req[BUFFSIZE], res[BUFFSIZE];
  size_t buffsize = BUFFSIZE;
  int port, domain, size, option = 1;

  srand(time(NULL));

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
    exit(1);
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option))) {
    perror("setsockopt");
    exit(2);
  }

  addr_in.sin_family = domain;
  addr_in.sin_addr.s_addr = INADDR_ANY;
  addr_in.sin_port = htons(port);

  if (bind(sockfd, (sockaddr_t*)&addr_in, addr_len) < 0) {
    perror("bind");
    exit(3);
  }

  if (listen(sockfd, 3) < 0) {
    perror("listen");
    exit(4);
  }

  if ((clientfd = accept(sockfd, (sockaddr_t*)&addr_in, &addr_len)) < 0) {
    perror("accept");
    exit(5);
  }

  printf("Connected\n");
  while (strncmp(req, "kill", 5) != 0) {
    if (read(clientfd, req, buffsize) == 0b00000000) {
      printf("Client disconnected, awaiting connection...\n");
      close(clientfd);
      if ((clientfd = accept(sockfd, (sockaddr_t*)&addr_in, &addr_len)) < 0) {
        perror("accept");
        exit(5);
      }
      printf("Connected\n");
      continue;
    }
    terminate_command_string(req);

    size = run_command(req, res);
    send(clientfd, res, buffsize, 0);
  }

  close(clientfd);
  close(sockfd);

  return 0;
}
