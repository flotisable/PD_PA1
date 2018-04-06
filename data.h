#ifndef DATA_H
#define DATA_H

#include <vector>

struct Cell
{
  int               gain;
  std::vector<int>  nets;
};

struct Net
{
  std::vector<int> cells;
};

#endif
