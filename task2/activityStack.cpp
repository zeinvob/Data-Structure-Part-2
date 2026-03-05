#include "activityStack.hpp"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
#include <limits>
#include <ctime>

using namespace std;

// Get current date and time as string
string getCurrentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + ltm->tm_year,
            1 + ltm->tm_mon,
            ltm->tm_mday,
            ltm->tm_hour,
            ltm->tm_min,
            ltm->tm_sec);
    return string(buffer);
}

// Safe character input helper
char getCharInputSafe() {
    char c;
    cin >> c;
    // Clear any remaining input
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return c;
}

// Check if learner has saved progress
bool ActivityStack::hasSavedProgress(int learnerId) {
    ifstream fin("progress.txt");
    if (!fin.is_open()) return false;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        if (token.empty()) continue; // Skip if no valid data
        try {
            if (stoi(token) == learnerId) {
                fin.close();
                return true;
            }
        } catch (...) {
            continue; // Skip invalid lines
        }
    }
    fin.close();
    return false;
}

// Load saved progress for a learner
void ActivityStack::loadSavedProgress(int learnerId, int &chapter, int answers[5], int &currentQ, int &elapsedTime) {
    ifstream fin("progress.txt");
    if (!fin.is_open()) return;
    elapsedTime = 0; // Default
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        if (token.empty()) continue; // Skip if no valid data
        try {
            if (stoi(token) == learnerId) {
                getline(ss, token, ','); chapter = stoi(token);
                getline(ss, token, ','); currentQ = stoi(token);
                for (int i = 0; i < 5; ++i) {
                    getline(ss, token, ',');
                    answers[i] = stoi(token);
                }
                // Load elapsed time if present
                if (getline(ss, token, ',') && !token.empty()) {
                    elapsedTime = stoi(token);
                }
                break;
            }
        } catch (...) {
            continue; // Skip invalid lines
        }
    }
    fin.close();
}

// Save progress to file
void ActivityStack::saveProgress(int learnerId, int chapter, int answers[5], int currentQ, int elapsedTime) {
    // First, remove any existing progress for this learner
    clearSavedProgress(learnerId);
    
    // Append new progress
    ofstream fout("progress.txt", ios::app);
    if (fout.is_open()) {
        fout << learnerId << "," << chapter << "," << currentQ;
        for (int i = 0; i < 5; ++i) {
            fout << "," << answers[i];
        }
        fout << "," << elapsedTime; // Save elapsed time
        fout << "\n";
        fout.close();
    }
}

// Clear saved progress after completion
void ActivityStack::clearSavedProgress(int learnerId) {
    ifstream fin("progress.txt");
    if (!fin.is_open()) return;
    
    string lines[100];
    int count = 0;
    string line;
    while (getline(fin, line) && count < 100) {
        if (line.empty()) continue; // Skip empty lines
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        if (token.empty()) continue; // Skip if no valid data
        try {
            if (stoi(token) != learnerId) {
                lines[count++] = line;
            }
        } catch (...) {
            // Skip invalid lines
        }
    }
    fin.close();
    
    // Rewrite file without this learner's progress
    ofstream fout("progress.txt");
    if (fout.is_open()) {
        for (int i = 0; i < count; ++i) {
            fout << lines[i] << "\n";
        }
        fout.close();
    }
}

void ActivityStack::addResult(int learnerId, int chapter, int score, int duration, int failAttempts, int totalAttempt, bool complete) {
    if (resultCount < 100) {
        results[resultCount].learnerId = learnerId;
        results[resultCount].activityId = chapter; // Using activityId to store chapter number
        results[resultCount].score = score;
        results[resultCount].duration = duration;
        results[resultCount].failAttempts = failAttempts;
        results[resultCount].totalAttempt = totalAttempt;
        results[resultCount].complete = complete;
        results[resultCount].datetime = getCurrentDateTime();
        resultCount++;
    }
}

