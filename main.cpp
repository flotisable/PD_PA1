#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "parser.h"
#include "FMPartiter.h"

int main( int argc, char* argv[] )
{
  string      inputFileName;
  string      outputFileName;
  Parser      parser;
  FMPartiter  partiter;

  if( argc != 3 )
  {
    cerr << "the number of argument is wrong!\n";
    return 2;
  }

  inputFileName   = argv[1];
  outputFileName  = argv[2];

  if( parser.parse( inputFileName ) )
  {
    ofstream file( outputFileName.c_str() );

#ifndef NDEBUG
    clog << parser.parseResult() << "\n";
#endif

    partiter.initialize( parser.parseResult() );
    partiter.partite();

#ifndef NDEBUG
    //clog << partiter.partitionResult() << "\n";
#endif

    file << partiter.partitionResult() << "\n";
  }
  else
  {
    cerr << "parser error!\n";
    return 1;
  }

  return 0;
}
