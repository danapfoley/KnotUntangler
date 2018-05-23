//This file is a part of KnotUntangler
//    Copyright (C) 2017  Dana Foley
//    
//    KnotUntangler is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//    
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//        
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.



#include "Knot.h"

#define KnotListFilePath "KnotList.txt"

//Set to 1 to print out basic debug info during execution
#define Debug 0

//Set to 1 to run basic operative tests
#define doTests 0

#if doTests
#define test(x) x
#else
#define test(x)
#endif

#if Debug
#define dPrint(x) x
#else
#define dPrint(x)
#endif



void runAllTests();
int rm1Test();
int rm2Test();
int copyTest();
int conversionTest();
int crossingTurnTest();
int findStrandsOfLengthTest();
int executeTM2Test();


//Basic control function for untangling
bool Untangle(Knot &knot) {
    
    //Keep track of what the original size of the knot was
    //To see if we ended up reducing at all
    int origSize = knot.size();
    
    
    //While the knot is not the unknot
    while (knot.size()!=0) {
        
        //Try immediate reductions first
        if (knot.rm2()) {
            dPrint(cout << "RM2 performed: " << knot << endl;)
            continue;
        }
        if (knot.rm1()) {
            dPrint(cout << "RM1 performed: " << knot << endl;)
            continue;
        }
        
        //Try translations
        //tm2 internally tests for potential success
        
        if (knot.TM()) {
            dPrint(cout << "TM2 performed: " << knot << endl;)
            continue;
        }
        
        //If nothing else works, try dealing with 1-tangles
        if (knot.remove1Tangles()) {
            continue;
        }
        
        //Control only reaches here if knot is fully simplified and nontrivial
        break;
    }
    
    knot.reinsert1Tangles();
    
    //If untangling succeeded to at least some extent
    if (knot.size() < origSize)
        return true;
    else
        return false;
    
}




int main(int argc, char * argv[]) {
    string knotString = ""; //The string of the knot to be untangled
    
    dPrint(cout << "Debug mode ON" << endl;)

    test(runAllTests();)

    string argStr = "";
    for (int i = 1; i < argc; i++) {
        argStr += argv[i];
        argStr += " ";
    }

    if (argStr != "") {
        argStr.pop_back(); //Remove trailing space
        knotString = argStr;
    }
    else {
        vector<string> knotList;
        string line;
        ifstream knotFile(KnotListFilePath);
        if (knotFile.is_open())
        {
            int choiceNumber = -1;
            int optionNumber = 1;
            int counter = 1;

            cout << "0: Enter your own knot" << endl;

            while (knotFile.good())
            {
                getline(knotFile, line);
                if (line == "") continue;

                if (counter % 2 == 1) {
                    cout << optionNumber << ": " << line << endl;
                    optionNumber++;
                }
                else
                    knotList.push_back(line);
                counter++;
            }
            knotFile.close();

            cout << endl << "Choose a knot to untangle, or input 0 to enter your own knot: ";

            while (choiceNumber < 0 || choiceNumber > optionNumber)
                cin >> choiceNumber;

            if (choiceNumber == 0) {
                cout << "Enter an Extended Gauss Code list (Ex: [1, -2, 3, 1, 2, 3]): ";
                cin.ignore();
                getline(cin, knotString);
            }
            else
                knotString = knotList.at(choiceNumber - 1);

        }
        else {
            dPrint(cout << "Unable to open file" << endl;)
                return 1;
        }
    }
    
    dPrint(cout << "Knot: " << knotString << endl;)
    
    Knot knot(knotString);
    
    dPrint(cout << "Knot object after being created: " << knot << endl << endl;)
    
    Untangle(knot);
    
    dPrint(cout << "Knot after being untangled: ";)
        
    cout << knot << endl;
    
//    
//    //Get array from final knot for naming program
//    int * finalKnotArray = knot.toArray();
//    int knotLength = knot.size()*2; //size returns the number of crossings, and we need twice that for arrays
//    Name(finalKnotArray, knotLength);
//    

    dPrint(cin.get(); cin.get();)

    return 0;
}

void runAllTests() {
    int numTestsFailed = 0
    
    + rm1Test()
    + rm2Test()
    + copyTest()
    + conversionTest()
    + crossingTurnTest()
    + findStrandsOfLengthTest()
    + executeTM2Test();
    
    cout << "Total tests failed: " << -1 * numTestsFailed << endl;
}

int rm1Test() {
    
    string knotString = "[1, -2, 3, 4, 4, 1, 2, 3]";
    
    Knot knot(knotString);
    
    if (knot.rm1()) {
        cout << "RM1 test passed" << endl;
        return 0;
    }
    else {
        cout << "RM1 test FAILED" << endl;
        return -1;
    }
    
}

int rm2Test() {
    
    string knotString = "[1, 5, 4, -2, 3, 4, -5, 1, 2, 3]";
    
    Knot knot(knotString);
    
    if (knot.rm2()) {
        cout << "RM2 test passed" << endl;
        return 0;
    }
    else {
        cout << "RM2 test FAILED" << endl;
        return -1;
    }
    
}

int copyTest() {
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";

    Knot knot(knotString);

    Knot knot2(knot);
    Knot knot3 = knot;

    if ((knot.toExtGaussString() != knot2.toExtGaussString()) || (knot.toExtGaussString() != knot3.toExtGaussString())) {
        cout << "Copy test FAILED" << endl;
        return -1;
    }
    cout << "Copy test passed" << endl;
    return 0;
}

int conversionTest() {
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    Knot knot(knotString);
    
    if (knot.toExtGaussString() != knotString) {
        cout << "Conversion to string FAILED" << endl;
        cout << "Expected string: " + knotString << endl;
        cout << "Received string: " + knot.toExtGaussString() << endl;
        return -1;
    }
    
    Knot knot2(knot.toVector());
    if (knot2.toExtGaussString() != knotString) {
        cout << "Conversion to array FAILED" << endl << knot2.toExtGaussString();
        return -1;
    }
    else {
        cout << "Conversions to string/array passed" << endl;
        return 0;
    }
}

int crossingTurnTest() {
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    Knot knot(knotString);
    
    return knot.crossingTurnTest();
}

int findStrandsOfLengthTest() {
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    Knot knot(knotString);
    
    return knot.findStrandsOfLengthTest();
}

int executeTM2Test() {
    
    string knotStringExt = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    string expectedResultExt = "[1, -2, 3, -4, 5, -6, 7, -1, 8, 10, -11, 12, -13, -20, -14, -5, 15, -3, 17, -18, 9, 19, 20, 14, 21, -17, 22, -8, 23, -7, 2, -22, 18, -9, -10, 24, -25, 26, 6, -15, 4, -21, -19, 27, -12, 28, -26, 29, -24, 11, -27, 13, -28, 25, -29, -23]";
    
    Knot knot(knotStringExt);
    
    knot.TM();
    
    if (knot.toGaussString() != expectedResultExt) {
        cout << "Executing tm2 FAILED" << endl;
        cout << "Expected Result: " << endl << expectedResultExt << endl << "(length " << expectedResultExt.length() << ")" << endl;
        cout << "Actual Result: " << endl << knot.toExtGaussString() << endl << "(length " << knot.toExtGaussString().length() << ")" << endl;
        return -1;
    }
    else {
        cout << "Executing tm2 test passed" << endl;
        cout << "Result: " << endl << knot.toExtGaussString() << endl;
        return 0;
    }
}
