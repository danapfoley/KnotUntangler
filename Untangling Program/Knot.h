#ifndef KNOT
#define KNOT

#include "Naming.h"



//----- Add typdef statement here

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
    
    //Knot assignment operator, similar to copy constructor
    Knot & operator=(const Knot &origKnot);
    
    
    //------------------------------------
    
    //Tries to perform Reduction Move 1, returns true/false on success/failure
    bool rm1();
    
    //Tries to perform Reduction Move 2, returns true/false on success/failure
    bool rm2();
    
    //Knot tm1();
    
    void tm2();
    
    //Gets length of the longest continuous over/underpass segment
    int getLongestStrandLength();
    
    
private:
    /*
     * SUGGESTION: Even though you're only going to use the Crossing class
     * within the Knot class, I think it's cleaner to put it in its own h/cpp
     * files. That way when you read the interface to Knot you don't need to
     * look at the Crossing interface as well, which you will never use outside
     * of Knot.
     */ 
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
        
        
    };
    
    
    //Insert a crossing at a specified index. Not currently used
    void insert(int index, int numValue);
    
    //Removes a crossing from a given index, being sure to reconnect the knot structure around it
    void erase(int index);
    
    //Removes a given crossing based on a pointer to it
    void erase(Crossing * crossingA);
    
    //Starting crossing of the knot
    //This is somewhat arbitrary
    //but we need one pointer to be our reference to the knot
    Crossing * start;
    
    //Size of the knot. A knot with n crossings will have mySize of n/2
    int mySize;

    
};


ostream& operator<<(ostream& out, const Knot &myKnot);

#endif