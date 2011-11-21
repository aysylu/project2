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

  printf("no_intersection? %d l2_with_l1? %d, l1_with_l2? %d\n", intersect(&l1, &l2, 0.5) == NO_INTERSECTION,  intersect(&l1, &l2, 0.5) == L2_WITH_L1, intersect(&l1, &l2, 0.5) == L1_WITH_L2);
  EXPECT_TRUE(intersect(&l1, &l2, 0.5) == NO_INTERSECTION);
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
