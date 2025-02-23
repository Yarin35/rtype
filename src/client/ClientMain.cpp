/*
** EPITECH PROJECT, 2025
** rtype
** File description:
** ClientMain
*/

#include "../../include/network/UDPClient.hpp"
#include "../../include/network/Protocol.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <unordered_map>
#include <vector>

struct EntityState
{
    int id;
    std::string type;
    float x, y;
};

std::vector<EntityState> parseWorldState(const std::string &data)
{
    std::vector<EntityState> entities;
    std::istringstream ss(data);
    std::string line;

    while (std::getline(ss, line))
    {
        std::istringstream ls(line);
        std::string keyword;
        EntityState entity;

        ls >> keyword >> entity.id >> entity.type >> entity.x >> entity.y;
        if (keyword == "ENTITY")
        {
            if (entity.type == "BULLET" && entity.x == 0.0f && entity.y == 0.0f)
                continue;
            entities.push_back(entity);
        }
    }

    return entities;
}

void setNonBlockingInput(bool enable)
{
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    if (enable)
    {
        tty.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo.
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    }
    else
    {
        tty.c_lflag |= ICANON | ECHO;
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS> <PORT>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ipAddress = argv[1];
    int port = std::stoi(argv[2]);

    Network::UDPClient client(ipAddress, port);
    sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type Client");
    window.setFramerateLimit(60);

    std::unordered_map<int, sf::CircleShape> entityViews;
    setNonBlockingInput(true);

    while (window.isOpen())
    {
        // Handle SFML events (window close).
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Handle input (both terminal and keyboard).
        Network::InputMessage input{};
        char key;
        while (read(STDIN_FILENO, &key, 1) > 0)
        {
            switch (key)
            {
            case 'w':
                input.moveUp = true;
                break;
            case 'a':
                input.moveLeft = true;
                break;
            case 's':
                input.moveDown = true;
                break;
            case 'd':
                input.moveRight = true;
                break;
            case ' ':
                input.shoot = true;
                break;
            default:
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            input.moveUp = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            input.moveLeft = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            input.moveDown = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            input.moveRight = true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            input.shoot = true;

        // Send input and receive world state.
        client.sendInput(input);
        std::string worldState = client.receiveMessage();
        if (!worldState.empty())
        {
            if (worldState == "TERMINATE")
            {
                std::cout << "[Client] Termination message received. Exiting...\n";
                exit(0);
            }

            auto entities = parseWorldState(worldState);

            // Update entity views.
            std::unordered_map<int, bool> activeEntities;
            for (const auto &entity : entities)
            {
                activeEntities[entity.id] = true;
                if (entityViews.find(entity.id) == entityViews.end())
                {
                    sf::CircleShape shape(entity.type == "BULLET" ? 5.0f : 15.0f);
                    shape.setFillColor(entity.type == "BULLET" ? sf::Color::Red : sf::Color::Green);
                    entityViews[entity.id] = shape;
                }
                entityViews[entity.id].setPosition(entity.x, entity.y);
            }

            // Remove entities that are no longer in the world active.
            for (auto it = entityViews.begin(); it != entityViews.end();)
            {
                if (activeEntities.find(it->first) == activeEntities.end())
                {
                    it = entityViews.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        // Render SFML window.
        window.clear();
        for (auto &shape : entityViews)
        {
            window.draw(shape.second);
        }
        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    setNonBlockingInput(false);
    return 0;
}
