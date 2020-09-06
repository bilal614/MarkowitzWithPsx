#pragma once
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace MeanVarianceFrontier
{
    class Portfolio
    {
    public:
        Portfolio(unsigned int numberOfAssets, boost::numeric::ublas::matrix<double> covarianceMatrix, std::vector<double> expReturns, std::vector<double> variances);

        std::vector<double> generateRandomWeights();
        double computeMeanWith(std::vector<double> portfolioWeights);
        double computeVarianceWith(std::vector<double> portfolioWeights);
        const boost::numeric::ublas::matrix<double> getCovarianceMatrix();
        const boost::numeric::ublas::vector<double> getExpReturnsVector();
    private:
        double fRand(double fMin, double fMax);
    private:
        struct PortfolioMeanVariance
        {
            PortfolioMeanVariance(unsigned int nmbrOfAssets) : numberOfAssets{ nmbrOfAssets } {}
            unsigned int numberOfAssets;
            std::vector<double> portfolioWeights;
            double portfolioMeanReturn;
            double portfolioVariance;
        } m_portfolioMeanVariance;

        boost::numeric::ublas::matrix<double> m_covarianceMatrix;
        boost::numeric::ublas::vector<double> m_expReturnsVector;
        boost::numeric::ublas::vector<double> m_returnsVarianceVector;
    };
}
