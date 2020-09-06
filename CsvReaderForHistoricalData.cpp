#include "CsvReaderForHistoricalData.h"

namespace MeanVarianceFrontier
{
    CsvReaderForHistoricalData::CsvReaderForHistoricalData(std::string csvFileDir, std::string fileName) :
        m_fileName{ fileName },
        m_csvFilePath{ csvFileDir + m_fileName },
        m_csvFile{ m_csvFilePath, std::ifstream::in }
    {
        if (m_csvFile)
        {
            readAndPopulateHistoricalPrices();
        }
    }

    CsvReaderForHistoricalData::CsvReaderForHistoricalData(std::string fileName) :
        m_fileName{fileName},
        m_csvFilePath{ std::experimental::filesystem::current_path().u8string() + "\\data\\" + m_fileName },
        m_csvFile{ m_csvFilePath, std::ifstream::in }
    {
        if (m_csvFile)
        {
            readAndPopulateHistoricalPrices();
        }
    }

    std::vector<std::pair<boost::gregorian::date, double>> CsvReaderForHistoricalData::getHistoricalPrices() const
    {
        return m_historicalPrices;
    }

    void CsvReaderForHistoricalData::readAndPopulateHistoricalPrices()
    {
        std::string line;
        std::stringstream iss;
        std::getline(m_csvFile, line);//skip first line
        while (std::getline(m_csvFile, line))
        {
            iss.str(line);
            std::string val;
            m_historicalPrices.emplace_back(parseDateAndPriceFrom(line));
        }
    }

    std::pair<boost::gregorian::date, double> CsvReaderForHistoricalData::parseDateAndPriceFrom(std::string& line)
    {
        boost::erase_all(line, "\"");
        std::stringstream iss(line);
        std::string dateString;
        std::string skip;
        std::getline(iss, dateString, ',');
        auto date = convertStringToDate(dateString);
        std::string priceString;
        std::getline(iss, priceString, ',');
        boost::erase_all(priceString, "\"");
        boost::erase_all(priceString, ",");
        auto price = ::atof(priceString.c_str());
        return std::make_pair(date, price);
    }

    boost::gregorian::date CsvReaderForHistoricalData::convertStringToDate(std::string dateString)
    {
        std::stringstream iss(dateString);
        std::string month;
        std::getline(iss, month, '-');
        std::string day;
        std::getline(iss, day, '-');
        std::string year;
        std::getline(iss, year);
        return boost::gregorian::from_uk_string(day + "-" + month + "-" + year);
    }
}