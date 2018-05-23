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
vector<int> getInput(string knotString) {
    vector<int> extGauss; 
    //cout << "Enter knot to untangle: ";
    //getline(cin,knotString);
    
    
    if (knotString[0]=='[') {
        knotString.erase(0,1);
    }
    if (knotString[knotString.length() - 1] == ']') {
        knotString.erase(knotString.length() - 1, 1);
    }
    
    replace(knotString.begin(), knotString.end(), ',', ' ');
    
    stringstream knotStream(knotString);
    
    int tempNum;
    while (knotStream >> tempNum) {
        extGauss.push_back(tempNum);
    }
    return extGauss;
}

//index_of is in the header file

//Find the index of an item in an array whose abs. value matches
    //the abs. value of the item to find
int index_of_abs(vector<int> &vec, int item) {
    
    for (int i=0; i<vec.size(); i++) {
        if (abs(vec.at(i)) == abs(item)) {
            return i;
        }
    }
    
    return -1;
}

//Simple swap for two items in an array
void swap(vector<int>& vec, int idx1, int idx2) {
    vec[idx1] = vec[idx1] + vec[idx2];
    vec[idx2] = vec[idx1] - vec[idx2];
    vec[idx1] = vec[idx1] - vec[idx2];
}



