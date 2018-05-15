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

#include "Naming.h"


void flip(vector<int>& extGauss) {
    for (int &item : extGauss) {
        item = -item;
    }
}

void reNumber(vector<int>& extGauss) {
    vector<int> temp;
    int crossingNumIterator = 1;
    int negativeIndex;
    
    for (int i=0; i<extGauss.size(); i++) {
        if (extGauss[i] == 0)
            continue;
        
        
        temp[i] = extGauss.at(i); //Put first half of crossing into temp (not reduced yet)
        extGauss[i] = 0; //So index_of_abs finds the other half of the crossing, not this one
        negativeIndex = index_of_abs(extGauss, temp.at(i)); //Find the other half of the crossing
        
        temp[i] = getSign(temp.at(i)) * crossingNumIterator; //Set 1st reduced num
        temp[negativeIndex] = getSign(extGauss.at(negativeIndex)) * crossingNumIterator; //Set 2nd reduced num
        extGauss[negativeIndex] = 0; //So we skip over the other half of the crossing when we hit it
        
        crossingNumIterator++;
    }
    
    //Put values of temp back into extGauss
    for (int i=0; i<extGauss.size(); i++)
        extGauss[i] = temp.at(i);
    temp.clear();
}

void Name(vector<int> extGauss) {
    if (extGauss[0]<0)
        flip(extGauss);
    reNumber(extGauss);
    cout << "Renamed: [";
    for (int i=0; i<extGauss.size(); i++) {
        cout << extGauss[i] <<((i==extGauss.size()-1) ? ("") : (", "));
    }
    cout << "]" << endl;
    
}
