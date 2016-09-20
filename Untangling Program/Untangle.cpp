


#include "Untangle.h"


void Untangle(Knot &knot) {
    while (knot.mySize!=0) {
        if (knot.rm2()) {
            cout << "RM2 performed: " << knot << endl;
            continue;
        }
        if (knot.rm1()) {
            cout << "RM1 performed: " << knot << endl;
            continue;
        }
        //Translation stuff
        
        
        break;
    }
    
}