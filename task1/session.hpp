#ifndef SESSION_HPP
#define SESSION_HPP

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct SessionData
{
    int id;
    string name;
};

class SessionQueue
{
private:
    SessionData activeLearners[5];
    int count;
    const string filename = "session.txt";

public:
    SessionQueue() : count(0)
    {
        loadSession();
    }

    bool isFull()
    {
        return count == 5;
    }
    bool isEmpty()
    {
        return count == 0;
    }

    SessionData getCurrentPlayer()
    {
        if (isEmpty())
        {
            return {0, "Empty"};
        }
        return activeLearners[0];
    }

    bool enqueue(int id, string name)
    {
        if (isFull())
            return false;

        for (int i = 0; i < count; i++)
        {
            if (activeLearners[i].id == id)
            {
                cout << "\nError: Learner [ID: " << id << "] is already in the session!" << endl;
                return false;
            }
        }

        activeLearners[count].id = id;
        activeLearners[count].name = name;
        count++;

        saveSession();
        return true;
    }

    int dequeue()
    {
        if (isEmpty())
            return -1;

        int finishedID = activeLearners[0].id;

        for (int i = 0; i < count - 1; i++)
        {
            activeLearners[i] = activeLearners[i + 1];
        }

        count--;
        saveSession();

        return finishedID;
    }

    void saveSession()
    {
        ofstream outFile(filename);
        for (int i = 0; i < count; i++)
        {
            outFile << activeLearners[i].id << "," << activeLearners[i].name << endl;
        }
        outFile.close();
    }

    void loadSession()
    {
        ifstream inFile(filename);
        string line;
        count = 0;
        while (getline(inFile, line) && count < 5)
        {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos)
            {
                activeLearners[count].id = stoi(line.substr(0, commaPos));
                activeLearners[count].name = line.substr(commaPos + 1);
                count++;
            }
        }
        inFile.close();
    }

    void displayActive()
    {
        if (isEmpty())
        {
            cout << "\nNo one in session.\n";
            return;
        }
        cout << "\n--- Active Session (Limit 5) ---\n";
        for (int i = 0; i < count; i++)
        {
            cout << (i + 1) << ". [" << activeLearners[i].id << "] " << activeLearners[i].name;
            if (i == 0)
                cout << " (Doing Activity Now)";
            cout << endl;
        }
    }
};

#endif