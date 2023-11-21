#pragma once

namespace TommyTools {

    void splitPairsToVectors(const std::vector<std::pair<double, double>>& pairs,
        std::vector<double>& xValues,
        std::vector<double>& yValues) {
        xValues.clear();
        yValues.clear();

        for (const auto& pair : pairs) {
            xValues.push_back(pair.first);
            yValues.push_back(pair.second);
        }
    }

};