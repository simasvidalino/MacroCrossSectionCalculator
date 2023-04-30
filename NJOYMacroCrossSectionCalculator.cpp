#include "NJOYMacroCrossSectionCalculator.h"

#include <QDebug>
#include <iostream>

#define  barn  10E-24; //1barn = 10E-24cm2
#define avogadro 6.02214076E+23 //mol-1

NJOYMacroCrossSectionCalculator::NJOYMacroCrossSectionCalculator(double materialDensity,  double atomicMass) :
    materialDensity(materialDensity),
    atomicMass(atomicMass)
{
    calculateNumberDensity();
}

NJOYMacroCrossSectionCalculator::~NJOYMacroCrossSectionCalculator()
{

}

double NJOYMacroCrossSectionCalculator::getNumberDensity() const
{
    return numberDensity;
}

void NJOYMacroCrossSectionCalculator::calculateMacroCrossSection(std::map<int, long double> &macroCrossSection)
{
    for (auto it = macroCrossSection.begin(); it != macroCrossSection.end(); ++it)
    {
        it->second = numberDensity*(it->second)*barn;

        std::cout<<it->second<<" ";
    }

    std::cout<<std::endl;
}

void NJOYMacroCrossSectionCalculator::calculateScatteringMacroCrossSection(
        std::map<std::pair<int, int>, std::vector<long double>> &crossSectionByGrouptoGroup)
{
    for (auto it = crossSectionByGrouptoGroup.begin(); it != crossSectionByGrouptoGroup.end(); ++it)
    {
        auto crossSectionByLengendre = it ->second;

        for (auto &value:crossSectionByLengendre)
        {
            value = numberDensity*value*barn;
        }
    }
}

void NJOYMacroCrossSectionCalculator::calculateNumberDensity()
{
    numberDensity = (materialDensity*avogadro)/atomicMass;
}
