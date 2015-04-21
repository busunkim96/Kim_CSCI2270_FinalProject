#include "courses.h"

/*courses(string)
    The constructor takes the name of the .csv file as a string.
    It constructs the hashtable with the course list based on the file.
        Parameters:
            + string csvFileName
        Pre-Condition: The function expects one string with the format "fileName.csv"
        Post-Condition: A searchable hash table will be constructed.
*/

courses::courses(std::string csvFileName)
{
    std::string line;//stores each line of the csv file with individual course information
    std::string discardedValue, yearTerm, yearSubString, termSubString, courseNumberString, courseName, instructorName;
    int year, term, courseNumber;

    std::ifstream courseFile(csvFileName);
    if(courseFile.is_open()){
        std::cout<< "open!" << std::endl;
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
        }

    }
    courseFile.close();
}

courses::~courses()
{
    //dtor
}


