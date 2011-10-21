class Quadtree{
  int start_width;
  int end_width;

  int start_height;
  int end_height;
 public:
  Quadtree(int, int, int, int);
  DivideSelf();
  
 private:
  Quadtree * one;
  Quadtree * two;
  Quadtree * three;
  Quadtree * four;
}
