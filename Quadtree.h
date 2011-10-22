#include "Line.h"
#include <vector>
using namespace std;

class Quadtree{
  int start_width;
  int end_width;

  int start_height;
  int end_height;

  vector<Line *> lines;
 public:
  Quadtree(int, int, int, int);
  void descend(vector<Line *>);
  
 private:
  Quadtree * one;
  Quadtree * two;
  Quadtree * three;
  Quadtree * four;
  void divideSelf();
}