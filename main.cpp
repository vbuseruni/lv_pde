#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <execution>

#include "payoff.hpp"

#include "Financial_PDE.hpp"
#include "boundary_conditions.hpp"
#include "pde_matrix_builder.hpp"
#include "eigen-3.4.0/Eigen/Dense"
#include "system_solver.hpp"
#include "mesh.hpp"
#include "closed_form.hpp"
#include "volatility_diffusion.hpp"
#include "rate_diffusion.hpp"
#include "closed_form.hpp"


void test_pricer_greeks(double K, double initial_r, double initial_vol, double T, double theta, double nb_time_steps, double nb_spot_steps,
	double initial_spot, double constant_coeffs, double eps, Payoff* payoff, VolatilityDiffusion* vol_diff, RateDiffusion* rate,
	Financial_PDE* bs_pde, BoundaryConditions* bound_x0, BoundaryConditions* bound_xN, PDEMatrixBuilder* mat_build, SystemSolver* syst_solv,
	Mesh* mesh)
{
	Eigen::MatrixXd log_spots = mesh->log_spot_prices();
	Eigen::MatrixXd mesh_prices = mesh->comp_mesh(log_spots);

	// Mesh values
	double price = mesh->extract_price(mesh_prices, log_spots);
	std::cout << "Mesh price: " << price << std::endl;
	double delta = mesh->comp_delta(mesh_prices, log_spots);
	std::cout << "Mesh Delta: " << delta << std::endl;
	double gamma = mesh->comp_gamma(mesh_prices, log_spots);
	std::cout << "Mesh Gamma: " << gamma << std::endl;
	double theta_greek = mesh->comp_theta_greek(mesh_prices, log_spots);
	std::cout << "Mesh Theta: " << theta_greek << std::endl;
	double vega = mesh->comp_vega(price, log_spots);
	std::cout << "Mesh Vega: " << vega << std::endl;

	// Closed form values
	double price_explicit = bs_price(initial_spot * exp(initial_r * T), K, initial_vol, T, true);
	std::cout << "closed form price: " << price_explicit << std::endl;
	double delta_explicit = delta_call(initial_spot, K, initial_r, initial_vol, T);
	std::cout << "closed form delta: " << delta_explicit << std::endl;
	double gamma_explicit = gamma_call(initial_spot, K, initial_r, initial_vol, T);
	std::cout << "closed form gamma: " << gamma_explicit << std::endl;
	double theta_explicit = theta_call(initial_spot, K, initial_r, initial_vol, T);
	std::cout << "closed form theta: " << theta_explicit << std::endl;
	double vega_explicit = vega_call(initial_spot, K, initial_r, initial_vol, T);
	std::cout << "closed form vega: " << vega_explicit << std::endl;

	// comparing the results with the two methods (error expressed in percentage):
	double price_error = abs((price - price_explicit) / price_explicit);
	std::cout << "price percentage error: " << price_error << std::endl;
	double delta_error = abs((delta - delta_explicit) / delta_explicit);
	std::cout << "delta percentage error: " << delta_error << std::endl;
	double gamma_error = abs((gamma - gamma_explicit) / gamma_explicit);
	std::cout << "gamma percentage error: " << gamma_error << std::endl;
	double theta_error = abs((theta_greek - theta_explicit) / theta_explicit);
	std::cout << "theta percentage error: " << theta_error << std::endl;
	double vega_error = abs((vega - vega_explicit) / vega_explicit);
	std::cout << "vega percentage error: " << vega_error << std::endl;
}

int main() {
	
	//Option Parameters:
	double K = 10.0;
	double initial_r = 0.02;
	double initial_vol = 0.15;
	double T = 1.0;

	//Theta scheme parameter:
	double theta = 0.5;

	// Mesh parameters:
	int nb_time_steps = 252;
	int nb_spot_steps = 200;
	double initial_spot = 8.72;
	bool constant_coeffs = true;
	double eps = 0.001;

	Payoff* payoff = new CallPayoff(K);
	VolatilityDiffusion* vol_diff = new ConstantVolatility();
	RateDiffusion* rate_diff = new ConstantRate();
	Financial_PDE* bs_pde = new BS_PDE(vol_diff, rate_diff);
	BoundaryConditions* bound_x0 = new Boundaryx0(bs_pde, theta);
	BoundaryConditions* bound_xN = new BoundaryxN(bs_pde, theta);
	PDEMatrixBuilder* mat_build = new PDEMatrixBuilder(bound_x0, bound_xN);
	SystemSolver* syst_solv = new SystemSolver(mat_build);
	Mesh* mesh = new Mesh(syst_solv, payoff, nb_spot_steps, nb_time_steps, initial_spot, K, initial_vol, T, initial_r, constant_coeffs, eps);

	test_pricer_greeks(K, initial_r, initial_vol, T, theta, nb_time_steps, nb_spot_steps, initial_spot, constant_coeffs, eps, payoff, vol_diff,
		rate_diff, bs_pde, bound_x0, bound_xN, mat_build, syst_solv, mesh);

	// RAJOUTER LES DELETE POINTEURS!
	delete mesh;
	mesh = NULL;
	delete mat_build;
	mat_build = NULL;
	delete bound_xN;
	bound_xN = NULL;
	delete bound_x0;
	bound_x0 = NULL;
	delete bs_pde;
	bs_pde = NULL;
	delete payoff;
	payoff = NULL;

	return 0;
}