//
//  Naming.cpp
//  Untangling Program
//
//  Created by Dana Foley on 10/25/16.
//  Copyright © 2016 Brian Foley. All rights reserved.
//

#include "Naming.h"


void flip(int extGauss[], int knotLength) {
    for (int i=0; i<knotLength; i++) {
        extGauss[i]=-extGauss[i];
    }
}

void reNumber(int extGauss[], int knotLength) {
    int temp[knotLength];
    int crossingNumIterator = 1;
    int negativeIndex;
    
    for (int i=0; i<knotLength; i++) {
        if (extGauss[i] == 0)
            continue;
        
        
        temp[i] = extGauss[i]; //Put first half of crossing into temp (not reduced yet
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
