/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Protocol
*/

#include "../../include/network/Protocol.hpp"
#include <sstream>

namespace Network {

    std::string serializeInput(const InputMessage& input) {
        std::ostringstream oss;
        oss << input.moveUp << input.moveDown << input.moveLeft
            << input.moveRight << input.shoot;
        return oss.str();
    }

    InputMessage deserializeInput(const std::string& data) {
        InputMessage input;
        input.moveUp    = data[0] == '1';
        input.moveDown  = data[1] == '1';
        input.moveLeft  = data[2] == '1';
        input.moveRight = data[3] == '1';
        input.shoot     = data[4] == '1';
        return input;
    }

    std::string serializeState(const std::vector<EntityState>& states) {
        std::ostringstream oss;
        for (const auto& state : states) {
            oss << state.entityId << ' ' << state.x << ' ' << state.y << ' ' << state.isBullet << ';';
        }
        return oss.str();
    }

    std::vector<EntityState> deserializeState(const std::string& data) {
        std::vector<EntityState> states;
        std::istringstream iss(data);
        std::string entityData;
        while (std::getline(iss, entityData, ';')) {
            EntityState state;
            std::istringstream entityStream(entityData);
            entityStream >> state.entityId >> state.x >> state.y >> state.isBullet;
            states.push_back(state);
        }
        return states;
    }

}
