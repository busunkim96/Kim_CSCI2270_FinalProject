#include "courses.h"

courses::courses()
{
    hashTableMade = false;
}
courses::~courses()
{
    if(hashTableMade)
        deleteHash();

}
/** void courses::deleteHash()
*   This function destroys the elements in the hash table and the hash table itself.
*
*   Example:
*   courses example;
*   courses.deleteHash();
*
*   Pre: A hash table (empty or filled) should exist before this function is called.
*   Post: The existing hash table is deleted and the memory freed.
*/
void courses::deleteHash(){
     for(int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];
        if(thisCourse != NULL){//more than one entry
            while(thisCourse != NULL){
                Course *temp = thisCourse->next;
                thisCourse->offeredFall.clear();
                thisCourse->offeredSpring.clear();
                delete thisCourse;
                thisCourse = temp;
            }
        }
   }
   delete[] coursesHashTable;
}
/** void courses::makeEmptyTable(int tableSize)
*   This function creates an empty hash table of the size specified in the arguments passed to the constructor.
*
*   Example:
*   courses example;
*   courses.makeEmptyTable(15);
*
*   Pre: An integer value greater than 0 must be passed to the function.
*   Post: An empty hash table is created ready to be populated with the appropriate values with the fillTable function.
*/
void courses::makeEmptyTable(int tableSize){
    Course **hashTable = new Course*[tableSize];//new hashtable of specified size
    coursesHashTable = hashTable;
    hashTableSize = tableSize;

    for(int i = 0; i < hashTableSize; i++){//populating hashtable with blank course entries
        coursesHashTable[i]= NULL;
    }
    hashTableMade = true;
    hashTableEmpty = true;
}
/** fillTable(string csvFileName)
*   The function takes the name of the .csv file as a string and constructs a hash table with the course list based on the file.
*
*   Example:
*   courses example;
*   example.fillTable("courseList.csv")
*
*   Pre: A empty hash table must exist before this function is called. The function expects one string with the format "fileName.csv".
*   The csv file must have the same format as the example files in the repo. The data file should be filtered to
*   include the header line and should only have the columns from YearTerm to insname1. The rows after all the course data should be cleared
*   to make sure there are no empty strings that will result in failure of this function.
*   Post: A searchable hash table with course information will be constructed.
**/
void courses::fillTable(std::string csvFileName){
    std::string line;//stores each line of the csv file with individual course information
    std::string discardedValue, yearTerm, yearSubString, termSubString, dept, courseNumberString, courseName, instructorName;
    int year, term, courseNumber, hashedCourseNum;

    std::ifstream courseFile(csvFileName);
    if(courseFile.is_open()){
        getline(courseFile, line);//this discards the header line

        while(getline(courseFile, line) && line != " "){
            std::stringstream inLine(line);
            getline(inLine, yearTerm, ',');//yearTerm is of the form [year][1 or 7] (e.g., 20107) The last digit indicates the term. 1 is Fall, 7 is Spring
            //std::cout<<yearTerm << std::endl;
            yearSubString = yearTerm.substr(0,4);
            termSubString = yearTerm.substr(4,1);
            //std::cout<<yearSubString<<std::endl;
            year = stoi(yearSubString);
            term = stoi(termSubString);
            getline(inLine, dept, ',');
            //two more discarded terms:  EN, ,CSCI
            for(int j = 0; j < 3; j++){
                getline(inLine,discardedValue, ',');

            }
            //obtain course number and course name
            getline(inLine,courseNumberString,',');
            courseNumber = stoi(courseNumberString);
            getline(inLine, courseName,',');

            //discard 30 terms
            for(int j = 0; j < 30; j++){
                getline(inLine,discardedValue, ',');

            }
            getline(inLine, discardedValue, '"');//to discard leading quotation mark around instructor name (e.g., "HOENIGMAN, RHONDA OLCOTT" --> HOENIGMNA< RHONDA OLCOTT)
            getline(inLine, instructorName, '"');

            //END OF DATA EXTRACTION OF FILE. REST OF FUNCTION IS ADDING TO/UPDATING HASH TABLE
            Course *sameCourse = findCourse(courseNumber);//search hash table for same course number
            Section newSection;
            newSection.instructor = instructorName;
            newSection.year = year;

            if(sameCourse == NULL){//course does not yet exist in hash table
                Course *newCourse = new Course;
                newCourse->courseName = courseName;
                newCourse->courseValue = courseNumber;
                newCourse->next = NULL;
                if(term == 1){
                    newCourse->offeredSpring.push_back(newSection);
                }
                else{
                    newCourse->offeredFall.push_back(newSection);
                }
                int index = makeHashSum(courseNumber);
                insertNewCourse(index, newCourse);

            }
            else{//course exists in hashTable, so update
                if(sameCourse->courseName != courseName){
                    sameCourse->courseName = courseName;
                }
                if(term == 1){
                    sameCourse->offeredSpring.push_back(newSection);
                }
                else{
                    sameCourse->offeredFall.push_back(newSection);
                }
            }

        }
    std::cout<<csvFileName << " successfully read." <<std::endl;
    courseFile.close();
    hashTableEmpty = false;
    }
    else{
        std::cout<<"File not found!" <<std::endl;
    }

}
/**void courses::insertNewCourse(int index, Course *newCourse)
*   This function places a new course into the appropriate place in the hash table. Courses are first stored based on hash sum. Collisions are stored
*   in a linked list, with smaller course values preceding larger ones. e.g,(1300, 2270, 3140)
*
*   Example:
*   course newCourse;
*   insertNewCourse(5, newCourse)
*
*   Pre: A hash table must exist, and the appropriate index (hash code) and a pointer to a course object must be passed to the function.
*   Post: The movie is added to the hash table.
*/
void courses::insertNewCourse(int index, Course *newCourse){

    if(coursesHashTable[index]==NULL){//if no courses yet have this hash value
        coursesHashTable[index] = newCourse;
    }
    else{
        Course *thisCourse = coursesHashTable[index];
        Course *prev;
        bool foundPlace = false;
        while(thisCourse!=NULL && foundPlace == false){//find appropriate place to put in ordered linked list from smaller to larger course numbers
            if(prev != NULL && thisCourse->courseValue > newCourse->courseValue ){//insertion in middle
                prev->next = newCourse;
                newCourse->next = thisCourse;
                foundPlace = true;
            }
            else if(prev == NULL && thisCourse->courseValue > newCourse->courseValue){//insert at head
                newCourse->next = thisCourse;
                coursesHashTable[index] = newCourse;
                foundPlace = true;
            }
            else if (thisCourse->next == NULL && thisCourse->courseValue < newCourse->courseValue){//insertion at end
                thisCourse->next = newCourse;
                newCourse->next = NULL;
                foundPlace = true;
            }
            else{//not at right index, so move forward
                prev = thisCourse;
                thisCourse = thisCourse->next;
            }

    }

    }
}
/** int courses::makeHashSum(int courseNum)
*   This function takes the course number and mods it by the hash table size to produce a hash number.
*
*   Example:
*   makeHashSum(2270)
*
*   Pre: A hash table size and a course number must exist before this function can be used.
*   Post: The int value returned will be the hash value for the course number.
*/
int courses::makeHashSum(int courseNum){
    int hashSum = courseNum%hashTableSize;
    return hashSum;
}

