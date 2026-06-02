/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      errorcode.h
 * @brief     错误代码
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


#ifndef ERRORCODE_H
#define ERRORCODE_H

#define ERR_POINTER_NULL    (-121)      //指针为空
#define ERR_SOCKET          (-111)      //SOCKET创建失败
#define ERR_FAIL            (-1)        //失败
#define SUCCESS             (0)         //成功
#define ERR_RECEIVE         (-134)      //接受失败
#define ERR_OPENFIAL        (-123)      //打开失败
#define ERR_GLOB            (-145)      //glob失败
#define ERR_PTHREAD_FAIL    (-126)      //线程创建失败

#endif /*ERRORCODE_H*/
