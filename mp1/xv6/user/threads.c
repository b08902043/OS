#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0

static struct thread* current_thread = NULL;
static struct thread* root_thread = NULL;
static int id = 1;
static jmp_buf env_st; //return to main
//static jmp_buf env_tmp;
// TODO: necessary declares, if any


struct thread *thread_create(void (*f)(void *), void *arg){
    
    struct thread *t = (struct thread*) malloc(sizeof(struct thread));
    //unsigned long stack_p = 0;
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack +0x100*8-0x2*8;
    t->fp = f;
    t->arg = arg;
    t->ID  = id;
    t->buf_set = 0;
    t->stack = (void*) new_stack;
    t->stack_p = (void*) new_stack_p;
    id++;
    return t;
}
void PrintNode(struct thread *root,int layer){
    if(root == NULL) return;
    PrintNode(root->left,layer+1);
    PrintNode(root->right,layer+1);
    return;
}
void thread_add_runqueue(struct thread *t){
    t->left = NULL;
    t->right = NULL;
    if(current_thread == NULL){
        // TODO
        current_thread = t;
        root_thread = t;
        current_thread->parent = t;
    }
    else{
        // TODO
        t->parent = current_thread;
        if(current_thread->left == NULL){
            current_thread->left = t;
        }
        else if(current_thread->right == NULL){
            current_thread->right = t;
        }
        else{
            free(t->stack);
            free(t);
        }
    }
    
}
void thread_yield(void){
    // TODO
    if(setjmp(current_thread->env) == 0){
        schedule();
        dispatch();
    }
    // from dispatch(back to thread function)
}
void dispatch(void){
    // TODO
    if(current_thread->buf_set == 1){
        //has set the function
        longjmp(current_thread->env,1);
    }
    else{
        if(setjmp(current_thread->env) == 0){
            current_thread->env->sp=(unsigned long)current_thread->stack_p;
            current_thread->buf_set=1;
            longjmp(current_thread->env,1);
        }
        current_thread->fp(current_thread->arg);
        thread_exit();
    }
}
void schedule(void){
    // TODO
    //schedule next thread  
    if(current_thread->left != NULL){
        current_thread = current_thread->left;
    }
    else if(current_thread->right != NULL){
        current_thread = current_thread->right;
    }
    else{
        struct thread *find_next = current_thread;
        while(find_next->parent != find_next && (find_next == find_next->parent->right || (find_next == find_next->parent->left && find_next->parent->right == NULL))){
            find_next = find_next->parent;
        }
        if(find_next == root_thread) current_thread = find_next;
        else  current_thread = find_next->parent->right;
    }
}
void thread_exit(void){
    if(current_thread == root_thread && current_thread->left == NULL && current_thread->right == NULL){
        // TODO
        // Hint: No more thread to execute
        free(current_thread->stack);
        free(current_thread);
        longjmp(env_st,1);
    }
    else{
        // TODO
        struct thread* exit_thread = NULL;
        if(current_thread->left == NULL && current_thread->right == NULL){
            exit_thread = current_thread;
            schedule();
            if(exit_thread->parent->left == exit_thread) exit_thread->parent->left = NULL;
            else exit_thread->parent->right = NULL;
            free(exit_thread->stack);
            free(exit_thread);
            dispatch();
        }
        else{
            struct thread *exchange_thread = current_thread;
            //find the last node in subtree
            while(exchange_thread->left != NULL || exchange_thread->right != NULL){
                if(exchange_thread->right != NULL) exchange_thread = exchange_thread->right;
                else exchange_thread = exchange_thread->left;
            }
            //remove exchange node from its parent
            if(exchange_thread->parent->left == exchange_thread) exchange_thread->parent->left = NULL;
            else if(exchange_thread->parent->right == exchange_thread)exchange_thread->parent->right = NULL;
            if(current_thread != root_thread){
                if(current_thread == current_thread->parent->left){
                    current_thread->parent->left = exchange_thread;
                }
                else{
                    current_thread->parent->right = exchange_thread;
                } 
                exchange_thread->parent = current_thread->parent;
            }
            else{
                exchange_thread->parent = exchange_thread;
            }
            //add child to exchange node
            exchange_thread->left = current_thread->left;
            if(current_thread->left != NULL)current_thread->left->parent = exchange_thread;
            exchange_thread->right = current_thread->right;
            if(current_thread->right != NULL)current_thread->right->parent = exchange_thread;
            exit_thread = current_thread;
            if(current_thread == root_thread) root_thread = exchange_thread;
            current_thread = exchange_thread;
            free(exit_thread->stack);
            free(exit_thread);
            schedule();
            dispatch();
        }
        
    }
}
void thread_start_threading(void){
    // TODO
    if(setjmp(env_st) == 0){
        // start threads
        dispatch();
    }
    //jump from thread_exit(finish all threads)
}
