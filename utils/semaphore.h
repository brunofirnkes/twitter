#include <mutex>
#include <condition_variable>

class Semaphore {
    private:
    std::mutex _mutex;
    std::condition_variable _condition;
    int _count;

    public:
    Semaphore(int count = 0) : _count(count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(_mutex);
        _condition.wait(lock, [this]() { return _count > 0; });
        _count--;
    }

    void release() {
        std::lock_guard<std::mutex> lock(_mutex);
        _count++;
        _condition.notify_one();
    }
};
