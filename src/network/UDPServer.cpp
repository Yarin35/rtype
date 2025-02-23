/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** UDPServer
*/

#include "../../include/network/UDPServer.hpp"
#include "../../include/network/Protocol.hpp"
#include "../../include/ecs/Component.hpp"

int generateUniqueClientId(std::unordered_set<int> &usedIds)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 1000000);

    int clientId;
    do
    {
        clientId = dis(gen);
    } while (usedIds.find(clientId) != usedIds.end());

    usedIds.insert(clientId);
    return clientId;
}

namespace Network
{

    // Constructor
    UDPServer::UDPServer(const std::string &ipAdress, int port, ECS::Registry &registry) : registry(registry)
    {
        socketFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (socketFd < 0)
        {
            throw std::runtime_error("Failed to create UDP socket");
        }

        // Set socket to non-blocking mode
        int flags = fcntl(socketFd, F_GETFL, 0);
        fcntl(socketFd, F_SETFL, flags | O_NONBLOCK);

        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        inet_pton(AF_INET, ipAdress.c_str(), &serverAddr.sin_addr);

        if (bind(socketFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            close(socketFd);
            throw std::runtime_error("Failed to bind socket to port");
        }

        running = true;
        std::cout << "[Server] Started on port " << port << std::endl;
    }

    // Destructor
    UDPServer::~UDPServer()
    {
        stop();
        std::cout << "[Server] Shutdown" << std::endl;
    }

    // Start receiving messages
    void UDPServer::start()
    {
        receiveThread = std::thread(&UDPServer::receiveMessages, this);
    }

    // Stop the server
    void UDPServer::stop()
    {
        running = false;
        if (receiveThread.joinable())
        {
            receiveThread.join();
        }
        close(socketFd);
    }

    // Send message to client
    void UDPServer::sendMessage(const std::string &message, const sockaddr_in &clientAddr)
    {
        sendto(socketFd, message.c_str(), message.size(), 0,
               (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    }

    // Receive incoming messages
    std::vector<std::pair<int, Network::InputMessage>> UDPServer::receiveMessages()
    {
        std::vector<std::pair<int, Network::InputMessage>> inputs;
        char buffer[1024];
        sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int bytesReceived = 0;

        while ((bytesReceived = recvfrom(socketFd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&clientAddr, &addrLen)) > 0)
        {
            buffer[bytesReceived] = '\0';
            std::string data(buffer);

            // Register client if new
            if (std::find_if(clients.begin(), clients.end(), [&](const sockaddr_in &addr)
                             { return addr.sin_addr.s_addr == clientAddr.sin_addr.s_addr &&
                                      addr.sin_port == clientAddr.sin_port; }) == clients.end())
            {
                clients.push_back(clientAddr);
                int clientId = generateUniqueClientId(usedClientIds);
                clientAddresses[clientId] = clientAddr;
                std::cout << "[Server] New client connected with ID: " << clientId << std::endl;
            }

            // Process input message
            Network::InputMessage input = Network::InputMessage::deserialize(data);
            int clientId = -1;

            for (const auto &pair : clientAddresses)
            {
                if (pair.second.sin_addr.s_addr == clientAddr.sin_addr.s_addr &&
                    pair.second.sin_port == clientAddr.sin_port)
                {
                    clientId = pair.first;
                    break;
                }
            }

            if (clientId == -1)
            {
                // Handle error: client not found
                std::cerr << "[Server] Error: Client not found for received message." << std::endl;
                continue;
            }

            inputs.emplace_back(clientId, input);
        }
        return inputs;
    }

    void UDPServer::broadcastMessage(const std::string &message)
    {
        for (const auto &clientAddr : clients)
        {
            sendMessage(message, clientAddr);
        }
    }

} // namespace Network
