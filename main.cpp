#include <iostream>
#include <string>
using namespace std;

#include "parser.h"
#include "FMPartiter.h"

int main()
{
  string      inputFileName = "input_pa1/input_1.dat";
  Parser      parser;
  FMPartiter  partiter;

  if( parser.parse( inputFileName ) )
  {
#ifndef NDEBUG
    cout << parser.parseResult() << "\n";
#endif

    partiter.initialize( parser.parseResult() );
    partiter.partite();

#ifndef NDEBUG
    cout << partiter.partitionResult() << "\n";
#endif
  }
  else
  {
    cerr << "parser error!\n";
    return 1;
  }

  return 0;
}
