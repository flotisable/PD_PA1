#ifndef FM_PARTITER_H
#define FM_PARTITER_H

#include <vector>
#include <list>
#include <ostream>

#include "data.h"
#include "parser.h"

class FMPartiter
{
  public:

    struct PartitionResult
    {
      int cutSize;

      std::vector<int> groupA;
      std::vector<int> groupB;
    };

    void            initialize( const Parser::ParseResult &result );
    void            partite();
    PartitionResult partitionResult();

  private:

    void                      reset       ();
    std::list<int>::iterator  getMovedCell();
    void                      updateCell  ( int index, bool inc );

    double balanceDegree;

    std::vector<Cell> cells;
    std::vector<Net>  nets;

    std::vector<std::list<int> > bucketA;
    std::vector<std::list<int> > bucketB;

    int maxGainIndexA;
    int maxGainIndexB;
    int middleGainIndex;
};

std::ostream& operator<<( std::ostream &out, const FMPartiter::PartitionResult &result );

#endif
