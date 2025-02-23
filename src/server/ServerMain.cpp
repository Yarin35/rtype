/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ServerMain
*/

#include "../../include/network/UDPServer.hpp"
#include "../../include/network/Protocol.hpp"
#include "../../include/ecs/Entity.hpp"
#include "../../include/ecs/Component.hpp"
#include "../../include/ecs/Registry.hpp"
#include "../../include/ecs/MovementSystem.hpp"
#include "../../include/ecs/CollisionSystem.hpp"
#include "../../include/ecs/SpawningSystem.hpp"
#include "../../include/ecs/ShootingSystem.hpp"
#include "../../include/server/ServerLogic.hpp"
#include <iostream>
#include <random>

void handleClientInput(Network::UDPServer &server, int clientId, const Network::InputMessage &input, ECS::Registry &registry)
{
    auto &clientToEntityMap = server.getClientToEntityMap();

    if (clientToEntityMap.find(clientId) == clientToEntityMap.end())
    {
        ECS::Entity player = registry.createEntity();
        registry.addComponent(player, ECS::Position{400.0f, 300.0f});
        registry.addComponent(player, ECS::Velocity{0.0f, 0.0f});
        clientToEntityMap[clientId] = player;
    }

    ECS::Entity entity = clientToEntityMap[clientId];
    auto vel = registry.getComponent<ECS::Velocity>(entity);
    if (vel)
    {
        float speed = 100.0f;
        vel->x = (input.moveRight ? speed : 0.0f) - (input.moveLeft ? speed : 0.0f);
        vel->y = (input.moveDown ? speed : 0.0f) - (input.moveUp ? speed : 0.0f);

        if (input.shoot)
        {
            std::cout << "[Server] Client " << clientId << " is shooting!\n";

            ECS::Entity bullet = registry.createEntity();
            auto pos = registry.getComponent<ECS::Position>(entity);
            if (pos)
            {
                registry.addComponent(bullet, ECS::Position{pos->x + 15, pos->y});
                registry.addComponent(bullet, ECS::Velocity{300, 0});
                registry.addComponent(bullet, ECS::Bullet{true});
            }
        }
    }
}

void gameLoop(Network::UDPServer &server, ECS::Registry &registry)
{
    while (true)
    {
        // 1. Receive inputs.
        auto inputs = server.receiveMessages();

        // 2. Process each input.
        for (auto &input : inputs)
        {
            handleClientInput(server, input.first, input.second, registry);
        }

        // 3. Update ECS.
        registry.updateSystems(0.008f);

        // 4. Broadcast updated world state.
        std::string worldState = Server::serializeWorldState(registry);
        server.broadcastMessage(worldState);

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ipAddress = argv[1];
    int port = std::stoi(argv[2]);

    ECS::Registry registry;

    registry.registerComponent<ECS::Position>();
    registry.registerComponent<ECS::Velocity>();
    registry.registerComponent<ECS::Bullet>();
    registry.registerComponent<ECS::Enemy>();
    registry.registerComponent<ECS::Player>();

    auto movementSystem = std::make_shared<ECS::MovementSystem>();
    movementSystem->setRegistry(&registry);
    registry.addSystem(movementSystem);

    auto collisionSystem = std::make_shared<ECS::CollisionSystem>();
    collisionSystem->setRegistry(&registry);
    registry.addSystem(collisionSystem);

    auto spawningSystem = std::make_shared<ECS::SpawningSystem>();
    spawningSystem->setRegistry(&registry);
    registry.addSystem(spawningSystem);

    auto shootingSystem = std::make_shared<ECS::ShootingSystem>();
    shootingSystem->setRegistry(&registry);
    registry.addSystem(shootingSystem);

    Network::UDPServer server(ipAddress, port, registry);
    collisionSystem->setServer(&server);
    server.start();

    std::cout << "[Server] Game server started on " << ipAddress << ":" << port << ".\n";

    gameLoop(server, registry);

    server.stop();
    return 0;
}