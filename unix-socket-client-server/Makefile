SERVER:=server
SERVER_OBJECTS:=server.o
CC:=gcc

all: build-server

build-server: $(SERVER_OBJECTS)
	$(CC) -o $(SERVER) $(SERVER_OBJECTS)

clean:
	rm -rf $(OBJECTS)
