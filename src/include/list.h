/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      list.h
 * @brief     
 * 
 * @version   V0.01
 * @date      2023-02-26
 * 
 * @note      历史记录: 
 *            - [2023-02-26] [Jiawshi] 创建初始版本
 * @warning   
 * @par       修改记录: 
 * <table>
 * <tr><th>date          <th>Version    <th>Author      <th>Description    </tr>
 * <tr><td>2023-02-26    <td>V0.01      <td>Jiawshi       <td>创建初始版本    </tr>
 * </table>
 */
#ifndef LIST_H
#define LIST_H


#include "medialib.h"
#include "server.h"

/**
 * @fn        hread_list_create
 * @brief     创建节目单线程
 * 
 * @param     [in] list     节目单 
 * @param     [in] listnum   节目单中的频道数
 * 
 * @return    int            
 */
int thread_list_create(struct media_chinnal_st *list, int listnum);

/**
 * @fn        thread_list_destory
 * @brief     销毁节目单线程
 * 
 * 
 */
void thread_list_destory(void);





#endif

