#ifndef MAINMENU_HPP
#define MAINMENU_HPP
#include <atomic>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "eventcontroller.hpp"
#include "resources.hpp"
#include "game.hpp"
#include "fps.hpp"
#include "windowmutex.hpp"

class MainMenu
{
public:

  ~MainMenu();

  int start(sf::RenderWindow & view);
  
  static MainMenu & getMainMenu(const sf::Vector2u rect)
  {
    static MainMenu instance(rect);
    return instance;
  }

private:

  MainMenu(const sf::Vector2u rect);
  
  bool m_needResize;
  sf::Vector2u m_windowRect;
  Game & m_game;
  bool m_quit;
  std::atomic_int m_part;

  friend void resizeMainMenu(const sf::Vector2u rect);

  void escKeyDown();

  void voidKeyUp();
};

#endif
