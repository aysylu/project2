#include "Quadtree.h"

Quadtree::Quadtree(double _start_width, double _end_width, double _start_height, double _end_height){
  start_width = _start_width;
  end_width = _end_width;
  start_height = _start_height;
  end_height = _end_height;
  divisionThresh = 3;
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

  timeStep = 0.5;
  collisions.numLineLineCollisions = 0;
  collisions.numLineWallCollisions = 0;
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

vector<Line *> * Quadtree::distributeLinesOne(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  double qtree_sw = qtree->start_width;
  double qtree_ew = qtree->end_width;
  
  double qtree_sh = qtree->start_height;
  double qtree_eh = qtree->end_height;

//  printf("Coordinates for box:\n");
//  printf("(sw=%f , ew=%f), (sh=%f, eh=%f)\n", qtree_sw,
//	 qtree_ew, qtree_sh, qtree_eh);
//  printf("Includes lines:\n");
  
  for (it=lines.begin(); it < lines.end(); it++){
    Line * line = *it;
    if((line->p1.x >= qtree_sw && line->p1.x <= qtree_ew) &&
       (line->p1.y >= qtree_sh && line->p1.y < qtree_eh)){
      // Check if the second point is in the box
      if((line->p2.x >= qtree_sw && line->p2.x <= qtree_ew) &&
	 (line->p2.y >= qtree_sh && line->p2.y < qtree_eh)){
	// The line is in the box...
	linesInTree->push_back(line);
      } else {
        // The line spans more than one quadtree
        qtree->parent->spanningLines.push_back(line);
//printf("spanning line: ((%f, %f), (%f, %f)) velX=%f, velY=%f\n", line->p1.x, line->p2.x, line->p1.y, line->p2.y, line->vel.x, line->vel.y);
      }
   }
  }
  return linesInTree;
}

vector<Line *> * Quadtree::distributeLinesTwo(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  double qtree_sw = qtree->start_width;
  double qtree_ew = qtree->end_width;
  
  double qtree_sh = qtree->start_height;
  double qtree_eh = qtree->end_height;

  for (it=lines.begin(); it < lines.end(); it++){
    Line * line = *it;
    if((line->p1.x > qtree_sw && line->p1.x <= qtree_ew) &&
       (line->p1.y >= qtree_sh && line->p1.y <= qtree_eh)){
      // Check if the second point is in the box
      if((line->p2.x > qtree_sw && line->p2.x <= qtree_ew) &&
	 (line->p2.y >= qtree_sh && line->p2.y <= qtree_eh)){
	// The line is in the box...
	linesInTree->push_back(line);
      } else {
        // The line spans more than one quadtree
        qtree->parent->spanningLines.push_back(line);
//printf("spanning line: ((%f, %f), (%f, %f)) velX=%f, velY=%f\n", line->p1.x, line->p2.x, line->p1.y, line->p2.y, line->vel.x, line->vel.y);
      }
   }
  }
  return linesInTree;
}

vector<Line *> * Quadtree::distributeLinesThree(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  double qtree_sw = qtree->start_width;
  double qtree_ew = qtree->end_width;
  
  double qtree_sh = qtree->start_height;
  double qtree_eh = qtree->end_height;

  for (it=lines.begin(); it < lines.end(); it++){
    Line * line = *it;
    if((line->p1.x >= qtree_sw && line->p1.x < qtree_ew) &&
       (line->p1.y >= qtree_sh && line->p1.y <= qtree_eh)){
      // Check if the second point is in the box
      if((line->p2.x >= qtree_sw && line->p2.x < qtree_ew) &&
	 (line->p2.y >= qtree_sh && line->p2.y <= qtree_eh)){
	// The line is in the box...
	linesInTree->push_back(line);
      } else {
        // The line spans more than one quadtree
        qtree->parent->spanningLines.push_back(line);
//printf("spanning line: ((%f, %f), (%f, %f)) velX=%f, velY=%f\n", line->p1.x, line->p2.x, line->p1.y, line->p2.y, line->vel.x, line->vel.y);
      }
   }
  }
  return linesInTree;
}

vector<Line *> * Quadtree::distributeLinesFour(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  double qtree_sw = qtree->start_width;
  double qtree_ew = qtree->end_width;
  
  double qtree_sh = qtree->start_height;
  double qtree_eh = qtree->end_height;

  for (it=lines.begin(); it < lines.end(); it++){
    Line * line = *it;
    if((line->p1.x >= qtree_sw && line->p1.x <= qtree_ew) &&
       (line->p1.y > qtree_sh && line->p1.y <= qtree_eh)){
      // Check if the second point is in the box
      if((line->p2.x >= qtree_sw && line->p2.x <= qtree_ew) &&
	 (line->p2.y > qtree_sh && line->p2.y <= qtree_eh)){
	// The line is in the box...
	linesInTree->push_back(line);
      } else {
        // The line spans more than one quadtree
        qtree->parent->spanningLines.push_back(line);
//printf("spanning line: ((%f, %f), (%f, %f)) velX=%f, velY=%f\n", line->p1.x, line->p2.x, line->p1.y, line->p2.y, line->vel.x, line->vel.y);
      }
   }
  }
  return linesInTree;
}

COLLISION Quadtree::descend(vector<Line *> _lines) {
  lines = _lines;
  int totalLineLineCollisions=0;
  int totalLineWallCollisions=0;
  
  if (lines.size() < divisionThresh) {
    totalLineLineCollisions = detectLineLineCollisions(&lines);
    totalLineWallCollisions = detectLineWallCollisions(&lines);
//printf("counting collisions now in square (%f, %f) (%f, %f)\n", start_width, end_width, start_height, end_height);
  } else {

    vector<Line *> * oneLines;
    vector<Line *> * twoLines;
    vector<Line *> * threeLines;
    vector<Line *> * fourLines;
    
    divideSelf();
    
    oneLines = distributeLinesOne(one);
    twoLines = distributeLinesTwo(two);
    threeLines = distributeLinesThree(three);
    fourLines = distributeLinesFour(four);

//    printf("Lengths: one=%d, two=%d, three=%d, four=%d, extra=%d, total=%d\n",
//          (int) oneLines->size(), (int) twoLines->size(), (int) threeLines->size(),
//           (int) fourLines->size(), (int) spanningLines.size(), (int) lines.size());

//    printf("Adds up: %d\n", 
//           (int) oneLines->size() + (int) twoLines->size() + (int) threeLines->size() +
//           (int) fourLines->size() + (int) spanningLines.size() == (int) lines.size());

    if (spanningLines.size() > 0) {
      totalLineLineCollisions += detectSpanningLineLineCollisions(&spanningLines,
                                  oneLines, twoLines, threeLines, fourLines);
      totalLineWallCollisions += detectLineWallCollisions(&spanningLines);
    }

    // totalLineLineCollisions += one->descend(*oneLines);
    // totalLineLineCollisions += two->descend(*twoLines);
    // totalLineLineCollisions += three->descend(*threeLines);
    // totalLineLineCollisions += four->descend(*fourLines);
    COLLISION ourCollisions;
    ourCollisions = one->descend(*oneLines);
    totalLineLineCollisions += ourCollisions.numLineLineCollisions;
    totalLineWallCollisions += ourCollisions.numLineWallCollisions;

    ourCollisions = two->descend(*twoLines);
    totalLineLineCollisions += ourCollisions.numLineLineCollisions;
    totalLineWallCollisions += ourCollisions.numLineWallCollisions;

    ourCollisions = three->descend(*threeLines);
    totalLineLineCollisions += ourCollisions.numLineLineCollisions;
    totalLineWallCollisions += ourCollisions.numLineWallCollisions;

    ourCollisions = four->descend(*fourLines);
    totalLineLineCollisions += ourCollisions.numLineLineCollisions;
    totalLineWallCollisions += ourCollisions.numLineWallCollisions;

  }
  collisions.numLineLineCollisions = totalLineLineCollisions;
  collisions.numLineWallCollisions = totalLineWallCollisions;
  
  return collisions;
}

int Quadtree::detectLineLineCollisionsTwoLines(vector<Line *> * _lines,
                                               vector<Line *> * otherLines) {
   vector<Line*>::iterator _it1, _it2;
   int numCollisions = 0;
   for (_it1 = _lines->begin(); _it1 != _lines->end(); ++_it1) {
      Line *l1 = *_it1;
      for (_it2 = otherLines->begin(); _it2 != otherLines->end(); ++_it2) {
         Line *l2 = *_it2;
         IntersectionType intersectionType = intersect(l1, l2, timeStep);
         if (intersectionType != NO_INTERSECTION) {
//printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");

//printf("collision (twolines): ((%f, %f), (%f, %f)) ((%f, %f), (%f, %f))\n", l1->p1.x, l1->p2.x, l1->p1.y, l1->p2.y, l2->p1.x, l2->p2.x, l2->p1.y, l2->p2.y); 

//     printf("Num of collisions so far %d\n", numLineLineCollisions);
//            collisionSolver(l1, l2, intersectionType);
            numCollisions++;
         }
      }
   }
//   printf("detectLineCollisions found: %d in the quadtree\n", numLineLineCollisions);
   return numCollisions;
  
}

//TODO: this needs to be fixed -- does double counting now
int Quadtree::detectSpanningLineLineCollisions(vector<Line *> * _lines,
                              vector<Line *> * _linesOne, vector<Line *> * _linesTwo,
                            vector<Line *> * _linesThree, vector<Line *> * _linesFour)
{
  int totalLineLineCollisions = 0;
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_lines, _linesOne);
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_lines, _linesTwo);
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_lines, _linesThree);
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_lines, _linesFour);
   totalLineLineCollisions += detectLineLineCollisions(_lines);
//   printf("detectLineCollisions found: %d in the quadtree\n", totalLineLineCollisions);
   return totalLineLineCollisions;
}

int Quadtree::detectLineLineCollisions(vector<Line *> * _lines) {
   vector<Line*>::iterator it1, it2;
   int totalLineLineCollisions = 0;
   for (it1 = _lines->begin(); it1 != _lines->end(); ++it1) {
      Line *l1 = *it1;
      for (it2 = it1 + 1; it2 != _lines->end(); ++it2) {
         Line *l2 = *it2;
         IntersectionType intersectionType = intersect(l1, l2, timeStep);
         if (intersectionType != NO_INTERSECTION) {
//printf("**********************************************************************\n");
//printf("collision (twolines): ((%f, %f), (%f, %f)) ((%f, %f), (%f, %f))\n", l1->p1.x, l1->p2.x, l1->p1.y, l1->p2.y, l2->p1.x, l2->p2.x, l2->p1.y, l2->p2.y); 
//     printf("Num of collisions so far %d\n", numLineLineCollisions);
//            collisionSolver(l1, l2, intersectionType);
            totalLineLineCollisions++;
         }
      }
   }
//   printf("detectLineCollisions found: %d in the quadtree\n", numLineLineCollisions);
   return totalLineLineCollisions;
}

