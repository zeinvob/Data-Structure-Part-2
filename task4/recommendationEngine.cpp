#include "recommendationEngine.hpp"
#include "riskCalculator.hpp"
#include "../model/activityResult.hpp"
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

using namespace std;

// Helper: any integer input
int getIntInputForMenu() {
    int value;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    return value;
}

// Helper: ositive integer input (for Learner ID)
int getPositiveInt() {
    int value;
    while (!(cin >> value) || value <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid ID. Enter a positive number: ";
    }
    return value;
}

vector<ActivityResult> readResults() {
    vector<ActivityResult> allResults;
    ifstream fin("result.txt");
    string line;

    while(getline(fin, line)) {
        stringstream ss(line);
        string field;
        ActivityResult res;

        getline(ss, field, ','); res.learnerId = stoi(field);
        getline(ss, field, ','); res.activityId = stoi(field);
        getline(ss, field, ','); res.score = stod(field);
        getline(ss, field, ','); res.duration = stoi(field);
        getline(ss, field, ','); res.failAttempts = stoi(field);
        getline(ss, field, ','); res.totalAttempt = stoi(field);
        getline(ss, field, ','); res.complete = (stoi(field) == 1);
        getline(ss, field, ','); res.datetime = field;

        allResults.push_back(res);
    }
    fin.close();
    return allResults;
}

map<int, vector<ActivityResult>> groupResultsByLearner(const vector<ActivityResult>& results) {
    map<int, vector<ActivityResult>> learnerMap;
    for(auto& r : results) {
        learnerMap[r.learnerId].push_back(r);
    }
    return learnerMap;
}

PriorityQueue calculateAllRisks() {
    vector<ActivityResult> results = readResults();
    map<int, vector<ActivityResult>> learnerMap = groupResultsByLearner(results);
    PriorityQueue pq;

    for(auto& pair : learnerMap) {
        string weakChapter;
        double score = calculateRiskScore(pair.second, weakChapter);
        string level = getRiskLevel(score);
        string rec = getRecommendation(level, weakChapter);

        AtRiskLearner l{pair.first, score, level, weakChapter, rec};
        pq.push(l);
    }

    return pq;
}

void exportAtRiskList(PriorityQueue& pq) {
    ofstream fout("atRiskLearners.txt");
    fout << "LearnerID,RiskScore,RiskLevel,WeakChapter,Recommendation\n";
    PQNode* current = pq.getHead();
    while(current) {
        fout << current->learner.learnerId << ","
             << current->learner.riskScore << ","
             << current->learner.riskLevel << ","
             << current->learner.weakChapter << ","
             << current->learner.recommendation << "\n";
        current = current->next;
    }
    fout.close();
}

void task4Menu() {
    PriorityQueue pq = calculateAllRisks();
    int choice;
    do {
        cout << "\n=== TASK 4: At-Risk Learner & Recommendation ===\n";
        cout << "1. Display ranked at-risk learners\n";
        cout << "2. Search at-risk learner & recommendation by Learner ID\n";
        cout << "3. Export current list to file\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        choice = getIntInputForMenu();
        cout << "\n";

        switch(choice) {
            case 1:
                pq.display();
                break;
            case 2: {
                int id;
                cout << "Enter Learner ID: ";
                id = getPositiveInt();
                
                vector<AtRiskLearner> temp;
                bool found = false;

                while(!pq.empty()) {
                    AtRiskLearner l = pq.pop();
                    if(l.learnerId == id) {
                        found = true;
                        cout << "\n------------------------------\n";
                        cout << "Learner ID: " << l.learnerId << "\n";
                        cout << "Risk Score: " << l.riskScore << "\n";
                        cout << "Risk Level: " << l.riskLevel << "\n";
                        cout << "Weak Chapter: " << l.weakChapter << "\n";
                        cout << "Recommendation: " << l.recommendation << "\n";
                        cout << "------------------------------\n";
                    }
                    temp.push_back(l);
                }

                for(auto &l : temp) pq.push(l);

                if(!found) {
                    cout << "No activity records found for the Learner ID " << id << ".\n";
                }

                break;
            }
            case 3:
                exportAtRiskList(pq);
                cout << "Exported atRiskLearners.txt successfully.\n";
                break;
            case 0:
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while(choice != 0);
}