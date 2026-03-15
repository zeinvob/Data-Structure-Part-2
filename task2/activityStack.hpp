#ifndef ACTIVITYSTACK_HPP
#define ACTIVITYSTACK_HPP

#include "myStack.hpp"
#include "../model/activityDef.hpp"
#include "../model/activityResult.hpp"

using namespace std;

class ActivityStack {
public:
    ActivityStack();
    void startChapterSession(int chapter, int learnerId = 10001); // session for a chapter
    int getNextChapter(); // returns next chapter to do (1-3), or 0 if all done
    bool allChaptersCompleted(); // true if all chapters done at least once
    bool hasSavedProgress(int learnerId); // check if learner has incomplete session
    void loadSavedProgress(int learnerId, int &chapter, int answers[5], int &currentQ, int &elapsedTime); 
    void saveProgress(int learnerId, int chapter, int answers[5], int currentQ, int elapsedTime); 
    void clearSavedProgress(int learnerId); // clear saved progress after completion
    void loadCompletedChapters(int learnerId); // load completed chapters from results array
    bool getPreviousChapterResult(int learnerId, int chapter, ActivityResult &previousResult); // previous attempts and all history from result.txt
    
    ActivityResult* getResults() { return results; }
    int getResultCount() { return resultCount; }
    
    static const int TIME_LIMIT = 300; // 5 minutes in seconds
private:
    int currentLearnerId = 10001; // track current learner
    bool completedChapters[3] = {false, false, false}; // track completion of ch 1,2,3
    MyStack<int> backStack; // Stack to track navigation history for going back
    ActivityResult results[100]; // Array to store session results
    int resultCount = 0; 
    void addResult(int learnerId, int chapter, int score, int duration, int failAttempts, int totalAttempt, bool complete);
};

#endif // ACTIVITYSTACK_HPP
