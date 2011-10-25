#ifndef QUADTREE_H
#define QUADTREE_H

#include "Line.h"
#include "IntersectionDetection.h"
#include <cilk/cilk.h>
#include <cilk/reducer_list.h>
#include <cilk/reducer_opadd.h>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

struct IntersectionInfo {
  Line *l1;
  Line *l2;
  IntersectionType intersectionType;

  IntersectionInfo(Line *l1, Line *l2, IntersectionType intersectionType) {
    this->l1 = l1;
    this->l2 = l2;
    this->intersectionType = intersectionType;
  }
};

static cilk::reducer_opadd<int> numLineLineCollisions;

class Quadtree{
  double start_width;
  double end_width;

  double start_height;
  double end_height;

  double timeStep;

  unsigned int maxDepth;
  unsigned int currentDepth;

  int divisionThresh;
 protected:
  Quadtree * parent;
  vector<Line *> spanningLines;
  
 public:
  Quadtree(double, double, double, double);
  /* int descend(); */
  int getNumLineLineCollisions();
  void resetNumLineLineCollisions();
  void descend();
  vector<Line *> lines;
  ~Quadtree();


  
 private:
  Quadtree * one;
  Quadtree * two;
  Quadtree * three;
  Quadtree * four;

  double qtree_sw;
  double qtree_ew;
  double qtree_sh;
  double qtree_eh;

  cilk::reducer_list_append<IntersectionInfo> * intersectedPairs;
  
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
