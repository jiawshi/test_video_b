#include "list.h"
#include <pthread.h>
#include "errorcode.h"
#include "proto.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "network.h"
#include "server.h"


struct media_chinnal_st *list_t;
int list_num;
pthread_t tid;


void *pthread_func(void *p)
{
    int i;
    int totalsize;
    int size;
    struct msg_list_st *msglist;
    struct msg_list_entry_st *msgentry;
    
    totalsize = sizeof(list_t->chinnal_num);
    
    for(i = 0; i < list_num; i++)
    {
        totalsize += sizeof(struct msg_list_entry_st) + strlen(list_t[list_num].descripe);    
    }

    msglist = malloc(totalsize);
    if(msglist == NULL){
        exit(1);
    }
    msglist->chinnal_id = MSG_LIST_CHINNAL;
    msgentry = msglist->entry;

    for(i = 0; i < list_num; i++){
        size = sizeof(struct msg_list_entry_st) + strlen(list_t[i].descripe);
        msgentry->chinnal_id = list_t[i].chinnal_num;
        msgentry->len = htons(size);
        strcpy(msgentry->descripe, list_t[i].descripe);
        msgentry = (void *)((char *)msgentry + size);    
    }

    senddata(&sd, msglist, size, &serveraddr, sizeof(serveraddr));


}

int thread_list_create(struct media_chinnal_st *list, int listnum)
{
    list_t = list;
    list_num = listnum;

    if(pthread_create(&tid, NULL, pthread_func, NULL) < 0){
        return ERR_PTHREAD_FAIL;
    }

    return SUCCESS;
}

void thread_list_destory(void)
{
    pthread_cancel(tid);
    pthread_join(tid, NULL);
}