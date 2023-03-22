#ifndef MYMATH_HPP
#define MYMATH_HPP

namespace baho
{
  template < typename T >
  T sqr(T value)
  {
    return value * value;
  }
  
  template < typename T>
  T pow(T value, unsigned long long power)
  {
    T result = static_cast< T >(1);
    for (unsigned long long i = 0; i < power; i++)
    {
      result *= value;
    }
    return value;
  }
  
  template < typename T >
  T abs(T value)
  {
    return (value < static_cast< T >(0))? -value:value;
  }
}

#endif // MYMATH_HPP