int Quadtree::detectLineWallCollisions(vector<Line *> * allLines)
{
   vector<Line*>::iterator it;
   int totalLineWallCollisions = 0;
   for (it = allLines->begin(); it != allLines->end(); ++it) {
      Line *line   = *it;
      bool collide = false;
printf("BOX_XMAX=%g, ew=%f\n", BOX_XMAX, end_width);

      // Right side
      if ((line->p1.x > BOX_XMAX  ||
            line->p2.x > BOX_XMAX) &&
            (line->vel.x > 0)) {
	//line->vel.x = -line->vel.x;
         collide  = true;
printf("true\n");
      }
      // Left side
      if ((line->p1.x < BOX_XMIN  ||
            line->p2.x < BOX_XMIN) &&
            (line->vel.x < 0)) {
	//line->vel.x = -line->vel.x;
         collide  = true;
printf("true\n");
      }
      // Top side
      if ((line->p1.y >  BOX_YMAX  ||
            line->p2.y >  BOX_YMAX) &&
            (line->vel.y > 0)) {
	//line->vel.y = -line->vel.y;
         collide  = true;
printf("true\n");
      }
      // Bottom side
      if ((line->p1.y < BOX_YMIN  ||
            line->p2.y < BOX_YMIN) &&
            (line->vel.y < 0)) {
	//line->vel.y = -line->vel.y;
         collide  = true;
printf("true\n");
      }
      // Update total number of collisions
      if (collide == true) {
printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
printf("line-wall collision line ((%f, %f), (%f, %f))\n", line->p1.x, line->p2.x, line->p1.y, line->p2.y);
	totalLineWallCollisions++;
      }
   }
   return totalLineWallCollisions;
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
