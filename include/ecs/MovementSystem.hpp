/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** MovementSystem
*/

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include "System.hpp"
#include "Component.hpp"
#include "Registry.hpp"
#include <iostream>

namespace ECS
{
    class MovementSystem : public System
    {
    public:
        void update(float dt) override
        {
            Entity::IdType maxEntityId = registry->getMaxEntityId();
            for (Entity::IdType entityId = 0; entityId < maxEntityId; entityId++)
            {
                Entity entity(entityId);
                auto position = registry->getComponent<Position>(entity);
                auto velocity = registry->getComponent<Velocity>(entity);

                if (position && velocity)
                {
                    position->x += velocity->x * dt;
                    position->y += velocity->y * dt;
                }
            }
        }

        void setRegistry(Registry *reg)
        {
            registry = reg;
        }

    private:
        Registry *registry;
    };
}

#endif /* !MOVEMENTSYSTEM_HPP_ */