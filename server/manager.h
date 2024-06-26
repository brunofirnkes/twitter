#ifndef MANAGER_H
#define MANAGER_H

#include <queue>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <map>

#include "../utils/semaphore.h"
#include "../utils/datatypes.h"
#include "../utils/constants.h"
#include "../utils/genericutils.h"

class Manager {
    private:
    static std::atomic<bool> stop_issued;   // Stop issued to stop the thread

    static std::queue<_message*> queue_msg; // Messages queue
    static Semaphore semaphore_msg;         // Messages semaphore
    pthread_t handler_thread;               // Thread to handle messages 

    /**
     * Safely get a message for processing
     * @return _message*
    */
    static _message* safeGetMessage();

    /**
     * Thread message handler
    */
    static void* messageHandler(void* arg);

    public:
    /**
     * Class constructor
    */
    Manager();

    /**
     * Class destructor
    */
    ~Manager();

    /**
     * Send a signal to stop the thread
    */
    void Stop();

    /**
     * Safely post a message for processing
     * @param msg to be processed
    */
    void safePostMessage(_message* msg);

    /**
     * Client commands
    */
    std::string commHelp();
    std::string commCreateUser();
};

#endif