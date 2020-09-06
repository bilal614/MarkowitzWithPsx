#pragma once
#include <boost/variant.hpp>
#include "CsvReaderForHistoricalData.h"
#include "ComputeReturns.h"
#include "CsvWriter.h"
#include "PrepareRawData.h"
#include "StatsAnalyzer.h"
#include "Portfolio.h"

namespace MeanVarianceFrontier
{
    class MVFrontierController
    {
    public:
        MVFrontierController(std::string csvFileDir, std::string csvOutputDir, std::vector<std::string> csvFileNames);
        void writeCsvsToOutputDir();
        void createPortfolio();
        std::shared_ptr<Portfolio> getPortfolio();
        void computeMeanVarianceFrontier();
    private:
        std::string m_csvFileDir;
        std::string m_csvOutputDir;
        std::vector<std::string> m_csvFileNames;
        std::vector<std::pair<std::string, MeanVarianceFrontier::CsvReaderForHistoricalData>> m_historicalData;
        std::vector<MeanVarianceFrontier::ComputeDailyReturns> m_dailyReturns;
        std::shared_ptr<Portfolio> m_portfolio;
    };
}