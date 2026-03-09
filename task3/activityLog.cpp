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

bool ActivityLog::isSameRecord(const ActivityResult &a, const ActivityResult &b) const
{
    return a.learnerId == b.learnerId &&
           a.activityId == b.activityId &&
           a.score == b.score &&
           a.duration == b.duration &&
           a.failAttempts == b.failAttempts &&
           a.totalAttempt == b.totalAttempt &&
           a.complete == b.complete &&
           a.datetime == b.datetime;
}

bool ActivityLog::contains(const ActivityResult &result) const
{
    for (int i = 0; i < logs.getCount(); i++)
    {
        ActivityResult current = logs.getAt(i);
        if (isSameRecord(current, result))
        {
            return true;
        }
    }
    return false;
}

int ActivityLog::findLatestIndex(ActivityResult arr[], int size, int learnerId, int activityId) const
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].learnerId == learnerId && arr[i].activityId == activityId)
        {
            return i;
        }
    }
    return -1;
}


void ActivityLog::addLog(const ActivityResult &result)
{
    if (!contains(result))
    {
        logs.enqueue(result);
    }
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

void ActivityLog::updateResultFile(const ActivityResult &result, const string &filename) const
{
    ActivityResult records[1000];
    int count = 0;

    ifstream fin(filename);
    string line;

    if (fin.is_open())
    {
        while (getline(fin, line) && count < 1000)
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string token;
            ActivityResult temp;

            try
            {
                getline(ss, token, ',');
                temp.learnerId = stoi(token);

                getline(ss, token, ',');
                temp.activityId = stoi(token);

                getline(ss, token, ',');
                temp.score = stod(token);

                getline(ss, token, ',');
                temp.duration = stoi(token);

                getline(ss, token, ',');
                temp.failAttempts = stoi(token);

                getline(ss, token, ',');
                temp.totalAttempt = stoi(token);

                getline(ss, token, ',');
                temp.complete = (stoi(token) == 1);

                getline(ss, token);
                temp.datetime = token;

                records[count++] = temp;
            }
            catch (...)
            {
                continue;
            }
        }
        fin.close();
    }

    int idx = findLatestIndex(records, count, result.learnerId, result.activityId);

    if (idx == -1)
    {
        if (count < 1000)
        {
            records[count++] = result;
        }
    }
    else
    {
        records[idx] = result;
    }

    ofstream fout(filename);
    if (!fout.is_open())
    {
        cout << "\nError: unable to open " << filename << ".\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fout << records[i].learnerId << ","
             << records[i].activityId << ","
             << records[i].score << ","
             << records[i].duration << ","
             << records[i].failAttempts << ","
             << records[i].totalAttempt << ","
             << (records[i].complete ? 1 : 0) << ","
             << records[i].datetime << "\n";
    }

    fout.close();
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
}

void ActivityLog::loadFromFile(const string &filename)
{
    ifstream fin(filename);

    if (!fin.is_open())
    {
        return;
    }

    logs.clear();

    string line;
    while (getline(fin, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string token;
        ActivityResult temp;

        try
        {
            getline(ss, token, ',');
            temp.learnerId = stoi(token);

            getline(ss, token, ',');
            temp.activityId = stoi(token);

            getline(ss, token, ',');
            temp.score = stod(token);

            getline(ss, token, ',');
            temp.duration = stoi(token);

            getline(ss, token, ',');
            temp.failAttempts = stoi(token);

            getline(ss, token, ',');
            temp.totalAttempt = stoi(token);

            getline(ss, token, ',');
            temp.complete = (stoi(token) == 1);

            getline(ss, token);
            temp.datetime = token;

            addLog(temp);
        }
        catch (...)
        {
            continue;
        }
    }

    fin.close();
}