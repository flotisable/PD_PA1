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
void FMPartiter::partite() /*{{{*/
{
  int largestPartialSum;

  do
  {
    vector<int> gainStep;
    vector<int> cellStep;

    reset();

    // update gain {{{
    for( size_t i = 0 ; cells.size() ; ++i )
    {
       list<int>::iterator  it      = getMovedCell();
       Cell                 &cell   = cells[*it];
       Group                from    = cell.group;

       // move the cell {{{
       cell.locked  = true;

       if( cell.group == groupA )
         bucketA[ middleGainIndex + cell.gain ].erase( it );
       else
         bucketB[ middleGainIndex + cell.gain ].erase( it );
       // end move the cell
       /*}}}*/
       // update gain {{{
       for( size_t j = 0 ; j < cell.nets.size() ; ++j )
       {
          Net       &net        = nets[cell.nets[j]];
          list<int> fromCells;
          list<int> toCells;

          // collect cell of from and to {{{
          for( size_t k = 0 ; k < net.cells.size() ; ++k )
          {
             Cell &cell = cells[net.cells[k]];

             if( cell.group == from ) fromCells.push_back ( net.cells[k] );
             else                     toCells.push_back   ( net.cells[k] );
          }
          // end collect cell of from and to
          /*}}}*/
          // update gain by to condition {{{
          if( toCells.size() == 0 )
          {
            for( size_t k = 0 ; k < net.cells.size() ; ++k )
               if( !cells[net.cells[k]].locked ) ++cells[net.cells[k]].gain;
          }
          else if( toCells.size() == 1 )
            if( !cells[toCells.front()].locked ) --cells[toCells.front()].gain;
          // end update gain by to condition
          /*}}}*/
          fromCells.remove( *it );
          toCells.push_back( *it );

          // update gain by from condition {{{
          if( fromCells.size() == 0 )
          {
            for( size_t k = 0 ; k < net.cells.size() ; ++k )
               if( !cells[net.cells[k]].locked ) --cells[net.cells[k]].gain;
          }
          else if( fromCells.size() == 1 )
            if( !cells[fromCells.front()].locked ) ++cells[fromCells.front()].gain;
          // end update gain by from condition }}}
       }
       // end update gain
       /*}}}*/
       gainStep.push_back( cell.gain );
       cellStep.push_back( *it );
    }
    // end update gain
    /*}}}*/
    // calculate max partial sum {{{
    size_t  largestPartialSumIndex  = 0;
    int     currentSum              = 0;

    largestPartialSum = 0;

    for( size_t i = 0 ; i < cellStep.size() ; ++i )
    {
       currentSum += gainStep[i];

       if( currentSum > largestPartialSum )
       {
         largestPartialSum      = currentSum;
         largestPartialSumIndex = i;
       }
    }
    // end calculate max partial sum
    /*}}}*/
    // move cells {{{
    for( size_t i = 0 ; i <= largestPartialSumIndex ; ++i )
    {
       Cell &cell = cells[cellStep[i]];

       cell.group = ( cell.group == groupA ) ?  groupB: groupA;
    }
    // end move cells
    /*}}}*/
  }while( largestPartialSum > 0 );
}
/*}}}*/
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
}
/*}}}*/
list<int>::iterator FMPartiter::getMovedCell() /*{{{*/
{
  list<int>::iterator it;
  list<int>::iterator itA       = bucketA[maxGainIndexA].begin();
  list<int>::iterator itB       = bucketB[maxGainIndexB].begin();
  int                 indexA    = maxGainIndexA;
  int                 indexB    = maxGainIndexB;
  int                 cellNumA  = 0;
  int                 cellNumB  = 0;
  double              cellLB    = ( 1 - balanceDegree ) / 2 * cells.size();
  double              cellUB    = ( 1 + balanceDegree ) / 2 * cells.size();

  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     if( cells[i].group == groupA ) ++cellNumA;
     else                           ++cellNumB;
  }

  while( true )
  {
    it = ( indexA < indexB ) ? itB : itA;

    // check size constraint {{{
    if( cells[*it].group == groupA )
    {
      if( cellLB > cellNumA - 1 || cellNumB + 1 > cellUB )
      {
        while( ++itA == bucketA[indexA].end() )
        {
          --indexA;
          itA = bucketA[indexA].begin();
        }
        continue;
      }
    }
    else
    {
      if( cellLB > cellNumB - 1 || cellNumA + 1 > cellUB )
      {
        while( ++itB == bucketB[indexB].end() )
        {
          --indexB;
          itB = bucketB[indexB].begin();
        }
        continue;
      }
    }
    // end check size constraint
    /*}}}*/
    return it;
  }
} /*}}}*/

// vim: foldmethod=marker
