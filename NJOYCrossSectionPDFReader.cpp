#include "NJOYCrossSectionPDFReader.h"

#include <QFile>
#include <QDebug>
#include <QRegularExpression>

NJOYCrossSectionPDFReader::NJOYCrossSectionPDFReader()
{
    fileName = "/home/andreiasimas/Documentos/output_N.txt";
}

void NJOYCrossSectionPDFReader::readFile(const QString &mFileName)
{
    fileName = mFileName;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"File Error";
        return;
    }

    const QString key1 = "mt";
    const QString key2 = "mf";

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();

        QString text = QString::fromUtf8(line);

        if (text.contains(key1) && text.contains(key2))
        {
            int index = text.indexOf(key1) + 1;

            QString mtNumber = text.mid(index + key1.length(), 2).trimmed();

            processLine(file, mtNumber.toInt());
        }
    }
}

void NJOYCrossSectionPDFReader::saveFile(const QString &mFileName)
{
    QFile file(mFileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {

        QTextStream out(&file);

        out << "Output Macroscopic Cross Section\n";

        for (const auto &mat: sigmas)
        {
            out<<"\nMaterial Number (MT) = " << mat.first << Qt::endl;

            auto crossSection = mat.second;

            for (const auto &value: crossSection)
            {
                out << value.first << " " << double(value.second) << Qt::endl;
            }
        }

        // Fecha o arquivo
        file.close();
    }
}

void NJOYCrossSectionPDFReader::processLine(QFile &file, int mtNumber)
{
    QString text;

    file.readLine();
    file.readLine();

    std::map<int, long double> value;
    std::map<std::pair<int, int>, std::vector<long double>> scatteringMatrix;

    while (!file.atEnd() && !text.contains("group constants"))
    {
        auto line = file.readLine();

        text = QString::fromUtf8(line).trimmed();

        auto textVect = text.split(" ");

        QRegularExpression exp("^.+$"); // expressão regular que corresponde a strings não vazias
        textVect = textVect.filter(exp); // filtra as strings que não estão vazias

        if (true == textVect.isEmpty())
        {
            continue;
        }
        else if (textVect.size() == 2)
        {
            bool isANumber = false;
            int group = textVect[0].toInt(&isANumber);

            if (false == isANumber)
            {
                continue;
            }

            long double sigmaValue;

            if (false == changeCientificNotation(textVect[1], sigmaValue))
            {
                continue;
            }

            if (true == isANumber)
            {
                value[group] = sigmaValue;
            }
        }
        else
        {
            std::pair<int, int> ggline;

            bool isANumber = false;
            int group1 = textVect[0].toInt(&isANumber);

            if (false == isANumber)
            {
                continue;
            }

            int group2 = textVect[1].toInt(&isANumber);

            if (false == isANumber)
            {
                continue;
            }

            ggline.first  = group1;
            ggline.second = group2;

            std::vector <long double> legendreValuesCrossSections;

            //std::cout<<group1<<" "<<group2<<" ";

            for (auto it = textVect.begin() + 2; it != textVect.end(); ++it)
            {
                long double value = 0;

                changeCientificNotation(*it, value);

                legendreValuesCrossSections.push_back(value);

                //std::cout<<value<<" ";
            }
            // std::cout<<std::endl;
            scatteringMatrix[ggline] = legendreValuesCrossSections;
        }
        //  std::cout<<std::endl;
    }

    // qInfo()<<"trocou de mt";

    if (false == value.empty())
        sigmas[mtNumber] = value;

    if (false == scatteringMatrix.empty())
        sigmasMatrix[mtNumber] = scatteringMatrix;
}

bool NJOYCrossSectionPDFReader::changeCientificNotation(QString &text, long double &number)
{
    bool isANumber = false;

    if (text.contains("+"))
    {
        number = text.replace("+", "E+").toDouble(&isANumber);
    }
    else if (text.contains("-"))
    {
        number = text.replace("-", "E-").toDouble(&isANumber);
    }

    return isANumber;
}

NJOYCrossSectionPDFReader::SCATTERING &NJOYCrossSectionPDFReader::getSigmasMatrix()
{
    return sigmasMatrix;
}

NJOYCrossSectionPDFReader::CROSS_SECTION &NJOYCrossSectionPDFReader::getSigmas()
{
    return sigmas;
}

QString NJOYCrossSectionPDFReader::getFileName() const
{
    return fileName;
}

void NJOYCrossSectionPDFReader::setFileName(const QString &newFileName)
{
    fileName = newFileName;
}
