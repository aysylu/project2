#include "IntersectionDetection.h"
#include "Line.h"
#include "Vec.h"
#include <assert.h>

// Detect if lines l1 and l2 will be intersected between now and the
// next time step.
IntersectionType intersect(Line *l1, Line *l2, double time)
{
  // Special cases:
  if((l1->type == VERTICAL) && (l2->type == VERTICAL)){
    return cheapIntersectionVertical(l1,l2);
  }
  else if((l1->type == HORIZONTAL) && (l2->type == HORIZONTAL)){
    return cheapIntersectionHorizontal(l1,l2);
  }
  else if ((l1->type == VERTICAL) && (l2->type == HORIZONTAL)) {
    return cheapIntersectionHV(l1, l2);
  }
  else if((l1->type == HORIZONTAL) && (l2->type == VERTICAL)) {
    return cheapIntersectionHV(l2, l1);
  }
  else if((l1->type == DIAGONAL) && (l2->type == DIAGONAL)){
    return cheapIntersectionDD(l1, l2);
  }
  else if (l1->type == DIAGONAL) {
    return cheapIntersectionWithDiagonal(l1, l2);
  }
  else if (l2->type == DIAGONAL) {
    return cheapIntersectionWithDiagonal(l2, l1);
  }
  // Else, run the normal line intersection code
   Vec vel;
   Vec  p1, p2;
   Vec v1(*l1), v2(*l2);

   // Get relative velocity
   vel = l2->vel - l1->vel;

   // Get the parallelogram
   p1 = l2->p1 + (vel * time);
   p2 = l2->p2 + (vel * time);

   int num_line_intersections = 0;
   bool top_intersected = false;
   bool bottom_intersected = false;

   if (intersectLines(l1->p1, l1->p2, l2->p1, l2->p2)) {
      return ALREADY_INTERSECTED;
   }
   if (intersectLines(l1->p1, l1->p2, p1, p2)) {
      num_line_intersections++;
   }
   if (intersectLines(l1->p1, l1->p2, p1, l2->p1)) {
      num_line_intersections++;
      top_intersected = true;
   }
   if (intersectLines(l1->p1, l1->p2, p2, l2->p2)) {
      num_line_intersections++;
      bottom_intersected = true;
   }

   if (num_line_intersections == 2) {
      return L2_WITH_L1;
   }

   if (pointInParallelogram(l1->p1, l2->p1, l2->p2, p1, p2) &&
         pointInParallelogram(l1->p2, l2->p1, l2->p2, p1, p2)) {

      return L1_WITH_L2;
   }

   if (num_line_intersections == 0) {
      return NO_INTERSECTION;
   }

   double angle = v1.angle(v2);

   if (top_intersected) {
      if (angle < 0) {
         return L2_WITH_L1;
      } else {
         return L1_WITH_L2;
      }
   }

   if (bottom_intersected) {
      if (angle > 0) {
         return L2_WITH_L1;
      } else {
         return L1_WITH_L2;
      }
   }

   return L1_WITH_L2;
}

// Check if a point is in the parallelogram
bool pointInParallelogram(Vec point,
                          Vec p1, Vec p2,
                          Vec p3, Vec p4)
{
   double d1 = direction(p1, p2, point);
   double d2 = direction(p3, p4, point);
   double d3 = direction(p1, p3, point);
   double d4 = direction(p2, p4, point);

   if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
         ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
      return true;
   }
   return false;
}

double our_abs(double number){
  if(number < 0)
    return -number;
  return number;
}

IntersectionType cheapIntersectionVertical(Line * l1, Line * l2){
  // Can only intersect if points of both lines have the
  // same x coordinates
  assert(l1->p1.x == l1->p2.x);
  assert(l2->p1.x == l2->p2.x);

  double difference = our_abs(l1->p1.x - l2->p1.x);
  if(difference > 0.000001)
    return NO_INTERSECTION;
    
  if(l2->p1.y >= l1->p1.y &&
     l2->p1.y <= l1->p2.y)
    return L1_WITH_L2;
  
  if(l2->p2.y >= l1->p1.y &&
     l2->p2.y <= l1->p2.y)
    return L1_WITH_L2;

//  if(l2->p1.y >= l1->p2.y &&
//     l2->p1.y <= l1->p1.y) {
//    printf("333333333333333\n");
//    return L1_WITH_L2;
//  }
  
//  if(l2->p2.y >= l1->p2.y &&
//     l2->p2.y <= l1->p1.y) {
//    printf("44444444444444444444\n");
//    return L1_WITH_L2;
//  }
  
  return NO_INTERSECTION;
}

