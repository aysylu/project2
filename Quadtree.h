#ifndef QUADTREE_H
#define QUADTREE_H

#include "Line.h"
#include "IntersectionDetection.h"
#include <vector>
#include <algorithm>
using namespace std;

typedef struct _COLLISION{
  int numLineLineCollisions;
  int numLineWallCollisions;
} COLLISION, *PCOLLISION;

class Quadtree{
  double start_width;
  double end_width;

  double start_height;
  double end_height;

  double timeStep;

  /* int numLineLineCollisions; */
  /* int numLineWallCollisions; */
  COLLISION collisions;

  vector<Line *> lines;

  int divisionThresh;
 protected:
  Quadtree * parent;
  vector<Line *> spanningLines;
  
 public:
  Quadtree(double, double, double, double);
  COLLISION descend(vector<Line *>);
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

  vector<Line*>* distributeLinesOne(Quadtree *);
  vector<Line*>* distributeLinesTwo(Quadtree *);
  vector<Line*>* distributeLinesThree(Quadtree *);
  vector<Line*>* distributeLinesFour(Quadtree *);

  int detectLineLineCollisions(vector<Line*> *);
  int detectSpanningLineLineCollisions(vector<Line*> *, vector<Line*> *,
                    vector<Line*> *,vector<Line*> *, vector<Line*> *);
  int detectLineLineCollisionsTwoLines(vector<Line*> *, vector<Line*> *);
  int detectLineWallCollisions(vector<Line *> *);
  //int detectLineWallCollisions(vector<Line*> *);
};

#endif /* QUADTREE_H*/
