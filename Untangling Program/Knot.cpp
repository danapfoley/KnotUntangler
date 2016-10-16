


#include "Knot.h"


//Construct a knot from an array of numbers
Knot::Knot(int extGauss[], int knotLength) {
    //Create new blank crossing to begin with

    start=new Crossing(abs(extGauss[0]));
    //Point to that blank crossing
    Crossing * nPtr=start;
    
    //Establish num, next, and prev for all crossings
    //and link them together
    for (int idx=1; idx<knotLength; idx++) {
        nPtr->next=new Crossing(abs(extGauss[idx]));
        nPtr->next->prev=nPtr;
        nPtr=nPtr->next;
    }
    //Connect last node to start to complete the loop
    nPtr->next=start;
    nPtr->next->prev=nPtr;
    nPtr=nPtr->next;
    
    //nPtr is now at start again
    //The knot now has all next and prev data, and nums. Still no neg, sign, or handedness
    

    //Here we get neg data and mySize
    //For each crossing, we search through to find it's corresponding
    	//negative crossing, and point them at each other
    Crossing * nPtr2; //Reminder that nPtr is still at start and we use that here too
    for (int idx=0; idx<knotLength; idx++) {
        nPtr2=start;
        for (int idx2=0; idx2<knotLength; idx2++) {
            if (idx==idx2) //Don't point the neg of a crossing to itself
                continue;
            if (nPtr->num==nPtr2->num) {
                nPtr->neg=nPtr2;
                nPtr2->neg=nPtr;
                break;
            }
            nPtr2=nPtr2->next;
        }
        nPtr=nPtr->next;
        mySize++;
    }
    mySize=mySize/2;
    
    
    //And here we get sign and handedness data
    //The sign of a crossing is determined by 
    	//the pos/neg sign of the first instance of a number in an extended Gauss Code list
    //Similarly, the handedness is determined by
    	//the pos/neg sign of the second instance
    nPtr=start;
    for (int idx=0; idx<knotLength; idx++) {
        nPtr2=start;

        //If for some reason the sign is already established, move on
        if (nPtr->sign!=0) {
            nPtr=nPtr->next;
            continue;
        }


        for (int idx2=0; idx2<knotLength; idx2++) {
            if (idx==idx2) {
                nPtr2=nPtr2->next;
                continue;
            }
            else if (abs(extGauss[idx])==abs(extGauss[idx2])) {
                if (idx<idx2) {
                    nPtr->sign  =getSign(extGauss[idx]);
                    nPtr2->sign =-getSign(extGauss[idx]);
                    nPtr->handedness  = getSign(extGauss[idx2]);
                    nPtr2->handedness = getSign(extGauss[idx2]);
                    break;
                }
                else /* if (idx2<idx) */ {
                    nPtr->sign  =-getSign(extGauss[idx2]);
                    nPtr2->sign =getSign(extGauss[idx2]);
                    nPtr->handedness  = getSign(extGauss[idx]);
                    nPtr2->handedness = getSign(extGauss[idx]);
                    break;
                }
            }
            nPtr2=nPtr2->next;
        }
        nPtr=nPtr->next;
    }
}

Knot::Knot(const Knot &origKnot) {
    if (origKnot.mySize==0) start= nullptr;
    else start = new Crossing(0);
    
    mySize=origKnot.mySize;
    
    if (mySize!=0) {
        Crossing * nPtr = start;
        Crossing * origPtr = origKnot.start;
        for (int numTimes=0; numTimes<mySize; numTimes++) {
            
            //overloaded Crossing = operator
            *nPtr=*origPtr;
            
            if (origPtr->next!=nullptr) {
                nPtr->next= new Crossing(0);
            }
            nPtr=nPtr->next;
            origPtr=origPtr->next;
        }
    }
}

Knot::~Knot() {
    if (start!=nullptr) {
        start->prev->next=nullptr;
        Crossing * temp = start;
        while (temp!= nullptr) {
            start=start->next;
            delete temp;
            temp=start;
        }
    }
    if (start==nullptr) cout << "Knot destroyed" << endl;
    else cout << "Knot not destroyed" << endl;
}

