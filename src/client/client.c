#include "proto.h"
#include "network.h"
#include "opt.h"
#include "client.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "print.h"

int main(int argc, char **argv)
{
    int sd;
    int pd[2];
    int pid;
    int len=0;
    int chosenum=0;
    struct sockaddr_in server_addr;
    socklen_t serverlen;
    struct msg_list_entry_st *pos;
    struct client_config client_conf = {
        .recvport = MSG_PORT,
        .mutigroup = MSG_GROUP,
        .player = PLAYER
    };

    get_opt(argc, argv, &client_conf);
    
    if((ClientInit(&sd, AF_INET, client_conf.recvport, 
                    SOCK_DGRAM, client_conf.mutigroup))< 0){
        exit(1);
    }

    if(pipe(pd) < 0){
        exit(1);
    }

    if((pid = fork()) < 0){
        exit(1);
    }

    if(pid == 0){
        close(sd);
        close(pd[1]);
        dup2(pd[0], 0);
        if(pd[0] > 0){
            close(pd[0]);
        }
        execl("/bin/sh", "sh", "-C", NULL);
        perror("execl");
        exit(1);
    }
    else{
        
        struct msg_list_st *listprogram;
        listprogram = malloc(MSG_LIST_CHINNAL);
        if(listprogram == NULL){
            exit(1);
        }

        serverlen = sizeof(server_addr);
        len = receive_list(&sd, listprogram, MSG_CHINNAL_MAX, &server_addr, &serverlen);

        print_descripe(listprogram, len);
        free(listprogram);

        /*接受用户选择的频道*/
        /*由于用户总可以选择什么时候输入，所以这里需要做成线程*/
        while(1){
            
            if(scanf("%d",&chosenum) !=1){
                exit(1);
            }
            if(chosenum > 0){
                break;
            }
        }


        struct msg_chinnal_st *chinnalinfo;
        chinnalinfo = malloc(MSG_DATA_MAX);
        if(chinnalinfo == NULL){
            exit(1);
        }
        
        if(receive_data(&sd, chinnalinfo, chosenum, pd[1], &server_addr) < 0){
            exit(1);
        }

        /*记得资源回收，用钩子函数*/

    }


    exit(0);
}

