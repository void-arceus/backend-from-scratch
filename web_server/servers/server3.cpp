#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "../server.hpp"
#include "../ThreadPool.hpp"

#define PORT 3002
#define BACKLOG 10

void server_three()
{
    int server_three_fd = create_and_bind_socket(PORT);
    if (server_three_fd == -1)
    {
        perror("SERVER THREE FD:");
        return;
    }

    start_listening(server_three_fd, BACKLOG);

    unsigned int num_threads = std::thread::hardware_concurrency();

    ThreadPool Pool(num_threads);

    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);

        int server_three_client_fd = accept(server_three_fd, (struct sockaddr *)&client_addr, &len);

        if (server_three_client_fd == -1)
        {
            continue;
        }
        Pool.addTask(handle_client, server_three_client_fd);
    }
}
