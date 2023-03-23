#include "game.hpp"

point_t findNearPathPoint(us (&path)[FIELD_WIDTH][FIELD_WIDTH], point_t point);

Game::Game():
  m_resources(Resources::getResources()),
  m_field(Field::getField()),
  m_myTank(m_path),
  m_preEnemy(m_path)
{
  m_textures[0] = m_resources.getTexture("beton");
  m_textures[1] = m_resources.getTexture("brick");
  m_textures[2] = m_resources.getTexture("ground");
  m_textures[3] = m_resources.getTexture("hedgehog");
  m_textures[4] = m_resources.getTexture("my_tank");
  m_textures[5] = m_resources.getTexture("emeny_tank");
  m_textures[6] = m_resources.getTexture("bullet");
  m_textures[7] = m_resources.getTexture("spawn");
  m_textures[8] = m_resources.getTexture("my_spawn");
  m_textures[9] = m_resources.getTexture("menu_item");
  m_buffer[0] = m_resources.getSoundBuffer("bang");
  m_buffer[1] = m_resources.getSoundBuffer("moving");
  m_buffer[2] = m_resources.getSoundBuffer("shot");
  m_tankBang.setBuffer(m_buffer[0]);
  m_tankMoving.setBuffer(m_buffer[1]);
  m_tankShot.setBuffer(m_buffer[2]);
}

Game::~Game()
{
}

