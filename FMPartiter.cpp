#include "FMPartiter.h"

using namespace std;

void FMPartiter::initialize( const Parser::ParseResult &result ) /*{{{*/
{
  // get parser result {{{
  vector<Cell> cells = result.cells;

  balanceDegree = result.balanceDegree;
  nets          = result.nets;
  // end get parser result
  /*}}}*/
  // initial partition {{{
  size_t middle = cells.size() / 2;

  for( size_t i = 0       ; i < middle        ; ++i ) cells[i].group = groupA;
  for( size_t i = middle  ; i < cells.size()  ; ++i ) cells[i].group = groupB;
  // end initial partition
  /*}}}*/
  // initialize gain and bucket {{{
  int maxGain = 0;

  // initialze gain {{{
  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     Cell  &cell  = cells[i];
     Group from   = cell.group;

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
  this->cells.resize( cells.size() );

  middleGainIndex = maxGain + 1;
  maxGainIndexA   = -maxGain;
  maxGainIndexB   = -maxGain;

  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     int cellIndex = middleGainIndex + cells[i].gain;

     if( cells[i].group == groupA )
     {
       bucketA[cellIndex].push_front( cells[i] );

       if( cellIndex > maxGainIndexA ) maxGainIndexA = cellIndex;

       this->cells[i] = bucketA[cellIndex].begin();
     }
     else
     {
       bucketB[cellIndex].push_front( cells[i] );

       if( cellIndex > maxGainIndexB ) maxGainIndexB = cellIndex;

       this->cells[i] = bucketB[cellIndex].begin();
     }
  }
  // end initialize bucket }}}
  // end initialize gain and bucket }}}
}
/*}}}*/
void FMPartiter::partite()
{
}

FMPartiter::PartitionResult FMPartiter::partitionResult()
{
  return PartitionResult();
}

// vim: foldmethod=marker
