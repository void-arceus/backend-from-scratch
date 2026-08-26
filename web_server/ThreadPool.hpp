#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

struct taskStruct
{
    std::function<void(int)> fn;
    int file_descriptor;
};

class ThreadPool
{
private:
    std::vector<std::thread> workers;
    std::queue<struct taskStruct> task_queue;
    std::condition_variable cv;
    std::mutex queue_mutex;
    bool flag = false;

    // internal loop executed by all worker thread
    void worker_loop();

public:
    ThreadPool(unsigned int num_threads);
    void addTask(std::function<void(int)> fn, int task_fd);
    ~ThreadPool();
};

#endif
