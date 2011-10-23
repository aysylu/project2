#ifndef QUADTREE_H
#define QUADTREE_H

#include "Line.h"
#include "IntersectionDetection.h"
#include <vector>
using namespace std;

class Quadtree{
  double start_width;
  double end_width;

  double start_height;
  double end_height;

  double timeStep;

  int numLineLineCollisions;

  vector<Line *> lines;

  int divisionThresh;
 public:
  Quadtree(double, double, double, double);
  void descend(vector<Line *>);
  ~Quadtree();
  
 private:
  Quadtree * one;
  Quadtree * two;
  Quadtree * three;
  Quadtree * four;
  void divideSelf();
  vector<Line*>* distributeLines(Quadtree *);
  void detectCollisions();
};

#endif /* QUADTREE_H*/
