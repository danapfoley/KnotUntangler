


#include "Knot.h"


//Construct a knot from an array of numbers
Knot::Knot(int extGauss[], int knotLength) {
    constructFromGauss(extGauss, knotLength);
}

Knot::Knot(const Knot &origKnot) {
    int * extGauss = origKnot.toArray();
    int knotLength = origKnot.mySize*2;
    
    constructFromGauss(extGauss, knotLength);
}

void Knot::constructFromGauss(int extGauss[], int knotLength) {
    deconstruct();
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
    deconstruct();
}

void Knot::deconstruct() {
    if (start != nullptr and mySize != 0) {
        start->prev->next=nullptr;
        Crossing * temp = start;
        while (temp!= nullptr) {
            start=start->next;
            delete temp;
            temp=start;
        }
    }
    mySize = 0;
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
    
    cout << "Couldn't find " << numToFind*signOfNum << endl;
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
    
    constructFromGauss(extGauss, knotLength);
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

Knot::Crossing* Knot::Crossing::turnLeft(int & direction) {
    direction = direction * (this->handedness) * (this->sign);
    return (this->neg)->advance(direction);
}

Knot::Crossing* Knot::Crossing::turnRight(int & direction) {
    direction = direction * (this->handedness) * (this->sign) * -1;
    return (this->neg)->advance(direction);
}

Knot::Crossing* Knot::Crossing::dummyTurnLeft(int direction) {
    direction = direction * (this->handedness) * (this->sign);
    return (this->neg)->advance(direction);
}

Knot::Crossing* Knot::Crossing::dummyTurnRight(int direction) {
    direction = direction * (this->handedness) * (this->sign) * -1;
    return (this->neg)->advance(direction);
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

bool Knot::dummyRM1() {
    Crossing * ptrA=start;
    
    do {
        if (ptrA->num==ptrA->next->num) {
            return true;
        }
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}

bool Knot::dummyRM2() {
    Crossing * ptrA=start;
    Crossing * ptrB;
    
    do {
        ptrB=ptrA->next;
        if (ptrA->sign==ptrB->sign and (ptrA->neg->next==ptrB->neg or ptrA->neg->prev==ptrB->neg))
            return true;
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}

bool Knot::tm2() {

    int maxStrandLength = getLongestStrandLength();
    
    int numStrandsForArray = 0;
    
    Crossing** strandArray = new Crossing*[mySize];
    
    for (int strandLength = maxStrandLength; strandLength >= 2; strandLength--) {
        for (int i = 0; i < mySize; i++) {
            strandArray[i] = nullptr;
        }
        findStrandsOfLength(strandLength, strandArray, numStrandsForArray);
        
        cout << "Length: " << strandLength << endl;
        
        for (int idx = 0; idx < numStrandsForArray; idx++) {
            cout << "strandArray[" << idx << "]: " << strandArray[idx]->num << endl;
            if (turnTrace(strandArray[idx], strandLength, 1))
                return true;
            //turnTrace(strandArray[idx], strandLength, -1);
        }
    }
    
    
    
//    int strandLength = 3;
//    findStrandsOfLength(strandLength, strandArray);
//    turnTrace(strandArray[2], strandLength, 1);
//    
//    
    delete [] strandArray;
    return false;
}

bool Knot::turnTrace(Knot::Crossing* strandPtr, int strandLength, int side) {
    
    Knot knotNoMovement(*this);
    //knotNoMovement = *this;
    
    //Holds all crossings in one strand
    Crossing** strandArray = new Crossing*[strandLength];
    for (int i = 0; i<strandLength; i++) {
        strandArray[i] = nullptr;
    }
    
    //Start and end crossings (where we trace from and to)
    Crossing* startCrossing;
    Crossing* endCrossing;
    
    //Total number of intersections on the far side of the tangle
    //Should be constrained to n (aka strandLength) for an n-tangle
    int numIntersections = 0;

    //Current direction being traced in.
    //This will get changed by the turn functions.
    int direction;
    
    
    
    //Reset strandPtr so it is pointing in the correct knot
    //We sometimes get issues with this since turnTrace throws out memory addresses
    strandPtr = find(strandPtr->num, strandPtr->sign);
    if (strandPtr == nullptr) {
        return false;
    }
    
    
    if (side == 1) {
        for (int i = strandLength - 1; i >= 0; i--) {
            strandArray[i] = strandPtr;
            strandPtr = strandPtr->next;
        }
        direction = -1;
    }
    
    else { //(side == -1)
        for (int i = 0; i < strandLength; i++) {
            strandArray[i] = strandPtr;
            strandPtr = strandPtr->next;
        }
        direction = 1;
    }
    
    startCrossing = strandArray[0];
    endCrossing = strandArray[strandLength-1];
    
    Crossing** pathArray = new Crossing*[mySize - strandLength];
    for (int i = 1; i < mySize - strandLength; i++) {
        pathArray[i]=nullptr;
    }
    int pathLength = 0;
    
    if (turnTraceHelper(startCrossing->turnLeft(direction), strandArray, strandLength, pathArray, pathLength, numIntersections, direction)) {
        cout << "Path Length: " << pathLength << endl;
        cout << "Path: ";
        for (int i = 0; i < pathLength; i++) {
            
            cout << pathArray[i]->num << ", ";
            
        }

        Crossing * tempPtr = startCrossing->dummyTurnLeft(direction);
        
        //This is a way to keep track of how we move crossings in the strand over the tangle
        //When we relocate a crossing, we increment/decrement strandIndex by 1
        
        //We initialized strandArray in a direction based on the side being used, so that is reflected here
        //The incrementation vs. decrementation of strandIndex is thus also dependent on side
        //int strandIndex = (side==1) ? (strandLength-1) : (0);
        int strandIndex = 0;
        
        //Move the strand over the tangle. Very important
        //Start by tracing through the path crossings and find intersections
        /*
        for (int i = 0; i <= pathLength - 2; i++) {
            
            
            
            //If we turned left at a crossing, nothing to do; move on
            if (tempPtr->dummyTurnLeft(direction) == pathArray[i+1]) {
                tempPtr = tempPtr->turnLeft(direction);
                continue;
            }
            
            //If we went straight or right, we have at least one crossing to relocate
            if (tempPtr->advance(direction) == pathArray[i+1]
                or tempPtr->dummyTurnRight(direction) == pathArray[i+1]) {
                
                //Detach old crossing location
                strandArray[strandIndex]->neg->prev->next = strandArray[strandIndex]->neg->next;
                strandArray[strandIndex]->neg->next->prev = strandArray[strandIndex]->neg->prev;
                
                
                Crossing* turnLeftCrossing = tempPtr->dummyTurnLeft(direction);
                
                //If turnLeftCrossing is facing into tempPtr (thus the "next")
                if (turnLeftCrossing->next == tempPtr->neg) {
                    
                    turnLeftCrossing->next->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = turnLeftCrossing->next;
                    
                    turnLeftCrossing->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = turnLeftCrossing;
                }
                //If turnLeftCrossing is facing away from tempPtr
                else { //if (turnLeftCrossing->prev == tempPtr->neg)
                    
                    turnLeftCrossing->prev->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = turnLeftCrossing->prev;
                    
                    turnLeftCrossing->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = turnLeftCrossing;
                }
                
                strandIndex++;
            }
            
            //If we went straight, move on
            if (tempPtr->advance(direction) == pathArray[i+1]) {
                tempPtr = tempPtr->advance(direction);
                continue;
            }
            
            //If we went right, we have one more crossing to relocate
            if (tempPtr->dummyTurnRight(direction) == pathArray[i+1]) {
                
                //Detach old crossing location
                strandArray[strandIndex]->neg->prev->next = strandArray[strandIndex]->neg->next;
                strandArray[strandIndex]->neg->next->prev = strandArray[strandIndex]->neg->prev;
                
                Crossing* goStraightCrossing = tempPtr->advance(direction);
                //If goStraightCrossing is facing into tempPtr (thus the "next")
                if (goStraightCrossing->next == tempPtr->neg) {
                    
                    goStraightCrossing->next->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = goStraightCrossing->next;
                    
                    goStraightCrossing->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = goStraightCrossing;
                }
                //If goStraightCrossing is facing away from tempPtr
                else { //if (goStraightCrossing->prev == tempPtr->neg)
                    
                    goStraightCrossing->prev->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = goStraightCrossing->prev;
                    
                    goStraightCrossing->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = goStraightCrossing;
                }
                
                tempPtr = tempPtr->turnRight(direction);
                
                strandIndex++;
                
            }
            
        }
        */
        
        cout << endl;
    }
    
    delete [] strandArray;
    delete [] pathArray;
    
    if (dummyRM1() or dummyRM2()) {
        cout << "TM2 performed: " << *this << endl;
        return true;
    }
    else {
        cout << "TM2 failed: " << *this << endl;
        //*this = knotNoMovement;
        return false;
    }
    
    
    
}

bool Knot::turnTraceHelper(Crossing* currentCrossing, Crossing** strandArray, int strandLength, Crossing** pathArray, int& pathLength, int numIntersections, int direction) {
    
    //Used for preventing direction from being affected by turn functions
    int dummyDirection;
    
    Crossing* endCrossing = strandArray[strandLength-1];
    
    
    //currentCrossing is pointing to one of the crossing completions of the strand
    bool boolInStrand = index_of(strandArray, strandLength, currentCrossing->neg) > -1;
    
    //currentCrossing has hit the rightmost strand endpoint
    bool boolHitRightEndpt = currentCrossing == endCrossing;
    
    //currentCrossing has hit the leftmost strand endpoint
    bool boolHitLeftEndpt = index_of(strandArray, strandLength, currentCrossing) == 0;
    
    //currentCrossing is at endCrossing
    bool boolAtEnd = currentCrossing->neg == endCrossing;
    
    //currentCrossing approached the end crossing from the proper direction
    dummyDirection = direction;
    bool boolRightDir = index_of(strandArray, strandLength, currentCrossing->turnRight(dummyDirection)) > -1;
    
    //currentCrossing hit a crossing it has already encountered
    bool boolLoopedBack = index_of(pathArray, pathLength, currentCrossing) > -1 or index_of(pathArray, pathLength, currentCrossing->neg) > -1;
    
    //correct number of intersections
    bool boolRightNumIntsx = numIntersections == strandLength;
    
    
    
    //If we hit a stop condition
    if ((boolInStrand and !boolAtEnd) or (boolAtEnd and !boolRightDir) or (boolHitRightEndpt) or (boolLoopedBack)) {
        //cout << "Returned false at A" << endl;
        return false;
    }
    
    //If we hit a complete halt condition
    if (boolHitLeftEndpt) {
        //cout << "Returned false at B" << endl;
        return false;
    }
    
    //If we:
    //1. Ended up at the proper end crossing
    //2. Encountered the end crossing from the right direction
    //2. Crossed the right number of strands along the way
    //Then return success
    if (boolAtEnd and boolRightDir and boolRightNumIntsx) {
        cout << "Found tangle" << endl;
        return true;
    }
    
    //Mark current crossing as part of solution path
    pathArray[pathLength] = currentCrossing;
    pathLength++;
    
    
//    string indentString = "";
//    for (int i = 0; i < pathLength; i++) {
//        indentString += "    ";
//    }
    
    
    
    //Try turning right
    dummyDirection = direction;
//    cout << indentString << "Trying to turn right at: " << currentCrossing->num << endl;
    if (numIntersections + 2 <= strandLength and turnTraceHelper(currentCrossing->turnRight(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections + 2, dummyDirection)) {
        
        
        return true;
    }
    //Try going straight
    dummyDirection = direction;
//    cout << indentString << "Trying to go straight at: " << currentCrossing->num << endl;
    if (numIntersections + 1 <= strandLength and turnTraceHelper(currentCrossing->advance(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections + 1, dummyDirection)) {
        
        return true;
    }
    //Try turning left
    dummyDirection = direction;
//    cout << indentString << "Trying to turn left at: " << currentCrossing->num << endl;
    if (numIntersections + 0 <= strandLength and turnTraceHelper(currentCrossing->turnLeft(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections + 0, dummyDirection)) {
        
        return true;
    }
    //Unmark current crossing as being part of solution path
    pathLength--;
    pathArray[pathLength]=0;
    return false;
}

void Knot::findStrandsOfLength(int length, Crossing** crossingPtrArray, int& arrayLength) {
    
    int idx = 0;
    Crossing * ptrA = start;
    Crossing * ptrB;
    bool isStrand;
    
    arrayLength = 0;
    
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
            arrayLength++;
        }
        
        ptrA = ptrA->next;
    } while (ptrA != start);
}




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
    int direction = -1;
    if (this->find(19, -1)->turnLeft(direction)->num != 20) {
        cout << "Crossing turn test FAILED" << endl;
        return -1;
    }
    
    cout << "Crossing turn test passed" << endl;
    return 0;
}

int Knot::findStrandsOfLengthTest() {
    Crossing* array[10];
    int arrayLength = 0;
    
    for (int i = 0; i<10; i++) {
        array[i]=nullptr;
    }
    
    findStrandsOfLength(3, array, arrayLength);
    
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




