/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Component
*/

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include "Entity.hpp"
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <SFML/Graphics.hpp>

namespace ECS {

// Position component for entities (players, bullets).
struct Position {
    float x = 0.0f;
    float y = 0.0f;
};

// Velocity component for movement.
struct Velocity {
    float x = 0.0f;
    float y = 0.0f;
};

// Bullet component to mark entities as projectiles.
struct Bullet {
    bool active = true;  // True if the bullet is still flying.
};

struct Enemy {
    bool active = true;  // True if the enemy is still alive.
};

struct Player {
    bool active = true;  // True if the player is still alive.
};

}  // namespace ECS


#endif /* !COMPONENT_HPP_ */
