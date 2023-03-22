#ifndef FIELD_HPP
#define FIELD_HPP
#include "definition.hpp"

class Field
{
public:
  ~Field();
  
  static constexpr us NOPATH = 0xFFFF;
  
  square_t getBoundSquare();
  
  float getCellLength();
  
  void setCellLength(float length);
  
  void setBoundSquare(square_t square);
  
  cell_t * operator [](int index);
  
  static Field & getField()
  {
    static Field field;
    return field;
  }
  
private:
  Field() {};
  Field(Field &);
  Field & operator = (Field &);
  square_t m_square;
  float m_cell_length;
  cell_t m_field[FIELD_WIDTH][FIELD_WIDTH];
};

#endif // FIELD_HPP
