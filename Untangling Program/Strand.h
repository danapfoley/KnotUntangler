



#ifndef Strand_h
#define Strand_h

#include "Knot.h"

class Strand
{
    
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
        
        
        Crossing(int numValue) {num=numValue; sign=0; handedness=0; next=nullptr; prev=nullptr; neg=nullptr;}
        
        Crossing(Crossing const &origCrossing);
        
        Crossing & operator=(const Crossing &origCrossing);
        
        inline int getNum() {return num;}
        inline int getSign() {return sign;}
        inline int getHandedness() {return handedness;}
        
        
    };
    
    
    
public:
    
    typedef Crossing * CrossingPointer;
    
    
    inline Strand() {start=nullptr; mySize=0;}
    
    
    Strand(int extGauss[],int strandLength);
    
    
    Strand(Strand const &origStrand);
    
    
    inline int size() const {return mySize;}
    
    
    void display(ostream & out) const;
    
    
    void insert(int index, int numValue);
    
    
    void erase(int index);
    
    
    void erase(CrossingPointer crossingA);
    
    
    ~Strand();
    
    
    Strand & operator=(const Strand &origStrand);
    
    
    //------------------------------------
    
    bool rm1();
    
    bool rm2();
    
    
    CrossingPointer start;
    int mySize;
    
};


ostream& operator<<(ostream& out, const Strand &myStrand);


#endif /* Strand_h */
