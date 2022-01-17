#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "pde_matrix_builder.hpp"



PDEMatrixBuilder::PDEMatrixBuilder(BoundaryConditions* boundaries_x0, BoundaryConditions* boundaries_xN) :
	boundaries_x0_(boundaries_x0), boundaries_xN_(boundaries_xN)
{

}

double PDEMatrixBuilder::alpha(const double& dx, const double& dt) const
{
	double alph = boundaries_x0_->pde_->coeff_c() - 2 * (boundaries_x0_->pde_->coeff_a()) / pow(dx, 2);
	return alph;
}

double PDEMatrixBuilder::beta(const double& dx, const double& dt) const
{
	double bet = (boundaries_x0_->pde_->coeff_a()) / pow(dx, 2) + (boundaries_x0_->pde_->coeff_b()) / (2 * (dx));
	return bet;
}

double PDEMatrixBuilder::xi(const double& dx, const double& dt) const
{
	double result = (boundaries_x0_->pde_->coeff_a()) / pow(dx, 2) - (boundaries_x0_->pde_->coeff_b()) / (2 * (dx));
	return result;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_row_k(const double& dx, const double& dt) const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(0) = -dt * (1 - boundaries_x0_->get_theta()) * PDEMatrixBuilder::xi(dx, dt);
	res(1) = 1 - dt * (1 - boundaries_x0_->get_theta()) * PDEMatrixBuilder::alpha(dx, dt);
	res(2) = -dt * (1 - boundaries_x0_->get_theta()) * PDEMatrixBuilder::beta(dx, dt);
	return res;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_row_ktilde(const double& dx, const double& dt) const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(0) = dt * boundaries_x0_->get_theta() * PDEMatrixBuilder::xi(dx, dt);
	res(1) = 1 + dt * boundaries_x0_->get_theta() * PDEMatrixBuilder::alpha(dx, dt);
	res(2) = dt * boundaries_x0_->get_theta() * PDEMatrixBuilder::beta(dx, dt);
	return res;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_system_constant(const int& nb_spot_steps, const double& dx, const double& dt) const
{
	Eigen::MatrixXd vect = Eigen::MatrixXd::Constant(nb_spot_steps + 1, 1, dt * boundaries_x0_->pde_->coeff_d()); // just a column vector of dim T+1
	return vect;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_k(const int& nb_spot_steps, const double& dx, const double& dt) const
{
	Eigen::MatrixXd k = Eigen::MatrixXd::Zero(nb_spot_steps+1, nb_spot_steps+1); // null square matrix of dimension T+1
	//Filling the first row (boundary condition in x0):
	k.block<1, 3>(0, 0) = boundaries_x0_->coeff_fn1(dx, dt);
	//filling the last row (boundary condition in xN):
	k.block<1, 3>(nb_spot_steps, nb_spot_steps - 2) = boundaries_xN_->coeff_fn1(dx, dt);

	//loop to fill the intermediary rows:
	for (int i = 1; i < nb_spot_steps; i++)
	{
		k.block<1, 3>(i, i - 1) = PDEMatrixBuilder::comp_row_k(dx, dt);
	}
	return k;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_ktilde(const int& nb_spot_steps, const double& dx, const double& dt) const
{
	Eigen::MatrixXd ktilde = Eigen::MatrixXd::Zero(nb_spot_steps +1, nb_spot_steps +1); // null square matrix of dimension T+1
	//Filling the first row (boundary condition in x0):
	ktilde.block<1, 3>(0, 0) = boundaries_x0_->coeff_fn(dx, dt);
	//filling the last row (boundary condition in xN):
	ktilde.block<1, 3>(nb_spot_steps, nb_spot_steps - 2) = boundaries_xN_->coeff_fn(dx, dt);

	//loop to fill the intermediary rows:
	for (int i = 1; i < nb_spot_steps; i++)
	{
		ktilde.block<1, 3>(i, i - 1) = PDEMatrixBuilder::comp_row_ktilde(dx, dt);
	}
	return ktilde;
}