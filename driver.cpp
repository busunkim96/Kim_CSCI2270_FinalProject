#include <iostream>
#include "courses.h"
using namespace std;

int main()
{
    courses test("courseList.csv", 30);
    test.findCoursePublic(3308);
    test.findCoursePublic(3302);
    test.printAllCourses();
}
