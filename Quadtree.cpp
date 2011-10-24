#include "Quadtree.h"

Quadtree::Quadtree(double _start_width, double _end_width, double _start_height, double _end_height){
  start_width = _start_width;
  end_width = _end_width;
  start_height = _start_height;
  end_height = _end_height;
  
  divisionThresh = 15;

  intersectedPairs = new cilk::reducer_list_append<IntersectionInfo>();
  
  one = NULL;
  two = NULL;
  three = NULL;
  four = NULL;
  parent = NULL;
  
  qtree_sw = 0;
  qtree_ew = 0;
  
  qtree_sh = 0;
  qtree_eh = 0;

  // If we don't want a maximum depth, simply set maxDepth = (unsigned int)-1
  maxDepth = 3;
  // The depth/level of the quadtree in the recurrence
  currentDepth = 0;

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

  // Allocate the Quadtree structures associated with each of the four
  // quadrants.
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

  // Since we're descending down one level, increment the current depth.
  one->currentDepth = currentDepth + 1;
  two->currentDepth = currentDepth + 1;
  three->currentDepth = currentDepth + 1;
  four->currentDepth = currentDepth + 1;
  
  one->parent = this;
  two->parent = this;
  three->parent = this;
  four->parent = this;
}

void Quadtree::distributeLines(Quadtree * qtreeOne, Quadtree * qtreeTwo, Quadtree * qtreeThree, Quadtree * qtreeFour,
			       vector<Line*> divideLines){
  vector<Line*>::iterator it;
  
  double qtreeOne_sw = qtreeOne->start_width;
  double qtreeOne_ew = qtreeOne->end_width;
  
  double qtreeOne_sh = qtreeOne->start_height;
  double qtreeOne_eh = qtreeOne->end_height;

  double qtreeTwo_sw = qtreeTwo->start_width;
  double qtreeTwo_ew = qtreeTwo->end_width;
  
  double qtreeTwo_sh = qtreeTwo->start_height;
  double qtreeTwo_eh = qtreeTwo->end_height;

  double qtreeThree_sw = qtreeThree->start_width;
  double qtreeThree_ew = qtreeThree->end_width;
  
  double qtreeThree_sh = qtreeThree->start_height;
  double qtreeThree_eh = qtreeThree->end_height;

  double qtreeFour_sw = qtreeFour->start_width;
  double qtreeFour_ew = qtreeFour->end_width;
  
  double qtreeFour_sh = qtreeFour->start_height;
  double qtreeFour_eh = qtreeFour->end_height;

  // Divide the lines into one of four quadtrees - else, consider the
  // line a spanning line. A spanning line spans multiple quadtrees.
  for(it=divideLines.begin(); it < divideLines.end(); it++){
    Line * line = *it;
    bool assignedQuadtree = false;

    // Is line in qtreeOne?
    if((line->p1.x >= qtreeOne_sw && line->p1.x <= qtreeOne_ew) &&
       (line->p1.y >= qtreeOne_sh && line->p1.y < qtreeOne_eh)){
      if((line->p2.x >= qtreeOne_sw && line->p2.x <= qtreeOne_ew) &&
	 (line->p2.y >= qtreeOne_sh && line->p2.y < qtreeOne_eh)){
	assignedQuadtree = true;
	qtreeOne->lines.push_back(line);
      }
    }
    // Is line in qtreeTwoOne?
    else if((line->p1.x >= qtreeTwo_sw && line->p1.x <= qtreeTwo_ew) &&
       (line->p1.y >= qtreeTwo_sh && line->p1.y < qtreeTwo_eh)){
      if((line->p2.x >= qtreeTwo_sw && line->p2.x <= qtreeTwo_ew) &&
	 (line->p2.y >= qtreeTwo_sh && line->p2.y < qtreeTwo_eh)){
	assignedQuadtree = true;
	qtreeTwo->lines.push_back(line);
      }
    }
    // Is line in qtreeThree?
    else if((line->p1.x >= qtreeThree_sw && line->p1.x <= qtreeThree_ew) &&
       (line->p1.y >= qtreeThree_sh && line->p1.y < qtreeThree_eh)){
      if((line->p2.x >= qtreeThree_sw && line->p2.x <= qtreeThree_ew) &&
	 (line->p2.y >= qtreeThree_sh && line->p2.y < qtreeThree_eh)){
	assignedQuadtree = true;
	qtreeThree->lines.push_back(line);
      }
    }
    // Is line in qtreeFour?
    else if((line->p1.x >= qtreeFour_sw && line->p1.x <= qtreeFour_ew) &&
       (line->p1.y >= qtreeFour_sh && line->p1.y < qtreeFour_eh)){
      if((line->p2.x >= qtreeFour_sw && line->p2.x <= qtreeFour_ew) &&
	 (line->p2.y >= qtreeFour_sh && line->p2.y < qtreeFour_eh)){
	assignedQuadtree = true;
	qtreeFour->lines.push_back(line);
      }
    }
    if(assignedQuadtree == false){
      spanningLines.push_back(line);
    }
  }
}