/** void courses::printOnlyFall()
*   This function prints a list of courses that have only been offered in the fall.
*
*   Example:
*   courses example;
*   example.printOnlyFall();
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: A list of courses that have only been offered in the fall will be printed.
*/
void courses::printOnlyFall(){
    std::cout<<"These courses are only offered in the fall" <<std::endl;
    for (int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];
        if(thisCourse != NULL){
            while(thisCourse->next != NULL){
                    if(thisCourse->offeredSpring.size() ==0)
                        std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << std::endl;
                    thisCourse = thisCourse->next;
            }
            if(thisCourse->offeredSpring.size() ==0)
                std::cout<<thisCourse->courseValue << ":" << thisCourse->courseName <<std::endl;//print last
        }
    }
    std::cout<<std::endl;
}
/** void courses::printOnlySpring()
*   This function prints a list of courses that have only been offered in the spring.
*
*   Example:
*   courses example;
*   example.printOnlySpring();
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: A list of courses that have only been offered in the spring will be printed.
*/
void courses::printOnlySpring(){
    std::cout<<"These courses are only offered in the spring" <<std::endl;
    for (int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];

        if(thisCourse != NULL){
            while(thisCourse->next != NULL){
                    if(thisCourse->offeredFall.size() ==0)
                        std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << std::endl;
                    thisCourse = thisCourse->next;
            }
            if(thisCourse->offeredFall.size() ==0)
                std::cout<<thisCourse->courseValue << ":" << thisCourse->courseName<< std::endl;//print last
            }
    }
    std::cout<<std::endl;
}
/** void courses::printMoreFall()
*   This function prints a list of courses that are more often offered in the fall. (excludes courses only offered in fall).
*
*   Example:
*   courses example;
*   example.printMoreFall();
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: A list of courses more often offered in the spring will be printed.
*/
void courses::printMoreFall(){
    float ratio;
    for (int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];
        while(thisCourse != NULL){
            if(thisCourse->offeredSpring.size()!=0){//if the size of the spring vector is 0, the course is only offered in the fall, and that value is excluded
                ratio = float(thisCourse->offeredFall.size())/float(thisCourse->offeredSpring.size());
                if(ratio>1){
                    std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << " is offered " << ratio << " times more often in the fall." << std::endl;
                }
            }
            thisCourse = thisCourse->next;
        }
    }

    std::cout<<"end"<<std::endl;
}
/** void courses::printMoreSpring()
*   This function prints a list of courses that are more often offered in the spring. (excludes courses only offered in spring).
*
*   Example:
*   courses example;
*   example.printMoreSpring();
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: A list of courses more often offered in the spring will be printed.
*/
void courses::printMoreSpring(){
    float ratio;
    for (int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];
        while(thisCourse != NULL){
            if(thisCourse->offeredFall.size()!=0){//if the size of the spring vector is 0, the course is only offered in the fall, and that value is excluded
                ratio = float(thisCourse->offeredSpring.size())/float(thisCourse->offeredFall.size());
                if(ratio>1){
                    std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << " is offered " << ratio << " times more often in the spring." << std::endl;
                }
            }

            thisCourse = thisCourse->next;
        }

    }
    std::cout<<std::endl;
}
/** void courses::printAllCourses()
*   This function prints all the courses in the order they exist in the hash table.
*
*   Example:
*   courses example;
*   example.printAllCourses();
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: A list of all courses with the course number and course name will be printed.
*/
void courses::printAllCourses(){

    for (int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];

        if(thisCourse !=NULL){
            while(thisCourse->next != NULL){

                    std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << std::endl;
                    thisCourse = thisCourse->next;

            }
            std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << std::endl;//print last
        }
    }

}
/** void courses::findCoursePublic(int courseNum)
*   This function finds the course with the specified course number and prints information on the terms it has been offered.
*
*   Example:
*   courses example;
*   example.findCoursePublic(2270);
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: If the course is found, information on which terms it has been offered and how many times it has been offered will be printed.
*/
void courses::findCoursePublic(int courseNum){

    Course* foundCourse = findCourse(courseNum);
    if(foundCourse == NULL){
        std::cout<< "Course not found." << std::endl;
    }
    else{
        std::cout <<"======"<<foundCourse->courseValue << " "<< foundCourse->courseName<<"======" <<std::endl;

        std::cout<<"This course has been offered:\n +" <<foundCourse->offeredFall.size() <<" times in the fall \n +" << foundCourse->offeredSpring.size() << " times in the spring\n" <<std::endl;

        std::cout<<"***Offered in the Fall Semester***" << std::endl;

        for(int i = 0; i < foundCourse->offeredFall.size(); i++){
            std::cout<<foundCourse->offeredFall[i].year << ": " <<foundCourse->offeredFall[i].instructor << std::endl;
        }
        std::cout << std::endl;

        std::cout<<"***Offered in the Spring Semester***" << std::endl;

        for(int i = 0; i < foundCourse->offeredSpring.size(); i++){
            std::cout<<foundCourse->offeredSpring[i].year <<": " <<foundCourse->offeredSpring[i].instructor << std::endl;
        }

        std::cout << std::endl;
    }

}
/** Course* courses::findCourse(int courseNum)
*   This function finds the course with the course number passed to the function.
*
*   Example:
*   Course* dataStructures = findCourse(2270);
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: A pointer to the course with the specified course number will be returned if the course exists. Otherwise NULL will be returned.
*/
Course* courses::findCourse(int courseNum){
    int hashTableIndex = makeHashSum(courseNum);

    Course *searchingFor = NULL;

    Course *thisCourse = coursesHashTable[hashTableIndex];
    if(thisCourse == NULL){//nothing at the hash index
        searchingFor = NULL;
    }
    else{
        if(thisCourse->courseValue == courseNum){//head of linked list is value
            searchingFor = thisCourse;
        }
        else{
            bool found = false;

            while(thisCourse != NULL && found ==false){//search through linked list until found or reach NULL node
                thisCourse = thisCourse->next;
                if(thisCourse != NULL && thisCourse->courseValue == courseNum){
                    searchingFor = thisCourse;
                    found = true;
                }
            }
        }
    }
    return searchingFor;

}
/** void courses::countCollisions()
*   This function returns information about the collisions in the hash table.
*
*   Example:
*   courses example;
*   example.countCollisions();
*
*   Pre: A filled hash table should exist before this function is called.
*   Post: Information on the collisions in the table will be returned along with a histogram with information on
*   the spread of the courses throughout the hash table.(Example below)
*
*   01:*
*   02:**
*   03:
*   04:***
*   You chose a table size of 4.
*   There are 1 blank indices.
*   There are 2 indices with exactly one course.
*   There are 2 indices with collisions.
*   There are 3 courses in the biggest collision.
*/

