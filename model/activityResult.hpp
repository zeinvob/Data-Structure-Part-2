#ifndef ACTIVITYRESULT_HPP
#define ACTIVITYRESULT_HPP

#include <string>
using namespace std;

struct ActivityResult {
    int learnerId;
    int activityId;
    double score;
    int duration; // duration in seconds
    int failAttempts;
    int totalAttempt;
    bool complete; // true if submitted, false if quit early
    string datetime; // format: "YYYY-MM-DD HH:MM:SS"
};

#endif // ACTIVITYRESULT_HPP
