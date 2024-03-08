#include "FireworkApp.h"

FireworksApp::FireworksApp(int width, int height)
    : window(sf::VideoMode(width, height), "Fireworks")
{
  window.setFramerateLimit(60);
}

void FireworksApp::run()
{
  while (window.isOpen())
  {
    processEvents();
    update();
    render();
  }
}

void FireworksApp::processEvents()
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed) window.close();

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
      fireworks.push_back(Firework(event.mouseButton.x, event.mouseButton.y));
    }
  }
}

void FireworksApp::update()
{
  for (size_t i = 0; i < fireworks.size(); ++i)
  {
    fireworks[i].update();
    if (!fireworks[i].isAlive())
    {
      fireworks.erase(fireworks.begin() + i);
      --i;
    }
  }
}

void FireworksApp::render()
{
  window.clear();
  for (const auto& firework : fireworks)
  {
    sf::CircleShape particle(2.f);
    particle.setFillColor(firework.getColor());
    particle.setPosition(firework.getPosition());
    window.draw(particle);
  }
  window.display();
}
