#ifndef FIREWORK_H
#define FIREWORK_H

#include <SFML/Graphics.hpp>

class Firework
{
 public:
  Firework(float posX, float posY);

  void update();
  bool isAlive() const;
  sf::Vector2f getPosition() const;
  sf::Color getColor() const;

 private:
  sf::Vector2f position;
  sf::Color color;
  float speed;
  int lifetime;
};

#endif
