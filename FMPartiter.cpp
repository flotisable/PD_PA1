#include "FMPartiter.h"

#ifndef NDEBUG
#include <iostream>
#endif

#include <limits>
#include <cassert>
using namespace std;

ostream& operator<<( ostream &out, const FMPartiter::PartitionResult &result ) /*{{{*/
{
  out << "Cutsize = " << result.cutSize << "\n";

  out << "G1 " << result.groupA.size() << "\n";

  for( size_t i = 0 ; i < result.groupA.size() ; ++i )
     out << "c" << result.groupA[i] + 1 << " ";
  out << ";\n";

  out << "G2 " << result.groupB.size() << "\n";

  for( size_t i = 0 ; i < result.groupB.size() ; ++i )
     out << "c" << result.groupB[i] + 1 << " ";
  out << ";\n";

  return out;
}
/*}}}*/
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
  // initialize bucket size {{{
  size_t maxPin = 0;

  for( size_t i = 0 ; i < cells.size() ; ++i )
     if( cells[i].nets.size() > maxPin )
       maxPin = cells[i].nets.size();

  bucketA.resize( maxPin * 2 + 1 );
  bucketB.resize( maxPin * 2 + 1 );

  middleGainIndex = maxPin;
  // end initialize bucket size }}}
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

#ifndef NDEBUG
    clog << "total cells : " << cells.size() << "\n";
#endif

    // update gain {{{
    for( size_t i = 0 ; cells.size() ; ++i )
    {
       list<int>::iterator it = getMovedCell();

       if( it == bucketA[0].end() || it == bucketB[0].end() ) break;

       Cell   &cell         = cells[*it];
       Group  from          = cell.group;
       int    maxGainIndex  = ( cell.group == groupA ) ? maxGainIndexA : maxGainIndexB;

#ifndef NDEBUG
       clog << "the " << i << "-th cell\n";
#endif

       cell.locked = true;

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
          if( toCells.size() == 0 ) /*{{{*/
          {
            for( size_t k = 0 ; k < net.cells.size() ; ++k )
            {
               Cell &cell = cells[net.cells[k]];

               if( !cell.locked )
               {
                 updateCell( net.cells[k], true );

                 if( middleGainIndex + cell.gain > maxGainIndex )
                   maxGainIndex = middleGainIndex + cell.gain;
               }
            }
          } /*}}}*/
          else if( toCells.size() == 1 ) /*{{{*/
          {
            if( !cells[toCells.front()].locked )
              updateCell( toCells.front(), false );
          } /*}}}*/
          // end update gain by to condition
          /*}}}*/
          fromCells.remove( *it );
          toCells.push_back( *it );

          // update gain by from condition {{{
          if( fromCells.size() == 0 ) /*{{{*/
          {
            for( size_t k = 0 ; k < net.cells.size() ; ++k )
               if( !cells[net.cells[k]].locked )
                 updateCell( net.cells[k], false );
          } /*}}}*/
          else if( fromCells.size() == 1 ) /*{{{*/
          {
            Cell &cell = cells[fromCells.front()];

            if( !cell.locked )
            {
              updateCell( fromCells.front(), true );

              if( middleGainIndex + cell.gain > maxGainIndex )
                maxGainIndex = middleGainIndex + cell.gain;
            }
          } /*}}}*/
          // end update gain by from condition }}}
       }
       // end update gain
       /*}}}*/
       gainStep.push_back( cell.gain );
       cellStep.push_back( *it );

       // move the cell {{{
       if( cell.group == groupA )
       {
         bucketA[ middleGainIndex + cell.gain ].erase( it );
         cell.group = groupB;
       }
       else
       {
         bucketB[ middleGainIndex + cell.gain ].erase( it );
         cell.group = groupA;
       }
       maxGainIndexA = maxGainIndex;

#ifndef NDEBUG
       clog << "maxGainIndexA : " << maxGainIndexA << "\n";
#endif

       while( bucketA[maxGainIndexA].size() == 0 && maxGainIndexA > 0 )
         --maxGainIndexA;

#ifndef NDEBUG
       clog << "maxGainIndexA : " << maxGainIndexA << "\n";
#endif

       maxGainIndexB = maxGainIndex;

#ifndef NDEBUG
       clog << "maxGainIndexB : " << maxGainIndexB << "\n";
#endif

       while( bucketB[maxGainIndexB].size() == 0 && maxGainIndexB > 0 )
         --maxGainIndexB;

#ifndef NDEBUG
       clog << "maxGainIndexB : " << maxGainIndexB << "\n";
