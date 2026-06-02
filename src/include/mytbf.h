/**
 * @copyright Copyright (c) 2023 Jiawshi
 * @author    Jiawshi (jiawshi@126.com)
 * 
 * @file      mytbf.h
 * @brief     这个文件主要是产生令牌，只有获得令牌才能够发送数据
 * 
 * @version   V0.01
 * @date      2023-02-27
 * 
 * @note      历史记录: 
 *            - [2023-02-27] [Jiawshi] 创建初始版本
 * @warning   
 * @par       修改记录: 
 * <table>
 * <tr><th>date          <th>Version    <th>Author      <th>Description    </tr>
 * <tr><td>2023-02-27    <td>V0.01      <td>Jiawshi       <td>创建初始版本    </tr>
 * </table>
 */


#ifndef MYTBF_H
#define MYTBF_H

/*这里是为了不暴露数据结构*/
typedef void mytbf_t;

/**
 * @fn        mytbf_init
 * @brief     令牌桶初始化函数
 * 
 * @param     [in] cps       每秒增加cps 个令牌
 * @param     [in] brust     令牌桶最多有多少令牌
 * 
 * @return    mytbf_t*       返回令牌桶
 */
mytbf_t *mytbf_init(int cps, int brust);



/**
 * @fn        mytbf_fetchtoken
 * @brief     取令牌
 * 
 * @param     [in] tbf       令牌桶
 * @param     [in] size      每次取多少个
 * 
 * @return    int            返回告知取了多少个令牌
 */
int mytbf_fetchtoken(mytbf_t *tbf, int size);

/**
 * @fn        mytbf_returntoken
 * @brief     返回没有用完的令牌
 * 
 * @param     [in] tbf       将令牌还回到这
 * @param     [in] size      还size个
 * 
 * @return    int            返回还回多少个
 */
int mytbf_returntoken(mytbf_t *tbf, int size);

// /**
//  * @fn        
//  * @brief     
//  * 
//  * @param     [in] tbf       
//  * 
//  * @return    int            
//  */
// int mytbf_checktoken(mytbf_t *tbf);

/**
 * @fn        mytbf_destorytoken
 * @brief     销毁令牌桶
 * 
 * @param     [in] tbf       需要销毁的令牌桶
 * 
 */
void mytbf_destorytoken(mytbf_t *tbf);


#endif 