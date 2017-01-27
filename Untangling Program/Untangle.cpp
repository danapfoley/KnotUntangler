


#include "Untangle.h"


int Untangle(Knot &knot) {
    
    int origSize = knot.size();
    
    while (knot.size()!=0) {
        if (knot.rm2()) {
            cout << "RM2 performed: " << knot << endl;
            continue;
        }
        if (knot.rm1()) {
            cout << "RM1 performed: " << knot << endl;
            continue;
        }
        //Translation stuff
        
        if (knot.tm2()) {
            continue;
        }
        
        break;
    }
    
    if (knot.size() < origSize)
        return 1;
    else
        return 0;
    
}
