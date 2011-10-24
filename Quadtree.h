#ifndef QUADTREE_H
#define QUADTREE_H

#include "Line.h"
#include "IntersectionDetection.h"
#include <vector>
#include <algorithm>
using namespace std;

class Quadtree{
  double start_width;
  double end_width;

  double start_height;
  double end_height;

  double timeStep;

  int numLineLineCollisions;

  unsigned int maxDepth;
  unsigned int currentDepth;

  int divisionThresh;
 protected:
  Quadtree * parent;
  vector<Line *> spanningLines;
  
 public:
  Quadtree(double, double, double, double);
  int descend();
  vector<Line *> lines;
  ~Quadtree();
  
 private:
  Quadtree * one;
  Quadtree * two;
  Quadtree * three;
  Quadtree * four;

  vector<Line*>* linesInTree;
  double qtree_sw;
  double qtree_ew;
  double qtree_sh;
  double qtree_eh;
  
  void divideSelf();

  void distributeLines(Quadtree *, Quadtree *, Quadtree *, Quadtree *,
		       vector<Line*>);

  vector<Line*>* distributeLinesOne(Quadtree *);
  vector<Line*>* distributeLinesTwo(Quadtree *);
  vector<Line*>* distributeLinesThree(Quadtree *);
  vector<Line*>* distributeLinesFour(Quadtree *);

  int detectLineLineCollisions(vector<Line*> *);
  int detectSpanningLineLineCollisions(vector<Line*> *, vector<Line*> *,
                    vector<Line*> *,vector<Line*> *, vector<Line*> *);
  int detectLineLineCollisionsTwoLines(vector<Line*> *, vector<Line*> *);
  void collisionSolver(Line *, Line *, IntersectionType);
};

#endif /* QUADTREE_H*/
