/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

#include "../../include/ecs/Registry.hpp"
#include <iostream>

namespace ECS {

    Entity Registry::createEntity() {
        return nextEntity++;
    }

    void Registry::destroyEntity(Entity entity) {
        for (auto& pair : componentArrays) {
            auto& componentArray = pair.second;
            componentArray->removeEntity(entity.id);
        }

        for (auto& system : systems) {
            system->entities.erase(entity);
        }
        std::cout << "Destroyed entity " << entity.id << "\n";
    }

    void Registry::updateSystems(float dt) {
        for (auto& system : systems) {
            system->update(dt);
        }
    }

}
