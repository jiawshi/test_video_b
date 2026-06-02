#ifndef CHINNA_H
#define CHINNA_H

#include "medialib.h"

/**
 * @fn        
 * @brief     频道线程创建
 * 
 * @param     [in] media_chinna  从这个结构体中获得频道号
 * @param     [in] num          标志这个是第几个频道
 * 
 * @return    int            success or fail
 */
int chinnal_pthread_create(struct media_chinnal_st *media_chinna, int num);

/**
 * @fn        void chinnal_pthread_destory(struct meida_chinnal_st *media_chinna)
 * @brief     销毁一个频道线程
 * 
 * @param     [in] media_chinna   销毁这个频道线程
 * 
 */
void chinnal_pthread_destory(struct media_chinnal_st *media_chinna);

/**
 * @fn        void chinnal_pthread_destoryall(void)
 * @brief     销毁所有的频道线程
 * 
 * 
 */
void chinnal_pthread_destoryall(void);


#endif
