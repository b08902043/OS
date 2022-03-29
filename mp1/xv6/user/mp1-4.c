#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
void f1(void *arg)
{  
    int *aaa = malloc(sizeof(int)*3);
    int *bbb = malloc(sizeof(int)*3);
    int *ccc = malloc(sizeof(int)*3);
    int id = ((int *)arg)[1];
    int depth = ((int *)arg)[2];
    int i = 0;

    if(depth<3){
        aaa[1] = 3*id;
        bbb[1] = 3*id+1;
        ccc[1] = 3*id+2;
        aaa[2] = bbb[2] = ccc[2] = depth + 1;
        struct thread *t1 = thread_create(f1, (void *)aaa);
        struct thread *t2 = thread_create(f1, (void *)bbb);
        thread_add_runqueue(t2);
        thread_add_runqueue(t1);
        for(int i=0;i<50000;++i){
            struct thread *t3 = thread_create(f1, (void *)ccc);
            thread_add_runqueue(t3);
        }
    }
    
    while(1) {
        printf("thread %d: %d\n", id, i++);
        if (i == 5) {
            thread_exit();
        }
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    printf("mp1-4\n");
    int *aaa = malloc(sizeof(int)*3);
    aaa[1] = 1;
    aaa[2] = 1;
    struct thread *t1 = thread_create(f1, (void *)aaa);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}

