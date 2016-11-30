


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
}

string Knot::toGaussString() const{
    Crossing * ptr = start;
    string outputString = "[";
    
    int counter = 0;
    
    if (ptr!=nullptr) {
        do {
            outputString += to_string((ptr->num)*(ptr->sign));
            outputString += (ptr->next!=start) ? (", ") : ("");
            ptr = ptr->next;
            
            counter++;
            if (counter > 60) break;
            
        } while (ptr!=start);
    }
    
    outputString += "]";
    
    return outputString;
}

string Knot::toExtGaussString() const{
    Crossing * ptrA = start;
    Crossing * ptrB;
    bool secondInst = false;
    string outputString = "[";
    int counter = 0;
    
    if (ptrA!=nullptr) {
        do {
            
            //Trace backward and see if we're on the second inst of a num
            //If so, we need to display its handedness, not its sign
            ptrB = ptrA;
            secondInst=false;
            while (ptrB!=start) {
                //Move backward
                ptrB = ptrB->prev;
                if (ptrB->num == ptrA->num) {
                    secondInst=true;
                    break;
                }
            }
            
            //If it was the second instance, multiply by handedness +1/-1
            //If not, multiply by sign +1/-1
            if (secondInst)
                outputString += to_string((ptrA->num) * (ptrA->handedness));
            else
                outputString += to_string((ptrA->num) * (ptrA->sign));
            outputString += (ptrA->next!=start) ? (", ") : ("");
            
            //cout << to_string((ptrA->num) * (ptrA->handedness)) << endl;
            ptrA = ptrA->next;
            
            counter++;
            if (counter > 60) break;
            
        } while (ptrA!=start);
    }
    
    outputString += "]";
    
    return outputString;
}

int * Knot::toArray() const{
    int knotLength = 0;
    int * extGauss = getInput(this->toExtGaussString(), knotLength);
    return extGauss;
}

void Knot::display(ostream & out) const{
    string outputString = this->toGaussString();
    //string outputString = this->toExtGaussString();
    out << outputString;
}

ostream& operator<<(ostream& out, const Knot &myKnot) {
    myKnot.display(out);
    return out;
}

