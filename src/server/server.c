#include "server.h"
#include "opt.h"
#include <stdio.h>
#include "proto.h"
#include <stdlib.h>
#include "errorcode.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "network.h"
#include "medialib.h"
#include "list.h"
#include "chinna.h"
#include <signal.h>
#include <arpa/inet.h>




int sd;
struct media_chinnal_st *list;
int listnum;
struct sockaddr_in serveraddr;
struct server_conf serverconf = {
    .mutigroup = MSG_GROUP,
    .mediadir = DEFAUTL_DIR,
    .recvport = MSG_PORT,
    .rundeamom = RUN_DEAMON
};

static int deamonize(void){

    int pid;
    int fd;
    if(pid = fork() < 0){
        return ERR_FAIL;
    }
    if(pid > 0){
        exit(0);
    }
    else{
        
        fd = open("/dev/dull",O_RDWR);
        if(fd < 0){
            return ERR_FAIL;
        }
        
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        if(fd > 2){
            close(fd);
        }

        chdir('/');
        umask(0);
        setsid();

        return 0;
    }
}


void deamoniize_exit(void)
{
    media_free(list);
    thread_list_destory();
    chinnal_pthread_destoryall();
}

int main(int argc, char **argv)
{
    struct sigaction sa;
    sa.sa_handler = deamoniize_exit;
    sigemptyset(&sa.sa_mask);

    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGTERM);

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);


    int i;
    struct server_conf serverconf = {
        .mutigroup = MSG_GROUP,
        .mediadir = DEFAUTL_DIR,
        .recvport = MSG_PORT,
        .rundeamom = RUN_DEAMON
    };
   

    if(get_opts(argc, argv, &serverconf) < 0){
        exit(1);
    }

    if(serverconf.rundeamom == RUN_DEAMON){
        /*后台模式*/
        if(deamonize() < 0){
            exit(1);
        }
    }
    else{

    }


    if(ServerInit(&sd, AF_INET, serverconf.recvport, 
                    SOCK_DGRAM, serverconf.mutigroup) < 0){
        exit(1);
    }
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(serverconf.recvport));
    inet_pton(AF_INET, "0.0.0.0", serveraddr.sin_addr.s_addr);

    if(media_get(&list,&listnum) < 0){
        exit(1);
    }

    if(thread_list_create(list, listnum) < 0){
        exit(1);
    }

    for(i=0; i< listnum; i++){
        if(chinnal_pthread_create(list, i) < 0){
            exit(1);
        }
    }
    
    while(1){
        pause();
    }
    exit(0);
}