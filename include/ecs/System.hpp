/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** System
*/

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include <set>
#include "Entity.hpp"

namespace ECS
{
    class System
    {
    public:
        std::set<Entity> entities;

        virtual void update(float dt) = 0;
    };
}

#endif /* !SYSTEM_HPP_ */
