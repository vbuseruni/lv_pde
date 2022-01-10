#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "pde_matrix_builder.hpp"


const int& PDEMatrixBuilder::get_nb_time_steps() const
{
	return nb_time_steps_;
}

PDEMatrixBuilder::PDEMatrixBuilder(BoundaryConditions* boundaries_x0, BoundaryConditions* boundaries_xN, const int& nb_time_steps) :
	boundaries_x0_(boundaries_x0), boundaries_xN_(boundaries_xN), nb_time_steps_(nb_time_steps)
{

}

double PDEMatrixBuilder::alpha() const
{
	double alph = boundaries_x0_->pde_->coeff_c() - 2 * (boundaries_x0_->pde_->coeff_a()) / pow(boundaries_x0_->get_dx(), 2);
	return alph;
}

double PDEMatrixBuilder::beta() const
{
	double bet = (boundaries_x0_->pde_->coeff_a()) / pow(boundaries_x0_->get_dx(), 2) + (boundaries_x0_->pde_->coeff_b()) / (2 * (boundaries_x0_->get_dx()));
	return bet;
}

double PDEMatrixBuilder::xi() const
{
	double result = (boundaries_x0_->pde_->coeff_a()) / pow(boundaries_x0_->get_dx(), 2) - (boundaries_x0_->pde_->coeff_b()) / (2 * (boundaries_x0_->get_dx()));
	return result;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_row_ktilde() const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(0) = -boundaries_x0_->get_dt() * (1 - boundaries_x0_->get_theta()) * PDEMatrixBuilder::xi();
	res(1) = 1 - boundaries_x0_->get_dt() * (1 - boundaries_x0_->get_theta()) * PDEMatrixBuilder::alpha();
	res(2) = -boundaries_x0_->get_dt() * (1 - boundaries_x0_->get_theta()) * PDEMatrixBuilder::beta();
	return res;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_row_k() const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(0) = boundaries_x0_->get_dt() * boundaries_x0_->get_theta() * PDEMatrixBuilder::xi();
	res(1) = 1 + boundaries_x0_->get_dt() * boundaries_x0_->get_theta() * PDEMatrixBuilder::alpha();
	res(2) = boundaries_x0_->get_dt() * boundaries_x0_->get_theta() * PDEMatrixBuilder::beta();
	return res;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_system_constant() const
{
	Eigen::MatrixXd vect = Eigen::MatrixXd::Constant(get_nb_time_steps() + 1, 1, boundaries_x0_->get_dt() * boundaries_x0_->pde_->coeff_d()); // just a column vector of dim T+1
	return vect;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_ktilde() const
{
	Eigen::MatrixXd ktilde = Eigen::MatrixXd::Zero(get_nb_time_steps()+1, get_nb_time_steps()+1); // null square matrix of dimension T+1
	//Filling the first row (boundary condition in x0):
	ktilde.block<1, 3>(0, 0) = boundaries_x0_->coeff_fn1();
	//filling the last row (boundary condition in xN):
	ktilde.block<1, 3>(get_nb_time_steps(), get_nb_time_steps() - 2) = boundaries_xN_->coeff_fn1();

	//loop to fill the intermediary rows:
	for (int i = 1; i < get_nb_time_steps(); i++)
	{
		ktilde.block<1, 3>(i, i - 1) = PDEMatrixBuilder::comp_row_ktilde();
	}
	return ktilde;
}

Eigen::MatrixXd PDEMatrixBuilder::comp_k() const
{
	Eigen::MatrixXd k = Eigen::MatrixXd::Zero(get_nb_time_steps()+1, get_nb_time_steps()+1); // null square matrix of dimension T+1
	//Filling the first row (boundary condition in x0):
	k.block<1, 3>(0, 0) = boundaries_x0_->coeff_fn();
	//filling the last row (boundary condition in xN):
	k.block<1, 3>(get_nb_time_steps(), get_nb_time_steps() - 2) = boundaries_xN_->coeff_fn();

	//loop to fill the intermediary rows:
	for (int i = 1; i < get_nb_time_steps(); i++)
	{
		k.block<1, 3>(i, i - 1) = PDEMatrixBuilder::comp_row_k();
	}
	return k;
}