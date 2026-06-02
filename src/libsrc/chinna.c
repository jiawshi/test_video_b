#include "chinna.h"
#include "proto.h"
#include <pthread.h>
#include "errorcode.h"
#include <stdio.h>
#include "medialib.h"
#include "network.h"
#include "server.h"


struct chinnal_list_pthread{
    chinnal_t chinnal_id;
    pthread_t tid;
};

struct chinnal_list_pthread chinnal_pthread_group[MSG_CHINNAL_MAX];


void *thread_func1(void *ptr)
{
    struct msg_chinnal_st *msg_list;
    struct media_chinnal_st *ptr1;
    ptr1 = ptr;
    msg_list = malloc(MSG_DATA_MAX);
    if(msg_list == NULL){
        exit(1);
    }
    msg_list->chinnal_id = ptr1->chinnal_num;

    senddata2(&sd, msg_list->data,msg_list->chinnal_id);
    pthread_exit(NULL);
}


int chinnal_pthread_create(struct media_chinnal_st * media_chinna, int num)
{
    int err;
    if((err = pthread_create(&chinnal_pthread_group[media_chinna->chinnal_num].tid, 
                    0, thread_func1, media_chinna)) < 0){

        return ERR_PTHREAD_FAIL;
    }

    chinnal_pthread_group[media_chinna->chinnal_num].chinnal_id = media_chinna->chinnal_num;
    return SUCCESS;

}

void chinnal_pthread_destory(struct media_chinnal_st *media_chinna)
{
    int i;
    for(i = 0; i < MSG_CHINNAL_MAX; i++){
        
        if(chinnal_pthread_group[i].chinnal_id == media_chinna->chinnal_num){
            pthread_cancel(chinnal_pthread_group[i].tid);
            pthread_join(chinnal_pthread_group[i].tid, NULL);
            chinnal_pthread_group[i].chinnal_id = -1;
            break;
        }
        
    }

}
void chinnal_pthread_destoryall(void)
{
    int i;
    for(i = 0; i < MSG_CHINNAL_MAX; i++){
        if(chinnal_pthread_group[i].chinnal_id > 0){
            pthread_cancel(chinnal_pthread_group[i].tid);
            pthread_join(chinnal_pthread_group[i].tid, NULL);
            chinnal_pthread_group[i].chinnal_id = -1;
        }

    }
}