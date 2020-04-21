an implementaion of od user lever thread library

how to run -
----------------
1. run Makefile
 or 

1. gcc mypthread.c test.c -o test.o
2. ./test.o


mypthread Library fuctions:
----------------------
functions are implemented.
1. create
2. yield
3. join
4. exit


circular queue is implemented which has  haead and tail pointers . it is queue of nodes conatining thread and thread id.
following functions:
1. enqueue() - insert node 
2. SNAT() - Searches next active thread
3. returnHead() - Returns Head node
4. returnTail - Returns Tail node
5. srcth - Searches  thread in queue


create()

	 getcontext() used for creation of threads. makecontext for initialization. first main thread is creadted and enqueue() at head of queue
	 tid for main thread is 1 and other thread are givien tid in incremented order. new thread state is active

yield()
	finds next active thread and swaps context using swapcontext() fun

join()
	 simmilar to yeild it finds next active thread and swapcontext()
	sets current thread state to blocked

exit()
	sets state as Dead . before doing so checks all thread  to join. free all context 



mypthread Library issues:
----------------------
 in some cases mypthread_join() has issues with swapping context . 















