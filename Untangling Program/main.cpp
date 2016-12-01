//
//  main.cpp
//  Untangling Program
//
//  Created by Dana Foley on 9/14/16.
//  Copyright © 2016 Dana Foley. All rights reserved.
//


#include "Untangle.h"

void runAllTests();
int rm1Test();
int rm2Test();
int conversionTest();
int crossingTurnTest();
int findStrandsOfLengthTest();
//int executeTM2Test();

int main() {
    int knotLength=0;
    
    string knotString = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss,knotLength);
    
    cout << "Knot after being created: " << knot << endl << endl;
    
    Untangle(knot);
    
    cout << "Knot after being untangled: " << knot << endl << endl;
    
    
    //Get array from final knot for naming program
    int * finalKnotArray = knot.toArray();
    knotLength = knot.size()*2; //size returns the number of crossings, and we need twice that for arrays
    Name(finalKnotArray, knotLength);
    
    
    //runAllTests();
    
    
    return 0;
}

void runAllTests() {
    int numTestsFailed = 0
    
    //+ rm1Test()
    //+ rm2Test()
    //+ conversionTest()
    +crossingTurnTest()
    +findStrandsOfLengthTest();
    
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

//int executeTM2Test() {
//    int knotLength=0;
//    
//    string knotStringExt = "[1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23]";
//    
//    string expectedResultNonExt = "[1, -2, 3, -4, 5, -6, 7, -1, 8, -9, 10, -20, -11, 12, -13, -5, 15, -3, 17, -18, 9, 19, 20, 14, 21, -17, 22, -8, 23, -7, 2, -22, 18, -10, -19, 24, -25, 26, 6, -15, 4, -21, -14, 27, -12, 28, -26, 29, -24, 11, -27, 13, -28, 25, -29, -23]";
//    
//    int * extGauss = getInput(knotStringExt, knotLength);
//    
//    Knot knot(extGauss,knotLength);
//    
//    knot.tm2();
//    
//    if (knot.toGaussString() != expectedResultNonExt) {
//        cout << "Executing tm2 FAILED" << endl;
//        cout << "Expected Result: " << endl << expectedResultNonExt << endl;
//        cout << "Actual Result: " << endl << knot.toGaussString() << endl;
//        return -1;
//    }
//    else {
//        cout << "Executing tm2 test passed" << endl;
//        cout << "Result: " << endl << knot.toGaussString() << endl;
//        return 0;
//    }
//}

