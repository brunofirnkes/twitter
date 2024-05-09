#include "manager.h"

// Static variables
std::queue<message*> Manager::queue_msg; 
Semaphore Manager::semaphore_msg;  
std::atomic<bool> Manager::stop_issued;

Manager::Manager(){
    stop_issued = false;
    // Spawn new thread for handling new messages
    if (pthread_create(&handler_thread, NULL, messageHandler, NULL) < 0) {
        std::cerr << "Could not create thread for handling messages " << std::endl;
    }
}

Manager::~Manager(){
    // Wait for thread to finish if stop was fired
    if (stop_issued)
        pthread_join(handler_thread, NULL);
}

void Manager::Stop(){
    // Signal the thread to stop
    stop_issued = true;
}

void Manager::safePostMessage(message* msg){
    semaphore_msg.acquire();
    queue_msg.push(msg);
    semaphore_msg.release(); 
}

message* Manager::safeGetMessage(){
    message* msg;

    semaphore_msg.acquire();
    if (!queue_msg.empty()){
        // Get the first message in queue
        msg = queue_msg.front();
        queue_msg.pop();
    } else {
        msg = nullptr;
    }
    semaphore_msg.release(); 
    return msg;
}

void* Manager::messageHandler(void* arg){
    message* msg;

    // Thread loop. Get messages from queue and process it
    while (!stop_issued && (msg = safeGetMessage())){
        std::cout << msg->msgId << std::endl;;
    }
}