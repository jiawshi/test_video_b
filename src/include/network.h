/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      network.h
 * @brief     所有关于网络函数的声明
 * 
 * @version   V0.01
 * @date      2023-02-20
 * 
 * @note      历史记录: 
 *            - [2023-02-20] [Jiawshi] 创建初始版本
 * @warning   
 * @par       修改记录: 
 * <table>
 * <tr><th>date          <th>Version    <th>Author      <th>Description    </tr>
 * <tr><td>2023-02-20    <td>V0.01      <td>Jiawshi       <td>创建初始版本    </tr>
 * </table>
 */


#ifndef NETWORK_H
#define NETWORK_H

#include "errorcode.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <stdlib.h>
#include "proto.h"


/**
 * @fn        int ClientInit(int *_sockfd, int _family, int _port, int _type, char *_ip);
 * @brief     客户端初始化函数
 * 
 * @param     [in] _sockfd   返回socket 
 * @param     [in] _family   协议族 AF_INET
 * @param     [in] _port     端口号
 * @param     [in] _type     数据类型SOCK_DGRAM
 * @param     [in] _ip       IP地址
 * 
 * @return    int            ClientInit函数执行是否正确，success 返回0；
 */
int ClientInit(int *_sockfd, int _family, int _port, int _type, char *_ip);


/**
 * @fn        int ServerInit(int *_sockfd, int _family, int _port, int _type, char *_ip);
 * @brief     server初始化函数
 * 
 * @param     [in] _sockfd   返回socket
 * @param     [in] _family   协议族 AF_INET
 * @param     [in] _port     端口号
 * @param     [in] _type     数据类型SOCK_DGRAM
 * @param     [in] _ip       IP地址
 * 
 * @return    int            
 */
int ServerInit(int *_sockfd, int _family, int _port, int _type, char *_ip);


/**
 * @fn        receive_list
 * @brief     接收节目单
 * 
 * @param     [in] _sockfd              sock 文件描述符   
 * @param     [in] list_program         节目单
 * @param     [in] size                 节目单大小
 * @param     [in] server_addr          这个可以隐藏
 * @param     [in] server_addr_len      这个也不需要，可以该进这个函数
 * 
 * @return    int            
 */
int receive_list(int *_sockfd,struct msg_list_st *list_program, 
                    size_t size,struct sockaddr_in *server_addr, socklen_t *server_addr_len);

/**
 * @fn        receive_data
 * @brief     接受数据
 * 
 * @param     [in] _sockfd              sock文件描述符
 * @param     [in] chinnal_info         频道信息
 * @param     [in] size                 频道数
 * @param     [in] sercer_addr 
 * @param     [in] server_addr_len 
 * 
 * @return    int                       success or fail
 */
int receive_data(int *_sockfd,struct msg_chinnal_st *chinnal_info, int num,
                    int pd, struct sockaddr_in *server_addr);



/**
 * @fn        write_dat
 * @brief     向管道里面写数据
 * 
 * @param     [in] chinnalinfo      频道信息
 * @param     [in] pd               管道的一端
 * 
 * @return    int                   success or fail
 */
int write_data(struct msg_chinnal_st *chinnalinfo, int pd);

/**
 * @fn        senddata
 * @brief     发送节目单数据
 * 
 * @param     [in] _sockfd      
 * @param     [in] buf       将这里面的数据发送出去
 * @param     [in] len       
 * 
 * @return    int            
 */
int senddata(int *_sockfd, void *buf, size_t len, struct sockaddr_in *dest_addr, socklen_t addrlen);

/**
 * @fn        senddata2
 * @brief     发送mp3数据
 * 
 * @param     [in] _sockfd   
 * @param     [in] buf       
 * @param     [in] chinnal_id 
 * 
 * @return    int            
 */
int senddata2(int *_sockfd, void *buf, chinnal_t chinnal_id);


#endif