void Game::start(sf::RenderWindow & window)
{
  int enemiesLeft = BEGIN_ENEMY_NUMBER;
  uc lastGeneredEnemyPosition = 0;
  static sf::Vector2u windowRect; 
  static uc moveBit = 0;
  moveBit = 0;
  windowRect = m_windowRect;
  if (windowRect.y > windowRect.x)
  {
    m_field.setBoundSquare(
    {
      static_cast < us >(0),
      static_cast < us >((windowRect.y - windowRect.x) / 2),
      static_cast < us >(windowRect.x)
    });
  }
  else
  {
    m_field.setBoundSquare(
    {
      static_cast< us >((windowRect.x - windowRect.y)/2),
      static_cast< us >(0),
      static_cast< us >(windowRect.y)
    });
  }
  m_field.setCellLength(m_field.getBoundSquare().length / FIELD_WIDTH);
  m_preEnemy.setSprite(sf::Sprite(m_textures[5]));
  m_myTank.setSprite(sf::Sprite(m_textures[4]));
  m_preBullet.setSprite(sf::Sprite(m_textures[6]));
  generateField(clock());
  srand(clock());
  EventController & eventController = EventController::getEventController();
  // Create a clock for measuring elapsed time
  sf::Clock clock;
  static bool escPressed = false;
  static bool exitBool = false;
  exitBool = false;
  static bool isContinued = false;
  bool gameFinished = false;
  isContinued = false;
  auto escDownListener = []()
  {
    escPressed = true;
  };
  auto escUpListener = []()
  {
    escPressed = false; 
  };
  eventController.addOnKeyListener(sf::Keyboard::Escape, escDownListener, escUpListener);
  auto leftDownListener = []()
  {
    moveBit = moveBit | BIT_LEFTKEY;
  };
  auto leftUpListener = []()
  {
    moveBit = moveBit & (~BIT_LEFTKEY);
  };
  eventController.addOnKeyListener(sf::Keyboard::Left, leftDownListener, leftUpListener);
  auto upDownListener = []()
  {
    moveBit = moveBit | BIT_UPKEY;
  };
  auto upUpListener = []()
  {
    moveBit = moveBit & (~BIT_UPKEY);
  };
  eventController.addOnKeyListener(sf::Keyboard::Up, upDownListener, upUpListener);
  auto rightDownListener = []()
  {
    moveBit = moveBit | BIT_RIGHTKEY;
  };
  auto rightUpListener = []()
  {
    moveBit = moveBit & (~BIT_RIGHTKEY);
  };
  eventController.addOnKeyListener(sf::Keyboard::Right, rightDownListener, rightUpListener);
  auto downDownListener = []()
  {
    moveBit = moveBit | BIT_DOWNKEY;
  };
  auto downUpListener = []()
  {
    moveBit = moveBit & (~BIT_DOWNKEY);
  };
  eventController.addOnKeyListener(sf::Keyboard::Down, downDownListener, downUpListener);
  static bool spacePressed = false;
  auto spaceDownListener = []()
  {
    spacePressed = true;
  };
  auto spaceUpListener = []()
  {
    spacePressed = false;
  };
  //Game * game = this;
  static sf::Sprite menuItem(m_textures[9]);
  auto menuOnClickListener = [](int x, int y)
  {
    if (x > (windowRect.x / 2 - menuItem.getLocalBounds().width / 2)
        && x < (windowRect.x / 2 + menuItem.getLocalBounds().width / 2))
    {
      if (y > (windowRect.y / 3 - menuItem.getLocalBounds().height / 2)
          && y < (windowRect.y / 3 + menuItem.getLocalBounds().height / 2))
      {
        isContinued = true;
      } 
      else if (y > (windowRect.y * 2 / 3 - menuItem.getLocalBounds().height / 2)
          && y < (windowRect.y * 2 / 3 + menuItem.getLocalBounds().height / 2))
      {
        exitBool = true;
      }
    }
  };
  eventController.addOnKeyListener(sf::Keyboard::Space, spaceDownListener, spaceUpListener);
  auto onClosedListener = []()
  {
    exitBool = true;
  };
  eventController.addOnKeyListener(sf::Keyboard::Space, spaceDownListener, spaceUpListener);
  sf::Font font = m_resources.getFont("font");
  while (!exitBool && !gameFinished)
  {
    {
      sf::Text text("3", font, m_field.getBoundSquare().length / 3);
      auto bound = m_field.getBoundSquare();
      text.setPosition(bound.left + bound.length / 2 - text.getGlobalBounds().width / 2,
                       bound.top + bound.length / 2 - text.getGlobalBounds().height);
      while (!exitBool && !escPressed)
      {
        baho::waitNextFrame();
        WindowMutex windowMutex;
        float time = clock.getElapsedTime().asSeconds();
        int timer = 3 - static_cast< int >(time);
        if (timer < 1) break;
        window.clear();
        redrawField(window);
        std::string str;
        str.push_back('0' + timer);
        text.setString(str);
        window.draw(text);
        // Display each view on screen
        window.display();
      }
    }
    float previousTime = clock.getElapsedTime().asSeconds();
    float prevShoot = clock.getElapsedTime().asSeconds();
    while (!exitBool && !escPressed && !gameFinished)
    {
      baho::waitNextFrame();
      WindowMutex windowMutex;
      float time = clock.getElapsedTime().asSeconds();
      float delta = time - previousTime;
      previousTime = time;
      // Clear views
      window.clear();
      point_t prevPoint = {static_cast< uc >(m_myTank.getX()), 
                           static_cast< uc >(m_myTank.getY())};
      if (moveBit != 0)
      {
        if (m_tankMoving.getStatus() != sf::Sound::Playing) m_tankMoving.play();
      }
      else
      {
        m_tankMoving.stop();
      }
      m_myTank.move(moveBit, delta, m_enemys.cbegin(), m_enemys.cend());
      if (prevPoint.x != static_cast< uc >(m_myTank.getX()) || 
          prevPoint.y != static_cast< uc >(m_myTank.getY())) updatePath();
      auto iterEnemies = m_enemys.begin();
      auto end = m_enemys.cend();
      while (iterEnemies != end)
      {
        if (iterEnemies->mayDelete())
        {
          iterEnemies = m_enemys.erase(iterEnemies);
          enemiesLeft--;
          if (enemiesLeft > 4)
          {
            uc nextGeneratedEnemyPosition = 255;
            for (uc i = (lastGeneredEnemyPosition + 1) % ENEMY_SPAWNER_NUMBER; 
                 i != lastGeneredEnemyPosition;
                 i = (i + 1) % ENEMY_SPAWNER_NUMBER)
            {
              auto secondIterEnemies = m_enemys.begin();
              while (secondIterEnemies != end)
              {
                if (m_spawners[i].x == static_cast<uc>(secondIterEnemies->getX()) &&
                    m_spawners[i].y == static_cast<uc>(secondIterEnemies->getY())) break;
                secondIterEnemies++;
              }
              if (secondIterEnemies == end) 
              {
                nextGeneratedEnemyPosition = i;
                break;
              }
            }
            if (nextGeneratedEnemyPosition < ENEMY_SPAWNER_NUMBER)
            {
              lastGeneredEnemyPosition = nextGeneratedEnemyPosition;
              m_preEnemy.setPosition(m_spawners[lastGeneredEnemyPosition]);
              m_preEnemy.setId(m_preEnemy.getId() + 1);
              m_enemys.push_back(m_preEnemy);
            }
          }
          else if (enemiesLeft < 1)
          {
            gameFinished = true;
          }
        }
        else
        {
          iterEnemies->move(delta, m_enemys.cbegin(), m_enemys.cend());
          
          if ((rand() % 500 * FRAMERATE_LIMIT < 100) && !(iterEnemies->isDestroyed()) && 
              ((time - m_lastShotTime[iterEnemies->getId() - 1]) > 1.f))
          {
            m_lastShotTime[iterEnemies->getId() - 1] = clock.getElapsedTime().asSeconds();
            m_bullets.push_back(iterEnemies->shot(m_preBullet));
            m_tankShot.play();
          }
          iterEnemies++;
        }
      }
      std::for_each(m_bullets.begin(), m_bullets.end(), [delta](Bullet & bullet)
      {
        bullet.move(delta);
      });
      auto iterBull = m_bullets.begin();
      while (iterBull != m_bullets.end())
      {
        auto frame = iterBull->getFrameRect();
        float width = frame.width / m_field.getCellLength() / 2;
        float height = frame.height / m_field.getCellLength() / 2;
        const int left = static_cast< uc >(iterBull->getX() - width);
        const int right = static_cast< uc >(iterBull->getX() + width);
        const int up = static_cast< uc >(iterBull->getY() - height);
        const int down = static_cast< uc >(iterBull->getY() + height);
        auto secondIterBull = iterBull;
        bool bulletDestr = false;
        while ((++secondIterBull) != m_bullets.end())
        {
          if (secondIterBull->containPoint(iterBull->getX() + width, iterBull->getY() + height)
           || secondIterBull->containPoint(iterBull->getX() - width, iterBull->getY() + height)
           || secondIterBull->containPoint(iterBull->getX() + width, iterBull->getY() - height)
           || secondIterBull->containPoint(iterBull->getX() - width, iterBull->getY() - height))
          {
            iterBull = m_bullets.erase(iterBull);
            if (iterBull == secondIterBull)
            {
              iterBull = m_bullets.erase(secondIterBull);
            }
            if (iterBull == m_bullets.end()) break;
            bulletDestr = true;
            break;
          }
        }
        if (bulletDestr) continue;
        cell_t * destroyedCell = nullptr;
        if ((m_field[right][down].type & BIT_UNSHOOTED) != 0)
        {
          if ((m_field[right][down].type) == TYPE_BRICK)
          {
            destroyedCell = &m_field[right][down];
          }
          bulletDestr = true;
        }
        else if ((m_field[left][down].type & BIT_UNSHOOTED) != 0)
        {
          if ((m_field[left][down].type) == TYPE_BRICK)
          {
            destroyedCell = &m_field[left][down];
          }
          bulletDestr = true;
        }
        else if ((m_field[right][up].type & BIT_UNSHOOTED) != 0)
        {
          if ((m_field[right][up].type) == TYPE_BRICK)
          {
            destroyedCell = &m_field[right][up];
          }
          bulletDestr = true;
        }
        else if ((m_field[left][up].type & BIT_UNSHOOTED) != 0)
        {
          if ((m_field[left][up].type) == TYPE_BRICK)
          {
            destroyedCell = &m_field[left][up];
          }
          bulletDestr = true;
        }
        if (bulletDestr)
        {
          if (destroyedCell != nullptr)
          {
            (destroyedCell->health)--;
            if (destroyedCell->health < 1)
            {
              destroyedCell->type = TYPE_GROUND;
              destroyedCell->image.setTexture(m_textures[2]);
            }
          }
          iterBull = m_bullets.erase(iterBull);
          bulletDestr = false;
          continue;
        }
        for (auto & tank : m_enemys)
        {
          if ((tank.containPoint(iterBull->getX() + width, iterBull->getY() + height) ||
               tank.containPoint(iterBull->getX() - width, iterBull->getY() + height) ||
               tank.containPoint(iterBull->getX() + width, iterBull->getY() - height) ||
               tank.containPoint(iterBull->getX() - width, iterBull->getY() - height)) &&
              (tank.getId() != iterBull->getTankId()))
          {
            if (!tank.isDestroyed()) m_tankBang.play();
            tank.destroy();
            iterBull = m_bullets.erase(iterBull);
            if (iterBull == m_bullets.end()) break;
          }
        }
        if (iterBull == m_bullets.end()) break;
        if ((m_myTank.containPoint(iterBull->getX() + width, iterBull->getY() + height) ||
             m_myTank.containPoint(iterBull->getX() - width, iterBull->getY() + height) ||
             m_myTank.containPoint(iterBull->getX() + width, iterBull->getY() - height) ||
             m_myTank.containPoint(iterBull->getX() - width, iterBull->getY() - height)) &&
            (m_myTank.getId() != iterBull->getTankId()))
        {
          m_myTank.destroy();
          m_tankBang.play();
          gameFinished = true;
          iterBull = m_bullets.erase(iterBull);
          if (iterBull == m_bullets.end()) break;
        }
        iterBull++;
      }
      if (spacePressed && ((time - prevShoot) > 0.5) && !m_myTank.isDestroyed())
      {
        m_bullets.push_back(m_myTank.shot(m_preBullet));
        m_tankShot.play();
        prevShoot = time;
      } 
      redrawField(window);
      // Display each view on screen
      window.display();
    }
    m_tankMoving.stop();
    escPressed = false; 
    isContinued = false;
    spacePressed = false;
    if (gameFinished)
    {
      sf::Text finalText("you won", font, m_windowRect.y / 8);
      finalText.setPosition(m_windowRect.x / 2, m_windowRect.y / 2);
      finalText.setOrigin(sf::Vector2f(finalText.getGlobalBounds().width / 2,
                                       finalText.getGlobalBounds().height / 2));
      if (m_myTank.isDestroyed())
      {
        finalText.setString("you lose");
        finalText.setColor(sf::Color(0xFF,0x00,0x00,0xFF));
      }
      else
      {
        finalText.setColor(sf::Color(0x00,0xFF,0x00,0xFF));
      }
      WindowMutex windowMutex;
      redrawField(window);
      window.draw(finalText);
      window.display();
    }
    while (!escPressed && !exitBool && !isContinued && !spacePressed)
    {
    }
    escPressed = false;
    eventController.setOnLMouseClickListener(menuOnClickListener);
    sf::Text continue_game("continue", font, menuItem.getTextureRect().height / 2);
    //sf::Text nothing("nothing", font, menuItem.getTextureRect().height / 2);
    sf::Text exit("quit", font, menuItem.getTextureRect().height / 2);
    window.clear();
    menuItem.setPosition(m_windowRect.x / 2, m_windowRect.y / 3);
    menuItem.setOrigin(sf::Vector2f(menuItem.getTexture()->getSize()) / 2.f);
    continue_game.setPosition(m_windowRect.x / 2 - continue_game.getGlobalBounds().width / 2, 
                            m_windowRect.y / 3 - continue_game.getGlobalBounds().height);
    window.draw(menuItem);
    //menuItem.setPosition(m_windowRect.right / 2, m_windowRect.bottom / 2);
    //nothing.setPosition(m_windowRect.right / 2 - highscores.getGlobalBounds().width / 2, 
    //                       m_windowRect.bottom / 2 - highscores.getGlobalBounds().height);
    //SFMLView.draw(menuItem);
    menuItem.setPosition(m_windowRect.x / 2, m_windowRect.y * 2 / 3);
    exit.setPosition(m_windowRect.x / 2 - exit.getGlobalBounds().width / 2, 
                     m_windowRect.y * 2 / 3 - exit.getGlobalBounds().height);
    {
      WindowMutex windowMutex;
      window.draw(menuItem);
      window.draw(continue_game);
      window.draw(exit);
      window.display();
    }
    while (!escPressed && !exitBool && !isContinued && !gameFinished);
    escPressed = false;
    isContinued = false;
  }
  m_bullets.clear();
  m_enemys.clear();
  eventController.removeOnKeyListener(sf::Keyboard::Escape, escDownListener);
  eventController.removeOnKeyListener(sf::Keyboard::Space, spaceDownListener);
  eventController.removeOnKeyListener(sf::Keyboard::Left, leftDownListener);
  eventController.removeOnKeyListener(sf::Keyboard::Right, rightDownListener);
  eventController.removeOnKeyListener(sf::Keyboard::Up, upDownListener);
  eventController.removeOnKeyListener(sf::Keyboard::Down, downDownListener);
  eventController.setOnLMouseClickListener(nullptr);
  eventController.setOnChangeSizeListener(nullptr);
  eventController.setOnClosedListener(nullptr);
}

