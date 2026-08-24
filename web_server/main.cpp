#include <thread>
#include <iostream>
#include <arpa/inet.h>

#include "server.hpp"
#include "ThreadPool.hpp"
#include "proxy.hpp"

#define PORT 8080
#define BACKLOG 10

int main()
{
    proxy(); 
    
    /*
    int server_fd = create_and_bind_socket(PORT);
    start_listening(server_fd, BACKLOG);

    std::cout << "SERVER is running on PORT: " << PORT << "\n";
    unsigned int num_threads = std::thread::hardware_concurrency();

    ThreadPool pool(num_threads);

    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);

        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
        if (client_fd == -1)
            continue;

        pool.addTask(client_fd);
    }
    */ 
    return 0;
}
