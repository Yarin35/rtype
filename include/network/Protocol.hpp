/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** Protocol
*/

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace Network {
    enum class MessageType : uint8_t {
        INPUT,     // Client -> Server: Move/Shoot
        STATE,     // Server -> Client: Entity positions
        CONNECT,   // Client -> Server: New connection
        DISCONNECT // Client -> Server: Client leaving
    };

    struct MessageHeader {
        MessageType type;
        uint32_t size;
    };

    struct InputMessage {
        bool moveLeft = false;
        bool moveRight = false;
        bool moveUp = false;
        bool moveDown = false;
        bool shoot = false;
    
        // Serialize inputs into a string for network transmission.
        std::string serialize() const {
            return std::string() +
                   (moveLeft ? '1' : '0') +
                   (moveRight ? '1' : '0') +
                   (moveUp ? '1' : '0') +
                   (moveDown ? '1' : '0') +
                   (shoot ? '1' : '0');
        }
    
        // Deserialize string back into an InputMessage.
        static InputMessage deserialize(const std::string& data) {
            InputMessage input;
            if (data.size() >= 5) {
                input.moveLeft = (data[0] == '1');
                input.moveRight = (data[1] == '1');
                input.moveUp = (data[2] == '1');
                input.moveDown = (data[3] == '1');
                input.shoot = (data[4] == '1');
            }
            return input;
        }
    };
    

    struct EntityState {
        uint32_t entityId;
        float x, y;
        bool isBullet;
    };

    std::string serializeInput(const InputMessage& input);
    InputMessage deserializeInput(const std::string& data);
    std::string serializeState(const std::vector<EntityState>& states);
    std::vector<EntityState> deserializeState(const std::string& data);
}


#endif /* !PROTOCOL_HPP_ */
