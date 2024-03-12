#ifndef FIREWORK_H
#define FIREWORK_H

#include <SFML/Graphics.hpp>
#include <deque>

class Particle
{
 public:
  Particle(float posX, float posY);

  void update(float deltaTime);
  bool isAlive() const;
  sf::Vector2f getPosition() const;
  sf::Color getColor() const;
  const std::deque<std::pair<sf::Vector2f, sf::Color>>& getTail() const;

 private:
  sf::Vector2f position;
  sf::Color mColor;
  sf::Vector2f speed;
  float lifetime;
  float secondsElapsed;
  std::deque<std::pair<sf::Vector2f, sf::Color>> tail;
};

#endif