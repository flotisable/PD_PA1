#include "parser.h"

#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdexcept>
using namespace std;
 
ostream& operator<<( ostream &out, const Parser::ParseResult &result ) /*{{{*/
{
  out << "balance degree : " << result.balanceDegree << "\n";

  // output net information {{{
  for( size_t i = 0 ; i < result.nets.size() ; ++i )
  {
     out << "net : " << i + 1 << " |";

     for( size_t j = 0 ; j < result.nets[i].cells.size() ; ++j )
        out << " c" << result.nets[i].cells[j];
     out << "\n";
  }
  // end output net information
  /*}}}*/
  // output cell information {{{
  for( size_t i = 0 ; i < result.cells.size() ; ++i )
  {
     out << "cell : " << i + 1 << ", gain : " << result.cells[i].gain << " |";

     for( size_t j = 0 ; j < result.cells[i].nets.size() ; ++j )
        out << " n" << result.cells[i].nets[j];
     out << "\n";
  }
  // end output cell information
  /*}}}*/
  return out;
}
/*}}}*/
bool Parser::parse( const string &fileName ) /*{{{*/
{
  // variables declaration {{{
  ifstream      file( fileName.c_str() );
  istringstream line;
  string        buffer;
  // end variables declaration
  /*}}}*/
  if( !file.is_open() ) return false;

  // get balance degree {{{
  getline( file, buffer );

  line.str( buffer );

  if( line >> buffer )
    result.balanceDegree = atof( buffer.c_str() );
  else
    return false;
  // end get balance degree
  /*}}}*/
  // get net information {{{
  while( getline( file, buffer ) )
  {
    line.clear();
    line.str( buffer );

    if( !( ( line >> buffer ) && buffer == "NET" ) ) continue;

    size_t netIndex;

    // get net index {{{
    if( !( ( line >> buffer ) && buffer[0] == 'n' ) ) return false;

    buffer    = buffer.substr( 1 );
    netIndex  = static_cast<size_t>( atoi( buffer.c_str() ) ) - 1;

    if( result.nets.size() <= netIndex )
      for( size_t i = result.nets.size() ; i <= netIndex ; ++i )
         result.nets.push_back( Net() );
    // end get net index
    /*}}}*/
    // get cell information {{{
    while( line >> buffer )
    {
      if( buffer == ";" ) break;

      size_t cellIndex;

      // get cell index {{{
      if( buffer[0] != 'c' ) return false;

      buffer    = buffer.substr( 1 );
      cellIndex  = static_cast<size_t>( atoi( buffer.c_str() ) ) - 1;

      if( result.cells.size() <= cellIndex )
        for( size_t i = result.cells.size() ; i <= cellIndex ; ++i )
           result.cells.push_back( Cell() );
      // end get cell index
      /*}}}*/
      // set net and cell information {{{
      Net   &net  = result.nets[netIndex];
      Cell  &cell = result.cells[cellIndex];

      cell.gain   = 0;
      cell.group  = groupA;

      net.cells.push_back( cellIndex  );
      cell.nets.push_back( netIndex   );
      // end set net and cell information }}}
    }
    // end get cell information }}}
  }
  // end get net information
  /*}}}*/
  return true;
}
/*}}}*/
// vim: foldmethod=marker
