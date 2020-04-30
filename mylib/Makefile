CC       = gcc 
CFLAGS   = -Wall -g -O -fPIC
CXXFLAGS = 
INCLUDE  = -I ./include
TARGET   = libmytest.so
LIBPATH  = ./libs/

#vpath %.h ./include

OBJS     = simple.o
SRCS     = ./src/simple.c

all:$(OBJS)
	$(CC) -shared -fPIC -o $(TARGET) $(OBJS)

$(OBJS):$(SRCS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $^


clean:
	rm -f *.o *.so
