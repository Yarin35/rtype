/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPServer
*/

#ifndef UDPSERVER_HPP_
#define UDPSERVER_HPP_

#include <string>
#include <thread>
#include <netinet/in.h>
#include <vector>
#include "Protocol.hpp"
#include "../ecs/Entity.hpp"
#include "../server/ServerLogic.hpp"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <fcntl.h>
#include <arpa/inet.h>
#include <random>

namespace Network
{
    class UDPServer
    {
    public:
        UDPServer(const std::string& ipAddress, int port, ECS::Registry &registry);
        ~UDPServer();

        void start();
        void stop();

        void sendMessage(const std::string &message, const sockaddr_in &clientAddr);
        void broadcastMessage(const std::string &message);
        std::vector<std::pair<int, InputMessage>> receiveMessages();

        std::unordered_map<int, ECS::Entity>& getClientToEntityMap() { return clientToEntityMap; }

        sockaddr_in getClientAddress(int clientId) const {
            auto it = clientAddresses.find(clientId);
            if (it != clientAddresses.end()) {
                return it->second;
            }
            throw std::runtime_error("Client ID not found");
        }

        private:
        std::vector<sockaddr_in> clients;
        int socketFd;
        std::thread receiveThread;
        bool running = false;
        std::unordered_map<int, ECS::Entity> clientToEntityMap;
        ECS::Registry &registry;
        std::unordered_map<int, sockaddr_in> clientAddresses;
        std::unordered_set<int> usedClientIds;
        
    };
}

#endif /* !UDPSERVER_HPP_ */
