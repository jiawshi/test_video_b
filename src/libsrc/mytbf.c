#include "mytbf.h"
#include <stdio.h>
#include <pthread.h>
#include "errorcode.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define min(x,y)   (x > y ? y : x)

struct mytbf_st{
    int cps;
    int brust;
    int token;
    int pos;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

static pthread_once_t once_init = PTHREAD_ONCE_INIT;
pthread_t tid;
pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t job_cond = PTHREAD_COND_INITIALIZER;


static struct mytbf_st *job[100];

static void mysleep(int i)
{
    sleep(1);
}

static void *thread_func(void *ptr)
{
    int i;
    while(1){
        pthread_mutex_lock(&job_mutex);
        for(i = 0; i < 100; i++){
            if(job[i] != NULL){
                pthread_mutex_lock(&job[i]->mutex);
                job[i]->token += job[i]->cps;
                if(job[i]->token > job[i]->brust){
                    job[i]->token = job[i]->brust;
                }
                pthread_cond_broadcast(&job_cond);
                pthread_mutex_unlock(&job[i]->mutex);
            }
        }
        pthread_mutex_unlock(&job_mutex);
        mysleep(1);
    }

}


static int get_job_pos(void)
{
    int i;
    for(i = 0; i < 100; i++){
        if(job[i] == NULL){
            return i;
        }
    }
    return -1;
}

static void module_unload(void)
{
    int i;
    pthread_cancel(tid);
    pthread_join(tid,NULL);
    for(i = 0; i < 100; i++){
        free(job[i]);
    }
}

static void module_load(void)
{
    if(pthread_create(&tid, NULL, thread_func, NULL) < 0){
        //fprintf(stderr,"thread_create fail %s\n", strerror(errno))
        goto ERR_EXIT;
    }
    atexit(module_unload);

ERR_EXIT:
    atexit(module_unload);

}

mytbf_t *mytbf_init(int cps, int brust)
{
    struct mytbf_st *tbf;
    int pos;

    pthread_once(once_init, module_load);

    tbf = malloc(sizeof(struct mytbf_st));
    if(tbf == NULL){
        return NULL;
    }
    tbf->cps = cps;
    tbf->brust = brust;
    tbf->token = 0;
    pthread_mutex_init(&tbf->mutex, NULL);
    pthread_cond_init(&tbf->cond, NULL);
    
    pthread_mutex_lock(&job_mutex);
    pos = get_job_pos();
    if(pos < 0){
        pthread_mutex_unlock(&job_mutex);
        return NULL;
    }

    tbf->pos = pos;
    job[tbf->pos] = tbf;
    pthread_mutex_unlock(&job_mutex);
    return tbf;
}




int mytbf_fetchtoken(mytbf_t *tbf, int size)
{
    if(tbf == NULL){
        return ERR_POINTER_NULL;
    }
    struct mytbf_st *tbf_t = tbf;
    int n;
    pthread_mutex_lock(&tbf_t->mutex);

    while(tbf_t->token <= 0){
        pthread_cond_wait(&tbf_t->cond, &tbf_t->mutex);
    }

    n = min(tbf_t->token, size);
    tbf_t->token -= n;
    /*要弄明白这里是再通知谁？*/
    pthread_cond_broadcast(&tbf_t->cond);
    pthread_mutex_unlock(&tbf_t->mutex);
   
    return n;
}


int mytbf_returntoken(mytbf_t *tbf, int size)
{
    if(tbf == NULL){
        return ERR_POINTER_NULL;
    }

    struct mytbf_st *tbf_st = tbf;
    pthread_mutex_lock(&tbf_st->mutex);
    tbf_st->token += size;
    if(tbf_st->token >= tbf_st->brust){
        tbf_st->token = tbf_st->brust;
    }
    /*这里要不要加上brodcast*/
    pthread_cond_broadcast(&tbf_st->cond);
    pthread_mutex_unlock(&tbf_st->mutex);

    return size;    
}
// int mytbf_checktoken(mytbf_t *tbf)
// {

// }


void mytbf_destorytoken(mytbf_t *tbf)
{
    struct mytbf_st *tbf_st = tbf;
    
    pthread_mutex_lock(&job_mutex);
    job[tbf_st->pos] = NULL;
    pthread_mutex_unlock(&job_mutex);

    pthread_mutex_destroy(&tbf_st->mutex);
    pthread_cond_destroy(&tbf_st->cond);
    free(tbf);
}
