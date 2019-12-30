#include <stdio.h>
#include <pthread.h>
#include "Thread.h"

std::mutex SynchroObjects::synchro_mutex; //Static initialization is needed

namespace gm {
    
void Thread::join() {
    pthread_join(thd, NULL);
}

void Thread::start() {
    pthread_create (&thd, NULL, &Thread::startThread, this);
}

void* Thread::startThread(void* thd) {
    ((Thread*)thd)->run();
    ((Runnable*)thd)->doCallback((Runnable*)thd);
}

}