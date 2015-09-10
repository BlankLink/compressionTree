#pragma once

#include <map>
#include <string>

enum response {
  _OK,                  //most functions will return this is all is well
  DECOMPRESSION_OK,     //should we return a different ok if on the decompression path?
  ERROR_INFILE,         //opening the infile error
  ERROR_OUTFILE,        //opening the outfile error
  ERROR_HEADER,         //reading the header error
  ERROR_TREEBUILD,      //the tree has not been created
  ERROR_FATAL           //blanket for unnamed errors
};

static std::map<response, std::string> responseToString;
