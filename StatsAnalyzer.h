#pragma once
#include <algorithm>
#include <numeric>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/accumulators/statistics/covariance.hpp>
#include <boost/accumulators/statistics/variates/covariate.hpp>
#include <boost/range/combine.hpp>
#include <boost/foreach.hpp>
#include <iterator>
#include "ComputeReturns.h"


namespace MeanVarianceFrontier
{
    using namespace boost::accumulators;
    class StatsAnalyzer
    {
    public:
        double getMean(std::vector<double> data);
        double getSum(std::vector<double> data);
        double getVariance(std::vector<double> data);
        double getStdDev(std::vector<double> data);
        std::vector<double> getAllMeans(std::vector<ComputeDailyReturns::ReturnsData> returnsData);
        std::vector<double> getAllVariance(std::vector<ComputeDailyReturns::ReturnsData> returnsData);
        boost::numeric::ublas::matrix<double> getVarianceCovarianceMatrix(std::vector<ComputeDailyReturns::ReturnsData> data);

        bool invertMatrix(const boost::numeric::ublas::matrix<double>& input, boost::numeric::ublas::matrix<double>& inverse);
        double getMinRiskForGivenExpReturn(double givenExpReturn, const boost::numeric::ublas::vector<double>& expReturns, const boost::numeric::ublas::matrix<double>& inverseCovarMat, boost::numeric::ublas::vector<double>& weights);

        void printMatrix(const boost::numeric::ublas::matrix<double>& mat);
        void printVector(const boost::numeric::ublas::vector<double>& vec);
    };

}