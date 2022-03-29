#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
int cnt = 0;
int a[6] = {0, 15, 12, 4, 3, 110};
int s[6] = {0, 10, 5, 1, 1, 100};
void f1(void *arg)
{   
    int i = (int)((long) arg);
    int id = cnt;

    if(cnt<5){
        struct thread *t1 = thread_create(f1, (void *)((long)(s[++cnt])));
        thread_add_runqueue(t1);
    }
    
    while(1) {
        printf("thread %d: %d\n", id, i++);
        if (i == a[id]) {
            thread_exit();
        }
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    printf("mp1-3\n");
    struct thread *t1 = thread_create(f1, (void *)((long)(s[++cnt])));
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}

