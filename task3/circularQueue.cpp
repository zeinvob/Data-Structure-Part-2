#include "circularQueue.hpp"

CircularQueue::CircularQueue(int size)
{
    capacity = size;
    front = 0;
    rear = -1;
    count = 0;
}

bool CircularQueue::isEmpty() const
{
    return count == 0;
}

bool CircularQueue::isFull() const
{
    return count == capacity;
}

int CircularQueue::getCount() const
{
    return count;
}

void CircularQueue::clear()
{
    front = 0;
    rear = -1;
    count = 0;
}

void CircularQueue::enqueue(const ActivityResult &item)
{
    if (isFull())
    {
        front = (front + 1) % capacity;
    }
    else
    {
        count++;
    }

    rear = (rear + 1) % capacity;
    data[rear] = item;
}

ActivityResult CircularQueue::getAt(int logicalIndex) const
{
    int actualIndex = (front + logicalIndex) % capacity;
    return data[actualIndex];
}