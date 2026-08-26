#include "ThreadPool.hpp"

void ThreadPool::worker_loop()
{
    while (true)
    {
        int client_fd;
        struct taskStruct task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // sleep until queue is not empty || server is stopping
            cv.wait(lock, [this]
                    { return !task_queue.empty() || flag; });

            if (flag && task_queue.empty())
                return;
            task = task_queue.front();
            task_queue.pop();
        }
        task.fn(task.file_descriptor);
    }
}

ThreadPool::ThreadPool(unsigned int num_threads)
{
    for (unsigned int i = 0; i < num_threads; ++i)
    {
        workers.emplace_back(&ThreadPool::worker_loop, this);
    }
}

void ThreadPool::addTask(std::function<void(int)> fn, int task_fd)
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push({fn, task_fd});
    }
    cv.notify_one();
}

// Destructor
ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        flag = true;
    }
    cv.notify_all();

    for (std::thread &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}
