
#include "stdio.h"
// join issue ronak -> sarik
#include "mypthread.h"
#include "time.h"
#include "stdlib.h"
#include "ucontext.h"
//--------------------- isuue with sheduling to be solved after swapping context//
int numOfNodes = 0;
int getcount() {
	return numOfNodes;
}

mypthread_t* returnHead() {
	return (head->n);
}

mypthread_t* returnTail() {
	return (tail->n);
}

void create_node() {
	mypthread_t data;
	temp = (struct node *) malloc(1 * sizeof(struct node));
	temp->prev = NULL;
	temp->next = NULL;
	numOfNodes++;
}

void enqueue(mypthread_t* data) {
	if (head == NULL) { 
		create_node();
		head = temp;
		tail = temp;
	} else {
		create_node();
		tail->next = temp;
		temp->prev = tail;
		tail = temp;
		tail->next = head;
		head->prev = tail;
	}
	temp->n = data;
}

mypthread_t* srhth(int tid) {
	temp = head;
	while (temp != NULL) {
		if (temp->n->th_id == tid)
			return (temp->n);
		else {
			temp = temp->next;
		}
	}
	printf("issue with %d !", tid);
	exit(0);
}

mypthread_t* SNAT(int tid) {
	
	temp = head;

	while (temp != NULL) {
		if (temp->n->th_id == tid) {
			//srhnd=temp;
			break;
		} else {
			temp = temp->next;
		}
	}
	if (temp->next != NULL) {
		temp = temp->next;
		while (temp != NULL) {
			if (temp->n->state == Pact) {
				return temp->n;
			} else {
				temp = temp->next;
			}
		}

	} else {
		exit(0);
	}

	printf("no active th later%d\n", tid);
	exit(0);
}

int curr_thread_id = 1; 

int numOfThreads = 1;

int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr,
		void *(*start_routine)(void *), void *arg) {

	if (getcount() == 0) {
		mypthread_t* main_thread = (mypthread_t *) malloc(sizeof(mypthread_t));
		main_thread->th_id = numOfThreads++;
		ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
		main_thread->ctx = context;
		main_thread->ctx->uc_stack.ss_sp = (char*) malloc(sizeof(char) * 4096);
		main_thread->ctx->uc_stack.ss_size = 4096;
		main_thread->state = Pact;
		enqueue(main_thread);
	}
	ucontext_t* context = (ucontext_t*) malloc(sizeof(ucontext_t));
	thread->ctx = context;
	getcontext(thread->ctx);
	(*thread).ctx->uc_stack.ss_sp = (char*) malloc(sizeof(char) * 4096);
	(*thread).ctx->uc_stack.ss_size = 4096;
	(*thread).state = Pact;
	thread->th_id = numOfThreads++;
	makecontext(thread->ctx, (void (*)()) start_routine, 1, arg);
	enqueue(thread);
	return 0;
}

int mypthread_join(mypthread_t thread, void **retval) {
	int target_th_id = thread.th_id;
	mypthread_t* cur_th = srhth(curr_thread_id);
	mypthread_t* target_thread = srhth(thread.th_id);
	if (target_thread->state != Pact) {
		return 0;
	} else {
		
		cur_th->state = Pblck;
		target_thread->joinfrom_th = curr_thread_id;
		curr_thread_id = target_th_id;
		swapcontext(cur_th->ctx, target_thread->ctx);
		//setcontext(target_thread->ctx);
		//mypthread_yield();
	}
	return 0;
}


int mypthread_yield(void) {

	mypthread_t* cur_th = srhth(curr_thread_id);
	mypthread_t* next_thread = SNAT(cur_th->th_id);

	if (curr_thread_id == next_thread->th_id)
		return 0;

	curr_thread_id = next_thread->th_id;
	swapcontext(cur_th->ctx, next_thread->ctx);
	return 0;
}

void mypthread_exit(void *retval) {

	mypthread_t* cur_th = srhth(curr_thread_id);
	cur_th->state = Pded;
	free(cur_th->ctx);
	if (cur_th->joinfrom_th != 0) {
		mypthread_t* join_thread = srhth(cur_th->joinfrom_th);
		join_thread->state = Pact;
		cur_th->joinfrom_th;
	}
	mypthread_t* next_thread = SNAT(cur_th->th_id);
	if (curr_thread_id == next_thread->th_id)
		return;
	curr_thread_id = next_thread->th_id;
	setcontext(next_thread->ctx);

}

