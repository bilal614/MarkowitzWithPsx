#pragma once
#include <vector>
#include <utility>
#include "boost/date_time/gregorian/gregorian.hpp"
#include <algorithm>
#include "boost/variant.hpp"
#include "CsvWriter.h"

namespace MeanVarianceFrontier
{
    class ComputeDailyReturns
    {
    public:
        struct ReturnsData
        {
            std::string financialDataName;
            boost::variant<std::vector<std::pair<boost::gregorian::date, double>>, std::vector<double>> returnsData;
        };
    public:
        ComputeDailyReturns(std::string financialDataName, std::vector<std::pair<boost::gregorian::date, double>>&& inputData);
        void writeDataToFile(std::string filePath);
        std::vector<std::pair<boost::gregorian::date, double>>& getReturnsData();
        ReturnsData getReturnsDataWithoutDates();
        ReturnsData getReturnsDataWithDates();
    private:
        void operator()();
        std::string m_financialDataName;
        std::vector<std::pair<boost::gregorian::date, double>> m_inputData;
        std::vector<std::pair<boost::gregorian::date, double>> m_returnsData;
    };
}