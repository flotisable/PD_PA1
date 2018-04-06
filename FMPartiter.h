#ifndef FM_PARTITER_H
#define FM_PARTITER_H

#include <vector>
#include <list>

#include "data.h"
#include "parser.h"

class FMPartiter
{
  public:

    struct PartitionResult
    {
      int cutSize;

      std::vector<Cell> groupA;
      std::vector<Cell> groupB;
    };

    void            initialize( const Parser::ParseResult &result );
    void            partite();
    PartitionResult partitionResult();

  private:

    void reset();

    double balanceDegree;

    std::vector<Cell> cells;
    std::vector<Net>  nets;

    std::vector<std::list<int> > bucketA;
    std::vector<std::list<int> > bucketB;

    int maxGainIndexA;
    int maxGainIndexB;
    int middleGainIndex;
};

#endif
