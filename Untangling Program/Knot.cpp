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
Knot::Knot(vector<int>& extGauss) {
    //We need to assign these so constructFromGauss doesn't try to run deconstruct()
    mySize = 0;
    start = nullptr;
    startCrossing1Tangle = nullptr;
    endCrossing1Tangle = nullptr;
    
    constructFromGauss(extGauss);
}

//Construct a knot from an Extended Gauss Code string
//We expect the user to input this correctly,
    //as we do not know a way to verify accurate codes
Knot::Knot(string inputString) {
    //We need to assign these so constructFromGauss doesn't try to run deconstruct()
    if (start != nullptr) deconstruct();
    mySize = 0;
    start = nullptr;
    startCrossing1Tangle = nullptr;
    endCrossing1Tangle = nullptr;
    
    //Use getInput to turn string into array
    vector<int> extGauss = getInput(inputString);
    constructFromGauss(extGauss);
}

//Copy Constructor
Knot::Knot(const Knot &origKnot) {
    mySize = 0;
    start = nullptr;
    startCrossing1Tangle = nullptr;
    endCrossing1Tangle = nullptr;
    vector<int> extGauss = origKnot.toVector();
    
    constructFromGauss(extGauss);
}

Knot Knot::move(Knot& origKnot) {
    Knot newKnot;
    newKnot.mySize = origKnot.mySize;
    newKnot.start = origKnot.start;
    newKnot.startCrossing1Tangle = origKnot.startCrossing1Tangle;
    newKnot.endCrossing1Tangle = origKnot.endCrossing1Tangle;

    origKnot.mySize = 0;
    origKnot.start = nullptr;
    origKnot.startCrossing1Tangle = nullptr;
    origKnot.endCrossing1Tangle = nullptr;

    return newKnot;
}

//Construct a knot from an Extended Gauss Code vector
void Knot::constructFromGauss(vector<int>& extGauss) {
    deconstruct();
    //Create new blank crossing to begin with

    int knotLength = extGauss.size();
    
    start = make_shared<Crossing>(abs(extGauss.at(0)));
    //Point to that blank crossing
    shared_ptr<Crossing> nPtr = start;
    
    //Establish num, next, and prev for all crossings
    //and link them together
    for (int idx=1; idx < knotLength; idx++) {
        nPtr->next = make_shared<Crossing>(abs(extGauss.at(idx)));
        nPtr->next->prev = nPtr;
        nPtr=nPtr->next;
    }
    //Connect last node to start to complete the loop
    nPtr->next = start;
    nPtr->next->prev = nPtr;
    nPtr=nPtr->next;
    
    //nPtr is now at start again
    //The knot now has all next and prev data, and nums. Still no neg, sign, or handedness
    
    
    //Here we get neg data and mySize
    //For each crossing, we search through to find it's corresponding
    //negative crossing, and point them at each other
    shared_ptr<Crossing> nPtr2; //Reminder that nPtr is still at start and we use that here too
    for (int idx=0; idx < knotLength; idx++) {
        nPtr2 = start;
        for (int idx2=0; idx2 < knotLength; idx2++) {
            if (idx == idx2) { //Don't point the neg of a crossing to itself
                nPtr2 = nPtr2->next;
                continue;
            }
            if (nPtr->num == nPtr2->num) {
                nPtr->neg = nPtr2;
                nPtr2->neg = nPtr;
                break;
            }
            nPtr2 = nPtr2->next;
        }
        nPtr = nPtr->next;
        mySize++;
    }
    //We incremented mySize to the number of crossing *objects*
    //But we want it to be just the number of crossings, period
    mySize = mySize/2;
    
    
    //And here we get sign and handedness data
    //The sign of a crossing is determined by
    //the pos/neg sign of the first instance of a number in an extended Gauss Code list
    //Similarly, the handedness is determined by
    //the pos/neg sign of the second instance
    nPtr = start;
    for (int idx=0; idx < knotLength; idx++) {
        nPtr2 = start;
        
        //If for some reason the sign is already established, move on
        if (nPtr->sign != 0) {
            nPtr = nPtr->next;
            continue;
        }
        
        
        for (int idx2=0; idx2 < knotLength; idx2++) {
            if (idx == idx2) {
                nPtr2 = nPtr2->next;
                continue;
            }
            else if (abs(extGauss.at(idx))==abs(extGauss.at(idx2))) {
                if (idx < idx2) {
                    nPtr->sign  = getSign(extGauss.at(idx));
                    nPtr2->sign = -getSign(extGauss.at(idx));
                    nPtr->handedness  = getSign(extGauss.at(idx2));
                    nPtr2->handedness = getSign(extGauss.at(idx2));
                    break;
                }
                else /* if (idx2<idx) */ {
                    nPtr->sign  = -getSign(extGauss.at(idx2));
                    nPtr2->sign = getSign(extGauss.at(idx2));
                    nPtr->handedness  = getSign(extGauss.at(idx));
                    nPtr2->handedness = getSign(extGauss.at(idx));
                    break;
                }
            }
            nPtr2 = nPtr2->next;
        }
        nPtr = nPtr->next;
    }
}

