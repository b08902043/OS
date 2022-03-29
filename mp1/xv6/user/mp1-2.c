#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
int a[8] = {0, 5, 3, 2, 4, 3, 4, 1};
void f1(void *arg)
{   
    int x = (int)((long) arg);
    int i = 0;

    if(x<=3){
        struct thread *t1 = thread_create(f1, (void *)((long)(2*x)));
        struct thread *t2 = thread_create(f1, (void *)((long)(2*x+1)));
        thread_add_runqueue(t1);
        thread_add_runqueue(t2);
    }
    
    while(1) {
        printf("thread %d: %d\n", x, i++);
        if (i == a[x]) {
            thread_exit();
        }
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    printf("mp1-2\n");
    struct thread *t1 = thread_create(f1, (void *)1);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}

