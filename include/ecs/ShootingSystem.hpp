/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ShootingSystem
*/

#ifndef SHOOTINGSYSTEM_HPP_
#define SHOOTINGSYSTEM_HPP_

#include "System.hpp"
#include "Component.hpp"
#include "Registry.hpp"
#include <random>

namespace ECS
{
    class ShootingSystem : public System
    {
    public:
        void update(float dt) override
        {
            static std::default_random_engine generator;
            static std::uniform_int_distribution<int> shootChance(0, 100);

            auto entities = registry->getEntities();
            for (const auto &entity : entities)
            {
                auto enemy = registry->getComponent<Enemy>(entity);
                if (enemy && enemy->active && shootChance(generator) < 5)
                {
                    auto pos = registry->getComponent<Position>(entity);
                    if (pos)
                    {
                        ECS::Entity bullet = registry->createEntity();
                        registry->addComponent(bullet, ECS::Position{pos->x - 15, pos->y});
                        registry->addComponent(bullet, ECS::Velocity{-300.0f, 0.0f});
                        registry->addComponent(bullet, ECS::Bullet{true});
                    }
                }

                auto bullet = registry->getComponent<Bullet>(entity);
                if (bullet && !bullet->active)
                {
                    registry->destroyEntity(entity);
                }

                auto pos = registry->getComponent<Position>(entity);
                if (pos && (pos->x < 0 || pos->x > 800 || pos->y < 0 || pos->y > 600))
                {
                    registry->destroyEntity(entity);
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
} // namespace ECS

#endif /* !SHOOTINGSYSTEM_HPP_ */