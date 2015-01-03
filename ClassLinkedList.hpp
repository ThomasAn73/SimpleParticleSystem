#ifndef TypeLinkedList_H_
#define TypeLinkedList_H_
#include <string>

class TypeLinkedList //class declaration
{
protected:
//Protected variables (visible to dirived classes)
    struct Node //Node element to be daisy-chained into a list
    {
        long Data;
        Node* Next;
        Node* Previous;
    };

//Protected Methods
    Node* AddressOfIndex (long ThisIndex); //Returns the location of the node corresponding to that index

private:
    struct TypePos { long Index; Node* ThisNode; };

    Node* Beginning; //Will be set to NULL by the constructor
    Node* Ending;    //Will be set to NULL by the constructor
    long Size;       //Keeps truck of number of elements currently in the list (could give incorrect value if size of list exceeds MAX_Limit of long)

    TypePos Bookmark;

//Private Methods
    void MoveBookmarkTo (long ThisIndex);

public:
//Constructors
    TypeLinkedList (); //Default constructor
    TypeLinkedList (int Value); //Overloaded constructor that accepts a value to inititalize first node
//Destructor
    ~TypeLinkedList (); //Destructor (take care of memory leaks; delete all nodes from memory)


//Public Methods
    void SetValue (long Value, long AtIndex); //Set value of an existing node at specified index

    Node* Detach (long AtIndex);              //Detach node at specified index, Return a pointer to the detached node;
    bool Delete (long AtIndex);               //Delete node at specified index, Return FALSE if not able to;
    bool DeleteValue (long DataValue);
    bool Attach (Node* ThisNode, long AtIndex=-1);
    bool Add (long Value=0, long AtIndex=-1); //(append, append_data, insert, insert_data all in one) Default index of -1 means it will append at the end of the list unless otherwise specified (careful for out of memory condition)
    long FindIndexOf (long Value);	          //Search the list for the specified value and return the index where it is at, or -1 if not found
    long ValueAtIndex (long ThisIndex);
    long getListSize ();		              //Report the current size of the list

    std::string Report ();	        //Return a space delimited string of all list data in order.
};

#endif
