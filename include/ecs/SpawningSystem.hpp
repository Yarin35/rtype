/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** SpawningSystem
*/

#ifndef SPAWNINGSYSTEM_HPP_
#define SPAWNINGSYSTEM_HPP_

#include "System.hpp"
#include "Component.hpp"
#include "Registry.hpp"
#include <random>

namespace ECS
{
    class SpawningSystem : public System
    {
    public:
        SpawningSystem() : enemySpawnCounter(0), maxEnemies(2) {}

        void update(float dt) override
        {
            if (++enemySpawnCounter > 500)
            {
                if (countActiveEnemies() < maxEnemies)
                {
                    spawnEnemy();
                }
                enemySpawnCounter = 0;
            }
        }

        void setRegistry(Registry *reg)
        {
            registry = reg;
        }

    private:
        void spawnEnemy()
        {
            static std::default_random_engine generator;
            static std::uniform_real_distribution<float> distributionY(0.0f, 600.0f);

            ECS::Entity enemy = registry->createEntity();
            registry->addComponent(enemy, ECS::Position{800.0f, distributionY(generator)});
            registry->addComponent(enemy, ECS::Velocity{0.0f, 0.0f});
            registry->addComponent(enemy, ECS::Enemy{true});
        }

        int countActiveEnemies()
        {
            int count = 0;
            auto entities = registry->getEntities();
            for (const auto &entity : entities)
            {
                auto enemy = registry->getComponent<Enemy>(entity);
                if (enemy && enemy->active)
                {
                    count++;
                }
            }
            return count;
        }

        Registry *registry;
        int enemySpawnCounter;
        const int maxEnemies;
    };
} // namespace ECS

#endif /* !SPAWNINGSYSTEM_HPP_ */