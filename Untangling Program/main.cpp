//
//  main.cpp
//  Untangling Program
//
//  Created by Dana Foley on 9/14/16.
//  Copyright © 2016 Dana Foley. All rights reserved.
//


#include "Untangle.h"


int* getInput(int &knotLength) {
    int * extGauss = new int[1];
    string knotString;
    cout << "Enter knot to untangle: ";
    getline(cin,knotString);
    
    
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



int main() {
    int knotLength=0;
    
    int * extGauss = getInput(knotLength);

//    for (int idx=0; idx<knotLength; idx++)
//        cout << extGauss[idx] << " ";
//    cout << endl;
    
    Knot knot(extGauss,knotLength);
    cout << "Knot after being created: " << knot << endl;
    
    Untangle(knot);
    
    cout << "Knot after being untangled: " << knot << endl;
    
    return 0;
}
