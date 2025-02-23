/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ServerLogic
*/

#ifndef SERVERLOGIC_HPP_
#define SERVERLOGIC_HPP_

#include "../ecs/Entity.hpp"
#include "../network/Protocol.hpp"
#include "../ecs/Registry.hpp"

namespace Server {
    std::string serializeWorldState(ECS::Registry& registry);
}


#endif /* SERVERLOGIC_HPP_ */
