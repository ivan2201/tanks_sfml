#include "mainmenu.hpp"
#include <SFML/Graphics.hpp>
#include <math.h>

void resizeMainMenu(const RECT rect)
{
  MainMenu & mainMenu = MainMenu::getMainMenu(rect);
  mainMenu.m_windowRect = rect;
  mainMenu.m_needResize = true;
}

MainMenu::MainMenu(const RECT windowRect):
  m_needResize(false),
  m_windowRect(windowRect),
  m_game(Game::getGame()),
  m_quit(false),
  m_part(0)
{
  m_game.setWindowRect(windowRect);
  /*m_quitPtr = &m_quit;
  m_partPtr = &m_part;
  m_gamePtr = &m_game;
  m_windowRectPtr = &m_windowRect;*/
}

MainMenu::~MainMenu()
{
}

int MainMenu::start(sf::RenderWindow & view)
{
  Resources & m_resources = Resources::getResources();
  EventController & eventController = EventController::getEventController();
  static std::atomic< int > & part = m_part;
  static bool & quit = m_quit;
  static RECT & windowRect = m_windowRect;
  while (!m_quit)
  {
    m_needResize = false;
    eventController.addOnKeyListener(sf::Keyboard::Escape, []()
    {
      int lastPart = part.load();
      if (lastPart != 2 && lastPart != 3) quit = true;
    }, []() {});
    sf::Texture logo = m_resources.getTexture("logo");
    sf::Sprite logoImage(logo);
    float width = logoImage.getTextureRect().width;
    logoImage.setOrigin(sf::Vector2f(logoImage.getTexture()->getSize()) / 2.f);
    logoImage.setPosition(logoImage.getOrigin());

    // Create a clock for measuring elapsed time
    sf::Clock clock;

    // Loop until a WM_QUIT message is received
    sf::Font font = m_resources.getFont("font");
    sf::Text tanks("TANKS", font, m_windowRect.bottom / 4);
    sf::Text pressKey("PRESS SPACE TO CONTINUE", font, m_windowRect.bottom / 20);
    tanks.setPosition(m_windowRect.right / 2 - tanks.getLocalBounds().width / 2, 0);
    pressKey.setPosition(m_windowRect.right / 2 - pressKey.getLocalBounds().width / 2,
                         m_windowRect.bottom - 30 - pressKey.getGlobalBounds().height);
    auto onSpaceListener = []()
    {
      switch (part.load())
      {
      case 0:
        part++;
        return;
      default:
        return;
      }
    };
    eventController.addOnKeyListener(sf::Keyboard::Space, []() {}, onSpaceListener);
    sf::Texture menuItemTexture(m_resources.getTexture("menu_item"));
    static sf::Sprite menuItem(menuItemTexture);
    menuItem.setOrigin(sf::Vector2f(menuItem.getTexture()->getSize()) / 2.f);
    auto LMouseListener = [](int x, int y)
    {
      if (part.load() == 1)
      {
        if (x > (windowRect.right / 2 - menuItem.getLocalBounds().width / 2)
            && x < (windowRect.right / 2 + menuItem.getLocalBounds().width / 2))
        {
          if (y > (windowRect.bottom / 4 - menuItem.getLocalBounds().height / 2)
              && y < (windowRect.bottom / 4 + menuItem.getLocalBounds().height / 2))
          {
            part = 2;
          } else if (y > (windowRect.bottom / 2 - menuItem.getLocalBounds().height / 2)
              && y < (windowRect.bottom / 2 + menuItem.getLocalBounds().height / 2))
          {
            part = 3;
          } else if (y > (windowRect.bottom * 3 / 4 - menuItem.getLocalBounds().height / 2)
              && y < (windowRect.bottom * 3 / 4 + menuItem.getLocalBounds().height / 2))
          {
            quit = true;
          }
        }
      }
    };
    float prevTime = clock.getElapsedTime().asSeconds();
    while (!m_quit && !m_needResize)
    {
      while ((clock.getElapsedTime().asSeconds() - prevTime) < (1.0f / FRAMERATE_LIMIT));
      prevTime = clock.getElapsedTime().asSeconds();
      while (!m_quit && m_part == 0)
      {
        float time = clock.getElapsedTime().asSeconds();
        // Clear views
        view.clear();
        // Draw sprite 1 on view 1
        logoImage.setPosition((0.5f + (m_windowRect.right - width) * std::cos(time * 0.75f) / 2
                              / m_windowRect.right) * m_windowRect.right,
                              tanks.getLocalBounds().height + logoImage.getLocalBounds().height + 10);
        view.draw(tanks);
        view.draw(pressKey);
        view.draw(logoImage);
        view.display();
      }
      eventController.setLMouseClickListener(LMouseListener);
      sf::Text new_game("new game", font, menuItem.getTextureRect().height / 2);
      sf::Text highscores("show logo", font, menuItem.getTextureRect().height / 2);
      sf::Text exit("quit", font, menuItem.getTextureRect().height / 2);
      view.clear();
      menuItem.setPosition(m_windowRect.right / 2, m_windowRect.bottom / 4);
      new_game.setPosition(m_windowRect.right / 2 - new_game.getGlobalBounds().width / 2,
                           m_windowRect.bottom / 4 - new_game.getGlobalBounds().height);
      view.draw(menuItem);
      menuItem.setPosition(m_windowRect.right / 2, m_windowRect.bottom / 2);
      highscores.setPosition(m_windowRect.right / 2 - highscores.getGlobalBounds().width / 2,
                             m_windowRect.bottom / 2 - highscores.getGlobalBounds().height);
      view.draw(menuItem);
      menuItem.setPosition(m_windowRect.right / 2, m_windowRect.bottom * 3 / 4);
      exit.setPosition(m_windowRect.right / 2 - exit.getGlobalBounds().width / 2,
                       m_windowRect.bottom * 3 / 4 - exit.getGlobalBounds().height);
      view.draw(menuItem);
      view.draw(new_game);
      view.draw(highscores);
      view.draw(exit);
      view.display();
      while (!m_quit && m_part == 1 && m_needResize)
      {
      }
      switch (m_part.load())
      {
      case 2:
        m_game.start(view);
        m_part = 1;
        break;
      case 3:
        //break;
      default:
        m_part = 0;
      }
    }
  }
  return 0;
}
