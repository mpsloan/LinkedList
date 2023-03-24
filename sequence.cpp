/* 
Michael Sloan
Project #3 - Linked Sequence Data Structure
This project implements a multitude of methods that can be used to
alter a sequence object. A Sequence object is made of SequenceNodes
that are linked to each other with a previous and next pointer. Each
sequence has one head and one tail. Each sequence node contains an integer
as it's element or data.
*/
#include "sequence.h"
#include <iostream>
using namespace std;

Sequence::Sequence(size_type sz):
    // initializing head and tail to null, and elements to 0
    head(nullptr),
    tail(nullptr),
    numElts(0)
{  
    /* creating sequence using pushback, modifies numElts
        as well as head and tail pointers */
    for (int i = 0; i < sz; i++)
    {
        // initializing with value of zero
        push_back(0);
    }
}

Sequence::Sequence(const Sequence& s): 
    // copy constructor intialization same as regular
    head(nullptr),
    tail(nullptr),
    numElts(0)
{   
    // if list is not empty, if it is do nothing
    if (s.head != nullptr) {
        // copy pointer to head of passed in sequence
        SequenceNode* copy = s.head;
        while (copy != nullptr)
        {
            /* pushing back each element, deals with head and tail
                moving copy to next node each iteration */
            push_back(copy->elt);
            copy = copy->next;
        }
    }
    
}

Sequence::~Sequence()
{
    /* if sequence is empty, nothing to destruct
        if i take conditional out, i get uncaught exception error
        because I'm trying to destruct something that doesn't exist
        in certain test cases */
    if (numElts > 0)
    {
        // clear method deletes the entire sequence
        clear();
    } 
}

Sequence& Sequence::operator=(const Sequence& s)
{
    // if list had elements before assignement, clear them
    if (numElts > 0) {
        clear();
    }

    // same code as copy constructor
    if (s.head != nullptr) { // cycle through each element in s, and copy
        SequenceNode* copy = s.head;
        while (copy != nullptr)
        {
            // add each element, cycle to next node
            push_back(copy->elt);
            copy = copy->next;
        }
    }
    // return pointer
    return *this;
}

Sequence::value_type& Sequence::operator[](size_type position)
{
    // if position is a valid range
    if (0 <= position <= numElts - 1)
    {
        if (head != nullptr) // list isn't empty 
        {
            SequenceNode* current = head;
            // cycle up to element in position
            for (int i = 0; i < position; i++)
            {
                current = current->next;
            }
            // return that element
            return current->elt;
        }
        else { // list is empty, create element
            push_back(0);
            return head->elt;
        }
        
    }
    // else it's an invalid range and throw exception
    else {
        throw exception();
    }
}

void Sequence::push_back(const value_type& value)
{
    // if list is empty, make head node
    if (head == nullptr)
    {
        head = new SequenceNode(value);
        numElts++;
        // only one element so tail and head are same
        tail = head;
    } 
    else { // else cycle through elements and append to end
        SequenceNode* current = head;
        while (current->next != nullptr)
        {
            current = current->next;

        }
        // adding new node to end
        current->next = new SequenceNode(value);
        // set pointers correctly
        SequenceNode* last = current->next;
        last->prev = current;
        // increment elements and set tail
        numElts++;
        tail = last;
    }
}

void Sequence::pop_back()
{
    if (numElts > 0) // there is a node to pop off
    {
        if (numElts == 1) {
            /* pop_back on a 1 node list is
            the same as clearing the entire list */
            clear();
        }
        else { // more than 1 node
            // instead of cycling with a loop, start at the tail
            SequenceNode* last = tail;
            SequenceNode* newLast = tail->prev;
            // fix the pointers
            newLast->next = nullptr;
            tail->prev = nullptr;
            // deleting the previous tail before resetting it
            delete last;
            tail = newLast;
            numElts--;
        }
    }
    else { // no node to pop off, throw exception
        throw exception();
    }
    
}

