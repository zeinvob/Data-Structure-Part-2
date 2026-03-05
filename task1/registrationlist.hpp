#ifndef REGISTRATION_LIST_HPP
#define REGISTRATION_LIST_HPP

#include "../model/learner.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class RegistrationList
{
private:
    Node *head;
    int nextID;

public:
    RegistrationList() : head(nullptr), nextID(10001)
    {
        syncNextID();
    }

    ~RegistrationList()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }

    void syncNextID()
    {
        ifstream inFile("learner.txt");
        if (!inFile.is_open())
            return;

        string line;
        while (getline(inFile, line))
        {
            size_t p1 = line.find(',');
            if (p1 != string::npos)
            {
                int idFromFile = stoi(line.substr(0, p1));
                if (idFromFile >= nextID)
                {
                    nextID = idFromFile + 1;
                }
            }
        }
        inFile.close();
    }

    bool isDuplicate(string email)
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            if (temp->data.email == email)
                return true;
            temp = temp->next;
        }

        ifstream inFile("learner.txt");
        string line;
        while (getline(inFile, line))
        {
            if (line.find(email) != string::npos)
                return true;
        }
        return false;
    }

    void registerLearner(string name, string email, int age)
    {
        if (isDuplicate(email))
        {
            cout << "\nThis email is already registered in the system!\n";
            return;
        }

        Learner newLearner = {nextID++, name, email, age};

        Node *newNode = new Node;
        newNode->data = newLearner;
        newNode->next = head;
        head = newNode;

        ofstream outFile("learner.txt", ios::app);
        if (outFile.is_open())
        {
            outFile << newLearner.id << "," << newLearner.name << ","
                    << newLearner.email << "," << newLearner.age << endl;
            outFile.close();
        }
        cout << "\nRegistration Successful! Assigned Learner ID: " << newLearner.id << endl;
    }

    void displayList()
    {
        if (head == nullptr)
        {
            cout << "\nNo new registrations in this session.\n";
            return;
        }
        cout << "\n--- Current Session Registrations ---\n";
        Node *temp = head;
        while (temp != nullptr)
        {
            cout << "ID: " << temp->data.id << " | Name: " << temp->data.name
                 << " | Email: " << temp->data.email << endl;
            temp = temp->next;
        }
    }

    string toUpper(string s)
    {
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }

    bool isRegistered(int id, string name)
    {
        ifstream inFile("learner.txt");
        string line;

        while (getline(inFile, line))
        {
            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);

            if (p1 != string::npos && p2 != string::npos)
            {
                int fileID = stoi(line.substr(0, p1));
                string fileName = line.substr(p1 + 1, p2 - p1 - 1);

                if (fileID == id && toUpper(fileName) == toUpper(name))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif