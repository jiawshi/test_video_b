
#包含的文件路径

INCDIR = $(MYPATH)/include 
LIBDIR = $(MYPATH)/libsrc


#定义c文件编译标志
CFLAGS = -Wall -pthread -I$(MYPATH)/include
LDFLAGS = -pthread