#include <iostream>
#include <string>
#include <sstream>
#include "ClassLinkedList.hpp" //quotations mean: look in present directory for this header

//------------------------------------------------------------------------------------------------------------
//Constructors

TypeLinkedList::TypeLinkedList ()
{   //std::cout<<"Creating an empty linked list\n";
    Size=0;
	Beginning=nullptr;
	Ending=nullptr;
	Bookmark.Index=-1; Bookmark.ThisNode=nullptr;
}

TypeLinkedList::TypeLinkedList (int thisVal)
{   //std::cout<<"Creating a linked list for a value "<<thisVal<<"\n";
    Size=0;
	Add(thisVal,-1);
}

//Destructor
TypeLinkedList::~TypeLinkedList ()
{
	//std::cout<<"\nInside destructor\n";

	Node* NextOne;
	while (Beginning != nullptr)
	{
		NextOne = Beginning->Next; //Store location of next node before deleting the current one
		delete Beginning;
		Beginning = NextOne;
	}

	//std::cout<<"List Destroyed\n";
}


//--------------------------------Methods------------------------------------------------------------------------

//Method to Report the contents of the list
std::string TypeLinkedList::Report ()
{
	Node* Runner=Beginning; //A pointer parsing the entire list of nodes sequencially
	std::stringstream ss;	//C++ can't even convert int to string without jumping hoops (I thought std::to_string was supported by C++11, but couldn't compile)

	while (Runner != nullptr)
	{
		ss<<Runner->Data;
		Runner=Runner->Next;
		if (Runner != nullptr ) ss<<" "; //Add a space only if not at the end
	}
	return ss.str();
}

//Append, append_data, insert, insert_data all in one
bool TypeLinkedList::Add (long Value, long AtIndex)
{
	//Exit if out of bounds (AtIndex=Size is a special condition we can handle)
	if (AtIndex<-1 || AtIndex>Size) return false;
	//In the future, may want to consider adding blank nodes if the AtIndex is beyond the current size

	Node* NewNode = new Node {Value,nullptr,nullptr};
    return Attach (NewNode, AtIndex);
}

bool TypeLinkedList::Attach (Node* ThisNode, long AtIndex)
{
    //Exit if out of bounds (AtIndex=Size is a special condition we can handle)
	if (AtIndex<-1 || AtIndex>Size) {std::cout<<"Cannot attach node to an out of bounds ["<<AtIndex<<"] linked-list location"; return false;}

    Bookmark.Index=AtIndex; Bookmark.ThisNode = ThisNode;
	if (Size>0)
	{
		if (AtIndex==-1 || AtIndex==Size) //Client implicitly or explicitly requests adding AFTER the end of the list
		{
			Ending->Next=ThisNode;		            //Link the formerly last node to the new one
			ThisNode->Previous=Ending;	            //Link the new node to the formerly last one
			Ending=ThisNode;    		            //Shift the ending pointer to the new node
			ThisNode->Next=nullptr;
		}
		else if (AtIndex==0) //Client requests making the new node the beginning of the list
		{
			ThisNode->Next=Beginning;       	    //Link the new node to the former beginning
			Beginning=ThisNode;			            //Set the beginning pointer to the new node
			Beginning->Next->Previous=ThisNode;     //Link baqckwards the former beginning node to the new node
			ThisNode->Previous=nullptr;
		}
		else //There are more than one nodes and client requests insertion somewhere in between
		{
			ThisNode->Next=AddressOfIndex(AtIndex);  //Link NewNode to the existing node at this index
			ThisNode->Previous=ThisNode->Next->Previous; //Link the NewNode to the preceding node from this index
			ThisNode->Next->Previous=ThisNode;        //Link backwords the existing node at index to the NewNode
			ThisNode->Previous->Next=ThisNode;        //Link forward the preceding node to NewNode; thus completnig the insertion
		}

	}
	else //List is empty and client might have requested either Index=-1 or Index=0
	{
		Beginning=ThisNode;
		Ending=ThisNode;
		Bookmark.Index=0;
		ThisNode->Next=nullptr;
		ThisNode->Previous=nullptr;
	}

	Size++;
	return true;
}

