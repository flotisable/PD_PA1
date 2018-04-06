#include <iostream>
using namespace std;

#include "parser.h"

int main()
{
  Parser parser;

  if( parser.parse( "input_pa1/input_1.dat" ) )
  {
    cout << parser.parseResult();
  }
  else
  {
    cerr << "parser error!\n";
    return 1;
  }

  return 0;
}
