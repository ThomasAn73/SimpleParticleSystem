#ifndef ClassListsMatrix_0101110_h
#define ClassListsMatrix_0101110_h

#include "ClassLinkedList.hpp"

//This creates objects of type: matrix of linked lists
//It has finite XY dimensions, specified at start, but has unlimited cell capacity
class TypeListsMatrix : public TypeLinkedList
{
private:
    unsigned GridXsize, GridYsize;  //This *size* not index
    TypeLinkedList*** CoreGrid; //This is going to be a 2D array of pointers to linked lists

public:
//constructor
    TypeListsMatrix ();
    TypeListsMatrix (unsigned Xsize, unsigned Ysize);
    ~TypeListsMatrix (); //destructor, delete entire matrix free mamory

//Public methods
    void Format (bool DestroyBaseMatrix=false);                         //format the Matrix set all pointers to null (if linked lists exist, delete them)
    bool Initialize (unsigned Xsize, unsigned Ysize);                   //Sometimes we need to set dimensions after a default constructor was called
    bool AddValueAt (unsigned Xindex, unsigned Yindex, long DataVal);   //add data-item to a grid locale (never mind about duplicate checking)
    bool DeleteValueAt (unsigned Xindex, unsigned Yindex, long DataVal);
    bool MoveDataValue (unsigned FromX, unsigned FromY, unsigned ToX, unsigned ToY, long DataVal);
    long getValueFrom (unsigned Xindex, unsigned Yindex, unsigned ListIndex);
    long getCellPopulation (unsigned Xindex, unsigned Yindex);
};

#endif
