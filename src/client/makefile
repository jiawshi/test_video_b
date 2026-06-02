# include $(MYPATH)/include/make.inc
# include $(MYPATH)/include/*
# all:client

# SRCS = $(wildcard *.c)
# OBJS = $(SRCS:.c=.o)


# client:$(OBJS)
# 	cc $^ -o $@ $(CFLAGS) $(LDFLAGS)

CFLAGS+=-I$(MYPATH)/include -L$(MYPATH)/libsrc -Wall -pthread
LDFLAGS+=-pthread
SRCS1 = $(wildcard *.c)
OBJS1 = $(SRCS1:.c=.o)
SRCS2 = $(wildcard *.c)
OBJS2 = $(SRCS2:.c=.o)


all:client

client:$(OBJS1) $(OBJS2)
	cc $^ -o $@ $(CFLAGS) $(LDFLAGS)