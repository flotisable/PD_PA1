#include "FMPartiter.h"

using namespace std;

void FMPartiter::initialize( const Parser::ParseResult &result ) /*{{{*/
{
  // get parser result {{{
  balanceDegree = result.balanceDegree;
  nets          = result.nets;
  cells         = result.cells;
  // end get parser result
  /*}}}*/
  // initial partition {{{
  size_t middle = cells.size() / 2;

  for( size_t i = 0       ; i < middle        ; ++i ) cells[i].group = groupA;
  for( size_t i = middle  ; i < cells.size()  ; ++i ) cells[i].group = groupB;
  // end initial partition
  /*}}}*/
}
/*}}}*/
void FMPartiter::partite()
{
  int largestPartialSum;

  do
  {
    reset();

  }while( largestPartialSum > 0 );
}

FMPartiter::PartitionResult FMPartiter::partitionResult()
{
  return PartitionResult();
}

void FMPartiter::reset() /*{{{*/
{
  int maxGain = 0;

  // initialze gain {{{
  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     Cell  &cell  = cells[i];
     Group from   = cell.group;

     cell.gain    = 0;
     cell.locked  = false;

     for( size_t j = 0 ; j < cell.nets.size() ; ++j )
     {
        Net &net    = nets[cell.nets[j]];
        int fromNum = 0;
        int toNum   = 0;

        for( size_t k = 0 ; k < net.cells.size() ; ++k )
        {
           if( cells[net.cells[k]].group == from )  ++fromNum;
           else                                     ++toNum;
        }
        if( fromNum == 1 ) ++cell.gain;
        if( toNum   == 0 ) --cell.gain;
     }
     if( cell.gain > maxGain ) maxGain = cell.gain;
  }
  // end initialze gain
  /*}}}*/
  // initialize bucket {{{
  bucketA.resize( maxGain * 2 + 1 );
  bucketB.resize( maxGain * 2 + 1 );

  for( size_t i = 0 ; i < bucketA.size() ; ++i )
     bucketA[i].clear();

  for( size_t i = 0 ; i < bucketB.size() ; ++i )
     bucketB[i].clear();

  middleGainIndex = maxGain + 1;
  maxGainIndexA   = -maxGain;
  maxGainIndexB   = -maxGain;

  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     int cellIndex = middleGainIndex + cells[i].gain;

     if( cells[i].group == groupA )
     {
       bucketA[cellIndex].push_front( i );

       if( cellIndex > maxGainIndexA ) maxGainIndexA = cellIndex;
     }
     else
     {
       bucketB[cellIndex].push_front( i );

       if( cellIndex > maxGainIndexB ) maxGainIndexB = cellIndex;
     }
  }
  // end initialize bucket }}}
} /*}}}*/

// vim: foldmethod=marker
