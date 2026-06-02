/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      opt.h
 * @brief     命令行参数分析
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


#ifndef OPT_H
#define OPT_H
#include "server.h"

/*client_config 用于记录端口，多播组，播放器的选择*/
struct client_config{
    char *recvport;
    char *mutigroup;
    char *player;
};

/**
 * @fn        get_opt
 * @brief     分析client端命令行参数
 * 
 * @param     [in] arc       
 * @param     [in] arv       
 * @param     [in] client_conf          分析的参数放这里
 * 
 * @return    int                       success or fail
 */
int get_opt(int arc, char **arv, struct client_config *client_conf);


/**
 * @fn        
 * @brief     分析sever端的命令行参数
 * 
 * @param     [in] arc       
 * @param     [in] arv       
 * @param     [in] server_conf 
 * 
 * @return    int            
 */
int get_opts(int arc, char **arv, struct server_conf *server_conf);



#endif


