#include "opt.h"
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include "print.h"
#include "errorcode.h"
#include <stdlib.h>

/*在输入的参数为H时打印帮助文档*/



int get_opt(int arc, char **arv, struct client_config *client_conf)
{
    int ch;
    while ((ch = getopt(arc, arv, "P:M:p:H")) > 0)
    {
        /* code */
        switch(ch){
            case 'P':
                client_conf->recvport = optarg;
                break;
            case 'M':
                client_conf->mutigroup = optarg;
                break;
            case 'p':
                client_conf->player = optarg;
                break;
            case 'H':
                print_help();
                break;
            default:
                abort();
                break;
            
        }
    }
    return SUCCESS;
    
}


int get_opts(int arc, char **arv, struct server_conf *server_conf)
{
    int ch;
    while((ch = getopt(arc, arv, "M:P:D:R:H")) > 0){
        
        switch(ch){

            case 'P':
                server_conf->recvport = optarg;
                break;
            case 'M':
                server_conf->mutigroup = optarg;
                break;
            case 'D':
                server_conf->mediadir = optarg;
                break;
            case 'R':
                server_conf->rundeamom = RUN_F;
                break;
            case 'H':
                print_help();
                return ERR_FAIL;
                break;
            default :
                abort();
                break;

        }
    }

    return SUCCESS;
}