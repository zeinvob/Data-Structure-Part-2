#ifndef ACTIVITYLOG_HPP
#define ACTIVITYLOG_HPP

#include "../model/activityResult.hpp"
#include "../model/activityDef.hpp"
#include "circularQueue.hpp"
#include <string>

using namespace std;

// class to manage and store activity logs
class ActivityLog
{
private:
    // circular queue to keep activity log records
    CircularQueue logs;

    // get learner name using learner id
    string getLearnerNameById(int learnerId) const;

    // get learner email using learner id
    string getLearnerEmailById(int learnerId) const;

    // get learner age using learner id
    int getLearnerAgeById(int learnerId) const;

    // get chapter label using activity id
    string getChapterLabel(int activityId) const;

    // check if two activity records are the same
    bool isSameRecord(const ActivityResult &a, const ActivityResult &b) const;

    // check if a result already exists in the log
    bool contains(const ActivityResult &result) const;

    // find the latest record index for a learner and activity
    int findLatestIndex(ActivityResult arr[], int size, int learnerId, int activityId) const;

public:
    // constructor with default queue size
    ActivityLog(int size = 100);

    // check if the log is empty
    bool isEmpty() const;

    // get total number of logs
    int getCount() const;

    // add a new activity log
    void addLog(const ActivityResult &result);

    // display all logs
    void displayAllLogs() const;

    // display logs for a specific learner
    void displayLogsByLearner(int learnerId) const;

    // display only completed logs
    void displayCompletedLogs() const;

    // display only incomplete logs
    void displayIncompleteLogs() const;

    // show summary of a learner's activities
    void showLearnerSummary(int learnerId) const;

    // export recent logs from circular queue to file only
    void exportToFile(const string &filename = "logActivities.txt") const;

    // update result file with latest record for each learner and chapter
    void updateResultFile(const ActivityResult &result, const string &filename = "result.txt") const;

    // load latest saved records into memory when program starts
    void loadFromFile(const string &filename = "result.txt");
};

#endif