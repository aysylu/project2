#include "Quadtree.h"

Quadtree::Quadtree(double _start_width, double _end_width, double _start_height, double _end_height){
  start_width = _start_width;
  end_width = _end_width;
  start_height = _start_height;
  end_height = _end_height;
  divisionThresh = 10;
  one = NULL;
  two = NULL;
  three = NULL;
  four = NULL;
  linesInTree = NULL;
  parent = NULL;
  
  qtree_sw = 0;
  qtree_ew = 0;
  
  qtree_sh = 0;
  qtree_eh = 0;

  //TODO: figure out what to do with init of lines
  timeStep = 0.5;
  numLineLineCollisions = 0;
}

void Quadtree::divideSelf() {
  // Width of first quadrant
  double start_width_one = start_width;
  double end_width_one = start_width + (end_width-start_width)/2;
  // Height of first quadrant
  double start_height_one = start_height;
  double end_height_one = start_height + (end_height-start_height)/2;

  // Width of second quadrant
  double start_width_two = end_width_one;
  double end_width_two = end_width;
  // Height of second quadrant
  double start_height_two = start_height;
  double end_height_two = start_height + (end_height-start_height)/2;

  // Width of third quadrant
  double start_width_three = start_width;
  double end_width_three = start_width + (end_width-start_width)/2;
  // Height of third quadrant
  double start_height_three = end_height_one;
  double end_height_three = end_height;


  // Width of fourth quadrant
  double start_width_four = end_width_one;
  double end_width_four = end_width;
  // Height of fourth quadrant
  double start_height_four = end_height_one;
  double end_height_four = end_height;

  Quadtree * _one = new Quadtree(start_width_one, end_width_one,
				start_height_one, end_height_one);
  Quadtree * _two = new Quadtree(start_width_two, end_width_two,
				start_height_two, end_height_two);
  Quadtree * _three = new Quadtree(start_width_three, end_width_three,
				start_height_three, end_height_three);
  Quadtree * _four = new Quadtree(start_width_four, end_width_four,
				start_height_four, end_height_four);
  one = _one;
  two = _two;
  three = _three;
  four = _four;

  one->parent = this;
  two->parent = this;
  three->parent = this;
  four->parent = this;
}

vector<Line *> * Quadtree::distributeLines(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  double qtree_sw = qtree->start_width;
  double qtree_ew = qtree->end_width;
  
  double qtree_sh = qtree->start_height;
  double qtree_eh = qtree->end_height;

  //printf("Coordinates for box:\n");
  //printf("(sw=%f , ew=%f), (sh=%f, eh=%f)\n", qtree_sw,
//	 qtree_ew, qtree_sh, qtree_eh);
//  printf("Includes lines:\n");
  
  for (it=lines.begin(); it < lines.end(); it++){
    Line * line = *it;
    if((line->p1.x >= qtree_sw && line->p1.x <= qtree_ew) &&
       (line->p1.y >= qtree_sh && line->p1.y <= qtree_eh)){
      // Check if the second point is in the box
      if((line->p2.x >= qtree_sw && line->p2.x <= qtree_ew) &&
	 (line->p2.y >= qtree_sh && line->p2.y <= qtree_eh)){
	// The line is in the box...
	linesInTree->push_back(line);
      }
      // The line spans more than one quadtree
      parent->spanningLines.push_back(line);
   }
  }

  return linesInTree;
}

int Quadtree::descend(vector<Line *> _lines) {
  lines = _lines;
  int totalCollisions=0;
  if (lines.size() < divisionThresh) {
    totalCollisions = detectLineCollisions();
  } else {

    vector<Line *> * oneLines;
    vector<Line *> * twoLines;
    vector<Line *> * threeLines;
    vector<Line *> * fourLines;
    
    divideSelf();
    
    //printf("Lines size is %d\n", (int) lines.size());
    oneLines = distributeLines(one);
    totalCollisions += one->descend(*oneLines);

    //printf("Lines size is %d\n", (int) lines.size());
    twoLines = distributeLines(two);
    totalCollisions += two->descend(*twoLines);

    //printf("Lines size is %d\n", (int) lines.size());
    threeLines = distributeLines(three);
    totalCollisions += three->descend(*threeLines);

    //printf("Lines size is %d\n", (int) lines.size());
    fourLines = distributeLines(four);
    totalCollisions += four->descend(*fourLines);

  }
  return totalCollisions;
  // vector<Line *>::iterator it;
  // for (it=lines.begin(); it < lines.end(); it++) {
  //     Line *l1 = *it;
  //     printf("p1 x, p1 y, p2 x, p2 y %f %f %f %f\n", l1->p1.x, l1->p1.y, l1->p2.x, l1->p2.y);
  //     printf("p1 = %llx\n", &(l1->p1));
  // }
}

int Quadtree::detectLineCollisions() {
   vector<Line*>::iterator it1, it2;
   for (it1 = lines.begin(); it1 != lines.end(); ++it1) {
      Line *l1 = *it1;
      for (it2 = it1 + 1; it2 != lines.end(); ++it2) {
         Line *l2 = *it2;
         IntersectionType intersectionType = intersect(l1, l2, timeStep);
         if (intersectionType != NO_INTERSECTION) {
// printf("**********************************************************************\n");
//     printf("Num of collisions so far %d\n", numLineLineCollisions);
            //collisionSolver(l1, l2, intersectionType);
            numLineLineCollisions++;
         }
      }
   }
   printf("detectLineCollisions found: %d in the quadtree\n", numLineLineCollisions);
   return numLineLineCollisions;
}

Quadtree::~Quadtree() {
  if (one != NULL) {
    delete one;
  }
  if (two != NULL) {
    delete two;
  }
  if (three != NULL) {
    delete three;
  }
  if (four != NULL) {
    delete four;
  }
  
}
