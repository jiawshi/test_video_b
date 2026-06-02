/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      proto.h
 * @brief     协议
 * 
 * @version   V0.01
 * @date      2023-02-19
 * 
 * @note      历史记录: 
 *            - [2023-02-19] [Jiawshi] 创建初始版本
 * @warning   
 * @par       修改记录: 
 * <table>
 * <tr><th>date          <th>Version    <th>Author      <th>Description    </tr>
 * <tr><td>2023-02-19    <td>V0.01      <td>Jiawshi       <td>创建初始版本    </tr>
 * </table>
 */


#ifndef PROTO_H
#define PROTO_H


#include "site_data.h"
#define MSG_GROUP 			"224.2.2.2" //多播组
#define MSG_PORT			"1999"  //接受端口
#define MSG_CHINNAL 		100 //频道数
#define MSG_LIST_CHINNAL 	0 //0号频道负责打印节目单
#define MSG_CHINNAL_MIN 	1 //最小的频道，区别于打印节目单的0号频道。
#define MSG_CHINNAL_MAX 	(MSG_CHINNAL+MSG_CHINNAL_MIN-1) //最大的频道
#define MSG_DATA_MAX		(65536-20-8)//最多能够传输的数据，除去了报头（ip，udp）等内容
#define MSG_DATA			(MSG_DATA_MAX - sizeof(chinnal_t)) //去除了频道号后，实际能够传输的数据大小
#define MSG_DES_MAX			(65536-20-8)//歌曲的描述
#define MSG_REL_DES 		(MSG_DES_MAX - sizeof(chinnal_t))//实际的描述最大为多少

/**
 *频道包的大小，包括频道号，以及需要传输的频道的内容 
*/
struct msg_chinnal_st{
	chinnal_t chinnal_id;
	uint8_t data[1];

}__attribute__((packed()));	

/**
 * 每个节目的内容，包括频道号，以及对频道的描述
*/
struct msg_list_entry_st{
	chinnal_t chinnal_id;
	uint16_t len;//该结构体的大小
	uint8_t descripe[1];//描述

}__attribute__((packed()));

/*节目单*/
struct msg_list_st{
	chinnal_t chinnal_id;			//0号频道，即节目单
	struct msg_list_entry_st entry[1];//节目单的具体内容
}__attribute__((packed()));//不对齐


#endif
