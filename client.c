#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>   // Windows-specific sockets library
#include <ws2tcpip.h>   // For inet_pton and other functions

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    int sock;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // Initialize WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Convert address to binary form and store in serverAddr
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        printf("Invalid address or address not supported\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Connection failed\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Send and receive messages in a loop
    while (1) {
        printf("Enter message: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(sock, message, strlen(message), 0);
        
        int read_size = recv(sock, buffer, BUFFER_SIZE, 0);
        if (read_size > 0) {
            buffer[read_size] = '\0';
            printf("Server response: %s\n", buffer);
        }

        memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer
    }

    closesocket(sock);
    WSACleanup();  // Clean up WinSock
    return 0;
}
