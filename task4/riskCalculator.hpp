#ifndef RISKCALCULATOR_HPP
#define RISKCALCULATOR_HPP

#include <string>
#include <vector>
#include "atRiskLearner.hpp"
#include "../model/activityResult.hpp"

double calculateRiskScore(const std::vector<ActivityResult>& results, std::string &weakChapter);
std::string getRiskLevel(double risk);
std::string getRecommendation(const std::string& level, const std::string& weakChapter);

#endif