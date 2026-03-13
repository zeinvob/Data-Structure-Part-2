#ifndef CIRCULARQUEUE_HPP
#define CIRCULARQUEUE_HPP

#include "../model/activityResult.hpp" // includes the activityresult class

class CircularQueue
{
private:
    ActivityResult data[100]; // array to store queue items
    int front;                // points to the front of the queue
    int rear;                 // points to the last item in the queue
    int count;                // keeps track of number of items
    int capacity;             // maximum size of the queue

public:
    CircularQueue(int size = 100); // constructor with default size 100

    bool isEmpty() const;   // checks if the queue is empty
    bool isFull() const;    // checks if the queue is full
    int getCount() const;   // returns the number of items

    void clear();                           // clears all items in the queue
    void enqueue(const ActivityResult &item); // adds a new item to the queue
    ActivityResult getAt(int logicalIndex) const; // gets item at a given position
};

#endif