void Game::setWindowRect(sf::Vector2u windowRect)
{
  m_windowRect = windowRect;
}

sf::Vector2u Game::getWindowRect()
{
  return m_windowRect;
}

void Game::generateField(const ull &seed)
{
  int randCount = 1;
  int rand2 = seed % 73;
  for (int y = 1; y < FIELD_WIDTH - 1; y++)
  {
    for (int x = 1; x < FIELD_WIDTH - 1; x++)
    {
      int sqr = baho::sqr((rand2 * randCount) % 25037);
      sqr = (seed + rand2 * sqr) % 25037;
      if (sqr < 3000)
      {
        m_field[x][y].type = TYPE_BETON;
      } 
      else if (sqr < 11000)
      {
        m_field[x][y].type = TYPE_BRICK;
      }
      else if (sqr < 14000)
      {
        m_field[x][y].type = TYPE_HADGEHOG;
      }
      else
      {
        m_field[x][y].type = TYPE_GROUND;
      }
      randCount++;
    }
  }
  //create field wall
  for (int i = 0; i < FIELD_WIDTH; i++)
  {
    m_field[0][i].type = TYPE_BETON;
    m_field[i][FIELD_WIDTH - 1].type = TYPE_BETON;
    m_field[FIELD_WIDTH - 1][FIELD_WIDTH - i].type = TYPE_BETON;
    m_field[FIELD_WIDTH - i][0].type = TYPE_BETON;
  }
  uc playerSpawnerX = FIELD_WIDTH / 2;
  uc playerSpawnerY = FIELD_WIDTH - 2;
  m_field[playerSpawnerY][playerSpawnerX].type = TYPE_SPAWN;
  for (uc i = 1; i < ENEMY_SPAWNER_NUMBER + 1; i++)
  {
    m_spawners[i - 1] = { i * ENEMY_SPAWNER_DELTA, 1 };
    m_field[i * ENEMY_SPAWNER_DELTA][1].type = TYPE_SPAWN;
  }
  //user spawner
  m_field[playerSpawnerX][playerSpawnerY].type = TYPE_SPAWN;
  for (uc i = ENEMY_SPAWNER_DELTA + 1; i < FIELD_WIDTH - ENEMY_SPAWNER_DELTA - 1; i++)
  {
    if (m_field[i][1].type == TYPE_BETON)
    {
      m_field[i][1].type = TYPE_BRICK;
    }
    else if (m_field[i][1].type == TYPE_HADGEHOG)
    {
      m_field[i][1].type = TYPE_GROUND;
    }
  }
  updatePath(m_path, {playerSpawnerX, playerSpawnerY});
  bool isPath = true;
  if (m_path[ENEMY_SPAWNER_DELTA][1] == Field::NOPATH)
  {
    isPath = false;
  }
  // if path from player to enemies field don't exists, then create simple path. 
  if (!isPath)
  {
    us pathFromEnemySpawn[FIELD_WIDTH][FIELD_WIDTH];    
    updatePath(pathFromEnemySpawn, {playerSpawnerX, 1});
    point_t maxDistancePoint = {playerSpawnerX, playerSpawnerY}; 
    us maxDistance = m_path[playerSpawnerY][playerSpawnerX];
    for (us y = 1; y < FIELD_WIDTH - 1; y++)
    {
      for (us x = 1; x < FIELD_WIDTH - 1; y++)
      {
        if (m_path[x][y] != Field::NOPATH && maxDistance < m_path[x][y])
        {
          maxDistance = m_path[x][y];
          maxDistancePoint = {x, y}; 
        }
      }
    }
    point_t end = findNearPathPoint(pathFromEnemySpawn, maxDistancePoint);
    if (end.x < maxDistancePoint.x)
    {
      us tmp = end.x;
      end.x = maxDistancePoint.x;
      maxDistancePoint.x = tmp;
    }
    if (end.y < maxDistancePoint.y)
    {
      us tmp = end.y;
      end.y = maxDistancePoint.y;
      maxDistancePoint.y = tmp;
    }
    for (int i = maxDistancePoint.x + 1; i <= end.x; i++)
    {
      if (m_field[i][maxDistancePoint.y].type == TYPE_BETON)
      {
        m_field[i][maxDistancePoint.y].type = TYPE_BRICK;
      }
      else if (m_field[i][maxDistancePoint.y].type == TYPE_HADGEHOG)
      {
        m_field[i][maxDistancePoint.y].type = TYPE_GROUND;
      }
    }
    for (int i = maxDistancePoint.y + 1; i <= end.y; i++)
    {
      if (m_field[end.x][i].type == TYPE_BETON)
      {
        m_field[end.x][i].type = TYPE_BRICK;
      }
      else if (m_field[end.x][i].type == TYPE_HADGEHOG)
      {
        m_field[end.x][i].type = TYPE_GROUND;
      }
    }
    updatePath(m_path, { playerSpawnerX, playerSpawnerY });
  }
  for (us y = 0; y < FIELD_WIDTH; y++)
  {
    for (us x = 0; x < FIELD_WIDTH; x++)
    {
      switch (m_field[x][y].type)
      {
      case TYPE_BETON:
        m_field[x][y].image.setTexture(m_textures[0]);
        break;
      case TYPE_GROUND:
        m_field[x][y].image.setTexture(m_textures[2]);
        break;
      case TYPE_HADGEHOG:
        m_field[x][y].image.setTexture(m_textures[3]);
        break;
      case TYPE_BRICK:
        m_field[x][y].image.setTexture(m_textures[1]);
        m_field[x][y].health = 3;
        break;
      default:
        if (m_path[x][y] == 0)
          m_field[x][y].image.setTexture(m_textures[8]);
        else
          m_field[x][y].image.setTexture(m_textures[7]);
      }
      m_field[x][y].image.setColor(sf::Color(0xFF,0xFF,0xFF,0xFF));
      m_field[x][y].image.setPosition(
          m_field.getBoundSquare().left + x * m_field.getCellLength(),
          m_field.getBoundSquare().top + y * m_field.getCellLength());
      auto bound = m_field[x][y].image.getLocalBounds();
      m_field[x][y].image.setScale(m_field.getCellLength() / bound.width, 
          m_field.getCellLength() / bound.height);
    }
  }
  m_myTank.setPosition({ playerSpawnerX, playerSpawnerY });
  for (us i = ENEMY_SPAWNER_DELTA;
       i < FIELD_WIDTH - ENEMY_SPAWNER_DELTA;
       i += ENEMY_SPAWNER_DELTA)
  {
    m_preEnemy.setPosition({i, 1});
    m_lastShotTime[m_preEnemy.getId()] = 0.0f;
    m_preEnemy.setId(m_preEnemy.getId() + 1);
    m_enemys.push_back(m_preEnemy);
  }
}

