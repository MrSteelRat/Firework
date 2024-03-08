#include "Firework.h"

Firework::Firework(float posX, float posY)
    : position(posX, posY), color(sf::Color::White), speed(2.f), lifetime(100)
{
}

void Firework::update()
{
  position.y -= speed;
  lifetime--;
}

bool Firework::isAlive() const { return lifetime > 0; }

sf::Vector2f Firework::getPosition() const { return position; }

sf::Color Firework::getColor() const { return color; }
