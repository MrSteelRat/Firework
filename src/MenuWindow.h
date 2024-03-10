#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <SFML/Graphics.hpp>

class MenuWindow
{
 public:
  MenuWindow(sf::RenderWindow& window);

  enum class MenuResult
  {
    Continue,
    Exit
  };

  MenuResult show();

 private:
  sf::RenderWindow& m_Window;
};

#endif
