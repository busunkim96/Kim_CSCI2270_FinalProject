#include <iostream>
#include "courses.h"
using namespace std;

int main()
{
    courses test = courses();
    bool deleteTable = false;
    string comm = "go";
    string strArraySize, strCourseNum, fileName;
    int arraySize = 0, courseNum;
    int choice = -1;
    while(comm != "quit" && comm != "1" && comm != "2" && comm != "3"
          && comm != "4" && comm !="5" && comm !="6" && comm != "7"
          && comm != "8" && comm !="9"){
        cout<<"======Main Menu======\n1. Create an empty table.\n"
        "2. Fill the table.\n3. Find a course.\n4. Print all courses.\n"
        "5. Print courses only offered one semester.\n6. Print courses more often offered in Fall.\n"
        "7. Print courses more often offered in Spring.\n8. Collision information.\n9. Quit" << endl;
        getline(cin, comm);
        if(comm != "1" && comm != "2" && comm != "3" && comm != "4" && comm !="5" && comm !="6" && comm != "7" && comm != "8" && comm!="9"){
            cout<< "Invalid Input" << endl;
        }
        else{

            choice = stoi(comm);
            switch(choice){
            case 1:
                try{
                    while(arraySize <= 0){
                        cout<<"Please provide an integer hash table size greater than 0: ";
                        getline(cin, strArraySize);
                        arraySize = stoi(strArraySize);
                    }
                    if(test.hashTableMade){
                        deleteTable = true;
                        test.deleteHash(deleteTable);//delete previously existing hash table and its elements
                        cout<<"Existing hash table deleted." <<endl;
                    }
                    test.makeEmptyTable(arraySize);
                    cout<<"Empty hash table of size " << arraySize<<" built."<<endl;
            }
            catch(exception invalid_argument){
                cout<<"Please provide an integer greater than 0."<<endl;
            }
            arraySize = 0;
            comm = -1;
            break;
        case 2:
            if(test.hashTableMade){
                if(!test.hashTableEmpty){//if there are elements in the hash table and user wants to fill with different values
                    deleteTable = false;
                    test.deleteHash(deleteTable);//only delete hash table elements, not table itself
                }
                cout<<"Enter a csv file name (e.g., ""appm.csv"", ""courseList.csv""): ";
                getline(cin, fileName);
                test.fillTable(fileName);
            }
            else{
                cout<<"Please create a new hash table." <<endl;
            }
            comm = -1;
            break;
        case 3:
            try{
                while(strCourseNum.size() != 4){
                    cout<<"Enter a 4 digit course number:";
                    getline(cin, strCourseNum);
                }
                courseNum = stoi(strCourseNum);
                test.findCoursePublic(courseNum);
            }
            catch(exception invalid_argument){
                cout<<"Please provide a 4 digit integer."<<endl;
            }
            strCourseNum = "";
            comm = -1;
            break;
        case 4:
            test.printAllCourses();
            comm = -1;
            break;
        case 5:
            test.printOnlyFall();
            test.printOnlySpring();
            comm = -1;
            break;
        case 6:
            test.printMoreFall();
            comm = -1;
            break;
        case 7:
            test.printMoreSpring();
            comm = -1;
            break;
        case 8:
            test.countCollisions();
            comm = -1;
            break;
        case 9:
            comm = "quit";
            cout<< "Goodbye!"<< endl;
            break;
            }

        }

    }

}
