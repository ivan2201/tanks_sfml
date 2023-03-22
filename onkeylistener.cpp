#include "onkeylistener.hpp"
#define WPARAM unsigned short

OnKeyListener::OnKeyListener()
{
}

OnKeyListener::~OnKeyListener()
{
}

void OnKeyListener::addListener(WPARAM key, void (*onKeyDown)(), void (*onKeyUp)())
{
  node_t current {key, onKeyDown, onKeyUp};
  listeners.push_back(current);
}

void OnKeyListener::removeListeners(void (*onKeyDown)())
{
  auto iterator = listeners.begin();
  while (iterator != listeners.end())
  {
    if (onKeyDown == (*iterator).onKeyDown)
    {
      iterator = listeners.erase(iterator);
    }
    iterator++;
  }
}

void OnKeyListener::onEvent(UINT message, WPARAM wParam)
{
  switch (message)
  {
  case WM_KEYDOWN:
    {
      for (node_t node : listeners)
      {
        if (wParam == node.key)
        {
          node.onKeyDown();
        }
      }
    }
    break;
  case WM_KEYUP:
    {
      for (node_t node : listeners)
      {
        if (wParam == node.key)
        {
          node.onKeyUp();
        }
      }
    }
  }
}
