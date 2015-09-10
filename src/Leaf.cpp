#include <Leaf.h>

leaf::leaf(){
  occurances=0;
}

leaf::leaf(char c, int _occurances){
  value = c;
  occurances = _occurances;
  left = 0;
  right = 0;
}