void ActivityStack::startChapterSession(int chapter, int learnerId) {
    if (chapter < 1 || chapter > 3) {
        cout << "Invalid chapter.\n";
        return;
    }
    
    // Load completed chapters if learner changed
    if (learnerId != currentLearnerId) {
        loadCompletedChapters(learnerId);
    }
    
    int startIdx = (chapter - 1) * 5;
    int answers[5] = {-1, -1, -1, -1, -1}; // -1 means not answered yet
    int currentQ = 0; // Current question index (0-4)
    int previousElapsedTime = 0; // Time already used from previous sessions

    // Check for saved progress
    if (hasSavedProgress(learnerId)) {
        int savedChapter;
        int savedAnswers[5];
        int savedQ;
        int savedElapsedTime = 0;
        loadSavedProgress(learnerId, savedChapter, savedAnswers, savedQ, savedElapsedTime);
        int remainingTime = TIME_LIMIT - savedElapsedTime;
        int remainingMin = remainingTime / 60;
        int remainingSec = remainingTime % 60;
        cout << "You have saved progress for Chapter " << savedChapter << ".\n";
        cout << "Time remaining: " << remainingMin << "m " << remainingSec << "s\n";
        cout << "Continue from where you left off? (y/n): ";
        char cont = getCharInputSafe();
        if (cont == 'y' || cont == 'Y') {
            chapter = savedChapter;
            startIdx = (chapter - 1) * 5;
            for (int i = 0; i < 5; ++i) answers[i] = savedAnswers[i];
            currentQ = savedQ;
            previousElapsedTime = savedElapsedTime;
        } else {
            clearSavedProgress(learnerId);
        }
    }

    // Start timing the session
    auto sessionStart = chrono::steady_clock::now();

    bool submitted = false;
    bool quit = false;
    bool timeUp = false;

    while (!submitted && !quit && !timeUp) {
        // Check remaining time
        auto now = chrono::steady_clock::now();
        int currentSessionTime = chrono::duration_cast<chrono::seconds>(now - sessionStart).count();
        int totalElapsed = previousElapsedTime + currentSessionTime;
        int remainingTime = TIME_LIMIT - totalElapsed;
        
        if (remainingTime <= 0) {
            cout << "\n*** TIME'S UP! Auto-submitting... ***\n";
            timeUp = true;
            break;
        }
        
        int remainingMin = remainingTime / 60;
        int remainingSec = remainingTime % 60;

        int actIdx = startIdx + currentQ;
        cout << "\n----------------------------------------\n";
        cout << "[Time remaining: " << remainingMin << "m " << remainingSec << "s]\n";
        cout << "Q" << (currentQ + 1) << " of 5: " << activities[actIdx].description << "\n";
        for (int opt = 0; opt < 4; ++opt) {
            cout << activities[actIdx].options[opt] << "\n";
        }
        
        // Show current answer if already answered
        if (answers[currentQ] >= 0) {
            cout << "[Current answer: " << (char)('A' + answers[currentQ]) << "]\n";
        }

        cout << "\nOptions: ";
        if (currentQ > 0) cout << "[P]revious  ";
        cout << "[A-D] Answer  ";
        if (currentQ < 4) cout << "[N]ext  ";
        cout << "[S]ubmit  [Q]uit\n";
        cout << "Enter choice: ";

        string input;
        cin >> input;

        if (input.length() == 1) {
            char c = input[0];
            
            // Check for quit command
            if (c == 'Q' || c == 'q') {
                cout << "Save progress and quit? (y/n): ";
                char confirm = getCharInputSafe();
                if (confirm == 'y' || confirm == 'Y') {
                    // Calculate total elapsed time including previous sessions
                    auto sessionEnd = chrono::steady_clock::now();
                    int currentSessionTime = chrono::duration_cast<chrono::seconds>(sessionEnd - sessionStart).count();
                    int totalElapsedTime = previousElapsedTime + currentSessionTime;
                    
                    saveProgress(learnerId, chapter, answers, currentQ, totalElapsedTime);
                    
                    int remainingTime = TIME_LIMIT - totalElapsedTime;
                    int remainingMin = remainingTime / 60;
                    int remainingSec = remainingTime % 60;
                    cout << "Progress saved. Time remaining: " << remainingMin << "m " << remainingSec << "s\n";
                    cout << "You can continue later.\n";
                    
                    // Count attempts from results array for incomplete attempt
                    int totalAttempt = 1;
                    for (int i = 0; i < resultCount; i++) {
                        if (results[i].learnerId == learnerId && results[i].activityId == chapter) {
                            totalAttempt++;
                        }
                    }
                    
                    // Add incomplete result to array
                    addResult(learnerId, chapter, 0, totalElapsedTime, 0, totalAttempt, false);
                    quit = true;
                }
                continue;
            }
            
            // Check for navigation commands using Stack
            if ((c == 'P' || c == 'p') && !backStack.isEmpty()) {
                // Pop from stack to go back to previous question
                currentQ = backStack.top();
                backStack.pop();
                continue;
            }
            if ((c == 'N' || c == 'n') && currentQ < 4) {
                // Push current question to stack before moving next
                backStack.push(currentQ);
                currentQ++;
                continue;
            }
            if (c == 'S' || c == 's') {
                // Check if all questions are answered
                bool allAnswered = true;
                for (int i = 0; i < 5; ++i) {
                    if (answers[i] < 0) {
                        allAnswered = false;
                        cout << "Q" << (i + 1) << " is not answered.\n";
                    }
                }
                if (!allAnswered) {
                    cout << "Please answer all questions before submitting.\n";
                    continue;
                }
                // Confirm submission
                cout << "Are you sure you want to submit? (y/n): ";
                char confirm = getCharInputSafe();
                if (confirm == 'y' || confirm == 'Y') {
                    submitted = true;
                }
                continue;
            }
            
            // Check for answer input
            int ans = -1;
            if (c >= 'A' && c <= 'D') ans = c - 'A';
            else if (c >= 'a' && c <= 'd') ans = c - 'a';
            else if (c >= '1' && c <= '4') ans = c - '1';
            
            if (ans >= 0 && ans < 4) {
                answers[currentQ] = ans;
                cout << "Answer saved: " << (char)('A' + ans) << "\n";
                // Auto-advance to next question if not last
                if (currentQ < 4) {
                    // Push current question to stack before auto-advancing
                    backStack.push(currentQ);
                    currentQ++;
                }
                continue;
            }
        }
        
        cout << "Invalid input. Try again.\n";
    }

    if (quit) return; // Exit if user quit

    // Clear saved progress since we're submitting (either manually or time's up)
    clearSavedProgress(learnerId);

    // Calculate score using maxScore from each question
    int score = 0;
    int answeredCount = 0;
    for (int q = 0; q < 5; ++q) {
        int actIdx = startIdx + q;
        if (answers[q] >= 0 && answers[q] == activities[actIdx].correctIndex) {
            score += activities[actIdx].maxScore;  // Use maxScore from question
        }
        if (answers[q] >= 0) answeredCount++;
    }

    // End timing the session
    auto sessionEnd = chrono::steady_clock::now();
    int currentSessionTime = chrono::duration_cast<chrono::seconds>(sessionEnd - sessionStart).count();
    int totalTimeUsed = previousElapsedTime + currentSessionTime;
    
    if (timeUp) {
        cout << "\nTime's up! Answered " << answeredCount << "/5 questions.\n";
    }
    cout << "\nSession complete! Your score: " << score << "/100\n";
    cout << "Total time used: " << totalTimeUsed << " seconds\n";

    // Mark chapter as completed
    completedChapters[chapter - 1] = true;

    // Count attempts from results array
    int totalAttempt = 1;
    int failAttempts = 0;
    for (int i = 0; i < resultCount; i++) {
        if (results[i].learnerId == learnerId && results[i].activityId == chapter) {
            totalAttempt++;
            if (results[i].score < 60) failAttempts++;
        }
    }
    if (score < 60) {
        failAttempts++;
    }

    // Add result to array (instead of file)
    addResult(learnerId, chapter, score, totalTimeUsed, failAttempts, totalAttempt, true);
}

