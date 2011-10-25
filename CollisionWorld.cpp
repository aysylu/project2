/*
 * CollisionWorld detects and handles the line segment intersections
 */

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "CollisionWorld.h"
#include "IntersectionDetection.h"
#include "Line.h"

// Constructor.
CollisionWorld::CollisionWorld()
{
   numLineWallCollisions = 0;
   numLineLineCollisions = 0;
   quadtree_lineLineCollisions = 0;
   timeStep = 0.5;
}


// Update the lines.
void CollisionWorld::updateLines()
{
   int linesLength = lines.size();

   Quadtree * qtree = new Quadtree(BOX_XMIN, BOX_XMAX, BOX_YMIN, BOX_YMAX);
   qtree->lines = lines;
   
   qtree->descend();
   quadtree_lineLineCollisions = qtree->getNumLineLineCollisions();
   
   lines = qtree->lines;
   delete(qtree);

   updatePosition();
   lineWallCollision();
}


// Update line positions.
void CollisionWorld::updatePosition()
{
   double t = timeStep;
   vector<Line*>::iterator it;
   for (it = lines.begin(); it != lines.end(); ++it) {
      Line *line = *it;

      line->p1 += (line->vel * t);
      line->p2 += (line->vel * t);
   }
}




// Handle line to wall collisions
void CollisionWorld::lineWallCollision()
{
   vector<Line*>::iterator it;
   for (it = lines.begin(); it != lines.end(); ++it) {
      Line *line   = *it;
      bool collide = false;

      // Right side
      if ((line->p1.x > BOX_XMAX  ||
            line->p2.x > BOX_XMAX) &&
            (line->vel.x > 0)) {
         line->vel.x = -line->vel.x;
         collide  = true;
      }
      // Left side
      if ((line->p1.x < BOX_XMIN  ||
            line->p2.x < BOX_XMIN) &&
            (line->vel.x < 0)) {
         line->vel.x = -line->vel.x;
         collide  = true;
      }
      // Top side
      if ((line->p1.y >  BOX_YMAX  ||
            line->p2.y >  BOX_YMAX) &&
            (line->vel.y > 0)) {
         line->vel.y = -line->vel.y;
         collide  = true;
      }
      // Bottom side
      if ((line->p1.y < BOX_YMIN  ||
            line->p2.y < BOX_YMIN) &&
            (line->vel.y < 0)) {
         line->vel.y = -line->vel.y;
         collide  = true;
      }
      // Update total number of collisions
      if (collide == true) {
         numLineWallCollisions++;
      }
   }
}


// Return the total number of lines in the box
unsigned int CollisionWorld::getNumOfLines()
{
   return lines.size();
}


// Add a line into the box
void CollisionWorld::addLine(Line *line)
{
   lines.push_back(line);
}


// Get the i-th line from the box
Line *CollisionWorld::getLine(unsigned int index)
{
   if (index >= lines.size())
      return NULL;
   return lines[index];
}


// Delete all lines in the box
void CollisionWorld::deleteLines()
{
   for (vector<Line*>::iterator it = lines.begin(); it < lines.end(); it++) {
     delete *it;
   }
   lines.clear();
}


// Get total number of line wall collisions
unsigned int CollisionWorld::getNumLineWallCollisions()
{
   return numLineWallCollisions;
}


// Get total number of Line Line collisions;
unsigned int CollisionWorld::getNumLineLineCollisions()
{
   return numLineLineCollisions;
}