int Quadtree::descend(){
  int totalLineLineCollisions=0;

  // If we have at least divisionThresh lines in our Quadtree, do not descend any further.
  // Also, if we've descended to a level that's greater than or equal to the current depth,
  // stop descending.
  if (lines.size() < divisionThresh ||
      currentDepth >= maxDepth) {
       totalLineLineCollisions = detectLineLineCollisions(&lines);
       list<IntersectionInfo> intersectList = intersectedPairs->get_value();
       for (list<IntersectionInfo>::iterator it = intersectList.begin(); it != intersectList.end(); ++it) {
         IntersectionInfo pair = *it;
         collisionSolver(pair.l1, pair.l2, pair.intersectionType);
       }
  } else {
    // Allocate the Quadtree structures for all of the quadrants
    divideSelf();

    // Distribute the lines in the parent Quadtree among the four child Quadtrees
    distributeLines(one, two, three, four, lines);

    // Since we don't propagate spanning lines into the child Quadtrees, we want to detect
    // collisions between the spanning lines and the childrens' lines now.
    if (spanningLines.size() > 0) {
      totalLineLineCollisions += detectSpanningLineLineCollisions(&spanningLines,
                                  &one->lines, &two->lines, &three->lines, &four->lines);
       list<IntersectionInfo> intersectList = intersectedPairs->get_value();
       // Once we find the collisions, we update the velocities of the lines to solve the
       // collisions.
       for (list<IntersectionInfo>::iterator it = intersectList.begin(); it != intersectList.end(); ++it) {
         IntersectionInfo pair = *it;
         collisionSolver(pair.l1, pair.l2, pair.intersectionType);
       }
    }
    // Recurse into the children, detecting collisions among their lines.
      totalLineLineCollisions += one->descend();
      totalLineLineCollisions += two->descend();
      totalLineLineCollisions += three->descend();
      totalLineLineCollisions += four->descend();

      // Aggregate the newly-updated lines into the parent's line vector
//      lines.clear();
//      lines.insert(lines.end(), one->lines.begin(), one->lines.end());
//      lines.insert(lines.end(), two->lines.begin(), two->lines.end());
//      lines.insert(lines.end(), three->lines.begin(), three->lines.end());
//      lines.insert(lines.end(), four->lines.begin(), four->lines.end());
//      lines.insert(lines.end(), spanningLines.begin(), spanningLines.end());

  }
  
  return totalLineLineCollisions;
}

int Quadtree::detectLineLineCollisionsTwoLines(vector<Line *> * _lines,
                                               vector<Line *> * otherLines) {
   vector<Line*>::iterator _it1, _it2;
   int numCollisions = 0;

   // Pairwise collision detection between members of _lines and members of otherLines
   cilk_for (int i = 0; i < _lines->size(); ++i) {
      Line *l1 = _lines->at(i);
      for (int j = 0; j < otherLines->size(); ++j) {
         Line *l2 = otherLines->at(j);
         IntersectionType intersectionType = intersect(l1, l2, timeStep);
         if (intersectionType != NO_INTERSECTION) {
	   // If the lines are intersecting, add them to the intersectedPairs list for
	   // later resolution.
           intersectedPairs->push_back(IntersectionInfo(l1, l2, intersectionType));
            numCollisions++;
         }
      }
   }
   return numCollisions;
  
}

