#include "Portfolio.h"
#include <time.h> 

namespace MeanVarianceFrontier
{
    Portfolio::Portfolio(unsigned int numberOfAssets, boost::numeric::ublas::matrix<double> covarianceMatrix, std::vector<double> expReturns, std::vector<double> variances) : 
        m_portfolioMeanVariance{ numberOfAssets }, 
        m_covarianceMatrix{ covarianceMatrix },
        m_expReturnsVector{ numberOfAssets },
        m_returnsVarianceVector{ numberOfAssets }
    {
        srand(static_cast<unsigned int>(time(NULL)));
        auto index = 0;
        for (auto expReturn : expReturns)
        {
            m_expReturnsVector(index) = expReturn;
            ++index;
        }
        
        index = 0;
        for (auto var : variances)
        {
            m_returnsVarianceVector(index) = var;
            ++index;
        }
    }

    std::vector<double> Portfolio::generateRandomWeights()
    {
        std::vector<double> portfolioWeights;
        double sumOfWeights = 0.0l;
        const double portfolioMax = 1.0l;
        for (unsigned int i = 0; i < m_portfolioMeanVariance.numberOfAssets; i++)
        {
            if (i != m_portfolioMeanVariance.numberOfAssets - 1)
            {
                double weight = fRand(sumOfWeights, portfolioMax) - sumOfWeights;
                portfolioWeights.push_back(weight);
                sumOfWeights += weight;
            }
            else
            {
                double weight = portfolioMax - sumOfWeights;
                portfolioWeights.push_back(weight);
            }
        }

        return portfolioWeights;
    }

    double Portfolio::computeMeanWith(std::vector<double> portfolioWeights)
    {
        unsigned int index = 0;
        double meanReturn = 0.0l;
        for (auto weight : portfolioWeights)
        {
            meanReturn += (m_expReturnsVector[index] * weight);
            ++index;
        }
        return meanReturn;
    }

    double Portfolio::computeVarianceWith(std::vector<double> portfolioWeights)
    {
        boost::numeric::ublas::vector<double> weights{ portfolioWeights .size() };
        unsigned int index = 0;
        for (auto weight : portfolioWeights)
        {
            weights(index) = weight;
            ++index;
        }
        return inner_prod(prod(weights, m_covarianceMatrix), weights);
    }

    const boost::numeric::ublas::matrix<double> Portfolio::getCovarianceMatrix()
    {
        return m_covarianceMatrix;
    }

    const boost::numeric::ublas::vector<double> Portfolio::getExpReturnsVector()
    {
        return m_expReturnsVector;
    } 

    double MeanVarianceFrontier::Portfolio::fRand(double fMin, double fMax)
    {
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }
}