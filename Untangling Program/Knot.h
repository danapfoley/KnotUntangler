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

#define knotDebug 0

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
    Knot(vector<int>& extGauss);
    
    //Knot constructor from string
    //This just creates an array and then calls constructfromGauss
    Knot(string inputString);
    
    //Constructs a knot object given an Extended Gauss code array
        //and its length
    void constructFromGauss(vector<int>& extGauss);
    
    //Knot copy constructor
    Knot(const Knot &origKnot);

    static Knot move(Knot &origKnot);
    
    //Return the size of the knot.
    //If the knot has n crossing objects in it,
    	//size() will return n/2, since each crossing object
    	//refers to only one half of the overall crossing
    inline int size() const {return mySize;}
    
    //Returns a string of the non-ext Gauss Code list
    //Using this should be avoided, as it can cause confusion
        //when expecting Extended Gauss Code
    string toGaussString() const;
    
    //Returns a string of the Extended Gauss Code list
    string toExtGaussString() const;
    
    //Turns the knot into an array
    //The size of the new array can be determined from the knot obj
    vector<int> toVector() const;
    
    //Prints the knot as a readable list of numbers (Extended Gauss Code)
    void display(ostream & out) const;
    
    //Destructor
    //calls deconstruct() and that's it
    ~Knot();
    
    //Deconstructor. Deletes all crossing objects, freeing all memory
    void deconstruct();
    
    //Knot assignment operator, similar to copy constructor
    Knot & operator=(const Knot &origKnot);
    
    
    //------------------------------------
    
    //Tries to perform Reduction Move 1, returns true/false on success/failure
    bool rm1();
    
    //Tries to perform Reduction Move 2, returns true/false on success/failure
    bool rm2();
    
    //Returns true/false if RM1 is possible, but does not execute it
    bool dummyRM1();
    
    //Returns true/false if RM2 is possible, but does not execute it
    bool dummyRM2();
    
    //Returns true if any possible configuration of the knot
        //at current complexity can be simplified
    bool TM();
    
    //Removes all 1-tangles from the knot, keeping track of them
        //to be reinserted later
    bool remove1Tangles();
    
    //Reinserts all 1-tangles at the end of the knot
    void reinsert1Tangles();
    
    //Gets length of the longest continuous over/underpass segment
    int getLongestStrandLength();
    
    int crossingTurnTest();
    
    int findStrandsOfLengthTest();
    
    
private:
    
    class Crossing
    {
    public:
        
        //Numeric ID for a crossing in a knot
        //Two crossing objects in a knot will have the same number
        int num;
        
        //Sign of a crossing object
        //+1 or -1 for over/undercrossing
        //One crossing with a num will have +1, the other will have -1
        int sign;
        
        //Handedness of a crossing
        //+1 for right-handed, -1 for left-handed
        //Both objects representing the same crossing will have the same handedness
        int handedness;
        
        //Pointer to the next crossing in the knot
            //with respect to order of tracing
        shared_ptr<Crossing> next;
        
        //Pointer to the previous crossing in the knot,
            //with respect to order of tracing
        shared_ptr<Crossing> prev;
        
        //Pointer to the complementary crossing object
        //Two crossing objs with the same num will have neg ptrs
            //that aim at each other
        shared_ptr<Crossing> neg;
        
        //Default constructor
        Crossing(int numValue):
        	num(numValue), 
        	sign(0), 
        	handedness(0), 
        	next(nullptr), 
        	prev(nullptr), 
        	neg(nullptr)
        {}
        
        //Copy constructor
        Crossing(const Crossing &origCrossing);
        
        //Assignment operator. Works the same way as copy constructor
        Crossing& operator=(const Crossing &origCrossing);
        
        //Simple get functions for members
        inline int getNum() {return num;}
        inline int getSign() {return sign;}
        inline int getHandedness() {return handedness;}
        
        
        //-----Turning functions-----
        
        //Advance moves forward/backward along a crossing
        //Depending on direction (1 == next, -1 == prev)
        shared_ptr<Crossing> advance(int direction);
        
        //Recede is the opposite of advance
        //(1 == prev, -1 == next)
        shared_ptr<Crossing> recede(int direction);
        
        //TurnLeft/Right use handedness information
        //to take a current tracing direction (1 or -1)
        //and turns left/right with respect to the current crossing
        //the direction var is then updated
            //so tracing can continue correctly
        shared_ptr<Crossing> turnLeft(int & direction);
        shared_ptr<Crossing> turnRight(int & direction);
        
        //DummyTurn functions do the same thing as regular turns
        //Except the direction var is not changed
        shared_ptr<Crossing> dummyTurnLeft(int direction);
        shared_ptr<Crossing> dummyTurnRight(int direction);
        
        
        
    };
    
    //Utilities for finding a crossing object
        //based on matching num and sign
    shared_ptr<Crossing> find(int numToFind, int signOfNum);
    shared_ptr<Crossing> find(int numToFindWithSign);
    
    //Gets the numerical index of a crossing within a knot
        //With respect to the start ptr
    //0-indexed
    int findIndex(shared_ptr<Crossing> crossingToFind);
    
    //Gets the length of a segment of crossings
    //Length of a to b:
        //[..., a, b, ...] = 2
        //[..., a, d, e, f, b, ...] = 5
    int lengthOfSeg(shared_ptr<Crossing> startCrossing, shared_ptr<Crossing> endCrossing);
    
    //Insert a crossing at a specified index. Not currently used
    void insert(int index, int numValue);
    
    //Removes a crossing from a given index, being sure to reconnect the knot structure around it
    void erase(int index);
    
    //Removes a given crossing based on a pointer to it
    void erase(shared_ptr<Crossing> crossingA);
    
    //Returns true if a segment of a knot, denoted by start/endCrossing,
        //is a complete 1-tangle
    //That is - if all crossings located in the range are completed within the range
    bool is1Tangle(shared_ptr<Crossing> startCrossing, shared_ptr<Crossing> endCrossing);
    
    bool remove1TanglesHelper(shared_ptr<Crossing>& startCrossing, shared_ptr<Crossing>& endCrossing);
    
    bool turnTrace(int firstCrossingNum, int strandLength, int side, vector<string>& triedDiagrams);
    
    bool turnTraceHelper(shared_ptr<Crossing> currentCrossing, vector<shared_ptr<Crossing>>& strandVector, vector<shared_ptr<Crossing>>& pathVector, int& pathLength, int numIntersections, int direction);
    
        
    //Locates all possible strands of given length
    vector<int> findStrandsOfLength(int length);
    
    //Starting crossing of the knot
    //This is somewhat arbitrary
    //but we need one pointer to be our reference to the knot
    shared_ptr<Crossing> start;
    
    //Size of the knot. A knot with n crossings will have mySize of n/2
    int mySize;
    
    //Pointers for keeping track of detatched 1-tangles
    //Multiple 1-tangles will be connected end-to-end
    //And we use these ptrs to hold onto them
    shared_ptr<Crossing> startCrossing1Tangle;
    shared_ptr<Crossing> endCrossing1Tangle;

    
};


ostream& operator<<(ostream& out, const Knot &myKnot);

#endif