void courses::countCollisions(){
    int emptyIndices=0;
    int onlyOne=0;
    int collisions=0;
    int biggestCollision=0;

    for(int i = 0; i < hashTableSize; i++){
        if(i < 10)
            std::cout<<0 << i <<":";
        else
            std::cout<<i <<":";
        if(coursesHashTable[i] == NULL){
            emptyIndices++;
        }
        else{
            if(coursesHashTable[i]->next == NULL){
                onlyOne++;
                std::cout<< "*";
            }
            else{
                collisions++;
                int numCollided = 1;
                std::cout<<"*";
                Course *thisCourse = coursesHashTable[i];
                while(thisCourse != NULL){
                    numCollided++;
                    std::cout<<"*";
                    thisCourse = thisCourse->next;
                }
                if(numCollided > biggestCollision){
                    biggestCollision = numCollided;
                }

            }
        }
        std::cout<<std::endl;
    }
    std::cout<<"You chose a table size of " << hashTableSize << "."<<std::endl;
    std::cout<<"There are " <<emptyIndices << " empty indices." << std::endl;
    std::cout<<"There are " << onlyOne << " indices with exactly one course." << std::endl;
    std::cout<<"There are " << collisions <<" indices with collisions"<< std::endl;
    std::cout<<"The are " << biggestCollision << " courses in the biggest collision." <<std::endl;

}
