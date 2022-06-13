CC=gcc
CFLAGS=-Wno-format-overflow

SERVER_TARGET_NAME=./server
CLIENT_TARGET_NAME=./client
BUILD_PATH=./build

all: $(SERVER_TARGET_NAME) $(CLIENT_TARGET_NAME)

$(BUILD_PATH)/%.o: %.c %.h
	dirname $@ | xargs mkdir -p
	$(CC) -c $(CFLAGS) $< -o $@

$(SERVER_TARGET_NAME): ./server.c $(BUILD_PATH)/common.o
	$(CC) $(CFLAGS) ./server.c $(BUILD_PATH)/common.o -o $(SERVER_TARGET_NAME)

$(CLIENT_TARGET_NAME): ./client.c $(BUILD_PATH)/common.o
	$(CC) $(CFLAGS) ./client.c $(BUILD_PATH)/common.o -o $(CLIENT_TARGET_NAME)

clean:
	rm -rf $(BUILD_PATH)/*
	rm $(SERVER_TARGET_NAME)
	rm $(CLIENT_TARGET_NAME)
