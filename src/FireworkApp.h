#ifndef FIREWORKSAPP_H
#define FIREWORKSAPP_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "Firework.h"

class FireworksApp
{
 public:
  FireworksApp(int width, int height);

  void run();

 private:
  sf::RenderWindow window;
  std::vector<Firework> fireworks;

  void processEvents();
  void update();
  void render();
};

#endif
