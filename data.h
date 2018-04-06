#ifndef DATA_H
#define DATA_H

#include <vector>

enum Group
{
  groupA,
  groupB
};

struct Cell
{
  int               gain;
  Group             group;
  std::vector<int>  nets;
};

struct Net
{
  std::vector<int> cells;
};

#endif
