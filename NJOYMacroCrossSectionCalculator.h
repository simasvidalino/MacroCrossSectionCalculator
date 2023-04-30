#pragma once

#include <vector>
#include <map>

class NJOYMacroCrossSectionCalculator
{
public:
    using CROSS_SECTION = std::map<int, std::map<int, long double>>;

    NJOYMacroCrossSectionCalculator(double materialDensity,  double atomicMass);
    ~NJOYMacroCrossSectionCalculator();

    void calculateMacroCrossSection(std::map<int, long double> &macroCrossSection);
    void calculateScatteringMacroCrossSection(
            std::map<std::pair<int, int>, std::vector<long double>> &crossSectionByGrouptoGroup);

    double getNumberDensity() const;

private:

    long double materialDensity = 1;
    double atomicMass = 1;
    std::vector <double> macroCrossSection;

    double numberDensity = 1;

    void calculateNumberDensity();
};
