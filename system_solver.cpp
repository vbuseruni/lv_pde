#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "pde_matrix_builder.hpp"
#include "system_solver.hpp"

SystemSolver::SystemSolver(PDEMatrixBuilder* mat_build) : mat_build_(mat_build)
{

}

Eigen::MatrixXd SystemSolver::inverse_ktilde(const int& nb_spot_steps, const double& dx, const double& dt) const
{
	Eigen::MatrixXd ktilde = mat_build_->comp_ktilde(nb_spot_steps, dx, dt);
	Eigen::MatrixXd res = ktilde.inverse();
	return res;
}

Eigen::MatrixXd SystemSolver::get_fn(Eigen::MatrixXd& fn1, const int& nb_spot_steps, const double& dx, const double& dt) const
{
	Eigen::MatrixXd fn = Eigen::MatrixXd::Zero(nb_spot_steps + 1, 1);
	fn = SystemSolver::inverse_ktilde(nb_spot_steps, dx, dt) * mat_build_->comp_k(nb_spot_steps, dx, dt) * fn1 - dt * mat_build_->comp_k(nb_spot_steps, dx, dt) * mat_build_->comp_system_constant(nb_spot_steps, dx, dt);
	return fn;

}