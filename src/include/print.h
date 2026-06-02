/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      print.h
 * @brief     一些打印输出函数
 * 
 * @version   V0.01
 * @date      2023-02-21
 * 
 * @note      历史记录: 
 *            - [2023-02-21] [Jiawshi] 创建初始版本
 * @warning   
 * @par       修改记录: 
 * <table>
 * <tr><th>date          <th>Version    <th>Author      <th>Description    </tr>
 * <tr><td>2023-02-21    <td>V0.01      <td>Jiawshi       <td>创建初始版本    </tr>
 * </table>
 */

#ifndef PRINT_H
#define PRINT_H

#include "proto.h"

void print_help(void);

/**
 * @fn        
 * @brief     打印描述信息
 * 
 * @param     [in] listprogrom           
 * @param     [in] len       
 * 
 */
void print_descripe(struct msg_list_st *listprogrom, int len);


#endif
