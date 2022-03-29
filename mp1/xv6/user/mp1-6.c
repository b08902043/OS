#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0
int cnt = -1;
void f1(void *arg)
{  
    printf("thread %d: exit\n", ++cnt);
    thread_yield();
    if(cnt < 20){
        struct thread *t = thread_create(f1, NULL);
        thread_add_runqueue(t);
    }
    thread_exit();

}

int main(int argc, char **argv)
{
    printf("mp1-6\n");
    struct thread *t = thread_create(f1, NULL);
    thread_add_runqueue(t);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}

