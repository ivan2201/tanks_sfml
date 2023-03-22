#include "eventlistener.hpp"

EventListener::EventListener()
{
}

EventListener::~EventListener()
{
}

void EventListener::addListener(sf::Window view, void (*onClick)())
{
  node_t current {view, onClick};
  listeners.push_back(current);
}

void EventListener::removeListeners(void (*onClick)())
{
  auto iterator = listeners.begin();
  while (iterator != listeners.end())
  {
    if (onClick == (*iterator).onClick)
    {
      iterator = listeners.erase(iterator);
    }
  }
}

void EventListener::onEvent(UINT message, LPARAM lParam)
{
  switch (message)
  {
    case WM_COMMAND:
    {
      for(auto iterator : listeners)
      {
        if (reinterpret_cast< HWND >(lParam) == iterator.view)
        {
          iterator.onClick();
          return;
        }
      }
    }
  }
}
