#pragma

#include <QString>
#include <QFile>
#include <iostream>

class NJOYCrossSectionPDFReader
{
public:
    using CROSS_SECTION = std::map<int, std::map<int, long double>>;
    using SCATTERING = std::map<int, std::map<std::pair<int, int>, std::vector<long double>>>;

    NJOYCrossSectionPDFReader();

    QString getFileName() const;
    void setFileName(const QString &newFileName);

    CROSS_SECTION &getSigmas();
    SCATTERING &getSigmasMatrix();

    void readFile(const QString &mFileName);

    void saveFile(const QString &mFileName);

private:
    void processLine(QFile &file, int mtNumber);

    bool changeCientificNotation(QString &text, long double &number);

    QString fileName;

    CROSS_SECTION sigmas;
    SCATTERING sigmasMatrix;
};

