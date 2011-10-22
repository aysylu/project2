#include "Quadtree.h"

Quadtree::Quadtree(int _start_width, int _end_width, int _start_height, int _end_height){
  start_width = _start_width;
  end_width = _end_width;
  start_height = _start_height;
  end_height = _end_height;
}

void Quadtree::divideSelf(){
  // Width of first quadrant
  int start_width_one = start_width;
  int end_width_one = start_width + (end_width-start_width)/2;
  // Height of first quadrant
  int start_height_one = start_height;
  int end_height_one = start_height + (end_height-start_height)/2;

  // Width of second quadrant
  int start_width_two = end_width_one+1;
  int end_width_two = end_width;
  // Height of second quadrant
  int start_height_two = start_height;
  int end_height_two = start_height + (end_height-start_height)/2;

  // Width of third quadrant
  int start_width_three = start_width;
  int end_width_three = start_width + (end_width-start_width)/2;
  // Height of third quadrant
  int start_height_three = start_height_one+1;
  int end_height_three = end_height;


  // Width of fourth quadrant
  int start_width_four = end_width_one+1;
  int end_width_four = end_width;
  // Height of fourth quadrant
  int start_height_four = start_height_one+1;
  int end_height_four = end_height;

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
}

vector<Line *> * Quadtree::distributeLines(Quadtree * qtree){
  vector<Line *> * linesInTree = new vector<Line*>();
  vector<Line*>::iterator it;
  
  int qtree_sw = qtree->start_width;
  int qtree_ew = qtree->end_width;
  int qtree_sh = qtree->start_height;
  int qtree_eh = qtree->end_height;

  printf("Coordinates for box:\n");
  printf("(sw=%d , ew=%d), (sh=%d, eh=%d)\n", qtree_sw,
	 qtree_ew, qtree_sh, qtree_eh);
  printf("Includes lines:\n");
  
  for (it=lines.begin(); it < lines.end(); it++){
    Line * line = *it;
    
    // Check if x coordinates of line fall into our box
    if(line->p1.x >= qtree_sw && line->p2.x <= qtree_ew){
      // Check if y coordinates of line fall into our box
      
      // Assume that top left corner is the (0,0) coordinate 
      if(line->p1.y >= qtree_sh && line->p2.y <= qtree_eh){
	// The line is inside the quadtree
	printf("(x1=%f, y1=%f), (x2=%f, y2=%f)\n", line->p1.x,
	       line->p1.y, line->p2.x, line->p2.y);
	
	linesInTree->push_back(line);
      }
    }
  }

  return linesInTree;
}

void Quadtree::descend(vector<Line *> _lines) {
  lines = _lines;

  if (lines.size() < divisionThresh) {
    // detectCollisions(); //TODO: implement this
  } else {
    vector<Line *> * oneLines;
    vector<Line *> * twoLines;
    vector<Line *> * threeLines;
    vector<Line *> * fourLines;
    
    divideSelf();
    
    oneLines = distributeLines(one); //TODO: implement this
    // one->descend(oneLines);

    twoLines = distributeLines(two);
    // two->descend(twoLines);

    threeLines = distributeLines(three);
    // three->descend(threeLines);

    fourLines = distributeLines(four);
    // four->descend(fourLines);
  }

  // vector<Line *>::iterator it;
  // for (it=lines.begin(); it < lines.end(); it++) {
  //     Line *l1 = *it;
  //     printf("p1 x, p1 y, p2 x, p2 y %f %f %f %f\n", l1->p1.x, l1->p1.y, l1->p2.x, l1->p2.y);
  //     printf("p1 = %llx\n", &(l1->p1));
  // }
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