Knot::Crossing * Knot::find(int numToFind, int signOfNum) {
    Crossing * nPtr = start;
    
    signOfNum = getSign(signOfNum); //Just in case the user enters something other than 1 or -1
    
    do {
        if (nPtr->num == numToFind and nPtr->sign == signOfNum) {
            return nPtr;
        }
        nPtr = nPtr->next;
    } while (nPtr != start);
    
    return nullptr;
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
    int * extGauss = origKnot.toArray();
    int knotLength = origKnot.mySize*2;
    
    //*this = Knot(extGauss, knotLength);
    
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


Knot::Crossing* Knot::Crossing::advance(int direction) {
    return (direction > 0) ? (this->next) : (this->prev);
}

Knot::Crossing* Knot::Crossing::recede(int direction) {
    return (direction > 0) ? (this->prev) : (this->next);
}

Knot::Crossing* Knot::Crossing::turnLeft(int direction) {
    return (this->neg)->advance(direction * (this->handedness) * (this->sign));
}

Knot::Crossing* Knot::Crossing::turnRight(int direction) {
    return (this->neg)->advance(direction * (this->handedness) * (this->sign) * -1);
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

void Knot::tm2() {
    //Knot workingKnot = *this;

    int maxStrandLength = getLongestStrandLength();
    
    Crossing** strandArray = new Crossing*[mySize];
    for (int i = 0; i < mySize; i++) {
        strandArray[i] = nullptr;
    }
    
    for (int strandLength = maxStrandLength; strandLength >= 2; strandLength--) {
        findStrandsOfLength(strandLength, strandArray);
        
        cout << "Length: " << strandLength << endl;
        int idx = 0;
        while (strandArray[idx]!=nullptr) {
            cout << "strandArray[" << idx << "]: " << strandArray[idx]->num << endl;
            turnTrace(strandArray[idx], strandLength, 1);
            turnTrace(strandArray[idx], strandLength, -1);
            
            idx++;
        }
    }
    
    
    delete [] strandArray;
}

void Knot::turnTrace(Knot::Crossing* strandPtr, int strandLength, int side) {
    //Holds all crossings in one strand
    Crossing** strandArray = new Crossing*[strandLength];
    
    for (int i = 0; i < strandLength; i++) {
        strandArray[i] = strandPtr;
        strandPtr = strandPtr->next;
    }
    
    
    
    
    delete [] strandArray;
}

void Knot::findStrandsOfLength(int length, Crossing** crossingPtrArray) {
    int idx = 0;
    Crossing * ptrA = start;
    Crossing * ptrB;
    bool isStrand;
    
    do {
        ptrB = ptrA;
        isStrand = true;
        
        for (int i = 0; i<length-1; i++) {
            if (ptrB->sign == ptrB->next->sign) {
                ptrB = ptrB->next;
            }
            else {
                isStrand = false;
                break;
            }
        }
        
        if (isStrand) {
            crossingPtrArray[idx] = ptrA;
            idx++;
        }
        
        ptrA = ptrA->next;
    } while (ptrA != start);
}

//void Knot::tm2() {
//    //Knot workingKnot = *this;
//    
//    int maxStrandLength = getLongestStrandLength();
//    
//    Crossing** crossingPointerArray = new Crossing*[maxStrandLength];
//    int * directionArray = new int[maxStrandLength];
//    int * strandPositionArray = new int[maxStrandLength];
//    
//    
//    
////    for (int currentStrandLength = maxStrandLength; currentStrandLength >= 2; currentStrandLength--) {
////
////    }
//    
//    
//    //This bit is all based on what I know about where the desired tangle is
//    crossingPointerArray[0] = this->find(19, 1);
//    crossingPointerArray[1] = this->find(20, 1);
//    crossingPointerArray[2] = this->find(14, 1);
//    
//    directionArray[0] = -1;
//    directionArray[1] = -1;
//    directionArray[2] = -1;
//    
//    strandPositionArray[0]=2; //19 is at the 0th place before the move, and 2nd after
//    strandPositionArray[1]=0; //20 is at the 1st place before the move, and 1st after
//    strandPositionArray[2]=1; //14 is at the 2nd place before the move, and 0th after
//    //We need to move all of these back later, in attemptMove2
//    
//    
//    
//    
//    if (attemptMove2(3, crossingPointerArray, directionArray, strandPositionArray)) {
//        cout << "attemptMove2 ran with a result of TRUE" << endl;
//    }
//    
//    
//    
//    
//    delete [] crossingPointerArray;
//    delete [] directionArray;
//    delete [] strandPositionArray;
//}
//
//bool Knot::attemptMove2(int strandLength, Crossing ** crossingPointerArray, int * directionArray, int * strandPositionArray) {
//    
//    int * numToMove = new int[strandLength];
//    numToMove[0] = 12;
//    numToMove[1] = 3;
//    numToMove[2] = 3;
//    
//    //Move each crossing of the strand over the appropriate number of crossings
//    for (int i = 0; i < strandLength; i++) {
//        Crossing * nPtr = crossingPointerArray[i]->neg;
//        
//        for (int j = 0; j < numToMove[i]; j++) {
//            nPtr = (directionArray[i] == 1) ? (nPtr->next) : (nPtr->prev);
//        }
//        
//        //Unlink the crossing from its current location, freeing up the crossings around it
//        crossingPointerArray[i]->neg->prev->next = crossingPointerArray[i]->neg->next;
//        crossingPointerArray[i]->neg->next->prev = crossingPointerArray[i]->neg->prev;
//        
//        //Put the crossing in its new (not final) location
//        if (directionArray[i] == 1) { //FORWARD
//            crossingPointerArray[i]->neg->next = nPtr->next;
//            crossingPointerArray[i]->neg->prev = nPtr;
//            
//            nPtr->next->prev = crossingPointerArray[i]->neg;
//            nPtr->next = crossingPointerArray[i]->neg;
//        }
//        if (directionArray[i] == -1) { //BACKWARD
//            
//            crossingPointerArray[i]->neg->prev = nPtr->prev;
//            crossingPointerArray[i]->neg->next = nPtr;
//            
//            nPtr->prev->next = crossingPointerArray[i]->neg;
//            nPtr->prev = crossingPointerArray[i]->neg;
//        }
//    }
//    
//    //The strands have changed order, so in order to properly construct the knot,
//        //we need to swap some crossings around
//    Crossing * tempCrossingPtr;
//    
//    for (int i = 0; i < strandLength; i++) {
//        int idx1 = i;
//        int idx2 = strandPositionArray[i];
//        
//        if (idx1 != idx2) { //If the crossing is not in its correct place
//            
//            //Swap prev's and next's pointers back to the original crossings
//            crossingPointerArray[idx1]->neg->next->prev = crossingPointerArray[idx2]->neg;
//            crossingPointerArray[idx1]->neg->prev->next = crossingPointerArray[idx2]->neg;
//            
//            crossingPointerArray[idx2]->neg->next->prev = crossingPointerArray[idx1]->neg;
//            crossingPointerArray[idx2]->neg->prev->next = crossingPointerArray[idx1]->neg;
//            
//            //Swap prev pointers
//            tempCrossingPtr = crossingPointerArray[idx1]->neg->prev;
//            crossingPointerArray[idx1]->neg->prev = crossingPointerArray[idx2]->neg->prev;
//            crossingPointerArray[idx2]->neg->prev = tempCrossingPtr;
//            
//            //Swap next pointers
//            tempCrossingPtr = crossingPointerArray[idx1]->neg->next;
//            crossingPointerArray[idx1]->neg->next = crossingPointerArray[idx2]->neg->next;
//            crossingPointerArray[idx2]->neg->next = tempCrossingPtr;
//            
//            
//            
//            //indicate in the array that we've made the swap
//            swap(strandPositionArray, i, strandPositionArray[i]);
//        }
//        
//        //If we need to go back and swap more stuff, reset the loop var i
//        if (i == strandLength -1)
//            for (int j = 0; j < strandLength; j++)
//                if (strandPositionArray[j] != j) {
//                    i = 0;
//                    continue;
//                }
//    }
//    
//    
//    delete [] numToMove;
//    return true;
//}




int Knot::getLongestStrandLength() {
    Crossing * ptrA = this->start;
    Crossing * ptrB;
    
    int maxLength = 1; //Max length of a continuous over/underpass segment
    int currentLength; //Current length of    "           "           "
    
    do {
        currentLength = 1;
        ptrB = ptrA->next;
        while (ptrA->sign == ptrB->sign) {
            currentLength++;
            ptrB = ptrB->next;
        }
        
        if (currentLength > maxLength)
            maxLength = currentLength;
        
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    cout << "Max Length: " << maxLength << endl;
    
    return maxLength;
}

int Knot::crossingTurnTest() {
    if (this->find(19, -1)->turnLeft(-1)->num != 20) {
        cout << "Crossing turn test FAILED" << endl;
        return -1;
    }
    
    cout << "Crossing turn test passed" << endl;
    return 0;
}

int Knot::findStrandsOfLengthTest() {
    Crossing* array[10];
    
    for (int i = 0; i<10; i++) {
        array[i]=nullptr;
    }
    
    findStrandsOfLength(3, array);
    
    for (int i = 0; i<10; i++) {
        if (array[i] != nullptr)
            cout << "array[" << i << "] = " << array[i]->num << endl;
        else
            cout << "array[" << i << "] = " << array[i] << endl;
    }
    
    if (array[0] == nullptr or array[0]->num != 13) {
        cout << "Find strands of length test FAILED"<< endl;
        return -1;
    }
    cout << "Find strands of length test passed" << endl;
    return 0;
}




