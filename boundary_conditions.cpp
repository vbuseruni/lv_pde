#include "boundary_conditions.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

// Base class:



const double& BoundaryConditions::get_theta() const
{
	return theta_;
}

BoundaryConditions::BoundaryConditions(Financial_PDE* pde, const double& theta) : pde_(pde), theta_(theta)
{

}


// Boundaryx0 class:
Boundaryx0::Boundaryx0(Financial_PDE* pde, const double& theta) : BoundaryConditions(pde, theta)
{

}

double Boundaryx0::gamma_x0(const double& dx, const double& dt) const
{
	double gamma = pde_->coeff_a() / pow(dx, 2) - pde_->coeff_b() / dx + pde_->coeff_c();
	return gamma;
}

double Boundaryx0::vega_x0(const double& dx, const double& dt) const
{
	double vega = -2*pde_->coeff_a() / pow(dx, 2) + pde_->coeff_b() / dx;
	return vega;
}

double Boundaryx0::mu_x0(const double& dx, const double& dt) const
{
	double mu = pde_->coeff_a() / pow(dx, 2);
	return mu;
}

Eigen::MatrixXd Boundaryx0::coeff_fn1(const double& dx, const double& dt) const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1,3);
	res(0) = 1 - dt * (1 - get_theta()) * Boundaryx0::gamma_x0(dx, dt);
	res(1) = - dt * (1 - get_theta()) * Boundaryx0::vega_x0(dx, dt);
	res(2) = - dt * (1 - get_theta()) * Boundaryx0::mu_x0(dx, dt);

	return res;
}

Eigen::MatrixXd Boundaryx0::coeff_fn(const double& dx, const double& dt) const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(0) = 1 + dt * get_theta() * Boundaryx0::gamma_x0(dx, dt);
	res(1) = dt * get_theta() * Boundaryx0::vega_x0(dx, dt);
	res(2) = dt * get_theta() * Boundaryx0::mu_x0(dx, dt);

	return res;
}

// BoundaryxN class:
BoundaryxN::BoundaryxN(Financial_PDE* pde, const double& theta) : BoundaryConditions(pde, theta)
{

}

double BoundaryxN::gamma_xN(const double& dx, const double& dt) const
{
	double gamma = pde_->coeff_a() / pow(dx, 2) + pde_->coeff_b() / dx + pde_->coeff_c();
	return gamma;
}

double BoundaryxN::vega_xN(const double& dx, const double& dt) const
{
	double vega = -2 * pde_->coeff_a() / pow(dx, 2) - pde_->coeff_b() / dx;
	return vega;
}

double BoundaryxN::mu_xN(const double& dx, const double& dt) const
{
	double mu = pde_->coeff_a() / pow(dx, 2);
	return mu;
}

Eigen::MatrixXd BoundaryxN::coeff_fn1(const double& dx, const double& dt) const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(2) = 1 - dt * (1 - get_theta()) * BoundaryxN::gamma_xN(dx, dt);
	res(1) = -dt * (1 - get_theta()) * BoundaryxN::vega_xN(dx, dt);
	res(0) = -dt * (1 - get_theta()) * BoundaryxN::mu_xN(dx, dt);

	return res;
}

Eigen::MatrixXd BoundaryxN::coeff_fn(const double& dx, const double& dt) const
{
	Eigen::MatrixXd res = Eigen::MatrixXd::Zero(1, 3);
	res(2) = 1 + dt * get_theta() * BoundaryxN::gamma_xN(dx, dt);
	res(1) = dt * get_theta() * BoundaryxN::vega_xN(dx, dt);
	res(0) = dt * get_theta() * BoundaryxN::mu_xN(dx, dt);

	return res;
}


