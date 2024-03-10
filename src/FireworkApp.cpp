#include "FireworkApp.h"

#include <iostream>
#include <sstream>  // For string manipulation

#include "MenuWindow.h"

FireworksApp::FireworksApp(int width, int height)
    : window(sf::VideoMode(width, height), "Fireworks")
{
  window.setFramerateLimit(60);

  // Load font for displaying FPS
  if (!font.loadFromFile("arial.ttf"))
  {
    std::cerr << "Arial_not found" << std::endl;
    ;
  }
  fpsText.setFont(font);
  fpsText.setCharacterSize(20);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition(10.f, 10.f);
}

void FireworksApp::run()
{
  while (window.isOpen())
  {
    MenuWindow menu(window);
    MenuWindow::MenuResult result = menu.show();

    if (result == MenuWindow::MenuResult::Continue)
    {
      while (window.isOpen())  // Game loop
      {
        processEvents();
        update();
        render();
        updateFPS();  
      }
    }
    else if (result == MenuWindow::MenuResult::Exit)
    {
      window.close();
    }
  }
}

void FireworksApp::processEvents()
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
      window.close();
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        fireworks.push_back(Firework(event.mouseButton.x, event.mouseButton.y));
      }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape)
      {
        MenuWindow menu(window);
        MenuWindow::MenuResult result = menu.show();

        if (result == MenuWindow::MenuResult::Continue)
        {
          // Continue game
        }
        else if (result == MenuWindow::MenuResult::Exit)
        {
          window.close();
        }
      }
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
  window.draw(fpsText);  // Draw FPS counter
  window.display();
}

void FireworksApp::updateFPS()
{
  // Calculate FPS and update FPS text
  float fps = 1.f / clock.restart().asSeconds();
  std::stringstream ss;
  ss << "FPS: " << static_cast<int>(fps);
  fpsText.setString(ss.str());
}
