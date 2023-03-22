#ifndef ONKEYLISTENER_HPP
#define ONKEYLISTENER_HPP
#define WPARAM unsigned short
#include <list>

class OnKeyListener
{
public:
  OnKeyListener();

  OnKeyListener(const OnKeyListener &) = delete;
  OnKeyListener(OnKeyListener &&) = delete;
  OnKeyListener & operator =(const OnKeyListener &) = delete;
  OnKeyListener & operator =(OnKeyListener &&) = delete;

  ~OnKeyListener();

  void addListener(short key, void (*onKeyDown)(), void (*onKeyUp)());
  void removeListeners(void (*onKeyDown)());

  void onEvent(unsigned int message, WPARAM wParam);

private:
  struct node_t
  {
    WPARAM key;

    void (*onKeyDown)();

    void (*onKeyUp)();
  };

  std::list< node_t > listeners;
};

#endif // ONKEYLISTENER_HPP
