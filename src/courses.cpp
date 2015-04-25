#include "courses.h"

courses::courses(std::string csvFileName, int hashArraySize)
{
    makeEmptyTable(15);
    fillTable(csvFileName);
    hashTableMade = true;
}

courses::~courses()
{
    //dtor
}
/**makeEmptyTable(int tableSize)
*   This function creates an empty hash table of the size specified in the arguments passed to the constructor.
*   Parameters:
*       + int tableSize
*   Pre: An integer value greater than 0 must be passed to the function.
*   Post: An empty hash table is created ready to be populated with the appropriate values with the fillTable function.
*/
void courses::makeEmptyTable(int tableSize){
    coursesHashTable = new Course*[tableSize];//new hashtable of specified size
    hashTableSize = tableSize;
    for(int i = 0; i < hashTableSize; i++){//populating hashtable with blank course entries
        Course *newCourse = new Course;
        newCourse->courseName = "";
        newCourse->courseValue = -1;
        newCourse->next = NULL;
        coursesHashTable[i] = newCourse;
    }
}
/**fillTable(string csvFileName)
*   The function takes the name of the .csv file as a string and
*   constructs a hashtable with the course list based on the file.
*   Parameters:
*       + string csvFileName
*   Pre: The function expects one string with the format "fileName.csv"
*   Post: A searchable hash table with course information will be constructed.
**/
void courses::fillTable(std::string csvFileName){
std::string line;//stores each line of the csv file with individual course information
    std::string discardedValue, yearTerm, yearSubString, termSubString, courseNumberString, courseName, instructorName;
    int year, term, courseNumber, hashedCourseNum;

    std::ifstream courseFile(csvFileName);
    if(courseFile.is_open()){
        getline(courseFile, line);//this discards the header line

        while(getline(courseFile, line) && line != ""){
            std::stringstream inLine(line);
            getline(inLine, yearTerm, ',');//yearTerm is of the form [year][1 or 7] (e.g., 20107) The last digit indicates the term. 1 is Fall, 7 is Spring
            yearSubString = yearTerm.substr(0,4);
            termSubString = yearTerm.substr(4,1);
            year = stoi(yearSubString);
            term = stoi(termSubString);

            //four more discarded terms: ,CSCI, EN, ,CSCI
            for(int j = 0; j < 4; j++){
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

            //END OF DATA EXTRACTION OF FILE. REST OF FUNCTION IS ADDING/UPDATING HASH TABLE
            Course *sameCourse = findCourse(courseNumber);//search hash table for same course number
            Section *newSection = new Section;
            newSection->instructor = instructorName;
            newSection->year = year;

            if(sameCourse == NULL){//course does not yet exist in hash table
                Course *newCourse = new Course;
                newCourse->courseName = courseName;
                newCourse->courseValue = courseNumber;

                if(term == 1){
                    newCourse->offeredFall.push_back(newSection);
                }
                else{
                    newCourse->offeredSpring.push_back(newSection);
                }
                int index = makeHashSum(courseNumber);
                insertNewCourse(index, newCourse);

            }
            else{//course exists in hashTable, so update
                if(sameCourse->courseName != courseName)
                    sameCourse->courseName == courseName;
                if(term == 1){
                    sameCourse->offeredFall.push_back(newSection);
                }
                else{
                    sameCourse->offeredSpring.push_back(newSection);
                }
            }

        }

    }
    courseFile.close();
}

void courses::insertNewCourse(int index, Course *newCourse){

    if(coursesHashTable[index]->courseValue == -1){//if no courses yet have this hash value
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
                foundPlace = true;
            }
            else{
                prev = thisCourse;
                thisCourse = thisCourse->next;
            }

    }

    }
}

int courses::makeHashSum(int courseNum){
    int hashSum = courseNum%hashTableSize;
    return hashSum;
}

void courses::printAllCourses(){
for (int i = 0; i < hashTableSize; i++){
        Course *thisCourse = coursesHashTable[i];

        if(thisCourse->courseValue != -1){
        while(thisCourse->next != NULL){
                if(thisCourse->courseValue != -1)
                int totalTimesOffered = thisCourse->offeredFall.size() + thisCourse->offeredSpring.size();
                float ratio;
                std::string offeredMoreOften = (thisCourse->offeredFall.size() > thisCourse->offeredSpring.size())?"Fall":"Spring";
                if(offeredMoreOften == "Spring"){
                    ratio = float(thisCourse->offeredSpring.size())/float(thisCourse->offeredFall.size());
                }
                else{
                    ratio = float(thisCourse->offeredFall.size())/float(thisCourse->offeredSpring.size());
                }

                std::cout << thisCourse->courseValue << ":" << thisCourse->courseName  << " is offered " << ratio << " times more often in the " << offeredMoreOften << std::endl;
            thisCourse = thisCourse->next;
        }
        std::cout<<thisCourse->courseValue << ":" << thisCourse->courseName<< ":"<<thisCourse->courseValue%hashTableSize << std::endl;//print last
    }
    }
}
void courses::findCoursePublic(int courseNum){

    Course* foundCourse = findCourse(courseNum);
    if(foundCourse == NULL){
        std::cout<< "Course not found." << std::endl;
    }
    else{
        std::cout <<"===============================================================================" <<std::endl;
        std::cout<<foundCourse->courseValue << " "<< foundCourse->courseName << std::endl;
        std::cout<<"This course has been offered " << foundCourse->offeredFall.size() <<" times in the fall and " << foundCourse->offeredSpring.size() << " times in the spring.\n" <<std::endl;

        std::cout<<"***Offered in the Fall Semester***" << std::endl;

        for(int i = 0; i < foundCourse->offeredFall.size(); i++){
            std::cout<<foundCourse->offeredFall[i]->year << ": " <<foundCourse->offeredFall[i]->instructor << std::endl;
        }
        std::cout << std::endl;

        std::cout<<"***Offered in the Spring Semester***" << std::endl;

        for(int i = 0; i < foundCourse->offeredSpring.size(); i++){
            std::cout<<foundCourse->offeredSpring[i]->year <<": " <<foundCourse->offeredSpring[i]->instructor << std::endl;
        }

        std::cout << std::endl;
    }

}

Course* courses::findCourse(int courseNum){
    int hashTableIndex = makeHashSum(courseNum);

    Course *searchingFor = NULL;

    Course *thisCourse = coursesHashTable[hashTableIndex];
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
    return searchingFor;

}
