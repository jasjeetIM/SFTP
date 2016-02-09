TARGET=P1

OBJS= \
main.o \
Server.o \
Client.o 

CC=gcc
CCFLAGS= -g -o 
LFLAG= -c

REBUILDABLES=$(OBJS) $(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CCFLAGS) $@ $^ -lpthread

clean: 
	rm -rf $(REBUILDABLES)


main.o: main.c
Server.o: Server.c Server.h
Client.o: Client.c Client.h 	
