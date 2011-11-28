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

#define NUMBER_CHILD_NODES 4
#define LAST_CHILD_NODE (NUMBER_CHILD_NODES-1)

static cilk::reducer_opadd<int> numLineLineCollisions;

class Quadtree{

  double timeStep;

 protected:
  unsigned int currentDepth;
  int divisionThresh;
  Quadtree * parent;
  vector<Line *> spanningLines;
  
 public:
  Quadtree(double, double, double, double);
  int getNumLineLineCollisions();
  void descend();
  vector<Line *> lines;
  ~Quadtree();

 private:
  double start_width;
  double end_width;

  double start_height;
  double end_height;

  Quadtree * child[NUMBER_CHILD_NODES];

  unsigned int maxDepth;
  
  cilk::reducer_list_append<IntersectionInfo> * intersectedPairs;
  
  void createChildQuadtrees();

  void distributeLines();

  int detectLineLineCollisions(vector<Line*> *);
  /* int detectSpanningLineLineCollisions(vector<Line*> *, vector<Line*> *, */
  /*                   vector<Line*> *,vector<Line*> *, vector<Line*> *); */
  int detectSpanningLineLineCollisions();
  int detectCollisionsTwoLineSets(vector<Line*> *, vector<Line*> *);
  void collisionSolver(Line *, Line *, IntersectionType);
  
};

#endif /* QUADTREE_H*/
