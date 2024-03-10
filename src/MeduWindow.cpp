#include "MenuWindow.h"

MenuWindow::MenuWindow(sf::RenderWindow& window) : m_Window(window) {}

MenuWindow::MenuResult MenuWindow::show()
{
  sf::Font font;
  if (!font.loadFromFile("arial.ttf"))
  {
    // Handle font loading failure
  }

  sf::Text continueText("Continue", font);
  continueText.setCharacterSize(24);
  continueText.setPosition(100, 100);

  sf::Text exitText("Exit", font);
  exitText.setCharacterSize(24);
  exitText.setPosition(100, 150);

  while (m_Window.isOpen())
  {
    sf::Event event;
    while (m_Window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_Window.close();
        return MenuResult::Exit;
      }
      else if (event.type == sf::Event::MouseButtonPressed)
      {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
          sf::Vector2i mousePos = sf::Mouse::getPosition(m_Window);
          // Check if mouse cursor is within "Continue" text
          if (continueText.getGlobalBounds().contains(
                  static_cast<float>(mousePos.x),
                  static_cast<float>(mousePos.y)))
            return MenuResult::Continue;
          // Check if mouse cursor is within "Exit" text
          else if (exitText.getGlobalBounds().contains(
                       static_cast<float>(mousePos.x),
                       static_cast<float>(mousePos.y)))
          {
            m_Window.close();
            return MenuResult::Exit;
          }
        }
      }
    }

    m_Window.clear();
    m_Window.draw(continueText);
    m_Window.draw(exitText);
    m_Window.display();
  }

  // If the window was closed before a choice was made, return Exit as default
  return MenuResult::Exit;
}
