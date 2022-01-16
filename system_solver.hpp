#pragma once

#include <iostream>
#include <Eigen/Dense>
#include "pde_matrix_builder.hpp"

/*
Short class that solves the matrix system to get f^n from f^{n+1}.
Public method: get_fn
Private method: inverse_ktilde
*/

class SystemSolver 
{
public:

	virtual ~SystemSolver() = default;
	SystemSolver(PDEMatrixBuilder* mat_build);
	PDEMatrixBuilder* mat_build_;
	Eigen::MatrixXd get_fn(Eigen::MatrixXd& fn1, const int& nb_spot_steps, const double& dx, const double& dt) const;

private:

	Eigen::MatrixXd inverse_ktilde(const int& nb_spot_steps, const double& dx, const double& dt) const;

};