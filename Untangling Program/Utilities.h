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

#ifndef Utilities_h
#define Utilities_h

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

int getSign(int num);

int* getInput(string knotString, int &knotLength);

template <class type>
int index_of(type array[], int sizeOfArray, type item) {
    for (int i=0; i<sizeOfArray; i++) {
        if (array[i] == item) {
            return i;
        }
    }
    return -1;
}

int index_of_abs(int * array, int sizeOfArray, int item);

void swap(int * array, int idx1, int idx2);

template <typename vectorType>
bool isInVector(vector<vectorType>& V, vectorType item) {
    return (find(V.begin(), V.end(), item) != V.end());
}

#endif /* Utilities_h */
