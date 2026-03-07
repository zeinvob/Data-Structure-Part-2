#ifndef ACTIVITYLOG_HPP
#define ACTIVITYLOG_HPP

#include "../model/activityResult.hpp"
#include "../model/activityDef.hpp"
#include "circularQueue.hpp"
#include <string>

using namespace std;

class ActivityLog
{
private:
    CircularQueue logs;

    string getLearnerNameById(int learnerId) const;
    string getLearnerEmailById(int learnerId) const;
    int getLearnerAgeById(int learnerId) const;
    string getChapterLabel(int activityId) const;

public:
    ActivityLog(int size = 100);

    bool isEmpty() const;
    int getCount() const;

    void addLog(const ActivityResult &result);

    void displayAllLogs() const;
    void displayLogsByLearner(int learnerId) const;
    void displayCompletedLogs() const;
    void displayIncompleteLogs() const;
    void showLearnerSummary(int learnerId) const;

    void exportToFile(const string &filename = "result.txt") const;
};

#endif