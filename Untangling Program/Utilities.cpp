//
//  Utilities.cpp
//  Untangling Program
//
//  Created by Dana Foley on 10/16/16.
//  Copyright © 2016 Brian Foley. All rights reserved.
//


#include "Utilities.h"


//Returns the sign of a number as 1 or -1 for positive or negative numbers respectively
int getSign(int num) {
    return abs(num)/num;
}

int* getInput(string knotString, int &knotLength) {
    int * extGauss = new int[1];
    //cout << "Enter knot to untangle: ";
    //getline(cin,knotString);
    
    
    if (knotString[0]=='[') {
        knotString.erase(0,1);
        knotString.erase(knotString.length()-1,knotString.length());
    }
    
    for (int idx=0; idx<knotString.length(); idx++)
        if (knotString[idx]==',')
            knotString.erase(idx, 1);
    
    stringstream knotStream(knotString);
    
    int tempNum;
    while (knotStream >> tempNum) {
        knotLength++;
        int * tempList = new int[knotLength];
        for (int idx=0; idx<knotLength-1; idx++) {
            tempList[idx]=extGauss[idx];
        }
        tempList[knotLength-1]=tempNum;
        delete [] extGauss;
        extGauss=tempList;
    }
    return extGauss;
}

int index_of(int array[], int sizeOfArray, int item) {
    int index = -1;
    
    for (int i=0; i<sizeOfArray; i++) {
        if (array[i] == item) {
            index = i;
            break;
        }
    }
    
    return index;
}

int index_of_abs(int array[], int sizeOfArray, int item) {
    int index = -1;
    
    for (int i=0; i<sizeOfArray; i++) {
        if (abs(array[i]) == abs(item)) {
            index = i;
            break;
        }
    }
    
    return index;
}



