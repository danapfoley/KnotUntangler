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


#include "Knot.h"


//Construct a knot from an array of numbers
Knot::Knot(int extGauss[], int knotLength) {
    //We need to assign these so constructFromGauss doesn't try to run deconstruct()
    mySize = 0;
    start = nullptr;
    startCrossing1Tangle = nullptr;
    endCrossing1Tangle = nullptr;
    
    constructFromGauss(extGauss, knotLength);
}

//Construct a knot from an Extended Gauss Code string
//We expect the user to input this correctly,
    //as we do not know a way to verify accurate codes
Knot::Knot(string inputString) {
    //We need to assign these so constructFromGauss doesn't try to run deconstruct()
    mySize = 0;
    start = nullptr;
    startCrossing1Tangle = nullptr;
    endCrossing1Tangle = nullptr;
    
    //Use getInput to turn string into array
    int knotLength = 0;
    int * extGauss = getInput(inputString, knotLength);
    constructFromGauss(extGauss, knotLength);
}

//Copy Constructor
Knot::Knot(const Knot &origKnot) {
    mySize = 0;
    start = nullptr;
    startCrossing1Tangle = nullptr;
    endCrossing1Tangle = nullptr;
    int * extGauss = origKnot.toArray();
    int knotLength = origKnot.mySize*2;
    
    constructFromGauss(extGauss, knotLength);
}

