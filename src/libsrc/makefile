
CFLAGS+=-I$(MYPATH)/include  -Wall -pthread
LDFLAGS+=-pthread
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)



OBJS:$(SRCS) 
	cc $^ -o $@ $(CFLAGS) $(LDFLAGS)