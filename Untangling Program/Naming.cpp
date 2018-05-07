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


void flip(int extGauss[], int knotLength) {
    for (int i=0; i<knotLength; i++) {
        extGauss[i]=-extGauss[i];
    }
}

void reNumber(int extGauss[], int knotLength) {
    int* temp = new int[knotLength];
    int crossingNumIterator = 1;
    int negativeIndex;
    
    for (int i=0; i<knotLength; i++) {
        if (extGauss[i] == 0)
            continue;
        
        
        temp[i] = extGauss[i]; //Put first half of crossing into temp (not reduced yet)
        extGauss[i] = 0; //So index_of_abs finds the other half of the crossing, not this one
        negativeIndex = index_of_abs(extGauss, knotLength, temp[i]); //Find the other half of the crossing
        
        temp[i] = getSign(temp[i]) * crossingNumIterator; //Set 1st reduced num
        temp[negativeIndex] = getSign(extGauss[negativeIndex]) * crossingNumIterator; //Set 2nd reduced num
        extGauss[negativeIndex] = 0; //So we skip over the other half of the crossing when we hit it
        
        crossingNumIterator++;
    }
    
    //Put values of temp back into extGauss
    for (int i=0; i<knotLength; i++)
        extGauss[i] = temp[i];
    delete[] temp;
}

void Name(int extGauss[], int knotLength) {
    if (extGauss[0]<0)
        flip(extGauss, knotLength);
    reNumber(extGauss, knotLength);
    cout << "Renamed: [";
    for (int i=0; i<knotLength; i++) {
        cout << extGauss[i] <<((i==knotLength-1) ? ("") : (", "));
    }
    cout << "]" << endl;
    
}
