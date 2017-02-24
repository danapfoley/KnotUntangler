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



#ifndef KNOT
#define KNOT

#include "Naming.h"

#define knotDebug 1

#if knotDebug
#define kPrint(x) x
#else
#define kPrint(x)
#endif



class Knot
{

public:
    
    //Empty Knot object constructor
    //Size 0, no crossings initialized
    inline Knot():
    	start(nullptr),
    	mySize(0)
    {}
    
    //Knot constructor from an Extended Gauss Code list
    //knotLength is the length of extGauss 
    	//and must be determined prior to calling this constructor
    //This is the constructor used by main
    	//to first set up the knot to be worked on
    Knot(int extGauss[],int knotLength);
    
    Knot(string inputString);
    
    void constructFromGauss(int extGauss[], int knotLength);
    
    Knot(const Knot &origKnot);
    
    //Return the size of the knot.
    //If the knot has n crossing objects in it,
    	//mySize will be n/2, since each crossing object
    	//refers to only one half of the overall crossing
    inline int size() const {return mySize;}
    
    //Returns a string of the non-ext Gauss Code list
    string toGaussString() const;
    
    //Returns a string of the Extended Gauss Code list
    string toExtGaussString() const;
    
    int * toArray() const;
    
    //Prints the knot as a readable list of numbers (non-ext Gauss Code)
    void display(ostream & out) const;
    
    //Destructor
    //Deletes all crossing objects
    ~Knot();
    
    void deconstruct();
    
    //Knot assignment operator, similar to copy constructor
    Knot & operator=(const Knot &origKnot);
    
    
    //------------------------------------
    
    //Tries to perform Reduction Move 1, returns true/false on success/failure
    bool rm1();
    
    //Tries to perform Reduction Move 2, returns true/false on success/failure
    bool rm2();
    
    bool dummyRM1();
    
    bool dummyRM2();
    
    bool remove1Tangles();
    
    void reinsert1Tangles();
    
    bool tm2();
    
    
    //Gets length of the longest continuous over/underpass segment
    int getLongestStrandLength();
    
    int crossingTurnTest();
    
    int findStrandsOfLengthTest();
    
    
private:
    
    class Crossing
    {
    public:
        
        int num;
        int sign;
        int handedness;
        Crossing * next;
        Crossing * prev;
        Crossing * neg;
        
        Crossing(int numValue):
        	num(numValue), 
        	sign(0), 
        	handedness(0), 
        	next(nullptr), 
        	prev(nullptr), 
        	neg(nullptr)
        {}
        
        Crossing(const Crossing &origCrossing);
        

        Crossing & operator=(const Crossing &origCrossing);
        
        inline int getNum() {return num;}
        inline int getSign() {return sign;}
        inline int getHandedness() {return handedness;}
        
        Crossing * advance(int direction);
        Crossing * recede(int direction);
        Crossing * turnLeft(int & direction);
        Crossing * turnRight(int & direction);
        
        Crossing * dummyTurnLeft(int direction);
        Crossing * dummyTurnRight(int direction);
        
        
        
    };
    
    Crossing * find(int numToFind, int signOfNum);
    Crossing * find(int numToFindWithSign);
    int findIndex(Crossing* crossingToFind);
    
    int lengthOfSeg(Crossing* startCrossing, Crossing* endCrossing);
    
    //Insert a crossing at a specified index. Not currently used
    void insert(int index, int numValue);
    
    //Removes a crossing from a given index, being sure to reconnect the knot structure around it
    void erase(int index);
    
    //Removes a given crossing based on a pointer to it
    void erase(Crossing* crossingA);
    
    bool is1Tangle(Crossing* startCrossing, Crossing* endCrossing);
    
    bool remove1TanglesHelper(Crossing*& startCrossing, Crossing*& endCrossing);
    
    bool turnTrace(Crossing* strandPtr, int strandLength, int side, vector<string>& triedDiagrams);
    
    bool turnTraceHelper(Crossing* currentCrossing, Crossing** strandArray, int strandLength, Crossing** pathArray, int& pathLength, int numIntersections, int direction);
    
        
    //Locates all possible strands of given length
    //crossingPtrArray is given as an empty array,
        //and gets filled with starting crossings for each strand
    void findStrandsOfLength(int length, int* crossingArray, int& arrayLength);
    
    //Starting crossing of the knot
    //This is somewhat arbitrary
    //but we need one pointer to be our reference to the knot
    Crossing* start;
    
    //Size of the knot. A knot with n crossings will have mySize of n/2
    int mySize;
    
    Crossing* startCrossing1Tangle;
    Crossing* endCrossing1Tangle;

    
};


ostream& operator<<(ostream& out, const Knot &myKnot);

#endif
