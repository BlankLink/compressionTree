
#include <iostream>
#include <string>
#include <Response.h>
#include <CompressionTree.h>

int main (int argc, char** argv){
  if (argc < 3){
    std::cerr << "ERROR: Must specify file with desired string to compress, or decompress" << std::endl;
    return 1;
  }
  std::string algo = argv[1];
  compressionTree tree(argv[2]);
  response rc = response::_OK;
  if (algo == "-c"){
    rc = tree.compress();
  }
  else if ( algo == "-d" ){
    rc = tree.decompress();
  }
  else {
    std::cerr << "-c or -d must be your first argument." << std::endl;
    return 1;
  }
  if( rc != response::_OK ){
    std::cerr << "ERROR: There was a problem!" << std::endl;
    return 1;
  }

  return 0;
}
