
#include "network.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server.h"
#include "medialib.h"
#include <sched.h>

#include <arpa/inet.h>
/**
 * @fn        int Socket(int family, int type, int protol)
 * @brief     创建socket
 * 
 * @param     [in] family    协议
 * @param     [in] type      SOCK_DGRAM
 * @param     [in] protol    默认是0
 * 
 * @return    int            返回socket描述符
 */
int Socket(int *_sockfd, int family, int type, int protol)
{
    
    *_sockfd = socket(family, type, protol);
    if(*_sockfd < 0){
        return ERR_SOCKET;
    }
    
    return 0;
}

/**
 * @fn        int Setsockopt(int *_sockfd,int _level, int _optname, const void *_optval, 
 *            socklen_t _optlen)
 * @brief     设置socket选项
 * 
 * @param     [in] _sockfd   
 * @param     [in] _level    
 * @param     [in] _optname  
 * @param     [in] _optval   
 * @param     [in] _optlen   
 * 
 * @return    int            
 */
int Setsockopt(int *_sockfd,int _level, int _optname, const void *_optval, socklen_t _optlen)
{
    
    if(setsockopt(*_sockfd, _level, _optname, _optval, _optlen) < 0){
        return ERR_SOCKET;
    }

    return SUCCESS;
}

/**
 * @fn        int Bind(int *_sockfd, int family, int port)
 * @brief     bind函数
 * 
 * @param     [in] _sockfd   
 * @param     [in] family    
 * @param     [in] port      
 * 
 * @return    int            
 */
int Bind(int *_sockfd, int family, int port)
{
    struct sockaddr_in local_addr;
    local_addr.sin_family = family,
    local_addr.sin_port = htons(atoi(port));
    inet_pton(family, "0.0.0.0", &local_addr.sin_addr);

    if(bind(*_sockfd, (void *)&local_addr, sizeof(local_addr)) < 0){
        return ERR_FAIL;
    }
    return SUCCESS;
}


int ClientInit(int *_sockfd, int _family, int _port, int _type, char *_ip)
{
    struct ip_mreqn mreqn;
    if(NULL == _sockfd){
        return ERR_POINTER_NULL;
    }
    if(Socket(_sockfd, _family, _type, 0) < 0){
        return ERR_FAIL;
    }
    if((*_sockfd) < 0){
        printf("client create err\n");
        return ERR_FAIL;
    }
    
    inet_pton(AF_INET, *_ip, &mreqn.imr_multiaddr);
    inet_pton(AF_INET,"0.0.0.0",&mreqn.imr_address);
    mreqn.imr_ifindex = if_nametoindex("eth0");

    if(Setsockopt(_sockfd,IPPROTO_IP, IP_ADD_MEMBERSHIP,
                 (void *)&mreqn, sizeof(mreqn)) < 0){
        return ERR_FAIL;
    }
    
    int val = 1;
    if(Setsockopt(_sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, 
                (void *)val, sizeof(val)) < 0){
        return ERR_FAIL;
    }
    
    if(Bind(_sockfd, AF_INET, _port) < 0){
        return ERR_FAIL;
    }

    return SUCCESS;

}



int ServerInit(int *_sockfd, int _family, int _port, int _type, char *_ip)
{
    struct ip_mreqn mreqn;
    if (Socket(_sockfd, _family, _type, 0) < 0){
        return ERR_FAIL;
    }

    inet_pton(_family, _ip, &mreqn.imr_multiaddr);
    inet_pton(_family, "0.0.0.0", &mreqn.imr_address);
    mreqn.imr_ifindex = if_nametoindex("eth0");

    if(Setsockopt(_sockfd, IPPROTO_IP, IP_MULTICAST_IF, (void *)&mreqn, sizeof(mreqn)) < 0){
        return ERR_SOCKET;
    }


    return 0;
}



int receive_list(int *_sockfd,struct msg_list_st *list_program, 
                    size_t size, struct sockaddr_in *server_addr, socklen_t *server_addr_len)
{
    int ret;

    while(1){
        
        ret = recvfrom(*_sockfd, list_program, MSG_DES_MAX, 0, (void *)server_addr, server_addr_len);
        // if(ret < 0){
        //     return ERR_RECEIVE;
        // }
        /*这里有一个printf语句，用于输出serveraddr*/
        if(ret < sizeof(*list_program)){
            printf("the list receive failure!\n");
            continue;
        }
        if(list_program->chinnal_id != MSG_LIST_CHINNAL){
            continue;
        }
        
        break;
    }
    return ret;
}


int receive_data(int *_sockfd,struct msg_chinnal_st *chinnal_info, int num,
                    int pd, struct sockaddr_in *server_addr)
{
    int len;
    struct sockaddr_in server_addr2;
    socklen_t server_addr2_len;
    server_addr2_len = sizeof(server_addr2);
    while(1){
        len = recvfrom(*_sockfd, (void *)chinnal_info, sizeof(*chinnal_info), 0,
                        (void *)&server_addr2,server_addr2_len);

        if(len < sizeof(*chinnal_info)){
            /*print something*/
            continue;
        }
        if(server_addr->sin_addr.s_addr != server_addr2.sin_addr.s_addr){
            /*print something*/
            continue;
        }
        if(server_addr->sin_port != server_addr2.sin_port){
            /*print something*/
            continue;
        }
        if(num == chinnal_info->chinnal_id){
            if(write_data(chinnal_info,pd) < 0){
                return ERR_FAIL;
            }
        }

    }


}


int write_data(struct msg_chinnal_st *chinnalinfo, int pd)
{

    int pos = 0;
    int len = sizeof(*chinnalinfo) - sizeof(chinnal_t);
    int count_num = 0;
    while(len > 0){
        count_num = write(pd, chinnalinfo->data, len + pos);
        if(count_num < 0){
            if(errno == EINTR){
                continue;
            }
            return ERR_FAIL;
        }
        pos += count_num;
        len -= count_num;
    }
    return 0;
    

}

void mysleep(int i){
    sleep(i);
}

int senddata(int *_sockfd, void *buf, size_t len, struct sockaddr_in *dest_addr, socklen_t addrlen)
{
    int ret;

    while(1){
        ret = sendto(*_sockfd, buf, len, 0, dest_addr, addrlen);
        if(ret < 0){
            /* do something */
        }
        else{
            printf("success\n");
        }
    }

    mysleep(1);
}



int senddata2(int *_sockfd, void *buf, chinnal_t chinnal_id){
    
    int ret;
    int len;
    while(1){
        len = media_read(chinnal_id, buf, 123);
        
        ret = sendto(*_sockfd, buf, sizeof(chinnal_t) + len, 0, 
                    (void *)&serveraddr, sizeof(serveraddr));
        if(ret < 0){
            break;            
        }
        sched_yield();        
    }

}
