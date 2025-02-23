/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ComponentArray
*/

#ifndef COMPONENTARRAY_HPP_
#define COMPONENTARRAY_HPP_

#include <unordered_map>
#include <memory>
#include <vector>
#include <stdexcept>

namespace ECS {

class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void removeEntity(std::size_t entityID) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
public:
    void insert(std::size_t entityID, const T& component) {
        if (entityToIndexMap.find(entityID) != entityToIndexMap.end())
            throw std::runtime_error("Component already exists for entity.");

        std::size_t newIndex = components.size();
        entityToIndexMap[entityID] = newIndex;
        indexToEntityMap[newIndex] = entityID;
        components.push_back(component);
    }

    void removeEntity(std::size_t entityID) override {
        if (entityToIndexMap.find(entityID) == entityToIndexMap.end())
            return;

        std::size_t indexToRemove = entityToIndexMap[entityID];
        std::size_t lastIndex = components.size() - 1;

        components[indexToRemove] = components[lastIndex];

        std::size_t lastEntity = indexToEntityMap[lastIndex];
        entityToIndexMap[lastEntity] = indexToRemove;
        indexToEntityMap[indexToRemove] = lastEntity;

        components.pop_back();
        entityToIndexMap.erase(entityID);
        indexToEntityMap.erase(lastIndex);
    }

    T* getComponent(std::size_t entityID) {
        if (entityToIndexMap.find(entityID) == entityToIndexMap.end())
            return nullptr;

        return &components[entityToIndexMap[entityID]];
    }

private:
    std::vector<T> components;
    std::unordered_map<std::size_t, std::size_t> entityToIndexMap;
    std::unordered_map<std::size_t, std::size_t> indexToEntityMap;
};

}  // namespace ECS


#endif /* !COMPONENTARRAY_HPP_ */
