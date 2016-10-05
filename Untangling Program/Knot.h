#ifndef KNOT
#define KNOT

#include <iostream>
using namespace std;

//----- Add typdef statement here

class Knot
{

/*
 * SUGGESTION: The header file for your class is where you look to see what the
 * interface (the public members) of the class is. So typically the public
 * stuff goes above the private stuff.
 */
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
        
        /*
         * SUGGESTION: You can use an initializer list rather than
         * assignments like so:
         *   Crossing(int numValue) 
         *     : num(numValue),
         *       sign(0),
         *       ...
         *       neg(nullptr)
         *   {}
         *
         * Probably cleaner to put this in the implementation file as well.   
         */
        Crossing(int numValue) {num=numValue; sign=0; handedness=0; next=nullptr; prev=nullptr; neg=nullptr;}
        
        Crossing(Crossing const &origCrossing);
        
        /*
         * SUGGESTION: "Crossing const" and "const Crossing" accomplish the
         * same thing, but it's good to pick one direction and stick with it.
         */ 

        Crossing & operator=(const Crossing &origCrossing);
        
        inline int getNum() {return num;}
        inline int getSign() {return sign;}
        inline int getHandedness() {return handedness;}
        
        
    };
    
    
    
public:
    /*
     * SUGGESTION: The public member functions are the interface to the class,
     * and so they should be well documented in the header.
     */

    // SUGGESTION: I think Crossing * is clearer and takes up less space
    // than CrossingPointer
    typedef Crossing * CrossingPointer;
    
    
    // SUGGESTION: This can also be an initializer list
    inline Knot() {start=nullptr; mySize=0;}
    
    
    Knot(int extGauss[],int knotLength);
    
    
    Knot(Knot const &origKnot);
    
    
    inline int size() const {return mySize;}
    
    
    void display(ostream & out) const;

    
    // SUGGESTION: Does insert and erase need to be public?
    void insert(int index, int numValue);
    

    void erase(int index);

    
    void erase(CrossingPointer crossingA);
    
    
    ~Knot();
    
    
    Knot & operator=(const Knot &origKnot);
    
    
    //------------------------------------
    
    bool rm1();
    
    bool rm2();
    
    
    // SUGGESTION: Do these need to be public?
    CrossingPointer start;
    int mySize;
    
};


ostream& operator<<(ostream& out, const Knot &myKnot);

#endif
