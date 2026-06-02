/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      server.h
 * @brief     
 * 
 * @version   V0.01
 * @date      2023-02-22
 * 
 * @note      历史记录: 
 *            - [2023-02-22] [Jiawshi] 创建初始版本
 * @warning   
 * @par       修改记录: 
 * <table>
 * <tr><th>date          <th>Version    <th>Author      <th>Description    </tr>
 * <tr><td>2023-02-22    <td>V0.01      <td>Jiawshi       <td>创建初始版本    </tr>
 * </table>
 */

#ifndef SERVER_H
#define SERVER_H
#define DEFAUTL_DIR  "../../medialib"

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>

//sever的运行模式， 前台或者守护进程
enum  {
    RUN_DEAMON = 1,
    RUN_F
};

struct server_conf{
    char *recvport;
    char *mutigroup;
    char *mediadir;
    int rundeamom;
};


extern struct sockaddr_in serveraddr;
extern struct server_conf serverconf;
extern int sd;


#endif