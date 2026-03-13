#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP

#include "atRiskLearner.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

struct PQNode {
    AtRiskLearner learner;
    PQNode* next;
};
        
class PriorityQueue {
private:
    PQNode* head;

public:
    PriorityQueue() { head = nullptr; }

    bool empty() { return head == nullptr; }

    void push(const AtRiskLearner& l) {
        PQNode* node = new PQNode{l, nullptr};
        if(!head || l.riskScore > head->learner.riskScore) {
            node->next = head;
            head = node;
            return;
        }

        PQNode* current = head;
        while(current->next && current->next->learner.riskScore >= l.riskScore)
            current = current->next;

        node->next = current->next;
        current->next = node;
    }

    AtRiskLearner pop() {
        if(empty()) throw runtime_error("Queue is empty");
        PQNode* temp = head;
        AtRiskLearner l = head->learner;
        head = head->next;
        delete temp;
        return l;
    }

    void display() {
        PQNode* current = head;

        const int idWidth = 10;
        const int scoreWidth = 10;
        const int levelWidth = 10;
        const int chapterWidth = 12;
        const int recWidth = 25;

        // header
        cout << "\n"
            << left 
            << setw(idWidth) << "LearnerID"
            << setw(scoreWidth) << "RiskScore"
            << setw(levelWidth) << "RiskLevel"
            << setw(chapterWidth) << "WeakChapter"
            << setw(recWidth) << "Recommendation" 
            << "\n";

        cout << string(idWidth + scoreWidth + levelWidth + chapterWidth + recWidth, '-') << "\n";

        // Print each learner
        while(current) {
            cout << left
                << setw(idWidth) << current->learner.learnerId
                << setw(scoreWidth) << fixed << setprecision(2) << current->learner.riskScore
                << setw(levelWidth) << current->learner.riskLevel
                << setw(chapterWidth) << current->learner.weakChapter
                << setw(recWidth) << current->learner.recommendation
                << "\n";
            current = current->next;
        }
    }

    PQNode* getHead() { return head; } // for export
};

#endif