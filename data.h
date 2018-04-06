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
  bool              locked;
  Group             group;
  std::vector<int>  nets;
};

struct Net
{
  std::vector<int> cells;
};

#endif
