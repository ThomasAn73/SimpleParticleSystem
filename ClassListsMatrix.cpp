#include "ClassListsMatrix.hpp"
#include <iostream>

//***TYPE GRIDofLISTS class implementation****************************************************************
//********************************************************************************************************

//---Constructor/Destructor-------------------------------------------------------------------------------
TypeListsMatrix::TypeListsMatrix ()
{   //Default constructor (we often do not know the size of the grid but the Grid object is constructed anyway)
    GridXsize=0;
    GridYsize=0;
    CoreGrid=nullptr; //std::cout<<"Null matrix here\n";
}

TypeListsMatrix::TypeListsMatrix (unsigned Xsize, unsigned Ysize)
{ Initialize(Xsize,Ysize); }

TypeListsMatrix::~TypeListsMatrix ()
{ Format(true); }

//---Methods----------------------------------------------------------------------------------------------
bool TypeListsMatrix::Initialize (unsigned Xsize, unsigned Ysize)
{
    if (CoreGrid!=nullptr) return false; //It is already initialized
    if (Xsize==0) Xsize=1;
    if (Ysize==0) Ysize=1;
    GridXsize=Xsize; GridYsize=Ysize;

    CoreGrid= new TypeLinkedList**[Xsize];  //Assign this tripple pointer a new array of double pointers
    for (unsigned i=0;i<Xsize;i++)          //For each double pointer in the array assign a new array of single pointers
        { CoreGrid[i]=new TypeLinkedList*[Ysize]; }

    for (unsigned i = 0; i < Xsize; ++i)    //Null all the single pointers in the array
        {for (unsigned j = 0; j < Ysize; ++j) {CoreGrid[i][j] = nullptr;} }

    std::cout<<"Initialized matrix to "<<GridXsize<<", "<<GridYsize<<"\n";
    return true;
}

void TypeListsMatrix::Format (bool DestroyBaseMatrix)
{
    if (CoreGrid==nullptr  || GridXsize==0 || GridYsize==0) return;

    for (unsigned i=0; i<GridXsize; ++i)    //delete all linked lists from the 2d array
    {   for (unsigned j=0; j<GridYsize; ++j)
        {
            if (CoreGrid[i][j] != nullptr) delete CoreGrid[i][j];   //Delete the linked list objects
            if (DestroyBaseMatrix==false) CoreGrid[i][j]=nullptr;
        }
        if (DestroyBaseMatrix==true) {delete [] CoreGrid[i];}       //Delete the Y 1D arrays of pointers
    }
    if (DestroyBaseMatrix==true) {delete [] CoreGrid;}              //Delete the X 1D array of pointers
}

bool TypeListsMatrix::AddValueAt (unsigned Xindex, unsigned Yindex, long DataVal)
{
    if(Xindex>=GridXsize || Yindex>=GridYsize) {std::cout<<"Cannot add to an out of bounds ["<<Xindex<<","<<Yindex<<"] index\n"; return false;}
    TypeLinkedList*& Cell=CoreGrid[Xindex][Yindex];
    if (Cell==nullptr) {Cell=new TypeLinkedList (DataVal);} else {Cell->Add(DataVal);}
    return true;
}

bool TypeListsMatrix::DeleteValueAt (unsigned Xindex, unsigned Yindex, long DataVal)
{
    if(Xindex>=GridXsize || Yindex>=GridYsize) {std::cout<<"Cannot delete from an out of bounds ["<<Xindex<<","<<Yindex<<"] index\n"; return false;}
    TypeLinkedList*& Cell=CoreGrid[Xindex][Yindex];
    if (Cell==nullptr) {return false;}

    return Cell->DeleteValue(DataVal);
}

bool TypeListsMatrix::MoveDataValue (unsigned FromX, unsigned FromY, unsigned ToX, unsigned ToY, long DataVal)
{
    if(FromX>=GridXsize || FromY>=GridYsize) {std::cout<<"Out of bounds ["<<FromX<<","<<FromY<<"] source move request\n"; return false;}
    if(ToX>=GridXsize || ToY>=GridYsize) {std::cout<<"Out of bounds ["<<ToX<<","<<ToY<<"] destination move request"; return false;}

    TypeLinkedList*& FromCell=CoreGrid[FromX][FromY];   //Get a handle to the source linked list
    if (FromCell==nullptr) {std::cout<<"Cannot move from NULL source cell ["<<FromX<<","<<FromY<<"]\n"; return false;}

    long FromIndex=FromCell->FindIndexOf(DataVal);      //locate the index of the value (if found)
    if (FromIndex==-1) {std::cout<<"Value was NOT found while searching inside ["<<FromX<<","<<FromY<<"] source cell\n"; return false;}

    if(FromX==ToX && FromY==ToY) {return true;}         //No need to do anything, source and destination are the same spot

    Node* DetachedNode=FromCell->Detach(FromIndex);     //detach the found node from the linked list
    if (DetachedNode==nullptr) {std::cout<<"Failed to detach source cell Node\n"; return false;}

    TypeLinkedList*& ToCell=CoreGrid[ToX][ToY];         //Get a handle of the destination linked list
    if (ToCell==nullptr) {ToCell=new TypeLinkedList();}

    return ToCell->Attach(DetachedNode);


}

long TypeListsMatrix::getValueFrom (unsigned Xindex, unsigned Yindex, unsigned ListIndex)
{
    long CellPopul=getCellPopulation(Xindex,Yindex);
    if(CellPopul==0 || ListIndex>=unsigned(CellPopul)) return -1;

    return CoreGrid[Xindex][Yindex]->ValueAtIndex (ListIndex);
}

long TypeListsMatrix::getCellPopulation (unsigned Xindex, unsigned Yindex)
{   //if someone tries to set Xindex to -1 then Xindex cycles to its maximum value and will cause it to be out of grid index bounds anyway
    if(CoreGrid==nullptr || Xindex>=GridXsize || Yindex>=GridYsize) return 0;
    if (CoreGrid[Xindex][Yindex] == nullptr) return 0;

    return CoreGrid[Xindex][Yindex]->getListSize();
}
