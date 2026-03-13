#include "riskCalculator.hpp"
#include "../task2/activityStack.hpp"
#include <algorithm>
#include <map>

double calculateRiskScore(const std::vector<ActivityResult>& results, std::string &weakChapter) {
    weakChapter = "";

    // Group results by chapter
    std::map<int, std::vector<ActivityResult>> chapterResults;
    for (const auto& res : results) {
        chapterResults[res.activityId].push_back(res);
    }

    double totalFailureRisk = 0.0;
    int chapterCount = chapterResults.size();

    // Global metrics
    int totalFailedAll = 0, totalAttemptsAll = 0, totalTimeAll = 0;
    double highestScoreAll = -1;
    double lowestScoreAll = 101;

    for (auto& [chapter, resVec] : chapterResults) {
        int failedAttempts = 0;
        int totalAttempts = 0;
        int timeSpent = 0;
        double lowestScoreChapter = 101;

        for (const auto& r : resVec) {
            failedAttempts += r.failAttempts;
            totalAttempts += r.totalAttempt;
            timeSpent += r.duration;

            if (r.score > highestScoreAll) highestScoreAll = r.score;
            if (r.score < lowestScoreAll) lowestScoreAll = r.score;

            if (r.score < lowestScoreChapter) lowestScoreChapter = r.score;
        }

        // Track weakest chapter based on lowest chapter score
        if (lowestScoreChapter == lowestScoreAll) {
            weakChapter = "Ch" + std::to_string(chapter);
        }

        // Chapter-level failure risk
        double chapterFailureRisk = (totalAttempts == 0 ? 0 : (double)failedAttempts / totalAttempts * 100);
        totalFailureRisk += chapterFailureRisk;

        // Accumulate global metrics
        totalFailedAll += failedAttempts;
        totalAttemptsAll += totalAttempts;
        totalTimeAll += timeSpent;
    }

    // Average failure risk across chapters
    double avgFailureRisk = (chapterCount == 0 ? 0 : totalFailureRisk / chapterCount);

    // Global risk components
    double consistencyRisk = highestScoreAll - lowestScoreAll;
    double scoreRisk = std::max(0.0, 60 - lowestScoreAll);
    double timeRisk = (lowestScoreAll < 60 ? (double)totalTimeAll / (chapterCount * ActivityStack::TIME_LIMIT) * 100 : 0);

    // Weighted risk score
    double riskScore = 0.4 * avgFailureRisk + 0.3 * consistencyRisk + 0.2 * scoreRisk + 0.1 * timeRisk;

    return riskScore;
}

std::string getRiskLevel(double risk) {
    if(risk >= 81) return "Critical";
    else if(risk >= 61) return "High";
    else if(risk >= 31) return "Medium";
    else return "Low";
}

std::string getRecommendation(const std::string& level, const std::string& weakChapter) {
    if(level == "Critical") return "Immediate intervention";
    else if(level == "High") return "Repeat " + weakChapter + " + easier activity";
    else if(level == "Medium") return "Review " + weakChapter;
    else return "Continue learning";
}