#include "IntersectionDetection.h"
#include "gtest/gtest.h"

// Anonymous namespaces are a C++ idiom for declaring symbols to be
// file-private, ie not exported, like declaring a function static in straight C.
namespace {

// The fixture for testing intersections.
class IntersectionDetectionTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  IntersectionDetectionTest() {
    // You can do set-up work for each test here.
  }

  virtual ~IntersectionDetectionTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }
};

TEST_F(IntersectionDetectionTest, crossingLinesIntersect) {
  Vec p1(0, 0);
  Vec p2(1, 1);
  Vec p3(1, 0);
  Vec p4(0, 1);
  EXPECT_TRUE(intersectLines(p1, p2, p3, p4));
}

TEST_F(IntersectionDetectionTest, parallelLinesDontIntersect) {
  Vec p1(0, 0);
  Vec p2(1, 0);
  Vec p3(0, 1);
  Vec p4(1, 1);
  EXPECT_FALSE(intersectLines(p1, p2, p3, p4));
}

TEST_F(IntersectionDetectionTest, verticalParallelLinesDontIntersect) {
  Vec p1(0.025, 0.025);
  Vec p2(0.025, 0.05);
  Vec p3(0.015, 0.025);
  Vec p4(0.015, 0.05);

  Line l1;
  l1.p1 = p1;
  l1.p2 = p2;
  l1.type = VERTICAL;
  
  Line l2;
  l2.p1 = p3;
  l2.p2 = p4;
  l2.type = VERTICAL;

  EXPECT_TRUE(intersect(&l1, &l2, 0.5) == NO_INTERSECTION);
}

  TEST_F(IntersectionDetectionTest, horizontalParallelLinesDontIntersect){
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.01, 0.05);

    Vec l2_p1(0.03, 0.01);
    Vec l2_p2(0.03, 0.05);

    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.type = HORIZONTAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.type = HORIZONTAL;

    EXPECT_TRUE(intersect(&l1, &l2, 0.5) == NO_INTERSECTION);
  }

  TEST_F(IntersectionDetectionTest, horizontalVerticalLinesIntersect){
    // Vertical line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.01, 0.05);

    // Horizontal line
    Vec l2_p1(0.001, 0.01);
    Vec l2_p2(0.02, 0.01);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = VERTICAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = HORIZONTAL;

    EXPECT_FALSE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }

  TEST_F(IntersectionDetectionTest, horizontalVerticalLinesIntersect2){
    // Vertical line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.01, 0.05);

    // Horizontal line
    Vec l2_p1(0.001, 0.03);
    Vec l2_p2(0.02, 0.03);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = VERTICAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = HORIZONTAL;

    EXPECT_FALSE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }

  // Write test cases for:
  // _ |
  // / |
  // / _
  TEST_F(IntersectionDetectionTest, diagonalVerticalLinesIntersect){
    // Diagonal line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.02, 0.02);

    // Vertical line
    Vec l2_p1(0.015, 0.001);
    Vec l2_p2(0.015, 0.02);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = DIAGONAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = VERTICAL;
    
    EXPECT_FALSE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }

  TEST_F(IntersectionDetectionTest, diagonalHorizontalLinesIntersect){
    // Diagonal line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.02, 0.02);

    // Horizontal line
    Vec l2_p1(0.0001, 0.01);
    Vec l2_p2(0.05, 0.01);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = DIAGONAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = HORIZONTAL;
    
    EXPECT_FALSE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }
  
  TEST_F(IntersectionDetectionTest, diagonalHorizontalLinesDontIntersect){
    // Diagonal line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.02, 0.02);

    // Horizontal line
    Vec l2_p1(0.03, 0.01);
    Vec l2_p2(0.05, 0.01);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = DIAGONAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = DIAGONAL;
    
    EXPECT_TRUE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }

  TEST_F(IntersectionDetectionTest, diagonalVerticalLinesDontIntersect){
    // Diagonal line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.02, 0.02);

    // Vertical line
    Vec l2_p1(0.03, 0.01);
    Vec l2_p2(0.03, 0.02);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = DIAGONAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = VERTICAL;
    
    EXPECT_TRUE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }
  

  TEST_F(IntersectionDetectionTest, horizontalVerticalLinesDontIntersect1){
    // Vertical line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.01, 0.05);

    // Horizontal line
    Vec l2_p1(0.001, 0.01);
    Vec l2_p2(0.009, 0.01);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = VERTICAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = HORIZONTAL;

    EXPECT_TRUE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
    
  }

  TEST_F(IntersectionDetectionTest, horizontalVerticalLinesDontIntersect){
    // Vertical line
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.01, 0.05);

    // Horizontal line
    Vec l2_p1(0.001, 0.009);
    Vec l2_p2(0.02, 0.009);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = VERTICAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = HORIZONTAL;

    EXPECT_TRUE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }

  TEST_F(IntersectionDetectionTest, diagonalParallelLinesDontIntersect){
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.02, 0.02);

    Vec l2_p1(0.03, 0.01);
    Vec l2_p2(0.04, 0.02);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = DIAGONAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = DIAGONAL;
    
    EXPECT_TRUE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }
  
  TEST_F(IntersectionDetectionTest, diagonalParallelLinesOverlap){
    Vec l1_p1(0.01, 0.01);
    Vec l1_p2(0.02, 0.02);

    Vec l2_p1(0.01, 0.01);
    Vec l2_p2(0.02, 0.02);
    
    Line l1;
    l1.p1 = l1_p1;
    l1.p2 = l1_p2;
    l1.vel = Vec(0.5, 0.5);
    l1.type = DIAGONAL;

    Line l2;
    l2.p1 = l2_p1;
    l2.p2 = l2_p2;
    l2.vel = Vec(0.5, 0.5);
    l2.type = DIAGONAL;
    
    EXPECT_FALSE(intersect(&l1, &l2, 0.0001) == NO_INTERSECTION);
  }

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
