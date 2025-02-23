/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Registry
*/

#ifndef REGISTRY_HPP_
#define REGISTRY_HPP_

#include "Entity.hpp"
#include "Component.hpp"
#include "System.hpp"
#include <unordered_map>
#include <memory>
#include <vector>
#include <typeindex>
#include <set>
#include <stdexcept>
#include <iostream>

namespace ECS
{
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void removeEntity(Entity entity) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray
    {
    public:
        void insert(Entity entity, T component)
        {
            if (entityToIndexMap.find(entity.getId()) != entityToIndexMap.end())
                throw std::runtime_error("Component already exists for entity.");

            std::size_t newIndex = components.size();
            entityToIndexMap[entity.getId()] = newIndex;
            indexToEntityMap[newIndex] = entity.getId();
            components.push_back(component);
        }

        void removeEntity(Entity entity) override
        {
            if (entityToIndexMap.find(entity.getId()) == entityToIndexMap.end())
                return;

            std::size_t indexToRemove = entityToIndexMap[entity.getId()];
            std::size_t lastIndex = components.size() - 1;

            components[indexToRemove] = components[lastIndex];

            Entity::IdType lastEntity = indexToEntityMap[lastIndex];
            entityToIndexMap[lastEntity] = indexToRemove;
            indexToEntityMap[indexToRemove] = lastEntity;

            components.pop_back();
            entityToIndexMap.erase(entity.getId());
            indexToEntityMap.erase(lastIndex);
        }

        T *getComponent(Entity entity)
        {
            if (entityToIndexMap.find(entity.getId()) == entityToIndexMap.end())
                return nullptr;

            return &components[entityToIndexMap[entity.getId()]];
        }

    private:
        std::vector<T> components;
        std::unordered_map<Entity::IdType, std::size_t> entityToIndexMap;
        std::unordered_map<std::size_t, Entity::IdType> indexToEntityMap;
    };

    class Registry
    {
    public:
        Entity createEntity()
        {
            Entity entity(nextEntity++);
            entities.insert(entity);
            return entity;
        }

        Entity::IdType getMaxEntityId() const
        {
            return nextEntity;
        }

        void destroyEntity(Entity entity)
        {
            for (auto &pair : componentArrays)
            {
                auto &componentArray = pair.second;
                componentArray->removeEntity(entity);
            }

            for (auto &system : systems)
            {
                system->entities.erase(entity);
            }

            entities.erase(entity);
        }

        template <typename T>
        void registerComponent()
        {
            const char *typeName = typeid(T).name();

            if (componentArrays.find(typeName) == componentArrays.end())
            {
                componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
            }
        }

        template <typename T>
        void addComponent(Entity entity, T component)
        {
            getComponentArray<T>()->insert(entity, component);
        }

        template <typename T>
        void removeComponent(Entity entity)
        {
            getComponentArray<T>()->removeEntity(entity);
        }

        template <typename T>
        T *getComponent(Entity entity)
        {
            return getComponentArray<T>()->getComponent(entity);
        }

        void addSystem(std::shared_ptr<System> system)
        {
            systems.push_back(system);
        }

        void updateSystems(float dt)
        {
            for (auto &system : systems)
            {
                system->update(dt);
            }
        }

        std::set<Entity> getEntities() const
        {
            return entities;
        }

    private:
        Entity::IdType nextEntity = 0;
        std::unordered_map<const char *, std::shared_ptr<IComponentArray>> componentArrays;
        std::vector<std::shared_ptr<System>> systems;
        std::set<Entity> entities;

        template <typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray()
        {
            const char *typeName = typeid(T).name();

            return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
        }
    };
}

#endif /* !REGISTRY_HPP_ */