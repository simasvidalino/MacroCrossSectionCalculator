#pragma once

#include <QMainWindow>

#include "NJOYCrossSectionPDFReader.h"

#include "NJOYMacroCrossSectionCalculator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalculate();
    void onOpenFile();

private:
    Ui::MainWindow *ui;

    QString fileName;

    long double atomicMass{1};
    long double materialDensity{1};
    NJOYMacroCrossSectionCalculator calculator;

    void calculateMacroCrossSection(NJOYCrossSectionPDFReader::CROSS_SECTION &sigmas);
    void calculateScatteringMacroCrossSection(NJOYCrossSectionPDFReader::SCATTERING &scattering );
    void setConnection();
};