//Delete a node at the corresponding index
TypeLinkedList::Node* TypeLinkedList::Detach (long AtIndex)
{
	if (AtIndex < 0 || AtIndex > (Size-1)) return nullptr;

	Node* Temp=AddressOfIndex(AtIndex);

	//LinkThe previous and next nodes to each other
	if (Temp->Previous != nullptr && Temp->Next != nullptr) // This node is in between two others
	{
		Temp->Next->Previous = Temp->Previous;
		Temp->Previous->Next = Temp->Next;
		Bookmark.Index=AtIndex; Bookmark.ThisNode=Temp->Next;
	}
	else if (Temp->Previous != nullptr && Temp->Next == nullptr) // It is the end node
	{
		Temp->Previous->Next = nullptr;
		Ending=Temp->Previous;
		Bookmark.Index=AtIndex-1; Bookmark.ThisNode=Ending;
	}
	else if (Temp->Previous == nullptr && Temp->Next != nullptr) // It is the beginning node
	{
		Temp->Next->Previous = nullptr;
		Beginning=Temp->Next;
		Bookmark.Index=0; Bookmark.ThisNode=Beginning;
	}
	else // It is the last remaining node
	{
		Beginning=Ending=nullptr;
		Bookmark.Index=-1; Bookmark.ThisNode=nullptr;
	}

	Size--;
	return Temp;
}

bool TypeLinkedList::Delete (long AtIndex)
{
    Node* Temp=Detach(AtIndex);
    if(Temp==nullptr) return false;

	delete Temp; return true;
}

bool TypeLinkedList::DeleteValue (long DataValue)
{
    if (Size==0) return false;
    long FoundIndex=FindIndexOf(DataValue);
    if (FoundIndex==-1) return false;
    return Delete(FoundIndex);
}

// Return location of node at the corresponding index
TypeLinkedList::Node* TypeLinkedList::AddressOfIndex (long ThisIndex)
{
    if (ThisIndex<-1 || ThisIndex>(Size-1)) return nullptr;
    MoveBookmarkTo (ThisIndex);
    return Bookmark.ThisNode;
}

long TypeLinkedList::ValueAtIndex (long ThisIndex)
{
    if (ThisIndex<-1 || ThisIndex>(Size-1)) return -1;
    MoveBookmarkTo (ThisIndex);
    return Bookmark.ThisNode->Data;
}

void TypeLinkedList::MoveBookmarkTo (long ThisIndex)
{
    if (ThisIndex<-1 || ThisIndex>(Size-1)) return;   //check validity of request
    if (ThisIndex==Bookmark.Index) return;            //Trivial easy case

    Node* Runner;
    long Counter;
    long fromMark=ThisIndex-Bookmark.Index;
    long fromEnd=Size-1-ThisIndex;
    short dir; if (fromMark<0) {dir=-1; fromMark=-fromMark;} else {dir=1;}

    if (ThisIndex<=fromMark) {Runner=Beginning; Counter=0; dir=1;}      //Start at the beginning
    else if (fromEnd<fromMark) {Runner=Ending; Counter=Size-1; dir=-1;} //Start at the end. Go backwards
    else {Runner=Bookmark.ThisNode; Counter=Bookmark.Index;}            //Start at the bookmark. Either backwards or forward

	while (Counter*dir < ThisIndex*dir)
	{
		if (dir>0) {Runner=Runner->Next;} else {Runner=Runner->Previous;}
		Counter=Counter+dir;
		//std::cout<<".";
	}
    Bookmark.Index=ThisIndex; Bookmark.ThisNode=Runner;
}

//Change the value of a node at the corresponding index
void TypeLinkedList::SetValue (long Value, long AtIndex)
{ if (AtIndex >-1 && AtIndex<Size) AddressOfIndex(AtIndex)->Data = Value; }

//Return the size of the list
long TypeLinkedList::getListSize() { return Size; }

//Search the list for a specific vlaue and if found return the index of the node where it is at
long TypeLinkedList::FindIndexOf (long Value)
{
	Node* Runner=Beginning;
	long Counter=0;

	while (Runner != nullptr)
	{
		if (Runner->Data == Value) return Counter;
		Runner=Runner->Next;
		Counter++;
	}

	return -1; //If the value is not found in the etire list return -1;
}
