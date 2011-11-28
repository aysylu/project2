#include "Quadtree.h"

Quadtree::Quadtree(double _start_width, double _end_width, double _start_height, double _end_height){
  start_width = _start_width;
  end_width = _end_width;
  start_height = _start_height;
  end_height = _end_height;
  
  divisionThresh = 15;

  intersectedPairs = new cilk::reducer_list_append<IntersectionInfo>();

  for(int i = 0; i < NUMBER_CHILD_NODES; i++){
    child[i] = NULL;
  }
  
  parent = NULL;
  
  // If we don't want a maximum depth, simply set maxDepth = (unsigned int)-1
  maxDepth = 3;
  // The depth/level of the quadtree in the recurrence
  currentDepth = 0;

  timeStep = 0.5;
}

void Quadtree::divideSelf() {
  double c_start_width[NUMBER_CHILD_NODES];
  double c_start_height[NUMBER_CHILD_NODES];
  double c_end_width[NUMBER_CHILD_NODES];
  double c_end_height[NUMBER_CHILD_NODES];
  
  // Width of first quadrant
  c_start_width[0] = start_width;
  c_end_width[0] = start_width + (end_width-start_width)/2;
  // Height of first quadrant
  c_start_height[0] = start_height;
  c_end_height[0] = start_height + (end_height-start_height)/2;

  // Width of second quadrant
  c_start_width[1] = c_end_width[0];
  c_end_width[1] = end_width;
  // Height of second quadrant
  c_start_height[1] = start_height;
  c_end_height[1] = start_height + (end_height-start_height)/2;

  // Width of third quadrant
  c_start_width[2] = start_width;
  c_end_width[2] = start_width + (end_width-start_width)/2;
  // Height of third quadrant
  c_start_height[2] = c_end_height[0];
  c_end_height[2] = end_height;


  // Width of fourth quadrant
  c_start_width[3] = c_end_width[0];
  c_end_width[3] = end_width;
  // Height of fourth quadrant
  c_start_height[3] = c_end_height[0];
  c_end_height[3] = end_height;

  for(int i = 0; i < NUMBER_CHILD_NODES; i++){
    // Allocate the Quadtree structure associated with the new quadrant
    child[i] = new Quadtree(c_start_width[i], c_end_width[i], c_start_height[i], c_end_height[i]);
    // Since we're descending down one level, increment the current depth.
    child[i]->currentDepth = currentDepth+1;
    child[i]->parent = this;
  }
}

void Quadtree::distributeLines(){
  vector<Line*>::iterator it;
  
  for(it=lines.begin(); it < lines.end(); it++){
    // Is the line in one of our quadtrees?
    bool assignedQuadtree = false;
    Line * line = *it;
    for(int i = 0; (i < NUMBER_CHILD_NODES) && (assignedQuadtree == false); i++){
      if((line->p1.x >= child[i]->start_width && line->p1.x <= child[i]->end_width) &&
	 (line->p1.y >= child[i]->start_height && line->p1.y < child[i]->end_height)){
	
	if((line->p2.x >= child[i]->start_width && line->p2.x <= child[i]->end_width) &&
	   (line->p2.y >= child[i]->start_height && line->p2.y < child[i]->end_height)){
	  
	  assignedQuadtree = true;
	  child[i]->lines.push_back(line);
	  
	}
      }
    }
    // If not, add it to the spanning lines list
    if(assignedQuadtree == false){
      spanningLines.push_back(line);
    }
  }
}

void Quadtree::descend(){
  int totalLineLineCollisions=0;

  // If we have at least divisionThresh lines in our Quadtree, do not descend any further.
  // Also, if we've descended to a level that's greater than or equal to the current depth,
  // stop descending.
  if (lines.size() < divisionThresh ||
      currentDepth >= maxDepth) {
    totalLineLineCollisions = detectLineLineCollisions(&lines);
       
    numLineLineCollisions += totalLineLineCollisions;
       
    list<IntersectionInfo> intersectList = intersectedPairs->get_value();
    for (list<IntersectionInfo>::iterator it = intersectList.begin(); it != intersectList.end(); ++it) {
      IntersectionInfo pair = *it;
      collisionSolver(pair.l1, pair.l2, pair.intersectionType);
    }
  } else {
    // Allocate the Quadtree structures for all of the quadrants
    divideSelf();

    // Distribute the lines in the parent Quadtree among the four child Quadtrees
    distributeLines();

    // Since we don't propagate spanning lines into the child Quadtrees, we want to detect
    // collisions between the spanning lines and the childrens' lines now.
    if (spanningLines.size() > 0) {
      totalLineLineCollisions += detectSpanningLineLineCollisions();
      
      numLineLineCollisions += totalLineLineCollisions;
      
      list<IntersectionInfo> intersectList = intersectedPairs->get_value();
      // Once we find the collisions, we update the velocities of the lines to solve the
      // collisions.
      for (list<IntersectionInfo>::iterator it = intersectList.begin(); it != intersectList.end(); ++it) {
	IntersectionInfo pair = *it;
	collisionSolver(pair.l1, pair.l2, pair.intersectionType);
      }
    }
    // Recurse into the children, detecting collisions among their lines.
    
    for(int i = 0; i < NUMBER_CHILD_NODES-1; i++){
      cilk_spawn child[i]->descend();
    }
    child[LAST_CHILD_NODE]->descend();
    cilk_sync;
      
    // We do not need to re-aggregate the lines into our parent member, as the
    // lines vector only stores pointers to lines, the contents of which are updated.
    // Since the number of lines in the program does not change, having all of the
    // line pointers in one place is sufficient to ensure correctness.
  }
}

int Quadtree::detectLineLineCollisionsTwoLines(vector<Line *> * _lines,
                                               vector<Line *> * otherLines) {
   vector<Line*>::iterator _it1, _it2;
   // int numCollisions = 0;
   cilk::reducer_opadd<int> numCollisions;

   // Pairwise collision detection between members of _lines and members of otherLines
   #pragma cilk grainsize=1
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
   return numCollisions.get_value();
  
}

int Quadtree::detectSpanningLineLineCollisions(){
   int totalLineLineCollisions = 0;

   // Detects collisions between spanning lines and the lines associated with each of the
   // four child Quadtrees
   for(int i = 0; i < NUMBER_CHILD_NODES; i++){
     totalLineLineCollisions += detectLineLineCollisionsTwoLines(&spanningLines, &child[i]->lines);
   }
   // Detect collisions between all lines that span the quadtrees
   totalLineLineCollisions += detectLineLineCollisions(&spanningLines);
   return totalLineLineCollisions;
}

int Quadtree::detectLineLineCollisions(vector<Line *> * _lines) {
   vector<Line*>::iterator it1, it2;
   cilk::reducer_opadd<int> totalLineLineCollisions;

   // Checks if any pair of lines in _lines has a collision. This is O(n^2).
   #pragma cilk grainsize=4
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
   return totalLineLineCollisions.get_value();
}

void Quadtree::collisionSolver(Line *l1, Line *l2, IntersectionType intersectionType)
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
   double m1 = (l1->p1 - l1->p2).length();
   double m2 = (l2->p1 - l2->p2).length();

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
  for(int i = 0; i < NUMBER_CHILD_NODES; i++){
    delete child[i];
  }
  
  delete intersectedPairs;
}

int Quadtree::getNumLineLineCollisions(){
  return numLineLineCollisions.get_value();
}
