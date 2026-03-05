#include "registrationlist.hpp"
#include "session.hpp"
#include <iostream>
#include <string>

RegistrationList _regList;
SessionQueue _activeSession;

void handleRegistration()
{
    string name, email;
    int age;

    cin.ignore(1000, '\n');

    while (true)
    {
        cout << "\nEnter Name (min 5 chars or 0 to Back): ";
        getline(cin, name);

        if (name == "0")
        {
            cout << "Returning to previous menu.\n";
            return;
        }

        if (name.length() >= 5)
        {
            if (!name.empty())
            {
                name[0] = toupper(name[0]);

                for (int i = 1; i < name.length(); i++)
                {
                    if (name[i - 1] == ' ')
                    {
                        name[i] = toupper(name[i]);
                    }
                    else
                    {
                        name[i] = tolower(name[i]);
                    }
                }
            }
            break;
        }
        else
        {
            cout << "Name too short. Please try again.\n";
        }
    }

    while (true)
    {
        cout << "Enter Email: ";
        getline(cin, email);
        if (email.find('@') != string::npos && email.find('.') != string::npos)
            break;
        cout << "Invalid email format. Please try again\n";
    }

    while (true)
    {
        cout << "Enter Age: ";
        if (cin >> age && age > 0)
        {
            cin.ignore(1000, '\n');
            break;
        }
        cout << "Please enter a valid positive age.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    _regList.registerLearner(name, email, age);
}

void handleJoinSession()
{
    string idInput;
    int id;
    string name;
    while (true)
    {
        cout << "Enter your Learner ID (e.g., 10001 or 0 to Back): ";
        cin >> idInput;

        if (idInput == "0")
        {
            cout << "Returning to previous menu.\n";
            return;
        }

        bool allDigits = true;
        for (char c : idInput)
        {
            if (!isdigit(c))
                allDigits = false;
        }

        if (idInput.length() == 5 && allDigits)
        {
            id = stoi(idInput);
            break;
        }
        else
        {
            cout << "Please enter Learner ID in correct format (numbers only)!\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    cin.ignore();
    while (true)
    {
        cout << "Enter Name (min 5 chars): ";
        getline(cin, name);

        if (name.length() >= 5)
        {
            if (!name.empty())
            {
                name[0] = toupper(name[0]);

                for (int i = 1; i < name.length(); i++)
                {
                    if (name[i - 1] == ' ')
                    {
                        name[i] = toupper(name[i]);
                    }
                    else
                    {
                        name[i] = tolower(name[i]);
                    }
                }
            }
            break;
        }
        else
        {
            cout << "Name must be at least 5 characters long!\n";
        }
    }

    if (!_regList.isRegistered(id, name))
    {
        cout << "You are not registered yet. Please register first!\n";
    }
    else if (_activeSession.isFull())
    {
        cout << "Session Full (5/5). Please wait for someone to exit.\n";
    }
    else if (_activeSession.enqueue(id, name))
    {
        cout << "Welcome" << name << "! You have seccessfully joined the session.\n";
    }
}