#pragma once

#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <utility>


namespace MeanVarianceFrontier
{
class CsvReaderForHistoricalData 
{
public:
    CsvReaderForHistoricalData(std::string csvFileDir, std::string fileName);
    CsvReaderForHistoricalData(std::string fileName);
    std::vector<std::pair<boost::gregorian::date, double>> getHistoricalPrices() const;
private:
    void readAndPopulateHistoricalPrices();
    std::pair<boost::gregorian::date, double> parseDateAndPriceFrom(std::string& line);
    boost::gregorian::date convertStringToDate(std::string dateString);

private:
    std::string m_fileName;
    std::string m_csvFilePath;
    std::ifstream m_csvFile;
    std::vector<std::pair<boost::gregorian::date, double>> m_historicalPrices;
};

}