#include "ComputeReturns.h"


namespace MeanVarianceFrontier
{
    ComputeDailyReturns::ComputeDailyReturns(std::string financialDataName, std::vector<std::pair<boost::gregorian::date, double>>&& inputData) :
        m_financialDataName{ financialDataName.substr(0, financialDataName.find('.')) }, m_inputData{ inputData }, m_returnsData{ }
    {
        (*this)();
    }

    void ComputeDailyReturns::operator()()
    {
        if (m_inputData.front().first > m_inputData.back().first)
        {
            std::sort(m_inputData.begin(), m_inputData.end());
        }
        if (m_inputData.size() > 1)
        {
            auto prevDataPrice = m_inputData.begin();
            auto currentDataPrice = prevDataPrice + 1;
            while (currentDataPrice != m_inputData.end())
            {
                auto ret = (currentDataPrice->second - prevDataPrice->second) / prevDataPrice->second;
                m_returnsData.emplace_back(std::make_pair(currentDataPrice->first, ret));
                ++prevDataPrice;
                ++currentDataPrice;
            }
        }
    }

    void ComputeDailyReturns::writeDataToFile(std::string filePath)
    {
        MeanVarianceFrontier::CsvWriter testCsv(filePath, { "Date", "Returns" });
        for (const auto& val : m_returnsData)
        {
            testCsv.writeLine(boost::gregorian::to_simple_string(val.first), std::to_string(val.second));
        }
    }
    std::vector<std::pair<boost::gregorian::date, double>>& ComputeDailyReturns::getReturnsData() 
    {
        return m_returnsData;
    }

    ComputeDailyReturns::ReturnsData ComputeDailyReturns::getReturnsDataWithoutDates()
    {
        std::vector<double> returnsDataWithoutDates;
        std::transform(m_returnsData.begin(), m_returnsData.end(), std::back_inserter(returnsDataWithoutDates), [](std::pair<boost::gregorian::date, double >& pair)
        {
            return pair.second;
        });
        return { m_financialDataName, returnsDataWithoutDates };
    }

    ComputeDailyReturns::ReturnsData ComputeDailyReturns::getReturnsDataWithDates()
    {
        return { m_financialDataName, m_returnsData };
    }
}