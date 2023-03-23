#include "mainmenu.hpp"

void resizeMainMenu(const sf::Vector2u rect)
{
  MainMenu & mainMenu = MainMenu::getMainMenu(rect);
  mainMenu.m_windowRect = rect;
  mainMenu.m_needResize = true;
}

MainMenu::MainMenu(const sf::Vector2u windowRect):
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
  //start eventListener
  EventController & eventController = EventController::getEventController();
  //setting eventListener's window for pollEvent
  eventController.setWindow(view);
  static std::atomic< int > & part = m_part;
  static bool & quit = m_quit, & needResize = m_needResize;
  sf::Vector2u & windowRect = m_windowRect;
  static sf::Vector2u touchRect = m_windowRect;
  sf::Texture logo = m_resources.getTexture("logo");
  sf::Sprite logoImage(logo);
  logoImage.setOrigin(sf::Vector2f(logoImage.getTexture()->getSize()) / 2.f);
  float logoWidth = logoImage.getOrigin().x;
  float logoHeight = logoImage.getOrigin().y;
  logoImage.setPosition(logoImage.getOrigin());

  // Create a clock for measuring elapsed time
  sf::Clock clock;
  sf::Font font = m_resources.getFont("font");
  
  sf::Texture menuItemTexture(m_resources.getTexture("menu_item"));
  static sf::Sprite menuItem(menuItemTexture);
  menuItem.setOrigin(sf::Vector2f(menuItem.getTexture()->getSize()) / 2.f);
  
  sf::Text new_game("new game", font, menuItem.getTextureRect().height / 2);
  sf::Text highscores("show logo", font, menuItem.getTextureRect().height / 2);
  sf::Text exit("quit", font, menuItem.getTextureRect().height / 2);
  auto LMouseListener = [](int x, int y)
  {
    if (part.load() == 1)
    {
      if (x > (touchRect.x / 2 - menuItem.getGlobalBounds().width / 2) &&
          x < (touchRect.x / 2 + menuItem.getGlobalBounds().width / 2))
      {
        if (y > (touchRect.y / 4 - menuItem.getGlobalBounds().height / 2) &&
            y < (touchRect.y / 4 + menuItem.getGlobalBounds().height / 2))
        {
          part = 2;
        } else if (y > (touchRect.y / 2 - menuItem.getGlobalBounds().height / 2) &&
                   y < (touchRect.y / 2 + menuItem.getGlobalBounds().height / 2))
        {
          part = 3;
        } else if (y > (touchRect.y * 3 / 4 - menuItem.getGlobalBounds().height / 2) &&
                   y < (touchRect.y * 3 / 4 + menuItem.getGlobalBounds().height / 2))
        {
          quit = true;
        }
      }
    }
  };
  
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
  
  auto onEscListener = []()
  {
    int lastPart = part.load();
    if (lastPart != 2 && lastPart != 3) quit = true;
  };
  
  auto onResizeListener = [](unsigned int x, unsigned int y)
  {
    needResize = true;
    touchRect = {x,y};
  };
  eventController.addOnKeyListener(sf::Keyboard::Escape, onEscListener, []() {});
  eventController.addOnKeyListener(sf::Keyboard::Space, []() {}, onSpaceListener);
  eventController.setOnChangeSizeListener(onResizeListener);
  eventController.setOnLMouseClickListener(LMouseListener);
  while (!quit)
  {   
    while (!m_quit)
    {
      needResize = false;
      sf::Text tanks("TANKS", font, windowRect.y / 4);
      sf::Text pressKey("PRESS SPACE TO CONTINUE", font, windowRect.y / 20);
      tanks.setPosition(windowRect.x / 2 - tanks.getLocalBounds().width / 2, 0);
      pressKey.setPosition(windowRect.x / 2 - pressKey.getLocalBounds().width / 2,
                           windowRect.y - 30 - pressKey.getGlobalBounds().height);
      while (!quit && m_part == 0 && !needResize)
      {
        baho::waitNextFrame();
        //lock view
        WindowMutex windowMutex;
        // Clear views
        view.clear();
        // Draw sprite 1 on view 1
        logoImage.setPosition(logoWidth + (0.5f + 
                                           std::sin(clock.getElapsedTime().asSeconds() * 
                                                    0.75f) / 2) *
                                          (windowRect.x - 2 * logoWidth),
                              tanks.getLocalBounds().height + logoHeight + 10);
        view.draw(tanks);
        view.draw(pressKey);
        view.draw(logoImage);
        view.display();
      }
      needResize = true;
      while (!quit && m_part == 1)
      {
        while (needResize && !quit && m_part == 1)
        {
          //lock view
          WindowMutex windowMutex;
          needResize = false;
          view.clear();
          menuItem.setPosition(m_windowRect.x / 2, m_windowRect.y / 4);
          new_game.setPosition(m_windowRect.x / 2 - new_game.getGlobalBounds().width / 2,
                               m_windowRect.y / 4 - new_game.getGlobalBounds().height);
          view.draw(menuItem);
          menuItem.setPosition(m_windowRect.x / 2, m_windowRect.y / 2);
          highscores.setPosition(m_windowRect.x / 2 - highscores.getGlobalBounds().width / 2,
                                 m_windowRect.y / 2 - highscores.getGlobalBounds().height);
          view.draw(menuItem);
          menuItem.setPosition(m_windowRect.x / 2, m_windowRect.y * 3 / 4);
          exit.setPosition(m_windowRect.x / 2 - exit.getGlobalBounds().width / 2,
                           m_windowRect.y * 3 / 4 - exit.getGlobalBounds().height);
          view.draw(menuItem);
          view.draw(new_game);
          view.draw(highscores);
          view.draw(exit);
          view.display();
        }
        baho::waitNextFrame();
      };
      switch (m_part.load())
      {
      case 2:
        m_game.start(view);
        //set listeners after return
        eventController.addOnKeyListener(sf::Keyboard::Escape, onEscListener, []() {});
        eventController.addOnKeyListener(sf::Keyboard::Space, []() {}, onSpaceListener);
        eventController.setOnChangeSizeListener(onResizeListener);
        eventController.setOnLMouseClickListener(LMouseListener);
        m_part = 1;
        break;
      default:
        m_part = 0;
      }
    }
  }
  eventController.finish();
  return 0;
}
