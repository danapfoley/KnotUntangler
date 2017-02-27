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


#include "Utilities.h"




//Returns the sign of a number as 1 or -1 for positive or negative numbers respectively
int getSign(int num) {
    return abs(num)/num;
}

//Turns a knot string into an array for further processing
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

//index_of is in the header file

//Find the index of an item in an array whose abs. value matches
    //the abs. value of the item to find
int index_of_abs(int array[], int sizeOfArray, int item) {
    
    for (int i=0; i<sizeOfArray; i++) {
        if (abs(array[i]) == abs(item)) {
            return i;
        }
    }
    
    return -1;
}

//Simple swap for two items in an array
void swap(int array[], int idx1, int idx2) {
    array[idx1] = array[idx1] + array[idx2];
    array[idx2] = array[idx1] - array[idx2];
    array[idx1] = array[idx1] - array[idx2];

}



