//
//  main.cpp
//  Untangling Program
//
//  Created by Dana Foley on 9/14/16.
//  Copyright © 2016 Dana Foley. All rights reserved.
//


#include "Untangle.h"



int main() {
    int knotLength=0;
    
    string knotString = "1, -2, 3, -4, 5, -6, 7, 1, 8, -9, 10, -11, 12, -13, -14, -5, 15, 3, 17, -18, -9, 19, 20, 14, 21, 17, 22, 8, 23, -7, -2, -22, 18, 10, 24, -25, 26, 20, 6, -15, -4, -21, 27, 12, 28, 26, 29, -24, 11, -27, -13, 28, -25, -29, 19, 23";
    
    int * extGauss = getInput(knotString, knotLength);
    
    Knot knot(extGauss,knotLength);
    
    cout << "Knot after being created: " << knot << endl << endl;
    
    Untangle(knot);
    
    cout << "Knot after being untangled: " << knot << endl << endl;
    
    int * finalKnotArray = knot.toArray();
    knotLength = knot.size()*2;
    
    for (int idx=0; idx<knotLength; idx++)
        cout << finalKnotArray[idx] << " ";
    cout << endl;
    
    Name(finalKnotArray, knotLength);
    
    return 0;
}
