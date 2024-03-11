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

  sf::Clock clock;             // Add clock for measuring time
  sf::Font font;               // Font for displaying FPS
  sf::Text fpsText;            // Text for displaying FPS
  sf::Text particleCountText;  // Text for displaying particle count
  sf::Text fireworkCountText;  // Text for displaying firework count

  int particleCount;
  int fireworkCount;

  void processEvents();
  void update();
  void render();
  void updateFPS();
};

#endif
