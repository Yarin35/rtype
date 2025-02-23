/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ServerLogic
*/

#include "../../include/server/ServerLogic.hpp"
#include "../../include/ecs/Component.hpp"
#include <sstream>
#include <iostream>

namespace Server
{
    std::string serializeWorldState(ECS::Registry &registry)
    {
        std::ostringstream ss;
        auto entities = registry.getEntities();
        for (const auto &entity : entities)
        {
            auto pos = registry.getComponent<ECS::Position>(entity);
            auto bullet = registry.getComponent<ECS::Bullet>(entity);

            if (pos)
            {
                ss << "ENTITY " << entity.getId() << " ";
                ss << (bullet ? "BULLET " : "CIRCLE ");
                ss << pos->x << " " << pos->y << "\n";
            }
        }
        return ss.str();
    }
}