Knot::~Knot() {
    deconstruct();
}

//Delete all crossings from the knot
void Knot::deconstruct() {
    if (start != nullptr && mySize != 0) {
        start->prev->next=nullptr;
        shared_ptr<Crossing> temp = start;
        while (temp!= nullptr) {
            start=start->next;
            temp.reset();
            temp=start;
        }
    }
    mySize = 0;
}

//Return a string of the non-extended Gauss Code
string Knot::toGaussString() const{
    shared_ptr<Crossing> ptr = start;
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
    shared_ptr<Crossing> ptrA = start;
    shared_ptr<Crossing> ptrB;
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

//Return the knot as an Extended Gauss Code vector
vector<int> Knot::toVector() const{
    //Honestly the easiest way to do this
    //is just to turn it into a string and use getInput
    return getInput(this->toExtGaussString());
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
shared_ptr<Knot::Crossing> Knot::find(int numToFind, int signOfNum) {
    shared_ptr<Crossing> nPtr = start;
    
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
shared_ptr<Knot::Crossing> Knot::find(int numToFindWithSign) {
    return find(abs(numToFindWithSign), getSign(numToFindWithSign));
}

//Find the index of a crossing with respect to start
int Knot::findIndex(shared_ptr<Knot::Crossing> crossingToFind) {
    shared_ptr<Crossing> ptr = start;
    for (int idx = 0; idx < mySize*2; idx++) {
        if (ptr == crossingToFind)
            return idx;
        ptr = ptr->next;
    }
    
    return -1;
}

//Get the length of a segment from startCrossing to endCrossing, inclusive
int Knot::lengthOfSeg(shared_ptr<Knot::Crossing> startCrossing, shared_ptr<Knot::Crossing> endCrossing) {
    int length = 1;
    shared_ptr<Crossing> tempPtr = startCrossing;
    while (tempPtr!=endCrossing) {
        length++;
        tempPtr = tempPtr->next;
    }
    
    return length;
}

//Insert a new crossing at an index
//This ended up being unused but could be maybe useful in the future
void Knot::insert(int index, int numValue) {
    shared_ptr<Crossing> nPtr = make_shared<Crossing>(numValue);
    
    if (index==0) {
        nPtr->next=start;
        start=nPtr;
        mySize++;
    }
    else if(index>0 && index<=mySize) {
        shared_ptr<Crossing> predPtr = make_shared<Crossing>(0);
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
    shared_ptr<Crossing> ptrA = start;
    
    for (int i=0; i<index; i++)
        ptrA=ptrA->next;
    
    this->erase(ptrA);
    
}

//Delete a crossing object given a pointer to it
//This one locates both the crossing and its corresponding neg, deleting both
void Knot::erase(shared_ptr<Crossing> ptrA) {
    shared_ptr<Crossing> ptrB=ptrA->neg;
    
    if (mySize==1) {
        ptrA.reset();
        ptrB.reset();
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
        ptrA.reset();
        ptrB.reset();
        
        mySize--;
    }
}


Knot & Knot::operator=(const Knot &origKnot) {
    vector<int> extGauss = origKnot.toVector();
    
    constructFromGauss(extGauss);
    return *this;
}


//Copy constructor
Knot::Crossing::Crossing(const Crossing &origCrossing) {
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
shared_ptr<Knot::Crossing> Knot::Crossing::advance(int direction) {
    return (direction > 0) ? (this->next) : (this->prev);
}

//Move backward
shared_ptr<Knot::Crossing> Knot::Crossing::recede(int direction) {
    return (direction > 0) ? (this->prev) : (this->next);
}

//Turn left. These functions are why we need handedness
shared_ptr<Knot::Crossing> Knot::Crossing::turnLeft(int & direction) {
    direction = direction * (this->handedness) * (this->sign);
    return (this->neg)->advance(direction);
}

//Turn right
shared_ptr<Knot::Crossing> Knot::Crossing::turnRight(int & direction) {
    direction = direction * (this->handedness) * (this->sign) * -1;
    return (this->neg)->advance(direction);
}

//Turn left without altering the direction variable
shared_ptr<Knot::Crossing> Knot::Crossing::dummyTurnLeft(int direction) {
    direction = direction * (this->handedness) * (this->sign);
    return (this->neg)->advance(direction);
}

//Turn right without altering the direction variable
shared_ptr<Knot::Crossing> Knot::Crossing::dummyTurnRight(int direction) {
    direction = direction * (this->handedness) * (this->sign) * -1;
    return (this->neg)->advance(direction);
}



//Reduction Move 1
bool Knot::rm1() {
    shared_ptr<Crossing> ptrA=start;
    
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
    shared_ptr<Crossing> ptrA=start;
    shared_ptr<Crossing> ptrB;
    
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
    shared_ptr<Crossing> ptrA=start;
    
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
    shared_ptr<Crossing> ptrA=start;
    shared_ptr<Crossing> ptrB;
    
    do {
        ptrB=ptrA->next;
        if (ptrA->sign==ptrB->sign && (ptrA->neg->next==ptrB->neg || ptrA->neg->prev==ptrB->neg))
            return true;
        ptrA=ptrA->next;
    } while (ptrA!=start);
    
    return false;
}

//See if current strand, bounded by start/endCrossing, is a complete 1-tangle
bool Knot::is1Tangle(shared_ptr<Knot::Crossing> startCrossing,shared_ptr<Knot::Crossing> endCrossing) {
    shared_ptr<Crossing> ptrA = startCrossing; //Probably just "start" unless we change something
    shared_ptr<Crossing> ptrB = endCrossing;
    
    
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
    shared_ptr<Crossing> startCrossing = nullptr;
    shared_ptr<Crossing> endCrossing = nullptr;
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
bool Knot::remove1TanglesHelper(shared_ptr<Knot::Crossing>& startCrossing, shared_ptr<Knot::Crossing>& endCrossing) {
    if (mySize <= 6) {
        return false;
    }
    
    shared_ptr<Crossing> origStart = startCrossing;
    shared_ptr<Crossing> origEnd = endCrossing;
    
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

    //We iterate through all possible diagram forms
    for (int vectorIdx = 0; vectorIdx < triedDiagrams.size(); vectorIdx++) {
        
        kPrint(cout << "Operating on [" << vectorIdx << "]" << endl;)
        
        //Retrieve current knot string and treat it as the current knot
        currentKnotString = triedDiagrams.at(vectorIdx);
        *this = Knot(currentKnotString);
        
        kPrint(cout << "Current knot string: " << currentKnotString << endl;)

        for (int strandLength = maxStrandLength; strandLength >= 2; strandLength--) {
            vector<int> strandVector = findStrandsOfLength(strandLength);
            
            kPrint(cout << "Strand length being tested: " << strandLength << endl;)
            
            for (int idx = 0; idx < strandVector.size(); idx++) {
                kPrint(cout << "First crossing of current strand: " << strandVector.at(idx) << endl;)
                
                //Try turnTrace on both sides.
                //If either one returns true, we can stop doing any further work
                    //Because that would mean that a reduction is possible
                if(turnTrace(strandVector.at(idx), strandLength, 1, triedDiagrams)) {
                    strandVector.clear();
                    return true;
                }
                
                if(turnTrace(strandVector.at(idx), strandLength, -1, triedDiagrams)) {
                    strandVector.clear();
                    return true;
                }
            }
            strandVector.clear();
        }
    }
    
    *this = origKnot;
    return false;
}

bool Knot::turnTrace(int firstCrossingNum, int strandLength, int side, vector<string>& triedDiagrams) {
    
    Knot knotNoMovement = move(*this);
    *this = Knot(knotNoMovement);

    //Holds all crossings in one strand
    vector<shared_ptr<Crossing>> strandVector;
    
    
    //Start and end crossings (where we trace from and to)
    shared_ptr<Crossing> startCrossing;
    shared_ptr<Crossing> endCrossing;
    
    //Total number of intersections on the far side of the tangle
    //Should be constrained to n (aka strandLength) for an n-tangle
    int numIntersections = 0;

    //Current direction being traced in.
    //This will get changed by the turn functions.
    int direction;


    shared_ptr<Crossing> strandPtr = find(firstCrossingNum);
    if (strandPtr == nullptr) {
        cout << "Something went horribly wrong :(" << endl;
        exit(1);
    }
    
    //Establish strand array based on side
    if (side == 1) {
        for (int i = 0; i < strandLength; i++) {
            strandVector.insert(strandVector.begin(), strandPtr);
            strandPtr = strandPtr->next;
        }
        direction = -1;
    }
    
    else { //(side == -1)
        for (int i = 0; i < strandLength; i++) {
            strandVector.push_back(strandPtr);
            strandPtr = strandPtr->next;
        }
        direction = 1;
    }
    
    startCrossing = strandVector.at(0);
    endCrossing = strandVector.back();
    
    vector<shared_ptr<Crossing>> pathVector;

    int pathLength = 0;
    
    int origDirection = direction;
    
    int dummyDirection = direction;
    if (turnTraceHelper(startCrossing->turnLeft(dummyDirection), strandVector, pathVector, pathLength, numIntersections, dummyDirection)) {
        
        kPrint({
            cout << "Path Length: " << pathVector.size() << endl;
            cout << "Path: ";
            for (int i = 0; i < pathVector.size(); i++) {
                
                cout << pathVector.at(i)->num * pathVector.at(i)->sign << ", ";
                
            }
            cout << endl << "strandVector: ";
            for (int i = 0; i < strandVector.size(); i++) {
                
                cout << strandVector.at(i)->num << ", ";
                
            }
        })

        pathVector.push_back(nullptr);

        shared_ptr<Crossing> tempPtr = startCrossing->dummyTurnLeft(direction);
        
        //This is a way to keep track of how we move crossings in the strand over the tangle
        //When we relocate a crossing, we increment/decrement strandIndex by 1
        
        //We initialized strandArray in a direction based on the side being used, so that is reflected here
        //The incrementation vs. decrementation of strandIndex is thus also dependent on side
        //int strandIndex = (side==1) ? (strandLength-1) : (0);
        int strandIndex = 0;
        
        //Move the strand over the tangle. Very important
        //Start by tracing through the path crossings and find intersections
        
        
        for (int i = 1; i <= pathLength; i++) {
            
            
            //If we turned left at a crossing, nothing to do; move on
            if (tempPtr->dummyTurnLeft(direction) == pathVector.at(i)
                || tempPtr->dummyTurnLeft(direction)->num == endCrossing->num) {
                
                tempPtr = tempPtr->turnLeft(direction);
                continue;
            }
            
            //If we went straight or right, we have at least one crossing to relocate
            if (tempPtr->advance(direction) == pathVector.at(i)
                || tempPtr->advance(direction)->num == endCrossing->num
                || tempPtr->dummyTurnRight(direction) == pathVector.at(i)
                || tempPtr->dummyTurnRight(direction)->num == endCrossing->num) {
                
                //Detach old crossing location
                strandVector.at(strandIndex)->neg->prev->next = strandVector.at(strandIndex)->neg->next;
                strandVector.at(strandIndex)->neg->next->prev = strandVector.at(strandIndex)->neg->prev;
                
                
                shared_ptr<Crossing> turnLeftCrossing = tempPtr->dummyTurnLeft(direction);
                
                //If turnLeftCrossing is facing into tempPtr (thus the "next")
                if (turnLeftCrossing->next == tempPtr->neg) {
                    
                    turnLeftCrossing->next->prev = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->next = turnLeftCrossing->next;
                    
                    turnLeftCrossing->next = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->prev = turnLeftCrossing;
                    
                    strandVector.at(strandIndex)->handedness = -1 * origDirection * strandVector.at(strandIndex)->sign;
                    strandVector.at(strandIndex)->neg->handedness = -1 * origDirection * strandVector.at(strandIndex)->sign;
                }
                //If turnLeftCrossing is facing away from tempPtr
                else { //if (turnLeftCrossing->prev == tempPtr->neg)

                    turnLeftCrossing->prev->next = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->prev = turnLeftCrossing->prev;
                    
                    turnLeftCrossing->prev = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->next = turnLeftCrossing;
                    
                    strandVector.at(strandIndex)->handedness = origDirection * strandVector.at(strandIndex)->sign;
                    strandVector.at(strandIndex)->neg->handedness = origDirection * strandVector.at(strandIndex)->sign;
                }
                
                strandIndex++;
                
            }
            
            //If we went straight, move on
            if (tempPtr->advance(direction) == pathVector.at(i)
                || tempPtr->advance(direction)->num == endCrossing->num) {
                tempPtr = tempPtr->advance(direction);
                continue;
            }
            
            //If we went right, we have one more crossing to relocate
            if (tempPtr->dummyTurnRight(direction) == pathVector.at(i)
                || tempPtr->dummyTurnRight(direction)->num == endCrossing->num) {
                
                //Detach old crossing location
                strandVector.at(strandIndex)->neg->prev->next = strandVector.at(strandIndex)->neg->next;
                strandVector.at(strandIndex)->neg->next->prev = strandVector.at(strandIndex)->neg->prev;
                
                shared_ptr<Crossing> goStraightCrossing = tempPtr->advance(direction);
                //If goStraightCrossing is facing into tempPtr (thus the "next")
                if (goStraightCrossing->next == tempPtr) {

                    goStraightCrossing->next->prev = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->next = goStraightCrossing->next;
                    
                    goStraightCrossing->next = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->prev = goStraightCrossing;
                    
                    strandVector.at(strandIndex)->handedness = -1 * origDirection * strandVector.at(strandIndex)->sign;
                    strandVector.at(strandIndex)->neg->handedness = -1 * origDirection * strandVector.at(strandIndex)->sign;
                }
                //If goStraightCrossing is facing away from tempPtr
                else { //if (goStraightCrossing->prev == tempPtr->neg)
                    
                    goStraightCrossing->prev->next = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->prev = goStraightCrossing->prev;
                    
                    goStraightCrossing->prev = strandVector.at(strandIndex)->neg;
                    strandVector.at(strandIndex)->neg->next = goStraightCrossing;
                    
                    strandVector.at(strandIndex)->handedness = origDirection * strandVector.at(strandIndex)->sign;
                    strandVector.at(strandIndex)->neg->handedness = origDirection * strandVector.at(strandIndex)->sign;
                }
                
                tempPtr = tempPtr->turnRight(direction);
                
                strandIndex++;
                
            }
            
            
        }
        
        kPrint(cout << endl;)
    }
    
    strandVector.clear();
    pathVector.clear();
    
    
    if (dummyRM1() || dummyRM2()) {
        return true;
    }
    
    string endKnotString = this->toExtGaussString();
    if (!isInVector<string>(triedDiagrams, endKnotString)) {
        kPrint(cout << "Knot form found: " << endKnotString << endl;)
        triedDiagrams.push_back(endKnotString);
    }
    
    
    *this = move(knotNoMovement);
    return false;
    
}

bool Knot::turnTraceHelper(shared_ptr<Crossing> currentCrossing, vector<shared_ptr<Crossing>>& strandVector, vector<shared_ptr<Crossing>>& pathVector, int& pathLength, int numIntersections, int direction) {
    
    //Used for preventing direction from being affected by turn functions
    int dummyDirection;
    
    shared_ptr<Crossing> endCrossing = strandVector.back();
    
    
    //currentCrossing is pointing to one of the crossing completions of the strand
    bool boolInStrand = index_of(strandVector, currentCrossing->neg) > -1;
    
    //currentCrossing has hit the rightmost strand endpoint
    bool boolHitRightEndpt = currentCrossing == endCrossing;
    
    //currentCrossing has hit the leftmost strand endpoint
    bool boolHitLeftEndpt = index_of(strandVector, currentCrossing) == 0;
    
    //currentCrossing is at endCrossing
    bool boolAtEnd = currentCrossing->neg == endCrossing;
    
    //currentCrossing approached the end crossing from the proper direction
    dummyDirection = direction;
    bool boolRightDir = index_of(strandVector, currentCrossing->turnRight(dummyDirection)) > -1;
    
    //currentCrossing hit a crossing it has already encountered
    bool boolLoopedBack = index_of(pathVector, currentCrossing) > -1 || index_of(pathVector, currentCrossing->neg) > -1;
    
    //correct number of intersections
    bool boolRightNumIntsx = numIntersections == strandVector.size();
    
    
    
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
    pathVector.push_back(currentCrossing);
    pathLength++;
    
    
//    string indentString = "";
//    for (int i = 0; i < pathLength; i++) {
//        indentString += "    ";
//    }
    
    
    
    //Try turning right
    dummyDirection = direction;
//    cout << indentString << "Trying to turn right at: " << currentCrossing->num << endl;
    if (numIntersections + 2 <= strandVector.size() && turnTraceHelper(currentCrossing->turnRight(dummyDirection), strandVector, pathVector, pathLength, numIntersections + 2, dummyDirection)) {
        
        
        return true;
    }
    //Try going straight
    dummyDirection = direction;
//    cout << indentString << "Trying to go straight at: " << currentCrossing->num << endl;
    if (numIntersections + 1 <= strandVector.size() && turnTraceHelper(currentCrossing->advance(dummyDirection), strandVector, pathVector, pathLength, numIntersections + 1, dummyDirection)) {
        
        return true;
    }
    //Try turning left
    dummyDirection = direction;
//    cout << indentString << "Trying to turn left at: " << currentCrossing->num << endl;
    if (numIntersections + 0 <= strandVector.size() && turnTraceHelper(currentCrossing->turnLeft(dummyDirection), strandVector, pathVector, pathLength, numIntersections + 0, dummyDirection)) {
        
        return true;
    }
    //Unmark current crossing as being part of solution path
    pathVector.pop_back();
    pathLength--;
    return false;
}

vector<int> Knot::findStrandsOfLength(int length) {
    
    vector<int> crossingVector;
    shared_ptr<Crossing> ptrA = start;
    shared_ptr<Crossing> ptrB;
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
            crossingVector.push_back(ptrA->num * ptrA->sign);
        }
        
        ptrA = ptrA->next;
    } while (ptrA != start);
    return crossingVector;
}




int Knot::getLongestStrandLength() {
    shared_ptr<Crossing> ptrA = this->start;
    shared_ptr<Crossing> ptrB;
    
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




