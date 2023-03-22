#include "field.hpp"

Field::~Field()
{
}

square_t Field::getBoundSquare()
{
  return m_square;
}


float Field::getCellLength()
{
  return m_cell_length;
}

void Field::setCellLength(float length)
{
  m_cell_length = length;
}
  
void Field::setBoundSquare(square_t square)
{
  m_square = square;
}

cell_t * Field::operator [](int index)
{
  return m_field[index];
}
  
