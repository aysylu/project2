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

vector<Line *> * Quadtree::distributeLinesOne(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  double qtree_sw = qtree->start_width;
  double qtree_ew = qtree->end_width;
  
  double qtree_sh = qtree->start_height;
  double qtree_eh = qtree->end_height;

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
      }
    }
  }
  return linesInTree;
}

int Quadtree::descend(vector<Line *> _lines) {
  lines = _lines;
  int totalLineLineCollisions=0;
  
  if (lines.size() < divisionThresh) {
    totalLineLineCollisions = detectLineLineCollisions(&lines);
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

    if (spanningLines.size() > 0) {
      totalLineLineCollisions += detectSpanningLineLineCollisions(&spanningLines,
                                  oneLines, twoLines, threeLines, fourLines);
    }

      totalLineLineCollisions += one->descend(*oneLines);

      totalLineLineCollisions += two->descend(*twoLines);

      totalLineLineCollisions += three->descend(*threeLines);

      totalLineLineCollisions += four->descend(*fourLines);
  }
  
  return totalLineLineCollisions;
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
            numCollisions++;
         }
      }
   }
   return numCollisions;
  
}

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
            totalLineLineCollisions++;
         }
      }
   }
   return totalLineLineCollisions;
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
