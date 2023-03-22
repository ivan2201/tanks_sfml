#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "eventcontroller.hpp"
#include "mainmenu.hpp"

int main()
{
    sf::RenderWindow SFMLView(sf::VideoMode(1280,1024), "tanks");
    SFMLView.setFramerateLimit(FRAMERATE_LIMIT);
    sf::Vector2u windowRect(1024, 1280);
    int result = 0;
    try
    {
      MainMenu & menu = MainMenu::getMainMenu(windowRect);
      result = menu.start(SFMLView);
    }
    catch(std::invalid_argument ex)
    {
      std::ofstream stream;
      stream.open("log.txt");
      if (stream.is_open())
      {
        stream << "error " << ex.what();;
        stream.flush();
        stream.close();
      }
      return 1;
    }
    catch(int res)
    {
      std::ofstream stream;
      stream.open("log.txt");
      if (stream.is_open())
      {
        stream << "error " << res;
        stream.flush();
        stream.close();
      }
      return res;
    }
    catch(...)
    {
      std::ofstream stream;
      stream.open("log.txt");
      if (stream.is_open())
      {
        stream << "unknown error";
        stream.flush();
        stream.close();
      }
      return 2;
    }
    return result;
}
