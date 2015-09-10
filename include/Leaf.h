#pragma once

class leaf {
 public:
  char value;
  int occurances;
  leaf* left;
  leaf* right;
  leaf();
  leaf(char c, int occurances);
};
