#include "MVFrontierController.h"
#include <experimental/filesystem>
#include <string>
#include "Portfolio.h"

int main(int argc, char** argv)
{
    std::string csvFileDir;
    std::string csvOutputDir;
    std::vector<std::string> csvFileNames{ 
                
        "Akzo Nobel Pak.csv", "Atlas Honda Ltd.csv", "Bank Al-Falah.csv",
        "Century Paper.csv", "Cherat Cement.csv", "Engro Corp.csv",
        "Fauji Fert.csv", "Habib Sugar.csv", "Highnoon (Lab).csv",
        "Lucky Cement.csv", "P.S.O.csv", "Oil & Gas Dev.csv",
        "GlaxoSmithKline.csv", "Honda Atlas Cars.csv", "Indus Motor Co.csv",
        "National Bank.csv", "Nishat (Chun.).csv"
    };

    if (argc > 1)
    {
        csvFileDir = std::string(argv[1]) + "/data/";
        csvOutputDir = std::string(argv[1]) + "/output/";
        if(!std::experimental::filesystem::exists(csvOutputDir))
            std::experimental::filesystem::create_directories(csvOutputDir);
    }
    else
    {
        csvFileDir = std::experimental::filesystem::current_path().u8string() + "/data/";
        csvOutputDir = std::experimental::filesystem::current_path().u8string() + "/output/";
        if (!std::experimental::filesystem::exists(csvOutputDir))
            std::experimental::filesystem::create_directories(csvOutputDir);
    }
    MeanVarianceFrontier::MVFrontierController mvFrontierController{ csvFileDir, csvOutputDir, csvFileNames };

    mvFrontierController.writeCsvsToOutputDir();

    auto myPortfolio = mvFrontierController.getPortfolio();
    MeanVarianceFrontier::CsvWriter csvWriter{ csvOutputDir + "MeanVarianceFrontier.csv", {"weighted portfolio variance", "weighted portfolio returns" } };
    for (int i = 0; i < 2500; i++)
    {
        auto weights = myPortfolio->generateRandomWeights();
        auto weightSum = std::accumulate(weights.begin(), weights.end(), 0.0l);
        auto portfolioMean = myPortfolio->computeMeanWith(weights) * 100;
        auto portfolioVariance = myPortfolio->computeVarianceWith(weights) * 100;
        csvWriter.writeLine(std::to_string(portfolioVariance), std::to_string(portfolioMean));
    }

    
    MeanVarianceFrontier::StatsAnalyzer statsAnalyzer;
    
    const auto covarMat = myPortfolio->getCovarianceMatrix();
    boost::numeric::ublas::matrix<double> inverseCovarMat{ covarMat.size1(),covarMat.size1() };
    statsAnalyzer.printMatrix(covarMat);
    statsAnalyzer.invertMatrix(covarMat, inverseCovarMat);
    statsAnalyzer.printMatrix(inverseCovarMat);

    const auto expReturnsVector = myPortfolio->getExpReturnsVector();
    statsAnalyzer.printVector(expReturnsVector);


    boost::numeric::ublas::vector<double> weights{ expReturnsVector.size() };
    for (size_t i = 0; i < weights.size(); i++)
    {
        weights(i) = 0.0l;
    }

    double expRet = 0.0l;
    double minVar = statsAnalyzer.getMinRiskForGivenExpReturn(expRet, expReturnsVector, inverseCovarMat, weights);
    std::cout << "MinVariance for" << expRet << " portfolio return: " << minVar << std::endl;
    statsAnalyzer.printVector(weights);
    MeanVarianceFrontier::CsvWriter csvWriterForEfficientFrontier{ csvOutputDir + "EfficientFrontier.csv",{ "weighted portfolio variance" , "weighted portfolio returns" } };
    
    std::vector<std::pair<double, double>> efficientFrontier;
    std::vector<double> portfolioVarianceDeltas;
    double prevPortfolioVariance = 0.0l;
    for (int i = 0; i < 1500; i++)
    {
        
        auto portfolioVariance = statsAnalyzer.getMinRiskForGivenExpReturn(expRet, expReturnsVector, inverseCovarMat, weights);
        //csvWriterForEfficientFrontier.writeLine(std::to_string(portfolioVariance*100), std::to_string(expRet * 100));
        efficientFrontier.emplace_back(portfolioVariance*100, expRet * 100);
        if(i > 0)
        {
            portfolioVarianceDeltas.emplace_back(std::abs(portfolioVariance - prevPortfolioVariance));
        }        
        prevPortfolioVariance = portfolioVariance;
        expRet += 0.0000025l;
    }
    
    int minVarianceIndex = std::min_element(portfolioVarianceDeltas.begin(),portfolioVarianceDeltas.end()) - portfolioVarianceDeltas.begin();
    std::cout << "minVarianceIndex=" << minVarianceIndex << std::endl;
    for(int i = minVarianceIndex; i < efficientFrontier.size(); i++)
    {
        csvWriterForEfficientFrontier.writeLine(std::to_string(efficientFrontier[i].first),std::to_string(efficientFrontier[i].second));
    }
}