//Construct a knot from an Extended Gauss Code array
//knotLength must be established as the size of the array
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
    //We incremented mySize to the number of crossing *objects*
    //But we want it to be just the number of crossings, period
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
                    nPtr->sign  = getSign(extGauss[idx]);
                    nPtr2->sign = -getSign(extGauss[idx]);
                    nPtr->handedness  = getSign(extGauss[idx2]);
                    nPtr2->handedness = getSign(extGauss[idx2]);
                    break;
                }
                else /* if (idx2<idx) */ {
                    nPtr->sign  = -getSign(extGauss[idx2]);
                    nPtr2->sign = getSign(extGauss[idx2]);
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

//Delete all crossings from the knot
void Knot::deconstruct() {
    if (start != nullptr && mySize != 0) {
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

//Return a string of the non-extended Gauss Code
string Knot::toGaussString() const{
    Crossing * ptr = start;
    string outputString = "[";
    
    //As long as the knot has crossings, we can print them
    if (ptr!=nullptr) {
        do {
            outputString += to_string((ptr->num)*(ptr->sign));
            outputString += (ptr->next!=start) ? (", ") : ("");
            ptr = ptr->next;
            
        } while (ptr!=start);
    }
    
    outputString += "]";
    
    return outputString;
}

//Return a string of the Extended Gauss Code
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

//Return the knot as an Extended Gauss Code array
int* Knot::toArray() const{
    //Honestly the easiest way to do this
    //is just to turn it into a string and use getInput
    int knotLength = 0;
    int * extGauss = getInput(this->toExtGaussString(), knotLength);
    return extGauss;
}

//Print out the knot
void Knot::display(ostream & out) const{
    string outputString = this->toExtGaussString();
    out << outputString;
}

ostream& operator<<(ostream& out, const Knot &myKnot) {
    myKnot.display(out);
    return out;
}

//Locate a crossing object in the knot based on its num and sign
Knot::Crossing * Knot::find(int numToFind, int signOfNum) {
    Crossing * nPtr = start;
    
    numToFind = abs(numToFind);
    
    do {
        if (nPtr->num == numToFind && nPtr->sign == signOfNum) {
            return nPtr;
        }
        nPtr = nPtr->next;
    } while (nPtr != start);
    
    kPrint(cout << "Couldn't find " << numToFind*signOfNum << endl;)
    return nullptr;
}

//Just an easy way to put a number into find without having to split it up into num and sign
Knot::Crossing * Knot::find(int numToFindWithSign) {
    return find(abs(numToFindWithSign), getSign(numToFindWithSign));
}

//Find the index of a crossing with respect to start
int Knot::findIndex(Knot::Crossing* crossingToFind) {
    Crossing* ptr = start;
    for (int idx = 0; idx < mySize*2; idx++) {
        if (ptr == crossingToFind)
            return idx;
        ptr = ptr->next;
    }
    
    return -1;
}

//Get the length of a segment from startCrossing to endCrossing, inclusive
int Knot::lengthOfSeg(Knot::Crossing* startCrossing, Knot::Crossing* endCrossing) {
    int length = 1;
    Crossing* tempPtr = startCrossing;
    while (tempPtr!=endCrossing) {
        length++;
        tempPtr = tempPtr->next;
    }
    
    return length;
}

//Insert a new crossing at an index
//This ended up being unused but could be maybe useful in the future
void Knot::insert(int index, int numValue) {
    Crossing * nPtr;
    nPtr = new Crossing(numValue);
    
    if (index==0) {
        nPtr->next=start;
        start=nPtr;
        mySize++;
    }
    else if(index>0 && index<=mySize) {
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

//Delete a crossing object at a certain index
//It doesn't appear to erase the corresponding neg crossing,
    //but it does upon calling the other erase function
void Knot::erase(int index) {
    Crossing * ptrA = start;
    
    for (int i=0; i<index; i++)
        ptrA=ptrA->next;
    
    this->erase(ptrA);
    
}

//Delete a crossing object given a pointer to it
//This one locates both the crossing and its corresponding neg, deleting both
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
        }
        
        else if(ptrB->next==ptrA) {
            ptrB->prev->next=ptrA->next;
            ptrA->next->prev=ptrB->prev;
        }
        
        else {
            ptrA->prev->next=ptrA->next;
            ptrA->next->prev=ptrA->prev;
            
            ptrB->prev->next=ptrB->next;
            ptrB->next->prev=ptrB->prev;
        }
        delete ptrA;
        delete ptrB;
        
        mySize--;
    }
}


Knot & Knot::operator=(const Knot &origKnot) {
    int * extGauss = origKnot.toArray();
    int knotLength = origKnot.mySize*2;
    
    constructFromGauss(extGauss, knotLength);
    return *this;
}


//Copy constructor
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

//Move forward based on a given direction (1 for forward, -1 for backward)
Knot::Crossing* Knot::Crossing::advance(int direction) {
    return (direction > 0) ? (this->next) : (this->prev);
}

//Move backward
Knot::Crossing* Knot::Crossing::recede(int direction) {
    return (direction > 0) ? (this->prev) : (this->next);
}

//Turn left. These functions are why we need handedness
Knot::Crossing* Knot::Crossing::turnLeft(int & direction) {
    direction = direction * (this->handedness) * (this->sign);
    return (this->neg)->advance(direction);
}

//Turn right
Knot::Crossing* Knot::Crossing::turnRight(int & direction) {
    direction = direction * (this->handedness) * (this->sign) * -1;
    return (this->neg)->advance(direction);
}

//Turn left without altering the direction variable
Knot::Crossing* Knot::Crossing::dummyTurnLeft(int direction) {
    direction = direction * (this->handedness) * (this->sign);
    return (this->neg)->advance(direction);
}

//Turn right without altering the direction variable
Knot::Crossing* Knot::Crossing::dummyTurnRight(int direction) {
    direction = direction * (this->handedness) * (this->sign) * -1;
    return (this->neg)->advance(direction);
}



//Reduction Move 1
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

//Reduction Move 2
bool Knot::rm2() {
    Crossing * ptrA=start;
    Crossing * ptrB;
    
    do {
        ptrB=ptrA->next;
        if (ptrA->sign==ptrB->sign && (ptrA->neg->next==ptrB->neg || ptrA->neg->prev==ptrB->neg)) {
            erase(ptrA);
            erase(ptrB);
            return true;
        }
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}

//See if Reduction Move 1 can work but don't actually do it
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

//See if Reduction Move 2 can work but don't actually do it
bool Knot::dummyRM2() {
    Crossing * ptrA=start;
    Crossing * ptrB;
    
    do {
        ptrB=ptrA->next;
        if (ptrA->sign==ptrB->sign && (ptrA->neg->next==ptrB->neg || ptrA->neg->prev==ptrB->neg))
            return true;
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}

//See if current strand, bounded by start/endCrossing, is a complete 1-tangle
bool Knot::is1Tangle(Knot::Crossing* startCrossing,Knot::Crossing* endCrossing) {
    Crossing* ptrA = startCrossing; //Probably just "start" unless we change something
    Crossing* ptrB = endCrossing;
    
    
    int idxOfA = findIndex(ptrA); //Probably 0
    int idxOfB = findIndex(ptrB);
    int negIdx;
    
    //Search to see if every crossing from A to B has its neg also in this range
    while (ptrA!=ptrB->next) { //Search everything in range [A, B], inclusive
        negIdx = findIndex(ptrA->neg);
        if (!(negIdx >= idxOfA && negIdx <= idxOfB)) { //Check if neg in range
            return false;
        }
        ptrA = ptrA->next;
    }
    return true;
}

//Remove all 1-tangles from a knot
//Returns true if anything is removed, false otherwise
bool Knot::remove1Tangles() {
    bool didRemove1Tangles = false;
    
    //First remove 1-tangles for later reinsertion
    //This is basically equivalent to sliding them all out of the way for other moves
    Crossing* startCrossing = nullptr;
    Crossing* endCrossing = nullptr;
    //While we can successfully keep removing 1-tangles
    while (remove1TanglesHelper(startCrossing, endCrossing)) {
        didRemove1Tangles = true;
        startCrossing1Tangle = startCrossing;
        endCrossing1Tangle = endCrossing;
    }
    
    
    return didRemove1Tangles;
}

//Utility for removing a single 1-tangle
//Called many times by remove1Tangles in order to get them all
bool Knot::remove1TanglesHelper(Knot::Crossing*& startCrossing, Knot::Crossing*& endCrossing) {
    if (mySize <= 6) {
        return false;
    }
    
    Crossing* origStart = startCrossing;
    Crossing* origEnd = endCrossing;
    
    //For every crossing object in the knot
    for (int idx = 0; idx < mySize*2; idx++) {
        
        //Start with length 6. Yes, 5 'next's looks dumb
        endCrossing = start->next->next->next->next->next;
        
        
        //Relevant 1-tangles that we want to pick out will never be less than
        //6 crossing objects (3 crossings)
        //and never more than half the size of the knot (mySize * 2)/2
        for (int length = 6; length < mySize; length+=2) {
            
            startCrossing = start;
            
            //If current segment is a 1-tangle
            if (is1Tangle(startCrossing, endCrossing)) {
                
                //Make start point to somewhere that will be in the knot
                //After we remove the 1-tangle
                endCrossing->next->prev = start->prev;
                start = endCrossing->next;
                
                //decrease size of knot appropriately
                mySize -= lengthOfSeg(startCrossing, endCrossing) / 2;
                
                //Perform detachment of 1-tangle
                //Crossings still exist, but not in the knot loop
                startCrossing->prev->next = endCrossing->next;
                startCrossing->prev = nullptr;
                endCrossing->next->prev = startCrossing->prev;
                endCrossing->next = nullptr;
                
                if (origEnd != nullptr) {
                    origEnd->next = startCrossing;
                    startCrossing->prev = origEnd;
                }
                if (origStart != nullptr) {
                    startCrossing = origStart;
                }
                return true;
                
            }
            
            endCrossing = endCrossing->next;
        }
        
        //We're relocating start itself rather than a starting pointer due to indexing.
        //If we did it the other way, we would have issues when a 1-tangle
        //loops around the end to start.
        start = start->next;
    }
    return false;
}

//Reinserts all 1-tangles at once back into the knot
void Knot::reinsert1Tangles() {
    if (startCrossing1Tangle == nullptr || endCrossing1Tangle == nullptr)
        return;
    
    //If knot is empty, reinsertion is simple
    if (mySize == 0) {
        start = startCrossing1Tangle;
        start->prev = endCrossing1Tangle;
        endCrossing1Tangle->next = start;
    }
    
    //Reconnect end of knot to beginning of 1-tangle
    start->prev->next = startCrossing1Tangle;
    startCrossing1Tangle->prev = start->prev;
    
    //Reconnect end of 1-tangle to beginning of knot
    endCrossing1Tangle->next = start;
    start->prev = endCrossing1Tangle;
    
    //Correct the length of the knot
    mySize += lengthOfSeg(startCrossing1Tangle, endCrossing1Tangle) / 2;
}

//Translation Move 2
bool Knot::TM() {
    
    Knot origKnot(*this);
    
    //List of diagrams of the knot
        //that have been moved to and examined already
    vector<string> triedDiagrams;
    string currentKnotString;
    
    //Since we have already examined the starting form
    triedDiagrams.push_back(this->toExtGaussString());

    int maxStrandLength = getLongestStrandLength();
    
    int numStrandsForArray = 0;
    
    int* strandArray = new int[mySize];
    
    int vectorIdx;

    //We iterate through all possible diagram forms
    vector<string>::iterator vectorIt = triedDiagrams.begin();
    while (vectorIt < triedDiagrams.end()) {
        
        //We must store the index of the iterator
            //So it can be recalculated later
        //We cannot hang onto the iterator, because
            //turnTrace may call push_back(),
            //Which invalidates the iterator
        vectorIdx = int(vectorIt - triedDiagrams.begin());
        
        kPrint(cout << "Operating on [" << vectorIdx << "] ";)
        
        //Retrieve current knot string and treat it as the current knot
        currentKnotString = *vectorIt;
        *this = Knot(currentKnotString);
        
        kPrint(cout << currentKnotString << endl;)
        
        
        for (int strandLength = maxStrandLength; strandLength >= 2; strandLength--) {
            for (int i = 0; i < mySize; i++) {
                strandArray[i] = 0;
            }
            findStrandsOfLength(strandLength, strandArray, numStrandsForArray);
            
            kPrint(cout << "Length: " << strandLength << endl;)
            
            for (int idx = 0; idx < numStrandsForArray; idx++) {
                kPrint(cout << "strandArray[" << idx << "]: " << strandArray[idx] << endl;)
                
                //Try turnTrace on both sides.
                //If either one returns true, we can stop doing any further work
                    //Because that would mean that a reduction is possible
                if(turnTrace(find(strandArray[idx]), strandLength, 1, triedDiagrams)) {
                    delete [] strandArray;
                    return true;
                }
                
                if(turnTrace(find(strandArray[idx]), strandLength, -1, triedDiagrams)) {
                    delete [] strandArray;
                    return true;
                }
            }
        }
        
        //Re-establish the vector iterator and advance it by 1
        vectorIt = triedDiagrams.begin() + vectorIdx + 1;
    }
    
    *this = origKnot;
    delete [] strandArray;
    return false;
}

bool Knot::turnTrace(Knot::Crossing* strandPtr, int strandLength, int side, vector<string>& triedDiagrams) {
    
    Knot knotNoMovement(*this);
    
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
        cout << "Something went horribly wrong :(" << endl;
        exit(1);
    }
    
    //Establish strand array based on side
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
    
    int origDirection = direction;
    
    int dummyDirection = direction;
    if (turnTraceHelper(startCrossing->turnLeft(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections, dummyDirection)) {
        
        kPrint({
            cout << "Path Length: " << pathLength << endl;
            cout << "Path: ";
            for (int i = 0; i < pathLength; i++) {
                
                cout << pathArray[i]->num << ", ";
                
            }
            cout << endl << "strandArray: ";
            for (int i = 0; i < strandLength; i++) {
                
                cout << strandArray[i]->num << ", ";
                
            }
        })

        Crossing * tempPtr = startCrossing->dummyTurnLeft(direction);
        
        //This is a way to keep track of how we move crossings in the strand over the tangle
        //When we relocate a crossing, we increment/decrement strandIndex by 1
        
        //We initialized strandArray in a direction based on the side being used, so that is reflected here
        //The incrementation vs. decrementation of strandIndex is thus also dependent on side
        //int strandIndex = (side==1) ? (strandLength-1) : (0);
        int strandIndex = 0;
        
        //Move the strand over the tangle. Very important
        //Start by tracing through the path crossings and find intersections
        
        
        for (int i = 0; i <= pathLength - 1; i++) {
            
            
            //If we turned left at a crossing, nothing to do; move on
            if (tempPtr->dummyTurnLeft(direction) == pathArray[i+1]
                || tempPtr->dummyTurnLeft(direction)->num == endCrossing->num) {
                
                tempPtr = tempPtr->turnLeft(direction);
                continue;
            }
            
            //If we went straight || right, we have at least one crossing to relocate
            if (tempPtr->advance(direction) == pathArray[i+1]
                || tempPtr->advance(direction)->num == endCrossing->num
                || tempPtr->dummyTurnRight(direction) == pathArray[i+1]
                || tempPtr->dummyTurnRight(direction)->num == endCrossing->num) {
                
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
                    
                    strandArray[strandIndex]->handedness = -1 * origDirection * strandArray[strandIndex]->sign;
                    strandArray[strandIndex]->neg->handedness = -1 * origDirection * strandArray[strandIndex]->sign;
                }
                //If turnLeftCrossing is facing away from tempPtr
                else { //if (turnLeftCrossing->prev == tempPtr->neg)

                    turnLeftCrossing->prev->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = turnLeftCrossing->prev;
                    
                    turnLeftCrossing->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = turnLeftCrossing;
                    
                    strandArray[strandIndex]->handedness = origDirection * strandArray[strandIndex]->sign;
                    strandArray[strandIndex]->neg->handedness = origDirection * strandArray[strandIndex]->sign;
                }
                
                strandIndex++;
                
            }
            
            //If we went straight, move on
            if (tempPtr->advance(direction) == pathArray[i+1]
                || tempPtr->advance(direction)->num == endCrossing->num) {
                tempPtr = tempPtr->advance(direction);
                continue;
            }
            
            //If we went right, we have one more crossing to relocate
            if (tempPtr->dummyTurnRight(direction) == pathArray[i+1]
                || tempPtr->dummyTurnRight(direction)->num == endCrossing->num) {
                
                //Detach old crossing location
                strandArray[strandIndex]->neg->prev->next = strandArray[strandIndex]->neg->next;
                strandArray[strandIndex]->neg->next->prev = strandArray[strandIndex]->neg->prev;
                
                Crossing* goStraightCrossing = tempPtr->advance(direction);
                //If goStraightCrossing is facing into tempPtr (thus the "next")
                if (goStraightCrossing->next == tempPtr) {

                    goStraightCrossing->next->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = goStraightCrossing->next;
                    
                    goStraightCrossing->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = goStraightCrossing;
                    
                    strandArray[strandIndex]->handedness = -1 * origDirection * strandArray[strandIndex]->sign;
                    strandArray[strandIndex]->neg->handedness = -1 * origDirection * strandArray[strandIndex]->sign;
                }
                //If goStraightCrossing is facing away from tempPtr
                else { //if (goStraightCrossing->prev == tempPtr->neg)
                    
                    goStraightCrossing->prev->next = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->prev = goStraightCrossing->prev;
                    
                    goStraightCrossing->prev = strandArray[strandIndex]->neg;
                    strandArray[strandIndex]->neg->next = goStraightCrossing;
                    
                    strandArray[strandIndex]->handedness = origDirection * strandArray[strandIndex]->sign;
                    strandArray[strandIndex]->neg->handedness = origDirection * strandArray[strandIndex]->sign;
                }
                
                tempPtr = tempPtr->turnRight(direction);
                
                strandIndex++;
                
            }
            
            
        }
        
        cout << endl;
    }
    
    delete [] strandArray;
    delete [] pathArray;
    
    
    if (dummyRM1() || dummyRM2()) {
        return true;
    }
    
    string endKnotString = this->toExtGaussString();
    if (!isInVector<string>(triedDiagrams, endKnotString)) {
        kPrint(cout << "Knot form found: " << endKnotString << endl;)
        triedDiagrams.push_back(endKnotString);
    }
    
    
    *this = knotNoMovement;
    return false;
    
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
    bool boolLoopedBack = index_of(pathArray, pathLength, currentCrossing) > -1 || index_of(pathArray, pathLength, currentCrossing->neg) > -1;
    
    //correct number of intersections
    bool boolRightNumIntsx = numIntersections == strandLength;
    
    
    
    //If we hit a stop condition
    if ((boolInStrand && !boolAtEnd) || (boolAtEnd && !boolRightDir) || (boolHitRightEndpt) || (boolLoopedBack)) {
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
    //3. Crossed the right number of strands along the way
    //Then return success
    if (boolAtEnd && boolRightDir && boolRightNumIntsx) {
        kPrint(cout << "Found tangle" << endl;)
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
    if (numIntersections + 2 <= strandLength && turnTraceHelper(currentCrossing->turnRight(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections + 2, dummyDirection)) {
        
        
        return true;
    }
    //Try going straight
    dummyDirection = direction;
//    cout << indentString << "Trying to go straight at: " << currentCrossing->num << endl;
    if (numIntersections + 1 <= strandLength && turnTraceHelper(currentCrossing->advance(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections + 1, dummyDirection)) {
        
        return true;
    }
    //Try turning left
    dummyDirection = direction;
//    cout << indentString << "Trying to turn left at: " << currentCrossing->num << endl;
    if (numIntersections + 0 <= strandLength && turnTraceHelper(currentCrossing->turnLeft(dummyDirection), strandArray, strandLength, pathArray, pathLength, numIntersections + 0, dummyDirection)) {
        
        return true;
    }
    //Unmark current crossing as being part of solution path
    pathLength--;
    pathArray[pathLength]=0;
    return false;
}

void Knot::findStrandsOfLength(int length, int* crossingArray, int& arrayLength) {
    
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
            crossingArray[idx] = ptrA->num * ptrA->sign;
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
    
    kPrint(cout << "Max Length: " << maxLength << endl;)
    
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
//    int* array[10];
//    int arrayLength = 0;
//    
//    for (int i = 0; i<10; i++) {
//        array[i]=i;
//    }
//    
//    findStrandsOfLength(3, array, arrayLength);
//    
//    for (int i = 0; i<10; i++) {
//        if (array[i] != nullptr)
//            cout << "array[" << i << "] = " << array[i]->num << endl;
//        else
//            cout << "array[" << i << "] = " << array[i] << endl;
//    }
//    
//    if (array[0] == nullptr or array[0]->num != 13) {
//        cout << "Find strands of length test FAILED"<< endl;
//        return -1;
//    }
//    cout << "Find strands of length test passed" << endl;
    return 0;
}




