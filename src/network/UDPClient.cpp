/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPClient
*/
#include "../../include/network/UDPClient.hpp"
#include "../../include/network/Protocol.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

namespace Network
{

    // Constructor
    UDPClient::UDPClient(const std::string& ipAddress, int port)
    {
        // Create socket.
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        // Set socket to non-blocking mode.
        int flags = fcntl(sockfd, F_GETFL, 0);
        fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

        // Initialize server address.
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);
    }
    
    // Destructor
    UDPClient::~UDPClient()
    {
        close(sockfd);
        std::cout << "[Client] Disconnected.\n";
    }

    // Send input message to server
    void UDPClient::sendInput(const InputMessage &input)
    {
        std::string message = input.serialize();
        std::cout << "[Client] Sending input: " << message << std::endl;

        int sentBytes = sendto(sockfd, message.c_str(), message.size(), 0,
                               (struct sockaddr *)&serverAddr, sizeof(serverAddr));

        if (sentBytes < 0)
            perror("[Client] Failed to send input");
        else
            std::cout << "[Client] Successfully sent " << sentBytes << " bytes.\n";
    }

    std::string UDPClient::receiveMessage()
    {
        char buffer[1024];
        sockaddr_in senderAddr;
        socklen_t addrLen = sizeof(senderAddr);

        int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                                     (struct sockaddr *)&senderAddr, &addrLen);

        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0';
            std::cout << "[Client] Received message: " << buffer << std::endl;
            return std::string(buffer);
        }

        std::cout << "[Client] No message received.\n";
        return "";
    }

}
