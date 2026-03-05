#include "activityDef.hpp"

ActivityDef activities[] = {
    // Q1=10, Q2=15, Q3=20, Q4=25, Q5=30 (total 100)
    {1, "Ch1-Q1", "What is a data structure?", 10,
        {"A. A way to store and organize data", "B. A type of algorithm", "C. A programming language", "D. None of the above"}, 0},
    {2, "Ch1-Q2", "Which is a linear data structure?", 15,
        {"A. Stack", "B. Queue", "C. Array", "D. Graph"}, 2},
    {3, "Ch1-Q3", "Which is not a primitive data structure?", 20,
        {"A. Integer", "B. Float", "C. Array", "D. Char"}, 2},
    {4, "Ch1-Q4", "Which operation is not possible on arrays?", 25,
        {"A. Traversal", "B. Insertion", "C. Deletion", "D. Random access"}, 2},
    {5, "Ch1-Q5", "Which is used for LIFO?", 30,
        {"A. Stack", "B. Queue", "C. Array", "D. Tree"}, 0},
    // Chapter 2
    {6, "Ch2-Q1", "What is the time complexity of push in stack?", 10,
        {"A. O(1)", "B. O(n)", "C. O(log n)", "D. O(n log n)"}, 0},
    {7, "Ch2-Q2", "Which data structure is FIFO?", 15,
        {"A. Stack", "B. Queue", "C. Tree", "D. Graph"}, 1},
    {8, "Ch2-Q3", "Which operation removes from the front in a queue?", 20,
        {"A. Push", "B. Pop", "C. Dequeue", "D. Enqueue"}, 2},
    {9, "Ch2-Q4", "Which is not a stack application?", 25,
        {"A. Recursion", "B. Expression evaluation", "C. Undo operation", "D. Level order traversal"}, 3},
    {10, "Ch2-Q5", "What is the maximum size of a circular queue?", 30,
        {"A. n", "B. n-1", "C. n+1", "D. Unlimited"}, 1},
    // Chapter 3
    {11, "Ch3-Q1", "What does a node in a singly linked list contain?", 10,
        {"A. Data and pointer", "B. Only data", "C. Only pointer", "D. Data and two pointers"}, 0},
    {12, "Ch3-Q2", "Which operation is fastest in a linked list?", 15,
        {"A. Access", "B. Insertion at head", "C. Deletion at tail", "D. Search"}, 1},
    {13, "Ch3-Q3", "What is the head of a linked list?", 20,
        {"A. Last node", "B. First node", "C. Middle node", "D. None"}, 1},
    {14, "Ch3-Q4", "Which is not a type of linked list?", 25,
        {"A. Singly", "B. Doubly", "C. Triply", "D. Circular"}, 2},
    {15, "Ch3-Q5", "What is the time complexity to access the nth node?", 30,
        {"A. O(1)", "B. O(log n)", "C. O(n)", "D. O(n log n)"}, 2}
};

const int numActivities = sizeof(activities) / sizeof(activities[0]);