// Load completed chapters for a learner
// First checks result.txt file (saved by Task 3), then checks current session's array
void ActivityStack::loadCompletedChapters(int learnerId) {
    currentLearnerId = learnerId;
    // Reset completed chapters
    for (int i = 0; i < 3; ++i) completedChapters[i] = false;
    
    // 1. Load from result.txt file (saved by Task 3 from previous sessions)
    ifstream fin("result.txt");
    if (fin.is_open()) {
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            try {
                getline(ss, token, ','); // learnerId
                int fileLearnerId = stoi(token);
                if (fileLearnerId == learnerId) {
                    getline(ss, token, ','); // activityId (chapter)
                    int chapter = stoi(token);
                    getline(ss, token, ','); // score
                    getline(ss, token, ','); // duration
                    getline(ss, token, ','); // failAttempts
                    getline(ss, token, ','); // totalAttempt
                    getline(ss, token, ','); // complete
                    bool complete = (stoi(token) == 1);
                    
                    if (complete && chapter >= 1 && chapter <= 3) {
                        completedChapters[chapter - 1] = true;
                    }
                }
            } catch (...) {
                continue; // Skip invalid lines
            }
        }
        fin.close();
    }
    
    // 2. Also check current session's results array (for chapters completed in this session)
    for (int i = 0; i < resultCount; i++) {
        if (results[i].learnerId == learnerId && results[i].complete) {
            int chapter = results[i].activityId;
            if (chapter >= 1 && chapter <= 3) {
                completedChapters[chapter - 1] = true;
            }
        }
    }
}

ActivityStack::ActivityStack() {
    resultCount = 0; // Initialize result count
}

int ActivityStack::getNextChapter() {
    for (int i = 0; i < 3; ++i) {
        if (!completedChapters[i]) return i + 1;
    }
    return 0; // All completed
}

bool ActivityStack::allChaptersCompleted() {
    return completedChapters[0] && completedChapters[1] && completedChapters[2];
}
