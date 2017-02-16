//
//  Utilities.h
//  Untangling Program
//
//  Created by Dana Foley on 10/16/16.
//  Copyright © 2016 Brian Foley. All rights reserved.
//

#ifndef Utilities_h
#define Utilities_h

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>


using namespace std;

int getSign(int num);

int* getInput(string knotString, int &knotLength);

template <class type>
int index_of(type array[], int sizeOfArray, type item) {
    int index = -1;
    for (int i=0; i<sizeOfArray; i++) {
        if (array[i] == item) {
            index = i;
            break;
        }
    }
    return index;
}

int index_of_abs(int * array, int sizeOfArray, int item);

void swap(int * array, int idx1, int idx2);
#endif /* Utilities_h */
