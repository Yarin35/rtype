/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** CollisionSystem
*/

#ifndef COLLISIONSYSTEM_HPP_
#define COLLISIONSYSTEM_HPP_

#include "System.hpp"
#include "Component.hpp"
#include "Registry.hpp"
#include "../../include/network/UDPServer.hpp"
#include <cmath>

namespace ECS
{
    class CollisionSystem : public System
    {
    public:
        void update(float dt) override
        {
            auto entities = registry->getEntities();
            for (const auto &entity : entities)
            {
                auto pos = registry->getComponent<Position>(entity);
                if (!pos) continue;

                for (const auto &target : entities)
                {
                    if (entity.getId() == target.getId())
                        continue;

                    auto targetPos = registry->getComponent<Position>(target);
                    if (!targetPos) continue;

                    float distance = std::hypot(pos->x - targetPos->x, pos->y - targetPos->y);
                    if (distance < 10.0f)
                    {
                        registry->destroyEntity(entity);
                        registry->destroyEntity(target);

                        // Send termination message to client
                        auto &clientToEntityMap = server->getClientToEntityMap();
                        for (const auto &pair : clientToEntityMap)
                        {
                            if (pair.second.getId() == entity.getId() || pair.second.getId() == target.getId())
                            {
                                try
                                {
                                    sockaddr_in clientAddress = server->getClientAddress(pair.first);
                                    server->sendMessage("TERMINATE", clientAddress);
                                }
                                catch (const std::runtime_error &e)
                                {
                                    std::cerr << "Error: " << e.what() << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }

        void setRegistry(Registry *reg)
        {
            registry = reg;
        }

        void setServer(Network::UDPServer *srv)
        {
            server = srv;
        }

    private:
        Registry *registry;
        Network::UDPServer *server;
    };
} // namespace ECS

#endif /* !COLLISIONSYSTEM_HPP_ */