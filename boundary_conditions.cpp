#include "boundary_conditions.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

// Base class:

const double& BoundaryConditions::get_dx() const
{
	return dx_;
}

const double& BoundaryConditions::get_dt() const
{
	return dt_;
}

const double& BoundaryConditions::get_theta() const
{
	return theta_;
}

BoundaryConditions::BoundaryConditions(Financial_PDE* pde, const double& dx, 
										const double& dt, const double& theta) : pde_(pde), dx_(dx), dt_(dt), theta_(theta)
{

}


// Boundaryx0 class:
Boundaryx0::Boundaryx0(Financial_PDE* pde, const double& dx,
	const double& dt, const double& theta) : BoundaryConditions(pde, dx, dt, theta)
{

}

double Boundaryx0::gamma_x0() const
{
	double gamma = pde_->coeff_a() / pow(get_dx(), 2) - pde_->coeff_b() / get_dx() + pde_->coeff_c();
	return gamma;
}

double Boundaryx0::vega_x0() const
{
	double vega = -2*pde_->coeff_a() / pow(get_dx(), 2) + pde_->coeff_b() / get_dx();
	return vega;
}

double Boundaryx0::mu_x0() const
{
	double mu = pde_->coeff_a() / pow(get_dx(), 2);
	return mu;
}

std::vector<double> Boundaryx0::coeff_fn1() const
{
	std::vector<double> res(3, 0.0);
	res[0] = 1 - get_dt() * (1 - get_theta()) * Boundaryx0::gamma_x0();
	res[1] = - get_dt() * (1 - get_theta()) * Boundaryx0::vega_x0();
	res[2] = - get_dt() * (1 - get_theta()) * Boundaryx0::mu_x0();

	return res;
}

std::vector<double> Boundaryx0::coeff_fn() const
{
	std::vector<double> res(3, 0.0);
	res[0] = 1 + get_dt() * get_theta() * Boundaryx0::gamma_x0();
	res[1] = get_dt() * get_theta() * Boundaryx0::vega_x0();
	res[2] = get_dt() * get_theta() * Boundaryx0::mu_x0();

	return res;
}

// BoundaryxN class:
BoundaryxN::BoundaryxN(Financial_PDE* pde, const double& dx,
	const double& dt, const double& theta) : BoundaryConditions(pde, dx, dt, theta)
{

}

double BoundaryxN::gamma_xN() const
{
	double gamma = pde_->coeff_a() / pow(get_dx(), 2) + pde_->coeff_b() / get_dx() + pde_->coeff_c();
	return gamma;
}

double BoundaryxN::vega_xN() const
{
	double vega = -2 * pde_->coeff_a() / pow(get_dx(), 2) - pde_->coeff_b() / get_dx();
	return vega;
}

double BoundaryxN::mu_xN() const
{
	double mu = pde_->coeff_a() / pow(get_dx(), 2);
	return mu;
}

std::vector<double> BoundaryxN::coeff_fn1() const
{
	std::vector<double> res(3, 0.0);
	res[0] = 1 - get_dt() * (1 - get_theta()) * BoundaryxN::gamma_xN();
	res[1] = -get_dt() * (1 - get_theta()) * BoundaryxN::vega_xN();
	res[2] = -get_dt() * (1 - get_theta()) * BoundaryxN::mu_xN();

	return res;
}

std::vector<double> BoundaryxN::coeff_fn() const
{
	std::vector<double> res(3, 0.0);
	res[0] = 1 + get_dt() * get_theta() * BoundaryxN::gamma_xN();
	res[1] = get_dt() * get_theta() * BoundaryxN::vega_xN();
	res[2] = get_dt() * get_theta() * BoundaryxN::mu_xN();

	return res;
}