int Quadtree::detectSpanningLineLineCollisions(vector<Line *> * _spanningLines,
                              vector<Line *> * _linesOne, vector<Line *> * _linesTwo,
                            vector<Line *> * _linesThree, vector<Line *> * _linesFour)
{
   int totalLineLineCollisions = 0;

   // Detects collisions between spanning lines and the lines associated with each of the
   // four child Quadtrees
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_spanningLines, _linesOne);
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_spanningLines, _linesTwo);
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_spanningLines, _linesThree);
   totalLineLineCollisions += detectLineLineCollisionsTwoLines(_spanningLines, _linesFour);
   totalLineLineCollisions += detectLineLineCollisions(_spanningLines);
   return totalLineLineCollisions;
}

int Quadtree::detectLineLineCollisions(vector<Line *> * _lines) {
   vector<Line*>::iterator it1, it2;
   int totalLineLineCollisions = 0;

   // Checks if any pair of lines in _lines has a collision. This is O(n^2).
   cilk_for (int i = 0; i < _lines->size(); ++i) {
      Line *l1 = _lines->at(i);
      for (int j = i + 1; j < _lines->size(); ++j) {
         Line *l2 = _lines->at(j);
         IntersectionType intersectionType = intersect(l1, l2, timeStep);
         if (intersectionType != NO_INTERSECTION) {
  	    // If the lines are intersecting, add them to the intersectedPairs list for
	    // later resolution.
            intersectedPairs->push_back(IntersectionInfo(l1, l2, intersectionType));
            totalLineLineCollisions++;
         }
      }
   }
   return totalLineLineCollisions;
}

void Quadtree::collisionSolver(Line *l1, Line *l2, IntersectionType
                                     intersectionType)
{
   // Despite our efforts to determine whether lines will intersect ahead of
   // time (and to modify their velocities appropriately), our simplified model
   // can sometimes cause lines to intersect.  In such a case, we compute
   // velocities so that the two lines can get unstuck in the fastest possible
   // way, while still conserving momentum and kinetic energy.
   if (intersectionType == ALREADY_INTERSECTED) {
      Vec p = getIntersectionPoint(l1->p1, l1->p2, l2->p1, l2->p2);

      if ((l1->p1 - p).length() < (l1->p2 - p).length()) {
         l1->vel = (l1->p2 - p).normalize() * l1->vel.length();
      } else {
         l1->vel = (l1->p1 - p).normalize() * l1->vel.length();
      }
      if ((l2->p1 - p).length() < (l2->p2 - p).length()) {
         l2->vel = (l2->p2 - p).normalize() * l2->vel.length();
      } else {
         l2->vel = (l2->p1 - p).normalize() * l2->vel.length();
      }
      return;
   }

   // Compute the collision face/normal vectors
   Vec face;
   Vec normal;
   if (intersectionType == L1_WITH_L2) {
      Vec v(*l2);
      face = v.normalize();
   } else {
      Vec v(*l1);
      face = v.normalize();
   }
   normal = face.orthogonal();

   // Obtain each line's velocity components with respect to the collision
   // face/normal vectors.
   double v1Face = l1->vel.dotProduct(face);
   double v2Face = l2->vel.dotProduct(face);
   double v1Normal = l1->vel.dotProduct(normal);
   double v2Normal = l2->vel.dotProduct(normal);

   // Compute the mass of each line (we simply use its length).
   // Retrieve the length of the line from the cache of line lengths computed
   // in createLines() in LineDemo.cpp
   double m1 = l1->mass;
   double m2 = l2->mass;

   // Perform the collision calculation (computes the new velocities along the
   // direction normal to the collision face such that momentum and kinetic
   // energy are conserved).
   double newV1Normal = ((m1 - m2) / (m1 + m2)) * v1Normal +
                        (2 * m2 / (m1 + m2)) * v2Normal;
   double newV2Normal = (2 * m1 / (m1 + m2)) * v1Normal +
                        ((m2 - m1) / (m2 + m1)) * v2Normal;

   // Combine the resulting velocities.
   l1->vel = normal * newV1Normal + face * v1Face;
   l2->vel = normal * newV2Normal + face * v2Face;

   return;
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

  delete intersectedPairs;
}
