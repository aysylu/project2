#ifndef QUADTREE_H
#define QUADTREE_H

#include "Line.h"
#include <vector>
using namespace std;

class Quadtree{
  int start_width;
  int end_width;

  int start_height;
  int end_height;

  vector<Line *> lines;

  int divisionThresh;
 public:
  Quadtree(int, int, int, int);
  void descend(vector<Line *>);
  ~Quadtree();
  
 private:
  Quadtree * one;
  Quadtree * two;
  Quadtree * three;
  Quadtree * four;
  void divideSelf();
  vector<Line*>* distributeLines(Quadtree *);
};

#endif /* QUADTREE_H*/
