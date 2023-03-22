#include "eventcontroller.hpp"

void checkEvents()
{
  EventController & eventController = EventController::getEventController();
  while (!eventController.m_finish)
  {
    sf::Event event;
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (eventController.m_onLMouseClick != nullptr)
      {
        eventController.m_onLMouseClick(event.mouseButton.x, event.mouseButton.y);
      }
      break;
    }
    else if (event.type == sf::Event::Resized)
    {
      if (eventController.m_onChangeSizeListener != nullptr)
      {
        eventController.m_onChangeSizeListener(event.size.width, event.size.height);
      }
      break;
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      eventController.m_onKeyListener.onEvent(event.key.code, true);
    }
    else if (event.type == sf::Event::KeyReleased)
    {
      eventController.m_onKeyListener.onEvent(event.key.code, false);
    }
  }
}

EventController::EventController():
  m_finish(false),
  m_eventControllerThread(checkEvents),
  m_onLMouseClick(nullptr)
{
}

EventController::~EventController()
{
  m_finish = true;
  m_eventControllerThread.join();
}

void EventController::addOnKeyListener(sf::Keyboard::Key key, void (*onKeyDown)(), void (*onKeyUp)())
{
  m_onKeyListener.addListener(key, onKeyDown, onKeyUp);
}

void EventController::setChangeSizeListener(void (*onChangeSizeListener)(unsigned int width, unsigned int height))
{
  m_onChangeSizeListener = onChangeSizeListener;
}

void EventController::setLMouseClickListener(void (*onClick)(int x, int y))
{
  m_onLMouseClick = onClick;
}

void EventController::removeOnKeyListener(sf::Keyboard::Key key, void (*onKeyDown)())
{
  m_onKeyListener.removeListeners(key, onKeyDown);
}

EventController::OnKeyListener::OnKeyListener()
{
}

EventController::OnKeyListener::~OnKeyListener()
{
}

void EventController::OnKeyListener::addListener(sf::Keyboard::Key key, void (*onKeyDown)(), void (*onKeyUp)())
{
  auto iter = m_listeners.find(key);
  if (iter != m_listeners.end())
  {
    iter->second.onDownListeners.push_back(onKeyDown);
    iter->second.onUpListeners.push_back(onKeyUp);
  }
  else
  {
    node_t current;
    current.onDownListeners.push_back(onKeyDown);
    current.onUpListeners.push_back(onKeyUp);
    m_listeners.insert({key, current});
  }
}

void EventController::OnKeyListener::removeListeners(sf::Keyboard::Key key, void (*onKeyDown)())
{
  auto iter = m_listeners.find(key);
  if (iter != m_listeners.end())
  {
    auto iterator = iter->second.onDownListeners.begin();
    while (iterator != iter->second.onDownListeners.end())
    {
      if (onKeyDown == (*iterator))
      {
        iterator = iter->second.onDownListeners.erase(iterator);
      }
      iterator++;
    }
  }
}

void EventController::OnKeyListener::onEvent(sf::Keyboard::Key key, bool pressed)
{
  auto iter = m_listeners.find(key);
  if (iter != m_listeners.cend())
  {
    node_t & node = iter->second;
    if (!pressed)
    {
      std::for_each(node.onUpListeners.begin(), node.onUpListeners.end(), [](void (*onClick)())
      {
        onClick();
      });
    }
    else
    {
      std::for_each(node.onDownListeners.begin(), node.onDownListeners.end(), [](void (*onClick)())
      {
        onClick();
      });
    }
  }
}

