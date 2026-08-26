#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "../server.hpp"
#include "../ThreadPool.hpp"

#define PORT 3000
#define BACKLOG 10

void server_one()
{
    int server_one_fd = create_and_bind_socket(PORT);
    if (server_one_fd == -1)
    {
        perror("SERVER ONE FD:");
        return;
    }

    start_listening(server_one_fd, BACKLOG);

    unsigned int num_threads = std::thread::hardware_concurrency();

    ThreadPool Pool(num_threads);

    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(struct sockaddr_in);

        int server_one_client_fd = accept(server_one_fd, (struct sockaddr *)&client_addr, &len);

        if (server_one_client_fd == -1)
        {
            continue;
        }
        Pool.addTask(handle_client, server_one_client_fd);
    }
}
