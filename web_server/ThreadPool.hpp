#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

class ThreadPool {
    private:
        std::vector<std::thread> workers; 
        std::queue<int> task_queue;
        std::condition_variable cv;
        std::mutex queue_mutex;
        bool flag = false;

        // internal loop executed by all worker thread
        void worker_loop(); 
    public: 
        ThreadPool(unsigned int num_threads);
        void addTask (int task_fd);
        ~ThreadPool();
};

#endif
