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

int index_of(int * array, int sizeOfArray, int item);

int index_of_abs(int * array, int sizeOfArray, int item);

void swap(int * array, int idx1, int idx2);
#endif /* Utilities_h */
