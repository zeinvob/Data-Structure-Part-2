#ifndef ATRISKLEARNER_HPP
#define ATRISKLEARNER_HPP

#include <string>
using namespace std;

struct AtRiskLearner {
    int learnerId;
    double riskScore;
    string riskLevel;
    string weakChapter;
    string recommendation;
};

#endif