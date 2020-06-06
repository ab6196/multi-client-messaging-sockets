#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9001

int main() {
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[1024];

    if (client_socket < 0){
        printf("[-] Error in creating socket\n");
        exit(1);
    }
    printf("Socket successfully created\n");
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    int connection_status;
    connection_status = connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection_status < 0) {
        printf("[-] Error in connecting to server\n");
        exit(1);
    }
    printf("Successfully connected to the server\n");

    while(1) {
        printf("Enter your message for the server: \t");
        scanf("%s", &buffer[0]);
        send(client_socket, buffer, sizeof(buffer), 0);

        recv(client_socket, buffer, 1024, 0);
        printf("Message from server: %s\n", buffer);
    }
    return 0;
}
