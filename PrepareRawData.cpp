#include "PrepareRawData.h"

namespace MeanVarianceFrontier
{
    PrepareRawData::PrepareRawData(std::vector<RawData> rawDataVector) :
        m_dailyReturnsData{ }
    {
        for (auto& returnsVector : rawDataVector)
        {
            m_dailyReturnsData.emplace_back(returnsVector.financialDataName, std::move(returnsVector.rawData));
        }
    }

    void PrepareRawData::limitDataByDate()
    {
        auto limitDate = findMaxEarliestDateAmongReturnsData();
        for (auto& data : m_dailyReturnsData)
        {
            auto& rawReturnsData = data.getReturnsData();
            auto removeAllElementsBeforeOrAfter = std::find_if(rawReturnsData.begin(), rawReturnsData.end(),
                [&limitDate](const std::pair<boost::gregorian::date, double>& x)
            { return x.first == limitDate; });
            if (rawReturnsData.front().first < rawReturnsData.back().first)
            {
                rawReturnsData.erase(rawReturnsData.begin(), removeAllElementsBeforeOrAfter);
            }
            else
            {
                rawReturnsData.erase(removeAllElementsBeforeOrAfter, rawReturnsData.end());
            }
        }

    }

    boost::gregorian::date PrepareRawData::findMaxEarliestDateAmongReturnsData()
    {
        std::vector<boost::gregorian::date> earliestDates;
        for (auto& data : m_dailyReturnsData)
        {
            auto rawReturnsData = data.getReturnsData();
            if (rawReturnsData.front().first < rawReturnsData.back().first)
                earliestDates.emplace_back(rawReturnsData.front().first);
            else
                earliestDates.emplace_back(rawReturnsData.back().first);
        }
        return *(std::max_element(earliestDates.begin(), earliestDates.end()));
    }

    void PrepareRawData::removeAllMissingValues()
    {
        size_t repeat = m_dailyReturnsData.size();
        for (size_t i = 0; i < repeat; i++)
        {
            removeMissingValues();
            if (allReturnDataSizesMatch())
                break;
        }
    }

    bool PrepareRawData::allReturnDataSizesMatch()
    {
        std::vector<size_t> returnDataSizes;
        for (auto& dailyReturns : m_dailyReturnsData)
        {
            returnDataSizes.push_back(dailyReturns.getReturnsData().size());
        }
        if (std::adjacent_find(returnDataSizes.begin(), returnDataSizes.end(), std::not_equal_to<>()) ==
            returnDataSizes.end())
            return true;
        return false;
    }

    void PrepareRawData::removeMissingValues()
    {
        MeanVarianceFrontier::ComputeDailyReturns* smallestDailyReturn = nullptr;
        size_t smallestVectorSize = SIZE_MAX;
        for (auto& dailyReturnsData : m_dailyReturnsData)
        {
            size_t dataSize = dailyReturnsData.getReturnsData().size();
            if (smallestVectorSize > dataSize)
            {
                smallestVectorSize = dailyReturnsData.getReturnsData().size();
                smallestDailyReturn = &dailyReturnsData;
            }
        }
        if (smallestDailyReturn != nullptr)
        {
            auto& smallestDailyReturnVector = smallestDailyReturn->getReturnsData();

            for (auto& dailyReturnsData : m_dailyReturnsData)
            {
                auto& otherVec = dailyReturnsData.getReturnsData();
                auto temp = getIntersectionOfDateReturns(smallestDailyReturnVector, otherVec);
                std::swap(temp, otherVec);
            }
        }
    }

    std::vector<MeanVarianceFrontier::ComputeDailyReturns> PrepareRawData::getDailyReturns()
    {
        return m_dailyReturnsData;
    }

    std::vector<std::pair<boost::gregorian::date, double>> PrepareRawData::getIntersectionOfDateReturns(
        const std::vector<std::pair<boost::gregorian::date, double>>& reference, 
        const std::vector<std::pair<boost::gregorian::date, double>>& toModify)
    {
        std::vector<std::pair<boost::gregorian::date, double>> temp;
        auto refIter = reference.begin();
        auto toModifyIter = toModify.begin();
        while (refIter != reference.end() || toModifyIter != toModify.end())
        {
            if (refIter->first == toModifyIter->first)
            {
                temp.push_back(*toModifyIter);
                ++refIter;
                if (refIter == reference.end())
                    break;
            }
            while (refIter->first < toModifyIter->first)
            {
                ++refIter;
            }
            if (refIter->first > toModifyIter->first)
            {
                ++toModifyIter;
                if (toModifyIter == toModify.end())
                    break;
            }
        }
        return temp;
    }
}