#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void Enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop;
};

#endif // !THREAD_MANAGER_H


