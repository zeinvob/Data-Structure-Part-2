#ifndef LEARNER_HPP
#define LEARNER_HPP

#include <string>
using namespace std;

struct Learner
{
    int id;
    string name;
    string email;
    int age;
};

struct Node
{
    Learner data;
    Node *next;
};

#endif // LEARNER_HPP