void Sequence::insert(size_type position, value_type value)
{
    // if position is valid
    if (0 <= position && position <= numElts - 1)
    {
        if (position == 0) // inserting at start of list
        {
            SequenceNode* start = head;
            // new Node before head
            start->prev = new SequenceNode(value);
            SequenceNode* newNode = start->prev;
            // nextNode is previous head
            newNode->next = start;
            // resetting head and increment elements
            head = newNode;
            numElts++;
        }
        else { // inserting anywhere else
            // creating two pointers
            SequenceNode* current = head;
            SequenceNode* newNode = new SequenceNode(value);
            // cycle to right before insertion
            for (int i = 0; i < position - 1; i++)
            {
                current = current->next;
            }
            // change pointers before newNode
            current->next = newNode;
            newNode->prev = current;
            SequenceNode* last = tail;
            // cycle backwards to first node after the newNode
            for (int j = numElts - 1; j > position; j--)
            {
                last = last->prev;
            }
            // set the pointers
            last->prev = newNode;
            newNode->next = last;
            numElts++;
        }
    }
    else { // invalid position, throw exception
        throw exception();
    }
}

const Sequence::value_type& Sequence::front() const
{
    // if list is not empty
    if (numElts > 0)
    {
        // return 1st value
        return head->elt;
    } 
    else { // else list is empty, throw exception
        throw exception();
    }
}

const Sequence::value_type& Sequence::back() const
{
    // if list isn't empty
    if (numElts > 0)
    {
        // return last value in list
        return tail->elt;
    }
    else { // list is empty, throw exception
        throw exception();   
    }
}

bool Sequence::empty() const
{
    // if list has a node, return false
    if (numElts > 0)
    {
        return false;
    }
    else { // no nodes, empty
        return true;
    }
    
}

Sequence::size_type Sequence::size() const
{
    // return the number of nodes
    return numElts;
}

void Sequence::clear()
{
    if (head == nullptr) // empty list, throw exception
    {
        throw exception();
    }
    else { // values in list
        // start at head and destroy every element
        SequenceNode* destroyNode = head;
        while (destroyNode != nullptr)
        {
            /*  pointer to next node, so you can move the node
                to destroy after destroying it without losing your place */
            SequenceNode* destroyNext = destroyNode->next;
            delete destroyNode;
            destroyNode = destroyNext;
        }
        // clearing all pointers and setting elements to 0
        head = nullptr;
        tail = nullptr;
        numElts = 0;
    }
      
}

void Sequence::erase(size_type position, size_type count)
{   // checking if index is valid
    if (position >= 0 && position + count - 1 <= numElts - 1)
    {
        if (position == 0) // if you erase first element, reset head
        {
            SequenceNode* first = head;
            SequenceNode* newFirst = head->next;
            for (int i = 1; i < position + count; i++)
            {
                newFirst = newFirst->next;  
                delete first;
                first = newFirst;
            }
            head = newFirst;
        }
        else { // erasing any other elements
            SequenceNode* current = head;
            // cycle to the position
            for (int i = 0; i < position; i++)
            {
                current = current->next;
            }
            // last node before deletion
            SequenceNode* placeHold = current->prev;
            // node to delete
            SequenceNode* deleteNode = current;
            // similar logic to clear()
            for (int j = 0; j < count; j++) 
            {
                // pointer to keep track of what to delete next
                SequenceNode* deleteNext = deleteNode->next;
                delete deleteNode;
                deleteNode = deleteNext;
            }
            // tail will be deleted
            if (position + count == numElts)
            {
                // setting placeHold to tail
                placeHold->next = nullptr;
                tail = placeHold;
            }
            else { // tail won't be deleted
                // reset the current pointer to the tail
                current = tail;
                // cycle backwards to last node that isn't deleted
                for (int k = numElts - 1; k > position + count; k--)
                {
                    current = current->prev;
                }
                // set pointers
                placeHold->next = current;
                current->prev = placeHold;
            }
            
        }
        numElts = numElts - count;  
    }
    else { // invalid index
        throw exception();
    }
}


ostream& operator<<(ostream& os, const Sequence& s)
{   // starting at head node
    Sequence::SequenceNode* current = s.head;
    // print brack to signify start of list
    os << "[";
    // cycle through list
    while (current != nullptr)
    {
        // print current element
        os << current->elt;
        // need if statement to fix extra comma at end
        if (current->next != nullptr) 
        {
            os << ", ";
        }
        current = current->next;
        
    }
    // end of list
    os << "]" << endl;

    return os;
}