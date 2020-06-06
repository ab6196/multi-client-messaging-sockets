#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9001

int main() {
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error in creating socket\n");
        exit(1);
    }
    printf("Socket created successfully! \n");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int new_socket;
    struct sockaddr_in new_addr;

    char buffer[1024];

    pid_t childPid;

    int bind_status;
    bind_status = bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (bind_status < 0){
        printf("Binding Unsuccessful\n");
        exit(1);
    }
    printf("Socket binding successful!\n");

    listen(server_socket, 10);
    socklen_t newAddrlen;
    
    while(1) {
        new_socket = accept(server_socket, (struct sockaddr *) &new_addr, &newAddrlen);
        if (new_socket < 0) {
            printf("Error in accepting data\n");
            exit(1);
        }
        printf("Connection Successful from %s: %d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));
        if((childPid = fork()) == 0){
            close(server_socket);
            while(1) {
            recv(new_socket, buffer, 1024, 0);
            printf("Message from Client IP: %s:\t %s\n", inet_ntoa(new_addr.sin_addr), buffer);
            send(new_socket, buffer, sizeof(buffer), 0);
            bzero(buffer, sizeof(buffer));
            }
        }
    }
    close(new_socket);
    return 0;
}