#ifndef FIREWORKSAPP_H
#define FIREWORKSAPP_H

#include <SFML/Graphics.hpp>
#include <condition_variable>
#include <functional>
#include <span>
#include <thread>
#include <vector>

#include "Particle.h"

class FireworksApp
{
 public:
  FireworksApp(int width, int height);

  void run();

 private:
  sf::RenderWindow window;
  std::vector<std::vector<Particle>> updateFireworks;
  std::vector<std::vector<Particle>> renderFireworks1;
  std::vector<std::vector<Particle>> renderFireworks2;
  std::vector<std::vector<Particle>> renderFireworks3;
  std::vector<std::vector<Particle>>* renderFireworksPtr1;
  std::vector<std::vector<Particle>>* renderFireworksPtr2;
  std::vector<std::vector<Particle>>* renderFireworksPtr3;
  std::vector<sf::Vector2f> mNewParticlePositions;
  std::mutex swapFireworksMutex;
  std::mutex newParticleMutex;

  sf::Clock clock;             // Add clock for measuring time
  sf::Font font;               // Font for displaying FPS
  sf::Text fpsText;            // Text for displaying FPS

  void processEvents();
  void update(float deltaTime);
  void mainUpdateLoop();
  void render(float deltaTime);
  void addParticles(std::span<sf::Vector2f> newParticlePositions);
};

#endif
