#include "print.h"
#include <stdio.h>
#include <arpa/inet.h>
void print_help(void)
{
    printf("you can use this func print some help\n");
}


void print_descripe(struct msg_list_st *listprogram, int len)
{
    struct msg_list_entry_st *pos;
    
    for(pos = listprogram->entry; (char *)pos < (char *)listprogram + len; 
        pos = (void *)(char *)pos + ntohs(pos->len)){
        printf("chinnal :%d  %s\n",pos->chinnal_id,pos->descripe);
    }
}