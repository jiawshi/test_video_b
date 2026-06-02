#ifndef MEDIALIB_H
#define MEDIALIB_H

#include "proto.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <glob.h>
#include "mytbf.h"
#include "server.h"
#define LINEBUFSIZE         (1024)

/*为了不对外暴露过多的频道信息，只给出频道号，以及频道的描述*/
struct media_chinnal_st{
    chinnal_t chinnal_num;      //频道号
    char *descripe;             //频道描述
};



/**
 * @fn        int media_get(struct media_chinnal_st **chinnal, int *return_num)
 * @brief     读取当前频道，及频道个数
 * 
 * @param     [in] chinnal      存放各频道
 * @param     [in] return_num   有多少个频道
 * 
 * @return    int               success or fail
 */
int media_get(struct media_chinnal_st **chinnal, int *return_num);


/**
 * @fn        int media_read(chinnal_t chinnal_num, void *ptr, size_t size)
 * @brief     读取当前频道内容到buf ，大小为size
 * 
 * @param     [in] chinnal_num  频道号
 * @param     [in] buf          将读取的内容放在这里       
 * @param     [in] size         读取多大的内容
 * 
 * @return    int               success or fail
 */
int media_read(chinnal_t chinnal_num, void *buf, size_t size);


/**
 * @fn        media_free
 * @brief     销毁频道线程
 * 
 * @param     [in] ptr       需要销毁的频道线程
 * 
 */
void media_free(struct media_chinnal_st *ptr);






#endif