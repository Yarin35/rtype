/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Entity
*/

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>

namespace ECS
{
    class Entity
    {
    public:
        using IdType = std::uint32_t;

        Entity() : id(0) {}
        explicit Entity(IdType id) : id(id) {}

        bool operator<(const Entity& other) const
        {
            return id < other.id;
        }

        IdType getId() const
        {
            return id;
        }

    private:
        IdType id;
    };

    const Entity::IdType MAX_ENTITIES = 5000;
}

#endif // ENTITY_HPP