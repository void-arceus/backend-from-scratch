#ifndef SERVER_HPP
#define SERVER_HPP

int create_and_bind_socket(int PORT); 
void start_listening(int socket_fd, int backlog);
void handle_client(int client_fd);

#endif

