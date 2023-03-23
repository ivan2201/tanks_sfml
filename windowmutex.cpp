#include "windowmutex.hpp"
WindowMutex::WindowMutex():
m_mutex(WindowMutex::StaticMutex::getInstance()),
m_lock(m_mutex.getMutex())
{
}


WindowMutex::StaticMutex & WindowMutex::StaticMutex::getInstance()
{
  static WindowMutex::StaticMutex staticMutex;
  return staticMutex;
}

std::mutex & WindowMutex::StaticMutex::getMutex()
{
  return m_mutex;
}
