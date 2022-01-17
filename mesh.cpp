#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "system_solver.hpp"
#include "mesh.hpp"

Mesh::Mesh(SystemSolver* syst_solv, const int& nb_spot_steps, const int& nb_time_steps, const double& initial_spot) :
	syst_solv_(syst_solv), nb_spot_steps_(nb_spot_steps), nb_time_steps_(nb_time_steps), initial_spot_(initial_spot)
{

}

const int& Mesh::get_nb_time_steps() const
{
	return nb_time_steps_;
}

const int& Mesh::get_nb_spot_steps() const
{
	return nb_spot_steps_;
}

const double& Mesh::get_initial_spot() const
{
	return initial_spot_;
}

double Mesh::comp_dt() const
{
	double maturity = syst_solv_->mat_build_->boundaries_x0_->pde_->Opt_->get_T();
	double dt = maturity / nb_time_steps_;
	return dt;
}

double Mesh::comp_dx() const
{
	double std = syst_solv_->mat_build_->boundaries_x0_->pde_->Opt_->get_vol() * 
		pow(syst_solv_->mat_build_->boundaries_x0_->pde_->Opt_->get_T(), 0.5);
	double width = 10*std;
	double dx = width / nb_spot_steps_;
	return dx;
}

Eigen::MatrixXd Mesh::log_spot_prices() const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, nb_spot_steps_ + 1);
	double std = syst_solv_->mat_build_->boundaries_x0_->pde_->Opt_->get_vol() *
		pow(syst_solv_->mat_build_->boundaries_x0_->pde_->Opt_->get_T(), 0.5);
	double initial_log_spot = log(initial_spot_);
	double x0 = initial_log_spot - 5 * std;
	res(0, 0) = x0;
	double dx = Mesh::comp_dx();
	for (int i = 1; i <= nb_spot_steps_; i++)
	{
		res(0, i) = x0 + i * dx;
	}
	return res;
}


Eigen::MatrixXd Mesh::mesh_maturity() const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, nb_spot_steps_ + 1);
	Eigen::MatrixXd log_spots = Mesh::log_spot_prices();
	for (int i = 0; i <= nb_spot_steps_; i++)
	{
		double log_spot = log_spots(0, i);
		res(0, i) = syst_solv_->mat_build_->boundaries_x0_->pde_->Opt_->m_payoff->operator()(exp(log_spot));
	}
	return res;
}

Eigen::MatrixXd Mesh::comp_mesh() const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(nb_time_steps_+1, nb_spot_steps_ + 1);
	//res.block<1, nb_time_steps_ + 1>(0, 0) = Mesh::mesh_maturity();
	res.row(0) = Mesh::mesh_maturity();
	Eigen::MatrixXd fn1 = Eigen::MatrixXd::Zero(nb_spot_steps_ + 1, 1);
	Eigen::MatrixXd solved = Eigen::MatrixXd::Zero(nb_spot_steps_ + 1, 1);
	double dx = Mesh::comp_dx();
	double dt = Mesh::comp_dt();

	for (int i = 1; i <= nb_time_steps_; i++)
	{
		fn1 = res.row(i - 1).transpose();
		solved = syst_solv_->get_fn(fn1, nb_spot_steps_, dx, dt);
		res.row(i) = solved.transpose();
	}
	return res;
}

double Mesh::extract_price(const Eigen::MatrixXd& full_mesh, const Eigen::MatrixXd& log_spots) const
{
	// Need to fill case where we need interpolation
	double price;
	if (nb_spot_steps_ % 2 == 0)
	{
		price = full_mesh(nb_time_steps_, nb_spot_steps_ / 2);
	}
	else
	{
		//linear interpolation to get the price of the option
		int position_a = (nb_spot_steps_ - 1) / 2;
		int position_b = (nb_spot_steps_ - 1) / 2 + 1;
		double x_a = log_spots(position_a);
		double x_b = log_spots(position_b);
		double price_a = full_mesh(nb_time_steps_, position_a);
		double price_b = full_mesh(nb_time_steps_, position_b);
		price = ((x_b - log(initial_spot_)) * price_a + (log(initial_spot_) - x_a) * price_b) / (x_b - x_a);
	}

	return price;
}