#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include "model/activityDef.hpp"
#include "model/activityResult.hpp"
#include "model/learner.hpp"
#include "task1/registrationlist.hpp"
#include "task1/session.hpp"
#include "task2/activityStack.hpp"
#include "task3/activityLog.hpp"
// g++ -o main main.cpp model/activityDef.cpp task2/activityStack.cpp

using namespace std;

extern RegistrationList _regList;
extern void handleRegistration();
extern SessionQueue _activeSession;
extern void handleJoinSession();
extern SessionData _activeLearner;
ActivityLog activityLog;

// Safe integer input with error handling
int getIntInput()
{
    int value;
    while (!(cin >> value))
    {
        cin.clear();                                         // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Invalid input. Please enter a number: ";
    }
    return value;
}

// Safe character input
char getCharInput()
{
    char c;
    cin >> c;
    return c;
}

// ==================== Task 1 Menu ====================
void showTask1Menu()
{
    cout << "\n===== Task 1: Learner Registration & Session =====\n";
    cout << "1. Register Learner\n";
    cout << "2. Join Session\n";
    cout << "3. Display Current Registration List\n";
    cout << "4. Display Active Session List\n";
    cout << "5. Remove Learner Leave Session\n";
    cout << "0. Back to Main Menu\n";
    cout << "Select option: ";
}

void task1Menu()
{
    int selection;
    do
    {
        showTask1Menu();
        selection = getIntInput();

        switch (selection)
        {
        case 1:
            handleRegistration();
            break;
        case 2:
            handleJoinSession();
            break;
        case 3:
            _regList.displayList();
            break;
        case 4:
            _activeSession.displayActive();
            break;
        case 5:
            if (_activeSession.isEmpty())
            {
                cout << "The session is empty.\n";
            }
            else
            {
                int removedID = _activeSession.dequeue();
                cout << "Learner ID " << removedID << " has finished and been removed.\n";
                cout << "The next learner is allowed to be added!\n";
            }
            break;
        case 0:
            cout << "Returning to Main Menu\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (selection != 0);
}

// ==================== Task 2 to Task 3 Sync Function ====================
void syncTask2ResultsToTask3(ActivityStack &sessionStack, ActivityLog &activityLog)
{
    static int lastSyncedCount = 0;

    ActivityResult* results = sessionStack.getResults();
    int count = sessionStack.getResultCount();

    for (int i = lastSyncedCount; i < count; i++)
    {
        activityLog.addLog(results[i]);  // in-memory recent circular queue
        activityLog.updateResultFile(results[i], "result.txt"); // latest result only
    }

    lastSyncedCount = count;

    activityLog.exportToFile("logActivities.txt"); // recent 100 logs only
}

// ==================== Task 2 Menu ====================
void showTask2Menu()
{
    int task2Option;
    // Use activities and numActivities from activityDef.cpp
    do
    {
        cout << "-------------------------------------------\n";
        cout << "Task 2: Activity Navigation & Session Flow\n";
        cout << "-------------------------------------------\n";
        cout << "1. Start / Continue Session Activities\n";
        cout << "0. Return to Main Menu\n";
        cout << "Select option: ";
        task2Option = getIntInput();
        switch (task2Option)
        {
        case 1:
        {
            // Get current player from active session (Task 1)
            SessionData currentPlayer = _activeSession.getCurrentPlayer();
            if (currentPlayer.id == 0)
            {
                cout << "No learner in active session. Please join session first (Task 1 > Option 2).\n";
                break;
            }

            int learnerId = currentPlayer.id;
            cout << "Current Learner: " << currentPlayer.name << " (ID: " << learnerId << ")\n";

            static ActivityStack sessionStack;
            sessionStack.loadCompletedChapters(learnerId);

            if (!sessionStack.allChaptersCompleted())
            {
                // Must do chapters in order
                int nextCh = sessionStack.getNextChapter();
                cout << "You must complete Chapter " << nextCh << " first.\n";
                cout << "Start Chapter " << nextCh << "? (y/n): ";
                char confirm = getCharInput();
                if (confirm == 'y' || confirm == 'Y')
                {
                    sessionStack.startChapterSession(nextCh, learnerId);
                    syncTask2ResultsToTask3(sessionStack, activityLog);
                }
            }
            else
            {
                // All chapters completed, can choose any to reattempt
                cout << "All chapters completed! Select chapter to reattempt (1-3): ";
                int ch = getIntInput();
                if (ch >= 1 && ch <= 3)
                {
                    sessionStack.startChapterSession(ch, learnerId);
                    syncTask2ResultsToTask3(sessionStack, activityLog);
                }
                else
                {
                    cout << "Invalid chapter.\n";
                }
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (task2Option != 0);
}
// ==================== Task 3 Menu ====================
void task3Menu(ActivityLog &activityLog)
{
    int choice;

    do
    {
        cout << "\n========== TASK 3: RECENT ACTIVITY LOG & PERFORMANCE HISTORY ==========\n";
        cout << "1. View All Logs\n";
        cout << "2. Search Logs by Learner ID\n";
        cout << "3. View Completed Logs\n";
        cout << "4. View Incomplete Logs\n";
        cout << "5. Show Learner Performance Summary\n";
        cout << "6. Export Activity Records\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            activityLog.displayAllLogs();
            break;

        case 2:
        {
            int learnerId;
            cout << "Enter Learner ID: ";
            cin >> learnerId;
            activityLog.displayLogsByLearner(learnerId);
            break;
        }

        case 3:
            activityLog.displayCompletedLogs();
            break;

        case 4:
            activityLog.displayIncompleteLogs();
            break;

        case 5:
        {
            int learnerId;
            cout << "Enter Learner ID: ";
            cin >> learnerId;
            activityLog.showLearnerSummary(learnerId);
            break;
        }

        case 6:
            activityLog.exportToFile("logActivities.txt");
            cout << "\nLogs exported successfully." << "\n";
            break;

        case 0:
            cout << "Returning...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

// ==================== Main Menu ====================

void showMainMenu()
{
    cout << "========================================\n";
    cout << "PLAPS - Main Menu\n";
    cout << "========================================\n";
    cout << "1. Task 1: Learner Registration & Session Management\n";
    cout << "2. Task 2: Activity Navigation & Session Flow\n";
    cout << "3. Task 3: Recent Activity Logging & Performance History\n";
    cout << "4. Task 4: At-Risk Learner Priority & Recommendations\n";
    cout << "0. Exit\n";
    cout << "----------------------------------------\n";
    cout << "Select option: ";
}

int main()
{
    activityLog.loadFromFile("logActivities.txt");
    int option;
    do
    {
        showMainMenu();
        option = getIntInput();
        switch (option)
        {
        case 1:
            task1Menu();
            break;
        case 2:
            showTask2Menu();
            break;
        case 3:
            task3Menu(activityLog);
            break;
        case 4:
            // Task 4 logic
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (option != 0);
    return 0;
}
