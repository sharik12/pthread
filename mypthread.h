
#include "ucontext.h"

enum mypthread_state {
	Pact,
	Pblck,
	Pded
	
};


typedef struct {
 
	int th_id;
	ucontext_t* ctx;
	enum mypthread_state state;
	int valctx;
	int connectid;
	int shedul;
	int joinfrom_th;
} mypthread_t;

typedef struct {
	/*remaing to code ronak -> sharik*/
} mypthread_attr_t;


void mypthread_exit(void *retval);

int mypthread_yield(void);

int mypthread_join(mypthread_t thread, void **retval);

int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg);

struct node {
	struct node *prev;
	mypthread_t *n;
	struct node *next;
}*head, *tail, *temp, *srhnd;









