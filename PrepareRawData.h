#pragma once
#include "ComputeReturns.h"

namespace MeanVarianceFrontier
{
    struct RawData
    {
        std::string financialDataName;
        std::vector<std::pair<boost::gregorian::date, double>> rawData;
    };

    class PrepareRawData
    {
    public:
        /*PrepareRawData(std::vector<std::vector<std::pair<boost::gregorian::date, double>>> rawDataVector);*/
        PrepareRawData(std::vector<RawData> rawDataVector);
        void limitDataByDate();
        boost::gregorian::date findMaxEarliestDateAmongReturnsData();
        void removeAllMissingValues();
        std::vector<MeanVarianceFrontier::ComputeDailyReturns> getDailyReturns();
    private:
        bool allReturnDataSizesMatch();
        void removeMissingValues();
        std::vector<std::pair<boost::gregorian::date, double>> getIntersectionOfDateReturns
        (const std::vector<std::pair<boost::gregorian::date, double>>& reference, 
            const std::vector<std::pair<boost::gregorian::date, double>>& toModify);

        std::vector<MeanVarianceFrontier::ComputeDailyReturns> m_dailyReturnsData;
    };
}