void Knot::display(ostream & out) const{
    Crossing * ptr = start;
    out << "[";
    
    if (ptr!=nullptr) {
        do {
            out << (ptr->num)*(ptr->sign) << ( (ptr->next!=start) ? (", ") : (""));
            ptr = ptr->next;
        } while (ptr!=start);
    }
    
    out << "]";
}

ostream& operator<<(ostream& out, const Knot &myKnot) {
    myKnot.display(out);
    return out;
}


void Knot::insert(int index, int numValue) {
    Crossing * nPtr;
    nPtr = new Crossing(numValue);
    
    if (index==0) {
        nPtr->next=start;
        start=nPtr;
        mySize++;
    }
    else if(index>0 and index<=mySize) {
        Crossing * predPtr;
        predPtr = new(nothrow) Crossing(0);
        predPtr->next=start;
        for (int i=0; i<index; i++)
            predPtr = predPtr->next;
        nPtr->next=predPtr->next;
        predPtr->next = nPtr;
        mySize++;
    }
    
    else {cout << "Error: index out of range (" << index << ")" << endl;}
    
}

void Knot::erase(int index) {
    Crossing * ptrA = start;
    
    for (int i=0; i<index; i++)
        ptrA=ptrA->next;
    
    this->erase(ptrA);
    
}

void Knot::erase(Crossing * ptrA) {
    Crossing * ptrB=ptrA->neg;
    
    if (mySize==1) {
        delete ptrA;
        delete ptrB;
        start=nullptr;
        mySize--;
    }
    else {
        if (ptrA==start) {
            if (ptrA->next==ptrB)
                start=ptrB->next;
            else
                start=ptrA->next;
        }
        else if (ptrB==start) {
            if (ptrB->next==ptrA)
                start=ptrA->next;
            else
                start=ptrB->next;
        }
        
        if (ptrA->next==ptrB) {
            ptrA->prev->next=ptrB->next;
            ptrB->next->prev=ptrA->prev;
            delete ptrA;
            delete ptrB;
        }
        
        else if(ptrB->next==ptrA) {
            ptrB->prev->next=ptrA->next;
            ptrA->next->prev=ptrB->prev;
            delete ptrA;
            delete ptrB;
        }
        
        else {
            ptrA->prev->next=ptrA->next;
            ptrA->next->prev=ptrA->prev;
            
            ptrB->prev->next=ptrB->next;
            ptrB->next->prev=ptrB->prev;
            
            delete ptrA;
            delete ptrB;
        }
        
        mySize--;
    }
}

Knot & Knot::operator=(const Knot &origKnot) {
    if (this!=&origKnot) {
        
        this->~Knot();
        
        if (origKnot.mySize==0) start= 0;
        else start = new Crossing(0);
        
        mySize=origKnot.mySize;
        
        if (mySize!=0) {
            Crossing * nPtr = start;
            Crossing * origPtr = origKnot.start;
            for (int numTimes=0; numTimes<mySize; numTimes++) {
                nPtr->num = origPtr->num;
                if (origPtr->next!=0)
                    nPtr->next= new Crossing(0);
                nPtr=nPtr->next;
                origPtr=origPtr->next;
            }
        }
    }
    
    return *this;
}


Knot::Crossing::Crossing(const Crossing & origCrossing) {
    num=origCrossing.num;
    sign=origCrossing.sign;
    handedness=origCrossing.handedness;
    next=nullptr;
    prev=nullptr;
    neg=nullptr;
}


Knot::Crossing & Knot::Crossing::operator=(const Crossing &origCrossing) {
    if (this!=&origCrossing) {
        num=origCrossing.num;
        sign=origCrossing.sign;
        handedness=origCrossing.handedness;
    }
    
    next=nullptr;
    prev=nullptr;
    neg=nullptr;
    
    return *this;
}



bool Knot::rm1() {
    Crossing * ptrA=start;
    
    do {
        if (ptrA->num==ptrA->next->num) {
            erase(ptrA);
            return true;
        }
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}


bool Knot::rm2() {
    Crossing * ptrA=start;
    Crossing * ptrB;
    
    do {
        ptrB=ptrA->next;
        if (ptrA->sign==ptrB->sign and (ptrA->neg->next==ptrB->neg or ptrA->neg->prev==ptrB->neg)) {
            erase(ptrA);
            erase(ptrB);
            return true;
        }
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}





