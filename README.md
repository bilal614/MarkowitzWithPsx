# MarkowitzWithPsx
Computation of Markowitz efficient frontier and minimum variance portfolio for a handful of PSX(Pakistan Stock Exchange) assets.

## Requirements
* Any Linux Distro (Preferably Ubuntu, Lubuntu or Debian)
* BOOST libraries
* Netbeans IDE (uses Makefiles)

## Description
The project provides a computation of the Efficient frontier and Minimum Variance Portfolio for a selected handful of PSX(Pakistan Stock Exchange) 
listed assets. The data for the selected stocks is included with the project as a set of CSV files. The Efficient frontier is computed as a Lagrangian 
optimization problem with allowance of negative weights.

For the constraint of "no negative weights", the portfolio weights are computed with a brute force method of randomly generating a set of portfolio return 
and variance points and computing the weights for such points. The computed output of efficient frontier and other computations is saved in output folder.
