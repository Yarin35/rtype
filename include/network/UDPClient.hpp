/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPClient
*/

#ifndef UDPCLIENT_HPP_
#define UDPCLIENT_HPP_

#include "Protocol.hpp"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Network {
    class UDPClient {
    public:
        UDPClient(const std::string& serverIp, int serverPort);
        ~UDPClient();

        void sendInput(const InputMessage& input);
        std::string receiveMessage();

    private:
        int sockfd;
        sockaddr_in serverAddr;
    };
}


#endif /* !UDPCLIENT_HPP_ */
