#pragma once
#ifndef ACTIVITYDEF_HPP
#define ACTIVITYDEF_HPP


#include <string>
using namespace std;

struct ActivityDef {
    int id;
    string name;
    string description;
    int maxScore;
    string options[4]; // 4 choices for MCQ
    int correctIndex;  // 0-based index of correct answer
};

extern ActivityDef activities[];
extern const int numActivities;

#endif // ACTIVITYDEF_H
