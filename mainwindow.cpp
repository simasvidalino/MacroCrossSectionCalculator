#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "qdebug.h"

#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      calculator(materialDensity, atomicMass)
{
    ui->setupUi(this);

    setConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onCalculate()
{
    atomicMass = ui->doubleSpinBoxAtomicMass->value();
    materialDensity = ui->doubleSpinBoxMaterialDensity->value();

    NJOYCrossSectionPDFReader reader;
    reader.readFile(fileName);

    auto &sigmas = reader.getSigmas();

    calculateMacroCrossSection(sigmas);

    auto &scattering = reader.getSigmasMatrix();

    calculateScatteringMacroCrossSection(scattering);

    reader.saveFile("/home/andreiasimas/Documentos/resultMacroCrossSection");
}

void MainWindow::onOpenFile()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    fileName = QFileDialog::getOpenFileName(this, tr("Open"), defaultPath ,
                                            tr("txt (*txt)"));
}

void MainWindow::calculateMacroCrossSection(NJOYCrossSectionPDFReader::CROSS_SECTION &sigmas)
{
    if (sigmas.empty())
    {
        qInfo()<<"sigmas values are empt";
        return;
    }

    NJOYMacroCrossSectionCalculator calculator(materialDensity, atomicMass);

    for (auto &crossSectionByGroup: sigmas)
    {
        calculator.calculateMacroCrossSection(crossSectionByGroup.second);
    }
}

void MainWindow::calculateScatteringMacroCrossSection(NJOYCrossSectionPDFReader::SCATTERING &scattering)
{
    if (scattering.empty())
    {
        qInfo()<<"scattering values are empt";
        return;
    }

    NJOYMacroCrossSectionCalculator calculator(materialDensity, atomicMass);

    for (auto &crossSectionByGrouptoGroup:scattering)
    {
        calculator.calculateScatteringMacroCrossSection(crossSectionByGrouptoGroup.second);
    }
}

void MainWindow::setConnection()
{
    connect(ui->toolButtonFile, &QToolButton::clicked, this, &MainWindow::onOpenFile);
    connect(ui->pushButtonCalculate, &QPushButton::clicked, this, &MainWindow::onCalculate);
}

