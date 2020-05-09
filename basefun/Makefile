TARGET = mod_basefun.so

SRCDIR = src/
#链接库名，带上-l，-lsvpn 静态库填写绝对路径
LIB = 

PREFIX_LIB =

APACHECTL=apachectl

INCLUDE += -I./include -I/usr/include/httpd -I/usr/include/apr-1

CFLAGS += -fPIC -std=c++11 -DLINUX=2 -D_REENTRANT -D_GNU_SOURCE -pthread -DLDAP_DEPRECATED -DDEBUG
LFLAGS += -shared -fPIC 

CC = gcc
CXX = g++

CPP_SOURCES = $(foreach d,$(SRCDIR),$(wildcard $(d)*.cpp)) 
CPP_OBJS = $(patsubst %.cpp, $(OBJDIR)%.o, $(CPP_SOURCES))
OBJS += $(CPP_OBJS)

.PHONY: clean all _all _clean _install

#功能是告诉make生成.o文件的源文件是.c和.cpp
.SUFFIXES: .c .cpp .o

#".c.o"相当于"%o : %c"，但需配合SUFFIXES
.c.o:
	$(CC) -c $*.c -o $*.o $(INCLUDE) $(CFLAGS)
.cpp.o:
	$(CXX) -c $*.cpp -o $*.o $(INCLUDE) $(CFLAGS)

all: $(TARGET)

$(TARGET): $(OBJS)
ifeq ($(CPP_OBJS),)
	$(CC)  $(LFLAGS) -o $(OUTDIR)$(TARGET) $^ $(LIB)
else
	$(CXX) $(LFLAGS) -o $(OUTDIR)$(TARGET) $^ $(LIB)
endif

clean: $(_clean)
	rm -f $(TARGET) $(OBJS)

restart:
	$(APACHECTL) restart

install:
	cp $(TARGET) /usr/lib64/httpd/modules/
