#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <ostream>

#include "data.h"

class Parser
{
  public:

    struct ParseResult
    {
      double            balanceDegree;
      std::vector<Cell> cells;
      std::vector<Net>  nets;
    };

    bool parse( const std::string &fileName );

    inline ParseResult parseResult() const;

  private:

    ParseResult result;
};

std::ostream& operator<<( std::ostream &out, const Parser::ParseResult &result );

inline Parser::ParseResult Parser::parseResult() const { return result; }

#endif