#endif
       // end move the cell
       /*}}}*/
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
    for( size_t i = largestPartialSumIndex + 1 ; i < cellStep.size() ; ++i )
    {
       Cell &cell = cells[cellStep[i]];

       cell.group = ( cell.group == groupA ) ? groupB: groupA;
    }
    // end move cells
    /*}}}*/
  }while( largestPartialSum > 0 );
}
/*}}}*/
FMPartiter::PartitionResult FMPartiter::partitionResult() /*{{{*/
{
  PartitionResult result;

  result.cutSize = 0;

  // calculate cut size {{{
  for( size_t i = 0 ; i < nets.size() ; ++i )
  {
     bool inGroupA = false;
     bool inGroupB = false;

     for( size_t j = 0 ; j < nets[i].cells.size() ; ++j )
     {
        if( cells[nets[i].cells[j]].group == groupA ) inGroupA = true;
        else                                          inGroupB = true;

        if( inGroupA && inGroupB )
        {
          ++result.cutSize;
          break;
        }
     }
  }
  // end calculate cut size
  /*}}}*/
  // split group a and b {{{
  for( size_t i = 0 ; i < cells.size() ; ++i )
     if( cells[i].group == groupA ) result.groupA.push_back( i );
     else                           result.groupB.push_back( i );
  // end split group a and b
  /*}}}*/
  return result;
}
/*}}}*/
void FMPartiter::reset() /*{{{*/
{
  maxGainIndexA = 0;
  maxGainIndexB = 0;

  for( size_t i = 0 ; i < bucketA.size() ; ++i )
     bucketA[i].clear();

  for( size_t i = 0 ; i < bucketB.size() ; ++i )
     bucketB[i].clear();

  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     Cell   &cell     = cells[i];
     Group  from      = cell.group;

     cell.gain    = 0;
     cell.locked  = false;

     // calculate initial gain {{{
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
     // end calculate initial gain
     /*}}}*/
     // add to bucket {{{
     int cellIndex = middleGainIndex + cell.gain;

     if( cell.group == groupA )
     {
       bucketA[cellIndex].push_front( i );

       if( cellIndex > maxGainIndexA ) maxGainIndexA = cellIndex;
     }
     else
     {
       bucketB[cellIndex].push_front( i );

       if( cellIndex > maxGainIndexB ) maxGainIndexB = cellIndex;
     }
     // end add to bucket }}}
  }
}
/*}}}*/
list<int>::iterator FMPartiter::getMovedCell() /*{{{*/
{
  // variables declaration {{{
  list<int>::iterator it;
  list<int>::iterator itA       = bucketA[maxGainIndexA].begin();
  list<int>::iterator itB       = bucketB[maxGainIndexB].begin();
  int                 indexA    = maxGainIndexA;
  int                 indexB    = maxGainIndexB;
  int                 cellNumA  = 0;
  int                 cellNumB  = 0;
  double              cellLB    = ( 1 - balanceDegree ) / 2 * cells.size();
  double              cellUB    = ( 1 + balanceDegree ) / 2 * cells.size();
  // end variables declaration
  /*}}}*/

#ifndef NDEBUG
  clog << "cellLB : " << cellLB << ", cellUB : " << cellUB << "\n";
#endif

  // calculate cell number in group a and b {{{
  for( size_t i = 0 ; i < cells.size() ; ++i )
  {
     if( cells[i].group == groupA ) ++cellNumA;
     else                           ++cellNumB;
  }
  // end calculate cell number in group a and b
  /*}}}*/

#ifndef NDEBUG
  clog << "cell number in A : " << cellNumA << ", cell number in B : " << cellNumB << "\n";
#endif

  while( true )
  {
    if( indexA < indexB )
      it = ( itB == bucketB[indexB].end() ) ? itA : itB;
    else
      it = ( itA == bucketA[indexA].end() ) ? itB : itA;

#ifndef NDEBUG
    clog  << "itV " << *it << " indexA " << indexA << " indexB " << indexB
          << " maxA " << maxGainIndexA << " maxB " << maxGainIndexB
          << " aNum " << bucketA[indexA].size() << " bNum " << bucketB[indexB].size() <<"\n";
#endif

    if( it == bucketA[indexA].end() || it == bucketB[indexB].end() ) return it;

    // check size constraint {{{
    if( cells[*it].group == groupA )
    {
      assert( it == itA );

      if( cellLB > cellNumA - 1 || cellNumB + 1 > cellUB )
      {
        ++itA;

#ifndef NDEBUG
        clog << "itA : " << *itA << "\n";
#endif

        while( itA == bucketA[indexA].end() && indexA > 0 )
        {
          --indexA;
          itA = bucketA[indexA].begin();
        }
        continue;
      }
    }
    else
    {
      assert( it == itB );

      if( cellLB > cellNumB - 1 || cellNumA + 1 > cellUB )
      {
        ++itB;

#ifndef NDEBUG
        clog << "itB : " << *itB << " " << ( itB == bucketB[indexB].end() ) << "\n";
#endif

        while( itB == bucketB[indexB].end() && indexB > 0 )
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
}
/*}}}*/
void FMPartiter::updateCell( int index, bool inc ) /*{{{*/
{
  Cell                &cell     = cells[index];
  vector<list<int> >  &bucket  = ( cell.group == groupA ) ? bucketA: bucketB;
  int                 cellIndex = middleGainIndex + cell.gain;

  bucket[cellIndex].remove( index );

  if( inc )
  {
    ++cell.gain;
    ++cellIndex;
  }
  else
  {
    --cell.gain;
    --cellIndex;
  }
  bucket[cellIndex].push_front( index );
} /*}}}*/

// vim: foldmethod=marker