IntersectionType cheapIntersectionHorizontal(Line * l1, Line * l2){
  // Can only intersect if points of both lines have the same
  // y coordinates
  double difference = our_abs(l1->p1.y - l2->p1.y);
  if(difference > 0.0000001)
    return NO_INTERSECTION;

  if(l2->p1.x >= l1->p1.x &&
     l2->p1.x <= l1->p2.x)
    return L1_WITH_L2;
  
  if(l2->p2.x >= l1->p1.x &&
     l2->p2.x <= l1->p2.x)
    return L1_WITH_L2;

  if(l2->p1.x >= l1->p2.x &&
     l2->p1.x <= l1->p1.x)
    return L1_WITH_L2;
  
  if(l2->p2.x >= l1->p2.x &&
     l2->p2.x <= l1->p1.x)
    return L1_WITH_L2;

  return NO_INTERSECTION;
}

IntersectionType cheapIntersectionHV(Line * l1, Line * l2){
  // We require that l1 is vertical
  // and l2 is horizontal

  // Can only intersect if l1 is within x range of l2
  //   and l2 and within y range of l1

  if (l1->p1.x >= l2->p1.x &&
      l1->p1.x <= l2->p2.x) {
    // the vertical line is within x range of the horizontal
    if (l2->p1.y >= l1->p1.y &&
        l2->p1.y <= l2->p2.y) {
    // the horizontal line is within y range of the vertical
        return ALREADY_INTERSECTED;
    }
  }

//  if (intersectLines(l1->p1, l1->p2, l2->p1, l2->p2)) {
//    return ALREADY_INTERSECTED;
//  }
  return NO_INTERSECTION;
}

IntersectionType cheapIntersectionDD(Line * l1, Line * l2){
  // We require that both l1 and l2 are diagonal.

  // The function works as follows: we determine the shorter
  // line and check whether its two points lie inside of the
  // larger line. If either do, then the lines have collided.
  Line * longer;
  Line * shorter;

  if((l1->p1 - l1->p2).length() > (l2->p1 - l2->p2).length()){
    longer = l1;
    shorter = l2;
  } else {
    longer = l2;
    shorter = l1;
  }

  if(onSegment(shorter->p1, longer->p1, longer->p2) ||
     onSegment(shorter->p2, longer->p1, longer->p2)){
    return ALREADY_INTERSECTED;
  }

  return NO_INTERSECTION;
}

IntersectionType cheapIntersectionWithDiagonal(Line * l1, Line * l2) {
  // We require that l1 is a diagonal line

  if (onSegment(l2->p1, l1->p1, l1->p2) || onSegment(l2->p2, l1->p1, l1->p2)) {
    return ALREADY_INTERSECTED;
  }
  return NO_INTERSECTION;
}

// Check if two lines are intersected
bool intersectLines(Vec p1, Vec p2, Vec p3, Vec p4)
{
   // Relative orientation
   double d1 = direction(p3, p4, p1);
   double d2 = direction(p3, p4, p2);
   double d3 = direction(p1, p2, p3);
   double d4 = direction(p1, p2, p4);

   // If (p1, p2) (p3, p4) straddle each other, the line segments must
   // intersect
   if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
         ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
      return true;
   }
   if (d1 == 0 && onSegment(p3, p4, p1)) {
      return true;
   }
   if (d2 == 0 && onSegment(p3, p4, p2)) {
      return true;
   }
   if (d3 == 0 && onSegment(p1, p2, p3)) {
      return true;
   }
   if (d4 == 0 && onSegment(p1, p2, p4)) {
      return true;
   }
   return false;
}


// Obtain the intersection point for two intersecting line segments.
Vec getIntersectionPoint(Vec p1, Vec p2, Vec p3, Vec p4)
{
   double u;

   u = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) /
       ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

   return p1 + (p2 - p1) * u;
}


// Check the direction of two lines (pi, pj) and (pi, pk)
double direction(Vec pi, Vec pj, Vec pk)
{
   return crossProduct(pk.x - pi.x,
                       pk.y - pi.y,
                       pj.x - pi.x,
                       pj.y - pi.y);
}


// Check if a point pk is in the line segment (pi, pj)
bool onSegment(Vec pi, Vec pj, Vec pk)
{
   if (((pi.x <= pk.x && pk.x <= pj.x) ||
         (pj.x <= pk.x && pk.x <= pi.x)) &&
         ((pi.y <= pk.y && pk.y <= pj.y) ||
          (pj.y <= pk.y && pk.y <= pi.y))) {
      return true;
   }
   return false;

}


// Calculate the cross product
double crossProduct(double x1, double y1, double x2, double y2)
{
   return x1 * y2 - x2 * y1;
}


