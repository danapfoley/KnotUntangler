//
//  main.cpp
//  Untangling Program
//
//  Created by Dana Foley on 9/14/16.
//  Copyright © 2016 Dana Foley. All rights reserved.
//


#include "Knot.h"


//Set to 1 to print out basic debug info during execution
#define Debug 1

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
        
        if (knot.tm2()) {
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




int main() {
    
    dPrint(cout << "Debug mode ON" << endl;)
    
    //SuperCulprit
    //string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    //Reduced version of SuperCulprit
    //string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, 10, -11, 12, -13, -5, 15, 3, 17, -18, 9, 17, 22, 8, 23, -7, -2, -22, 18, 9, 10, 24, -25, 26, 6, -15, -4, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 23]";
    
    
    //Twisted Trefoils
    string knotString = "[1, -2, 3, 1, 2, 3, 4, -5, 6, -7, 5, 6, 7, 4]";
    
    
    cout << "Knot: " << knotString << endl;
    
    Knot knot(knotString);
    
    dPrint(cout << "Knot object after being created: " << knot << endl << endl;)
    
    Untangle(knot);
    
    cout << "Knot after being untangled: " << knot << endl << endl;
    
//    
//    //Get array from final knot for naming program
//    int * finalKnotArray = knot.toArray();
//    int knotLength = knot.size()*2; //size returns the number of crossings, and we need twice that for arrays
//    Name(finalKnotArray, knotLength);
//    
    
    test(runAllTests();)
    
    
    return 0;
}

void runAllTests() {
    int numTestsFailed = 0
    
    + rm1Test()
    + rm2Test()
    + conversionTest()
    + crossingTurnTest()
    + findStrandsOfLengthTest()
    + executeTM2Test();
    
    cout << "Total tests failed: " << -1 * numTestsFailed << endl;
}

int rm1Test() {
    int knotLength = 0;
    
    string knotString = "[1, -2, 3, 4, 4, 1, 2, 3]";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss, knotLength);
    
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
    int knotLength = 0;
    
    string knotString = "[1, 5, 4, -2, 3, 4, -5, 1, 2, 3]";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss, knotLength);
    
    if (knot.rm2()) {
        cout << "RM2 test passed" << endl;
        return 0;
    }
    else {
        cout << "RM2 test FAILED" << endl;
        return -1;
    }
    
}

int conversionTest() {
    int knotLength=0;
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss,knotLength);
    
    if (knot.toExtGaussString() != knotString) {
        cout << "Conversion to string FAILED" << endl;
        return -1;
    }
    
    Knot knot2(knot.toArray(), knotLength);
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
    int knotLength = 0;
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss, knotLength);
    
    return knot.crossingTurnTest();
}

int findStrandsOfLengthTest() {
    int knotLength = 0;
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss, knotLength);
    
    return knot.findStrandsOfLengthTest();
}

int executeTM2Test() {
    
    string knotStringExt = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    string expectedResultNonExt = "[1, -2, 3, -4, 5, -6, 7, -1, 8, -9, 10, -20, -11, 12, -13, -5, 15, -3, 17, -18, 9, 19, 20, 14, 21, -17, 22, -8, 23, -7, 2, -22, 18, -10, -19, 24, -25, 26, 6, -15, 4, -21, -14, 27, -12, 28, -26, 29, -24, 11, -27, 13, -28, 25, -29, -23]";
    
    Knot knot(knotStringExt);
    
    knot.tm2();
    
    if (knot.toGaussString() != expectedResultNonExt) {
        cout << "Executing tm2 FAILED" << endl;
        cout << "Expected Result: " << endl << expectedResultNonExt << endl;
        cout << "Actual Result: " << endl << knot.toGaussString() << endl;
        return -1;
    }
    else {
        cout << "Executing tm2 test passed" << endl;
        cout << "Result: " << endl << knot.toGaussString() << endl;
        return 0;
    }
}
