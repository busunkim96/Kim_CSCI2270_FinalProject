###Spring 2015 Final Project for CSCI 2270 
######Project Summary:
This project uses information obtained on CU Boulder courses from Fall 2006 - Fall 2014 provided by the [Office of Planning, Budget, and Analysis](http://www.colorado.edu/pba/course/). A csv file with courses from one department is passed to the program. Each unique course is stored in a hashtable using a hashSum function on the course number (e.g., 2270). Each course has information on which terms it has been offered and by which instructor. There are functions to search for individual courses as well as to list courses that are only offered one semester or are offered more often in either semester. There is also a function to generate a histogram on the number of courses in each index in the array. 

######How to Run:
Using codeblocks should be the easiest way. Otherwise, you will need to have:
  1. courses.h, course.cpp, driver.cpp 
  2. At least one csv file with course information in the same location as the driver.cpp file.
The csv file must be formatted __exactly__ like the two examples provided. (courseList.csv and appm.csv)
The columns from YearTerm to insname1 should be included, and there must be nothing after the final cell with data.
You can create files for different deparments by downloading [this file]("http://www.colorado.edu/pba/course/IGroup.xlsx"), navigating to the "data" tab, and filtering for one department. Then copy the data (including the column headers) from YearTerm to insname1, paste it into a different excel file, and save it as a .csv file. 

######Dependencies
None. 
######System	Requirements
Make sure you're using C++11.
######Group	Members
+ Just me. 

######Contributors
+ List the people who	were not on the	project	team,	but may	have contributed comments, enhancements, and	bug	fixes.

######Open	issues/bugs
1. If the user tries to make a new hash table after already having created and populated one, the program behaves correctly, but it looks like the destructor might not be properly called (to destroy the original hash table before creating a new one).
2. The course name is not updating to the most recent version. For example, if in 2006 CSCI 2270 had a course name "DATA STRUCTURES" it does not change even if it was called "Data Structures" in 2013. 
