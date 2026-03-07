#include "activityLog.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

ActivityLog::ActivityLog(int size) : logs(size)
{
}

bool ActivityLog::isEmpty() const
{
    return logs.isEmpty();
}

int ActivityLog::getCount() const
{
    return logs.getCount();
}

void ActivityLog::addLog(const ActivityResult &result)
{
    logs.enqueue(result);
}

string ActivityLog::getLearnerNameById(int learnerId) const
{
    ifstream fin("learner.txt");
    if (!fin.is_open())
        return "Unknown";

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string token;
        int id;
        string name;

        try
        {
            getline(ss, token, ',');
            id = stoi(token);

            getline(ss, name, ',');

            if (id == learnerId)
            {
                fin.close();
                return name;
            }
        }
        catch (...)
        {
            continue;
        }
    }

    fin.close();
    return "Unknown";
}

string ActivityLog::getLearnerEmailById(int learnerId) const
{
    ifstream fin("learner.txt");
    if (!fin.is_open())
        return "Unknown";

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string token;
        int id;
        string name, email;

        try
        {
            getline(ss, token, ',');
            id = stoi(token);

            getline(ss, name, ',');
            getline(ss, email, ',');

            if (id == learnerId)
            {
                fin.close();
                return email;
            }
        }
        catch (...)
        {
            continue;
        }
    }

    fin.close();
    return "Unknown";
}

int ActivityLog::getLearnerAgeById(int learnerId) const
{
    ifstream fin("learner.txt");
    if (!fin.is_open())
        return -1;

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string token;
        int id, age;
        string name, email;

        try
        {
            getline(ss, token, ',');
            id = stoi(token);

            getline(ss, name, ',');
            getline(ss, email, ',');
            getline(ss, token, ',');
            age = stoi(token);

            if (id == learnerId)
            {
                fin.close();
                return age;
            }
        }
        catch (...)
        {
            continue;
        }
    }

    fin.close();
    return -1;
}

string ActivityLog::getChapterLabel(int activityId) const
{
    if (activityId == 1)
        return "Chapter 1";
    if (activityId == 2)
        return "Chapter 2";
    if (activityId == 3)
        return "Chapter 3";

    for (int i = 0; i < numActivities; i++)
    {
        if (activities[i].id == activityId)
        {
            return activities[i].name;
        }
    }

    return "Unknown";
}

void ActivityLog::displayAllLogs() const
{
    if (isEmpty())
    {
        cout << "\nNo activity logs available.\n";
        return;
    }

    cout << "\n=========================== ALL ACTIVITY LOGS ===========================\n";
    cout << left
         << setw(10) << "ID"
         << setw(18) << "Name"
         << setw(12) << "Activity"
         << setw(8) << "Score"
         << setw(8) << "Time"
         << setw(8) << "Fails"
         << setw(10) << "Attempt"
         << setw(12) << "Complete"
         << "Datetime\n";
    cout << "------------------------------------------------------------------------"
            "----------------\n";

    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult record = logs.getAt(i);

        cout << left
             << setw(10) << record.learnerId
             << setw(18) << getLearnerNameById(record.learnerId)
             << setw(12) << getChapterLabel(record.activityId)
             << setw(8) << record.score
             << setw(8) << record.duration
             << setw(8) << record.failAttempts
             << setw(10) << record.totalAttempt
             << setw(12) << (record.complete ? "Yes" : "No")
             << record.datetime << "\n";
    }
}

void ActivityLog::displayLogsByLearner(int learnerId) const
{
    if (isEmpty())
    {
        cout << "\nNo activity logs available.\n";
        return;
    }

    bool found = false;

    cout << "\n================ LOGS FOR LEARNER " << learnerId << " ================\n";
    cout << "Name  : " << getLearnerNameById(learnerId) << "\n";
    cout << "Email : " << getLearnerEmailById(learnerId) << "\n";
    int age = getLearnerAgeById(learnerId);
    if (age != -1)
        cout << "Age   : " << age << "\n";

    cout << left
         << setw(12) << "Activity"
         << setw(8) << "Score"
         << setw(8) << "Time"
         << setw(8) << "Fails"
         << setw(10) << "Attempt"
         << setw(12) << "Complete"
         << "Datetime\n";
    cout << "--------------------------------------------------------------------\n";

    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult record = logs.getAt(i);

        if (record.learnerId == learnerId)
        {
            found = true;
            cout << left
                 << setw(12) << getChapterLabel(record.activityId)
                 << setw(8) << record.score
                 << setw(8) << record.duration
                 << setw(8) << record.failAttempts
                 << setw(10) << record.totalAttempt
                 << setw(12) << (record.complete ? "Yes" : "No")
                 << record.datetime << "\n";
        }
    }

    if (!found)
    {
        cout << "No logs found for learner ID " << learnerId << ".\n";
    }
}

