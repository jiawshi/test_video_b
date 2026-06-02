#include "medialib.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "errorcode.h"
#include "proto.h"
#include <string.h>
#include "server.h"
#include <stdlib.h>
#include <glob.h>
#include "mytbf.h"
#include <unistd.h>
/*记录每个频道的一些信息*/
struct chinnal_context_st{
    chinnal_t chinnal_id;
    char *desc;
    glob_t mp3_glob;
    int pos;
    off_t offset;
    mytbf_t *mytbf;
    int fd;
};


struct chinnal_context_st chinnal_all[MSG_CHINNAL_MAX+1];

/**
 * @fn        int read_descirpetxt(char *path, char *linebuf
 * @brief     从path中读取txt文件到linebuf
 * 
 * @param     [in] path      
 * @param     [in] linebuf   
 * 
 * @return    int  是否执行成功
*/
int read_descirpetxt(char *path, char *linebuf){
    FILE *fp;
    char *pathstr = {'\0'};
    strcat(pathstr, path);
    strcat(pathstr,"/desc.txt");
    fp = fopen(pathstr, "r");
    if(fp == NULL){
        return ERR_OPENFIAL;
    }

    if(fgets(linebuf,LINEBUFSIZE, fp) == NULL){
        
        return ERR_OPENFIAL;
    }

    fclose(fp);
    
    return SUCCESS;
}


/**
 * @fn        int read_mp3(char *path, struct chinnal_context_st *context)
 * @brief     从path 中读取mp3文件 到context
 * 
 * @param     [in] path      
 * @param     [in] context   
 * 
 * @return    int            
 */
int read_mp3(char *path, struct chinnal_context_st *context)
{
    // /*难道这里chinnal_id 可以不停的增加吗？，后面为什么会出现++？*/
    static chinnal_t chinnal_id = MSG_CHINNAL_MIN;
    glob_t glob_result;
    char *pathstr = NULL;
    strcat(pathstr, path);
    strcat(pathstr, "/*.mp3");

    if(glob(pathstr,0, NULL, &glob_result) != 0){
        globfree(&glob_result);
        chinnal_id ++;
        return ERR_GLOB;
    }

    context->mp3_glob = glob_result;
    context->pos = 0;
    context->offset = 0;
    
    //这里的pos到底是在干什么？
    context->fd = open(glob_result.gl_pathv[context->pos], O_RDONLY);
    if(context->fd < 0){
        return ERR_OPENFIAL;
    }
    chinnal_id++;
    
    return SUCCESS;
}

int read_mp3_descripe(char *path, struct chinnal_context_st *context)
{
    char linebuf[LINEBUFSIZE] = {'\0'};
    if(read_descirpetxt(path, linebuf) < 0){
        return ERR_FAIL;
    }

    context->desc = strdup(linebuf);
    
    if(read_mp3(path, context) < 0){
        return ERR_FAIL;
    } 
   
    /*这里还需要改一下 ，要是mytbf_init()出错呢？*/
    context->mytbf = mytbf_init(1,2);
    if(context->mytbf == NULL){
        return ERR_POINTER_NULL;
    }

    return SUCCESS;
    
}


int media_get(struct media_chinnal_st **chinnal, int *return_num)
{
    struct chinnal_context_st context;
    glob_t glob_result;
    char *path = NULL;
    int num = 0;
    struct media_chinnal_st *ptr;
    int i;

    for(i=0; i < MSG_CHINNAL_MAX; i++){
        chinnal_all[i].chinnal_id = -1; 
    }

    snprintf(path, 256, "%s/*", serverconf.mediadir);
    if(glob(path, 0, NULL, &glob_result) != 0){

        return ERR_GLOB;
    }

    ptr = malloc(sizeof(struct media_chinnal_st) * glob_result.gl_pathc);
    if(ptr == NULL){
        return ERR_POINTER_NULL;
    }

    for(i = 0; i < glob_result.gl_pathc; i++){
        if(read_mp3_descripe(glob_result.gl_pathv[i], &context) < 0){
            return ERR_FAIL;
        }

        memcpy(chinnal_all + context.chinnal_id, &context, sizeof(context));
        ptr[num].chinnal_num = context.chinnal_id;
        ptr[num].descripe = strdup(context.desc);
        num++;
    }

    *chinnal = realloc(ptr, sizeof(struct media_chinnal_st) * num);
    if(*chinnal == NULL){
        return ERR_POINTER_NULL;
    }
    *return_num = num;

    return SUCCESS;
}

int open_next(chinnal_t chinnal_num)
{
    int i;
    for(i=0; i < chinnal_all[chinnal_num].mp3_glob.gl_pathc; i++){
        chinnal_all[chinnal_num].pos++;
        if(chinnal_all[chinnal_num].pos == chinnal_all[chinnal_num].mp3_glob.gl_pathc){
            return 0;
        }
        
        close(chinnal_all[chinnal_num].fd);
        chinnal_all[chinnal_num].fd = 
                    open(chinnal_all[chinnal_num].mp3_glob.gl_pathv[chinnal_all[chinnal_num].pos], O_RDONLY);
        if(chinnal_all[chinnal_num].fd < 0){
            /*should do something */
        }
        else{
            chinnal_all[chinnal_num].offset = 0;
            return 0;
        }

    }

   
}

int media_read(chinnal_t chinnal_num, void *buf, size_t size)
{
    int tbfsize;
    int len = 0;
    tbfsize = mytbf_fetchtoken(chinnal_all[chinnal_num].mytbf, size);

    while(1){
        len = pread(chinnal_all[chinnal_num].fd, buf, size, chinnal_all[chinnal_num].offset);
        if(len < 0){
            open_next(chinnal_num);
        }
        else if(len == 0){
            open_next(chinnal_num);
        }
        else if(len > 0){
            chinnal_all[chinnal_num].offset += len;

        }    
    }
    if(tbfsize -size > 0){
        mytbf_returntoken(chinnal_all[chinnal_num].mytbf, tbfsize -size);
    }
    return len;

}


void media_free(struct media_chinnal_st *ptr){
    free(ptr);
}