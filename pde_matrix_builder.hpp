#pragma once
#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>
#include "boundary_conditions.hpp"

/*
Basically this class constructs the matrices that are in the system of the Theta Scheme, where me have
$Kf^{n+1} = \tilde{K} f^n + dt * d$, where $d \in \R^{N+1}$ is a vector where all the elements are equal to d(x,t), the term of the PDE
which is equal to zero in the Black-Scholes PDE.
As such, this builds the matrices $K$ and $\tilde{K}$ that are of dimensions $N+1 x N+1$, with $\{0, ..., N\}$ the spot steps on the mesh. 
*/

/*
Need to build two matrices plus the vector of dt * d. We can just take as parameters a pointer towards a BoundaryConditions object in order
to have theta, dt, dx and the boundary conditions. Then just need to build functions that compute the elements on all the rows of the matrix.
The first and last rows are the boundary conditions (Neumann), the intermediate rows all have the same coefficients but shifted. 

Est-ce qu'on inverse la matrice ici ou est-ce qu'on le fait dans la partie mesh????

Comme paramètres en plus de ceux de boundary conditions on a besoin du nombre total de time steps inclus dans la mesh!!
*/

class PDEMatrixBuilder
{

public:

	virtual ~PDEMatrixBuilder() = default;
	PDEMatrixBuilder(BoundaryConditions* boundaries_x0, BoundaryConditions* boundaries_xN);
	Eigen::MatrixXd comp_k(const int& nb_spot_steps, const double& dx, const double& dt) const;
	Eigen::MatrixXd comp_ktilde(const int& nb_spot_steps, const double& dx, const double& dt) const;
	BoundaryConditions* boundaries_x0_;
	BoundaryConditions* boundaries_xN_;

	Eigen::MatrixXd comp_system_constant(const int& nb_spot_steps, const double& dx, const double& dt) const; //compute the constant vector in the system

protected:

	
	
private:

	double alpha(const double& dx, const double& dt) const;
	double beta(const double& dx, const double& dt) const;
	double xi(const double& dx, const double& dt) const;

	
	Eigen::MatrixXd comp_row_ktilde(const double& dx, const double& dt) const;
	Eigen::MatrixXd comp_row_k(const double& dx, const double& dt) const;
	

};

