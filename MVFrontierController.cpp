#include "MVFrontierController.h"


namespace MeanVarianceFrontier
{
    MeanVarianceFrontier::MVFrontierController::MVFrontierController(std::string csvFileDir, std::string csvOutputDir, std::vector<std::string> csvFileNames) :
        m_csvFileDir{ csvFileDir },
        m_csvOutputDir{ csvOutputDir },
        m_csvFileNames{ csvFileNames },
        m_historicalData{}
    {
        for (const auto& fileName : csvFileNames)
        {
            m_historicalData.emplace_back(std::make_pair(fileName,
                MeanVarianceFrontier::CsvReaderForHistoricalData{ m_csvFileDir, fileName }));
        }
        std::vector<RawData> rawDataInput;
        std::transform(m_historicalData.begin(), m_historicalData.end(), std::back_inserter(rawDataInput), [](std::pair<std::string, MeanVarianceFrontier::CsvReaderForHistoricalData>& pair)
        {
            return RawData{ pair.first, pair.second.getHistoricalPrices() };
        });
        MeanVarianceFrontier::PrepareRawData rawData{ rawDataInput };
        rawData.limitDataByDate();
        rawData.removeAllMissingValues();
        m_dailyReturns = rawData.getDailyReturns();
        createPortfolio();
    }

    void MeanVarianceFrontier::MVFrontierController::createPortfolio()
    {
        MeanVarianceFrontier::StatsAnalyzer stasAnalyzer;
        std::vector<ComputeDailyReturns::ReturnsData> returnsData;
        for (auto dailyReturns : m_dailyReturns)
        {
            //TEMP
            auto data = dailyReturns.getReturnsDataWithoutDates();
            std::cout << data.financialDataName << " average return = " << (stasAnalyzer.getMean(boost::get<std::vector<double>>(data.returnsData))) * 100 << "%" << std::endl;
            std::cout << data.financialDataName << " cumulative return = " << (stasAnalyzer.getSum(boost::get<std::vector<double>>(data.returnsData))) * 100 << "%" << std::endl;
            std::cout << data.financialDataName << " variance of return = " << (stasAnalyzer.getVariance(boost::get<std::vector<double>>(data.returnsData))) * 100 << "%" << std::endl;
            //TEMP
            returnsData.push_back(dailyReturns.getReturnsDataWithoutDates());
        }
        auto covarMat = stasAnalyzer.getVarianceCovarianceMatrix(returnsData);
        auto expRetVec = stasAnalyzer.getAllMeans(returnsData);
        auto retVarianceVec = stasAnalyzer.getAllVariance(returnsData);
        m_portfolio = std::make_shared<MeanVarianceFrontier::Portfolio>(static_cast<unsigned int>(returnsData.size()), covarMat, expRetVec, retVarianceVec);
        //TEMP
        std::cout << covarMat << std::endl;
        std::cout << "Asset Average Daily Returns ( ";
        for (auto expRet : expRetVec)
        {
            std::cout << expRet * 100 << "%, ";
        }
        std::cout << ")\n";
        //TEMP
    }

    std::shared_ptr<Portfolio> MVFrontierController::getPortfolio()
    {
        return m_portfolio;
    }

    void MVFrontierController::writeCsvsToOutputDir()
    {
        for (size_t i = 0; i < m_csvFileNames.size(); i++)
        {
            auto data = m_dailyReturns[i].getReturnsDataWithoutDates();
            m_dailyReturns[i].writeDataToFile(m_csvOutputDir + m_csvFileNames[i]);
        }
    }



}