void ActivityLog::displayCompletedLogs() const
{
    if (isEmpty())
    {
        cout << "\nNo activity logs available.\n";
        return;
    }

    bool found = false;
    cout << "\n==================== COMPLETED LOGS ====================\n";

    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult record = logs.getAt(i);

        if (record.complete)
        {
            found = true;
            cout << "Learner ID: " << record.learnerId
                 << " | Name: " << getLearnerNameById(record.learnerId)
                 << " | Activity: " << getChapterLabel(record.activityId)
                 << " | Score: " << record.score
                 << " | DateTime: " << record.datetime << "\n";
        }
    }

    if (!found)
    {
        cout << "No completed logs found.\n";
    }
}

void ActivityLog::displayIncompleteLogs() const
{
    if (isEmpty())
    {
        cout << "\nNo activity logs available.\n";
        return;
    }

    bool found = false;
    cout << "\n==================== INCOMPLETE LOGS ====================\n";

    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult record = logs.getAt(i);

        if (!record.complete)
        {
            found = true;
            cout << "Learner ID: " << record.learnerId
                 << " | Name: " << getLearnerNameById(record.learnerId)
                 << " | Activity: " << getChapterLabel(record.activityId)
                 << " | Score: " << record.score
                 << " | DateTime: " << record.datetime << "\n";
        }
    }

    if (!found)
    {
        cout << "No incomplete logs found.\n";
    }
}

void ActivityLog::showLearnerSummary(int learnerId) const
{
    if (isEmpty())
    {
        cout << "\nNo activity logs available.\n";
        return;
    }

    int totalLogs = 0;
    int completedCount = 0;
    int incompleteCount = 0;
    int totalDuration = 0;
    int totalFailAttempts = 0;
    double totalScore = 0.0;

    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult record = logs.getAt(i);

        if (record.learnerId == learnerId)
        {
            totalLogs++;
            totalScore += record.score;
            totalDuration += record.duration;
            totalFailAttempts += record.failAttempts;

            if (record.complete)
                completedCount++;
            else
                incompleteCount++;
        }
    }

    if (totalLogs == 0)
    {
        cout << "\nNo logs found for learner ID " << learnerId << ".\n";
        return;
    }

    cout << "\n================ LEARNER PERFORMANCE SUMMARY ================\n";
    cout << "Learner ID        : " << learnerId << "\n";
    cout << "Name              : " << getLearnerNameById(learnerId) << "\n";
    cout << "Email             : " << getLearnerEmailById(learnerId) << "\n";
    int age = getLearnerAgeById(learnerId);
    if (age != -1)
        cout << "Age               : " << age << "\n";
    cout << "Total Logs        : " << totalLogs << "\n";
    cout << "Completed         : " << completedCount << "\n";
    cout << "Incomplete        : " << incompleteCount << "\n";
    cout << "Average Score     : " << (totalScore / totalLogs) << "\n";
    cout << "Total Duration    : " << totalDuration << " seconds\n";
    cout << "Total FailAttempt : " << totalFailAttempts << "\n";
}

void ActivityLog::exportToFile(const string &filename) const
{
    ofstream fout(filename);

    if (!fout.is_open())
    {
        cout << "\nError: unable to open file.\n";
        return;
    }

    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult record = logs.getAt(i);

        fout << record.learnerId << ","
             << record.activityId << ","
             << record.score << ","
             << record.duration << ","
             << record.failAttempts << ","
             << record.totalAttempt << ","
             << (record.complete ? 1 : 0) << ","
             << record.datetime << "\n";
    }

    fout.close();
    cout << "\nLogs exported successfully to " << filename << ".\n";
}