#pragma once

#include <fstream>
#include <string>
#include <map>
#include <iostream>
#include <bitset>
#include <iomanip>

#include <Response.h>
#include <Leaf.h>

const int bytesize = 8;

class compressionTree {
  
 public:
  compressionTree(const char* fin);
  response compress();
  response decompress();
  
 private:
  leaf* root;
  std::string filepath;
  std::string filetype;
  std::map<char, std::string> sequencer;
  response buildTree();
  void buildTreeFromSequence();
  void buildHeader(std::ofstream& outf);
  response readHeader(std::ifstream& inf);
  void makeSeq(leaf* c, std::string sequence);
  void inorder(leaf* c);
  response buildMap(std::map<char,leaf*>& charoccurances);
  std::string prepareFileName(std::string extension = ".mcp");
};