void Game::redrawField(sf::RenderWindow & window)
{
  for (int y = 0; y < FIELD_WIDTH; y++)
  {
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
      if (m_field[x][y].type == TYPE_BRICK)
      {
        m_field[x][y].image.setColor(
            sf::Color(0xFF,0xFF,0xFF, static_cast<int>(0xFF - 0x7F * (3 - m_field[x][y].health) / 3)));
      }
      window.draw(m_field[x][y].image);
    }
  }
  m_myTank.draw(window);
  for (auto & bullet : m_bullets)
  {
    bullet.draw(window);
  }
  for (auto & tank : m_enemys)
  {
    tank.draw(window);
  }
}

void Game::updatePath()
{
  updatePath(m_path, { static_cast< us >(m_myTank.getX()), 
      static_cast< us >(m_myTank.getY()) });
}

void Game::updatePath(us (&path)[FIELD_WIDTH][FIELD_WIDTH], point_t begin)
{
  for (int y = 0; y < FIELD_WIDTH; y++)
    for (int x = 0; x < FIELD_WIDTH; x++)
      path[x][y] = Field::NOPATH;
  std::queue< point_t > queue;
  path[begin.x][begin.y] = 0;
  queue.push(begin);
  uc TYPE_UNPATHCELL = BIT_LOCK | BIT_UNDESTROYED;
  while (!queue.empty())
  {
    point_t curPoint = queue.front();
    queue.pop();
    if ((curPoint.y > 0) && (path[curPoint.x][curPoint.y - 1] == Field::NOPATH)
        && !((m_field[curPoint.x][curPoint.y - 1].type & TYPE_UNPATHCELL) == TYPE_UNPATHCELL))
    {
      path[curPoint.x][curPoint.y - 1] = path[curPoint.x][curPoint.y] + 1;
      queue.push({curPoint.x, static_cast< us >(curPoint.y - 1)});
    }
    if ((curPoint.x > 0) && (path[curPoint.x - 1][curPoint.y] == Field::NOPATH)
        && !((m_field[curPoint.x - 1][curPoint.y].type & TYPE_UNPATHCELL) == TYPE_UNPATHCELL))
    {
      path[curPoint.x - 1][curPoint.y] = path[curPoint.x][curPoint.y] + 1;
      queue.push({static_cast< us >(curPoint.x - 1), curPoint.y});
    }
    if ((curPoint.y + 1 < FIELD_WIDTH) && (path[curPoint.x][curPoint.y + 1] == Field::NOPATH)
        && !((m_field[curPoint.x][curPoint.y + 1].type & TYPE_UNPATHCELL) == TYPE_UNPATHCELL))
    {
      path[curPoint.x][curPoint.y + 1] = path[curPoint.x][curPoint.y] + 1;
      queue.push({curPoint.x, static_cast< us >(curPoint.y + 1)});
    }
    if ((curPoint.x + 1 < FIELD_WIDTH) && (path[curPoint.x + 1][curPoint.y] == Field::NOPATH)
        && !((m_field[curPoint.x + 1][curPoint.y].type & TYPE_UNPATHCELL) == TYPE_UNPATHCELL))
    {
      path[curPoint.x + 1][curPoint.y] = path[curPoint.x][curPoint.y] + 1;
      queue.push({static_cast< us >(curPoint.x + 1), curPoint.y});
    }
  }
}

point_t findNearPathPoint(us (&path)[FIELD_WIDTH][FIELD_WIDTH], point_t begin)
{
  for (int curDistance = 1; curDistance < 2 * FIELD_WIDTH; curDistance++)
  {
    for (int dx = -curDistance; dx <= curDistance; dx++)
    {
      int y1 = begin.y + curDistance - baho::abs(dx);
      int y2 = begin.y - curDistance + baho::abs(dx);
      int x = begin.x + dx;
      if (x < 1 || x > FIELD_WIDTH - 2) continue;
      if (y1 < FIELD_WIDTH && path[x][y1] != Field::NOPATH)
      {
        return {static_cast< us >(x), static_cast< us >(y1)};
      }
      if (y2 > -1 && path[x][y2] != Field::NOPATH)
      {
        return {static_cast< us >(x), static_cast< us >(y2)};
      }
    }
  }
  return begin;
}

