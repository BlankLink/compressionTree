
#include <CompressionTree.h>

compressionTree::compressionTree(const char* fin){
  filepath = fin;  // only set the name of the filepath to the file in question
}

response compressionTree::compress(){
  response rc = response::_OK;
  std::ifstream inf(filepath);
  std::ofstream outf(prepareFileName());
  if (!inf.is_open() || !outf.is_open())
    rc = response::ERROR_INFILE;
  if( rc == response::_OK ) rc = buildTree();
  if( rc == response::_OK ) {
    makeSeq(root, "");
    buildHeader(outf);
  
  // ALGORITHM:  1- get first char and sequence from map
  //             2- cycle through sequence converting chars to bits
  //             3- when bit buffer gets to a byte, write to file, and continue loop
  //             4- when eof reached:
  //                     fill buffer with eof char and write to file
    char c;
    int i=0;
    inf.get(c);
    std::bitset<bytesize> bits;
    while(!inf.eof()){
      for( int j = 0; j < sequencer[c].length(); ++j){
	bits[bytesize-i-1] = (sequencer[c][j] == '0' ? 0 : 1 );
	i++;
	if( i == bytesize ) {
	  char byte = (char) bits.to_ulong();
	  outf << byte;
	  bits.reset();
	  i=0;
	}
      }
      inf.get(c);
    }
    inf.clear();
    inf.seekg(0, inf.end);
    std::cout << "Congratulations - you achieved " << std::setprecision(2) << std::fixed << ((double)(outf.tellp())/inf.tellg())*100 << "% compression -- " << prepareFileName() << std::endl;
    outf.close();
  }
  return rc;
}

response compressionTree::decompress(){
  response rc = response::_OK;
  std::ifstream inf(prepareFileName());
  if (!inf.is_open()) rc = response::ERROR_INFILE;
  if ( rc == response::_OK ) rc = readHeader(inf);
  std::ofstream outf(prepareFileName(filetype));
  if ( !outf.is_open() ) rc = response::ERROR_OUTFILE;
  if ( rc == response::_OK ) {
    buildTreeFromSequence();
  
    // ALGORITHM: 1- read in character
    //            2- convert to bitset
    //            3- cycle through bitset until character is found
    //            4- print character
    //            5- if bitset is cycled adn chars remain, read new ch
    //            6- do until eof
    
    char ch;
    inf.get(ch);
    std::bitset<bytesize> bits(ch);
    leaf* c = root;
    while(!inf.eof()) {
      for( int i = 0; i < bytesize; i++){
	if (c->left == NULL && c->right == NULL) {
	  outf << c->value;
	  c = root;
	}
	if( bits[bytesize-i-1] == 0 ) {
	  c = c->left;
	}
	else if (bits[bytesize-i-1] == 1 )
	  c = c->right;
      }
      bits.reset();
      inf.get(ch);
      bits = ch;
    }
    
    outf << std::endl;
    std::cout << "Congratulations - you decompressed --" << prepareFileName(filetype) << std::endl;
  }
  return rc;
}

void compressionTree::inorder(leaf* c){
  if (c != NULL) {
    inorder(c->left);
    std::cout << c->value << " ";
    inorder(c->right);
  }
}

void compressionTree::buildHeader(std::ofstream& outf){
  int pos = filepath.find('.');
  std::string filetype = filepath.substr(pos, filepath.length());
  outf << "|~ " << filetype << std::endl;
  for( auto& mapItem : sequencer ){
    outf << mapItem.first << mapItem.second << std::endl;
  }
  outf << "|";
}

response compressionTree::readHeader(std::ifstream& inf){
  std::string dummy;
  std::string sequence;
  char first;
  char next;
  
  inf >> dummy;
  if (dummy != "|~") return response::ERROR_HEADER;
  inf >> filetype;
  inf.get(first);
  inf.get(first);
  //ALGORITHM: go through each character and sequence and put them in the maps yo
  while( first != '|' ) {
    inf.get(next);
    while ( next != '\n') {
      sequence += next;
      inf.get(next);
    }
    if( sequencer.find(first) == sequencer.end() ) sequencer[first]=sequence;
    sequence = "";
    inf.get(first);
  }
  return response::_OK;
}

void compressionTree::buildTreeFromSequence(){
  root = new leaf( '*', 0 );
  int i = 0;
  for (auto& seq : sequencer){
    leaf* c=root;
    std::string sequence = seq.second;
    for (i=0 ; i < sequence.length(); i++ ){
      if( sequence[i] == '0' ){
	if( c->left == NULL) c->left = new leaf( '*', 0);
	c=c->left;
      }
      else if ( sequence[i] == '1' ){
	if( c->right == NULL) c->right = new leaf('*', 0);
	c=c->right;
      }
    }
    c->value = seq.first;
  }
}

response compressionTree::buildTree(){
  //create map with value incremented each time given letter is found
  response rc = response::_OK;
  std::map<char, leaf*> charoccurances;
  buildMap(charoccurances);
  
  //cycle through map until only one remains, and create tree structure
  //ALGORITHM:  -1 set smallest1 -> first element; smallest2 -> following element
  //            -2 loop through rest of elements, changing smallest1 or smallest2 when smaller 
  //                              value found.
  //            -3 create a parent node with occurances = smallest1 + smallest2, 
  //                              link parent left/right to smallest1/smallest2
  //            -4 do until size = 1
  //            -5 set root = the only element in map
  
  while( charoccurances.size() > 1){
    std::map<char, leaf*>::iterator it = charoccurances.begin();
    leaf* smallest1 = it->second;
    ++it;
    leaf* smallest2 = it->second;
    ++it;
    for( std::map<char,leaf*>::iterator next=it ; next != charoccurances.end(); ++next ) {
      if( next->second->occurances < smallest1->occurances)
    	smallest1 = next->second;
      else if ( next->second->occurances < smallest2->occurances )
    	smallest2 = next->second;
    }
    leaf* parent = new leaf(smallest1->value, smallest1->occurances + smallest2->occurances);
    parent->left = smallest1;
    parent->right = smallest2;
    charoccurances.erase(smallest1->value);
    charoccurances.erase(smallest2->value);
    charoccurances[smallest1->value] = parent;
    
  }
  root = charoccurances.begin()->second;
  if ( root == NULL ) rc = response::ERROR_TREEBUILD;
  return rc;
}

response compressionTree::buildMap(std::map<char, leaf*>& charoccurances){
  response rc = response::_OK;
  char c;
  std::ifstream inf(filepath);
  if ( !inf.is_open()) rc = response::ERROR_INFILE;
  if ( rc == response::_OK ){
    inf.get(c);
    while(!inf.eof()){
      if( charoccurances.find(c) != charoccurances.end() )
	charoccurances[c]->occurances++; //increment times seen      
      else charoccurances[c]= new leaf(c, 1); //if its not in the map, add it correctly
      inf.get(c);
    }
  }
  return rc;
}

void compressionTree::makeSeq(leaf* c, std::string sequence ){
  if(c->left!= NULL && c->right != NULL) {
    if (c->left != NULL)
      makeSeq(c->left, sequence+"0");
    if (c->right != NULL)
      makeSeq(c->right, sequence+"1");
  }
  else sequencer[c->value] = sequence;
}

std::string compressionTree::prepareFileName(std::string extension){
  std::string filename;
  int pos = filepath.find('.');
  filename = filepath.substr(0, pos);
  filename.append(extension);
  return filename;  
}
