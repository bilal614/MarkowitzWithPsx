#include "StatsAnalyzer.h"

namespace MeanVarianceFrontier
{
    double StatsAnalyzer::getMean(std::vector<double> data)
    {
        accumulator_set< double, features<tag::mean>> acc;
        acc = std::for_each(data.begin(), data.end(), acc);
        return mean(acc);
    }
    double StatsAnalyzer::getSum(std::vector<double> data)
    {
        return std::accumulate(data.begin(), data.end(), 0.0L);
    }

    double StatsAnalyzer::getVariance(std::vector<double> data)
    {
        accumulator_set<double, stats<tag::variance(lazy)> > acc;
        acc = std::for_each(data.begin(), data.end(), acc);
        return variance(acc);
    }

    double StatsAnalyzer::getStdDev(std::vector<double> data)
    {
        return std::sqrt(getVariance(data));
    }

    std::vector<double> StatsAnalyzer::getAllMeans(std::vector<ComputeDailyReturns::ReturnsData> returnsData)
    {
        std::vector<double> expectedReturns;
        for (auto data : returnsData)
        {
            auto mean = getMean(boost::get<std::vector<double>>(data.returnsData));
            expectedReturns.push_back(mean);
        }
        return expectedReturns;
    }

    std::vector<double> StatsAnalyzer::getAllVariance(std::vector<ComputeDailyReturns::ReturnsData> returnsData)
    {
        std::vector<double> returnsVariance;
        for (auto data : returnsData)
        {
            
            auto variance = getVariance(boost::get<std::vector<double>>(data.returnsData));
            returnsVariance.push_back(variance);
        }
        return returnsVariance;
    }

    bool StatsAnalyzer::invertMatrix(const boost::numeric::ublas::matrix<double>& input, boost::numeric::ublas::matrix<double>& inverse)
    {
        size_t nrOfRows = input.size1();
        size_t nrOfCols = input.size2();
        if (nrOfRows != nrOfCols)
        {
            return false;
        }
        inverse.resize(nrOfRows, nrOfCols * 2, false);
        for (size_t i = 0; i < nrOfRows; i++)
        {
            for (size_t j = 0; j < nrOfCols; j++)
            {
                inverse(i, j) = input(i, j);
            }
        }

        for (size_t i = 0; i < nrOfRows; i++)
        {
            for (size_t j = 0; j < nrOfCols; j++)
            {
                if (i == j)
                {
                    inverse(i, j + nrOfCols) = 1;
                }
                else
                {
                    inverse(i, j + nrOfCols) = 0;
                }
            }
        }

        for (size_t i = 0; i < nrOfRows; i++)
        {
            if (inverse(i, i) == 0)
                return false;

            for (size_t j = 0; j < nrOfCols; j++)
            {
                double ratio = 0l;
                if (i != j)
                {
                    ratio = inverse(j, i) / inverse(i, i);
                    for (size_t k = 0; k < 2 * nrOfRows; k++)
                    {
                        inverse(j, k) = inverse(j, k) - ratio * inverse(i, k);
                    }
                }
            }
        }

        for (size_t i = 0; i < nrOfRows; i++)
        {
            for (size_t j = nrOfCols; j < 2*nrOfCols; j++)
            {
                inverse(i, j) = inverse(i, j) / inverse(i, i);
            }
        }

        for (size_t i = 0; i < nrOfRows; i++)
        {
            for (size_t j = 0; j < nrOfCols; j++)
            {
                inverse(i, j) = inverse(i, j + nrOfCols);
            }
        }
        inverse.resize(nrOfRows, nrOfCols, true);

        return true;
    }

    double StatsAnalyzer::getMinRiskForGivenExpReturn(double givenExpReturn, const boost::numeric::ublas::vector<double>& expReturns, const boost::numeric::ublas::matrix<double>& inverseCovarMat, boost::numeric::ublas::vector<double>& weights)
    {
        boost::numeric::ublas::vector<double> onesVector{ expReturns.size() };
        for (size_t i = 0; i < onesVector.size(); i++)
            onesVector(i) = 1.0l;

        auto a = inner_prod(prod(expReturns, inverseCovarMat), expReturns);
        auto b = inner_prod(prod(expReturns, inverseCovarMat), onesVector);
        auto c = inner_prod(prod(onesVector, inverseCovarMat), onesVector);
        auto d = a*c - b*b;
        
        auto lambda1 = (c*givenExpReturn - b) / d;
        auto lambda2 = (-1.0l*b*givenExpReturn + a) / d;
        double minVariance = (c*givenExpReturn*givenExpReturn - 2 * b*givenExpReturn + a) / d;
        weights.clear();
        weights = lambda1*prod(expReturns, inverseCovarMat) + lambda2*prod(onesVector, inverseCovarMat);
        return minVariance;
    }


    void StatsAnalyzer::printMatrix(const boost::numeric::ublas::matrix<double>& mat)
    {
        for (unsigned int i = 0; i < mat.size1(); i++)
        {
            for (unsigned int j = 0; j < mat.size2(); j++)
            {
                std::cout << std::setw(4) << mat(i, j);
                std::string delimiter = (j < (mat.size2() - 1)) ? ", " : "\n";
                std::cout << delimiter;
            }
        }
        std::cout << std::endl;
    }

    void StatsAnalyzer::printVector(const boost::numeric::ublas::vector<double>& vec)
    {
        for (size_t i = 0; i < vec.size(); i++)
        {
            std::cout << std::setw(4) << vec(i);
            std::string delimiter = (i < (vec.size() - 1)) ? ", " : "\n";
            std::cout << delimiter;
        }
    }

    boost::numeric::ublas::matrix<double> StatsAnalyzer::getVarianceCovarianceMatrix(std::vector<ComputeDailyReturns::ReturnsData> data)
    {
        auto matrixRowColSize = data.size();
        boost::numeric::ublas::matrix<double> varCovarMat{ matrixRowColSize, matrixRowColSize };
        for (auto outer_iterator = data.begin(); outer_iterator != data.end(); ++outer_iterator)
        {
            auto row = std::distance(data.begin(), outer_iterator);
            for (auto inner_iterator = data.begin(); inner_iterator != data.end(); ++inner_iterator)
            {
                auto col = std::distance(data.begin(), inner_iterator);
                accumulator_set<double, stats<tag::covariance<double, tag::covariate1> > > acc;
                try
                {
                    auto returnsForRow = boost::get<std::vector<double>>(outer_iterator->returnsData);
                    auto returnsForCol = boost::get<std::vector<double>>(inner_iterator->returnsData);
                    if (returnsForRow.size() == returnsForCol.size())
                    {
                        double rowVal, colVal;
                        BOOST_FOREACH(boost::tie(rowVal, colVal) , boost::combine(returnsForRow, returnsForCol))
                        {
                            acc(rowVal, covariate1 = colVal);
                        }
                        varCovarMat(row, col) = covariance(acc);
                    }
                }
                catch (boost::bad_get&) 
                {
                }
            }
        }
        return varCovarMat;
    }

}