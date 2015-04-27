###Spring 2015 Final Project for CSCI 2270 
######Project Summary:
This project uses information obtained on CU Boulder courses from Fall 2006 - Fall 2014 provided by the [Office of Planning, Budget, and Analysis](http://www.colorado.edu/pba/course/). A csv file with courses from one department is passed to the program. Each unique course is stored in a hashtable using a hashSum function on the course number (e.g., 2270). Each course has information on which terms it has been offered and by which instructor. There are functions to search for individual courses as well as to list courses that are only offered one semester or are offered more often in either semester. There is also a function to generate a histogram on the number of courses in each index in the array. 

######How to Run:
Opening the codeblocks project should be the easiest way. I've also added the executable. Otherwise, you will need to have:
  1. courses.h, course.cpp, driver.cpp 
  2. At least one csv file with course information in the same location as the driver.cpp file.
The csv file must be formatted like the two examples provided. (courseList.csv and appm.csv). You can create files for different deparments by following the steps below.
    - open [gradesall.xlsm](https://github.com/busunkim96/Kim_CSCI2270_FinalProject/blob/master/gradesall.xlsm)
    - go to the "data" tab
    - filter for one department(e.g., "APPM", "CSCI", "COEN", "MATH")
    - copy the data (including the column headers) from YearTerm to insname1
    - paste it into a different excel file and save it as a .csv file. 
    - Make sure that there are no extra blank cells below the cells with data. 

Once you run the program you will see a menu that you can interact with.
```
======Main Menu======
1. Create an empty table.
2. Fill the table.
3. Find a course.
4. Print all courses.
5. Print courses only offered one semester.
6. Print courses more often offered in Fall.
7. Print courses more often offered in Spring.
8. Collision information.
9. Quit
```
__Notes__
+ The program will not let you fill the table unless you have created a new empty table. (So if you'd like to create a new hash table to read in a different file, you must first go through 1 before you can access 2. 
+ 3 verifies that a course exists and prints out all the times it has been offered and by which instructor.
+ 4 prints out all the courses in the order they exist in the hash table.
+ 5 generates two lists. One has courses only offered in the fall, and the other courses that are only offered in the spring.
+ for 6 and 7, courses that are only offered in one semester are excluded.
+ 8 provides information on the collisions in the hash table and how the courses are spread out across the indices. You can generate hashtables of different sizes and compare what values minimize collisions while not wasting space. 


######Dependencies
None. 
######System	Requirements
Make sure you're using C++11.
######Contributors

######Open	issues/bugs

__Issues__

1. If the user tries to make a new hash table after already having created and populated one, the program behaves correctly, but it looks like the destructor might not be properly called (to destroy the original hash table before creating a new one).
2. The course name is not updating to the most recent version. For example, if in 2006 CSCI 2270 had a course name "DATA STRUCTURES" it does not change even if it was called "Data Structures" in 2013.

__Enhancements__

1. Expand so multiple deparmtents can be handled.
2. Add the average GPA info from the excel file to the "section" object. 
