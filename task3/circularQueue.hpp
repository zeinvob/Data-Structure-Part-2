#ifndef CIRCULARQUEUE_HPP
#define CIRCULARQUEUE_HPP

#include "../model/activityResult.hpp"

class CircularQueue
{
private:
    ActivityResult data[100];
    int front;
    int rear;
    int count;
    int capacity;

public:
    CircularQueue(int size = 100);

    bool isEmpty() const;
    bool isFull() const;
    int getCount() const;

    void clear();
    void enqueue(const ActivityResult &item);
    ActivityResult getAt(int logicalIndex) const;
};

#endif