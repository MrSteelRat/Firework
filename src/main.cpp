#include "FireworkApp.h"

int main()
{
  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;

  FireworksApp app(WINDOW_WIDTH, WINDOW_HEIGHT);
  app.run();

  return 0;
}
