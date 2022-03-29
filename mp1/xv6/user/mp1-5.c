#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
int rng[33]={0, 1, 11, 1, 2, 4, 6, 2, 6, 4, 7, 3, 6, 6, 5, 1, 3, 4, 3, 5, 5, 4, 6, 6, 7, 5, 4, 2, 1, 3, 5, 10, 1};
int cnt = 1;
void f1(void *arg)
{  
    int *aaa = malloc(sizeof(int)*3);
    int *bbb = malloc(sizeof(int)*3);
    int *ccc = malloc(sizeof(int)*3);
    int id = ((int *)arg)[1];
    int t = ++cnt;
    int depth = ((int *)arg)[2];
    int i = 0;

    if(depth<5){
        aaa[1] = 3*id;
        bbb[1] = 3*id+1;
        ccc[1] = 3*id+2;
        aaa[2] = bbb[2] = ccc[2] = depth + 1;
        struct thread *t1 = thread_create(f1, (void *)aaa);
        struct thread *t2 = thread_create(f1, (void *)bbb);
        struct thread *t3 = thread_create(f1, (void *)ccc);
        thread_add_runqueue(t3);
        thread_add_runqueue(t2);
        thread_add_runqueue(t1);
    }
    
    while(1) {
        printf("thread %d: %d\n", id, i++);
        if (i == rng[t]) {
            if(id%2)
                thread_exit();
            else
                return;
        }
        
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    printf("mp1-5\n");
    int *aaa = malloc(sizeof(int)*3);
    aaa[1] = 1;
    aaa[2] = 1;
    struct thread *t1 = thread_create(f1, (void *)aaa);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}

