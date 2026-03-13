#include "circularQueue.hpp"

// constructor to set the queue size and starting values
CircularQueue::CircularQueue(int size)
{
    capacity = size;
    front = 0;   // front starts at first position
    rear = -1;   // rear starts before the first element
    count = 0;   // queue is empty at the beginning
}

// checks if the queue has no elements
bool CircularQueue::isEmpty() const
{
    return count == 0;
}

// checks if the queue is full
bool CircularQueue::isFull() const
{
    return count == capacity;
}

// returns how many elements are in the queue
int CircularQueue::getCount() const
{
    return count;
}

// resets the queue to empty state
void CircularQueue::clear()
{
    front = 0;
    rear = -1;
    count = 0;
}

// adds a new item into the queue
void CircularQueue::enqueue(const ActivityResult &item)
{
    // if full, move front forward to overwrite oldest item
    if (isFull())
    {
        front = (front + 1) % capacity;
    }
    else
    {
        count++; // increase count if queue is not full
    }

    // move rear to next position in circular way
    rear = (rear + 1) % capacity;
    data[rear] = item; // store the new item
}

// gets an item using logical position from the front
ActivityResult CircularQueue::getAt(int logicalIndex) const
{
    int actualIndex = (front + logicalIndex) % capacity; // convert to real index
    return data[actualIndex];
}