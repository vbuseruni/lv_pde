#pragma once

#include <iostream>
#include <Eigen/Dense>
#include "pde_matrix_builder.hpp"
#include "system_solver.hpp"

/*
Construction de la mesh:
Passer en paramètres nb_time_steps et nb_spot_steps. Calculer dx et dt en divisant la maturité par nb_time_steps et ça donne le nombre
de lignes en hauteur (sans compter celle au temps zéro). Pour dx diviser l'interval [log(s0)-5*SD, log(s0)+5*SD] par nb_spot_steps.

Calculer les prix au temps T (à maturité):
Just utiliser la classe payoff en prenant exp(x)

Calcul des prix pendant la durée de vie de l'option:
Juste à faire une boucle qui part des prix finaux et appelle SystemSolver pour calculer à chaque fois les prix au temps d'avant.

Calculer le prix initial:
Si mesh bien spécifiée, pas de pb on le récupère direct.
Sinon besoin de faire une interpolation pour récupérer le prix initial (si le nb_spot_steps est impair on doit faire une interpolation).
*/


class Mesh {

public:

	virtual ~Mesh() = default;
	SystemSolver* syst_solv_;
	Mesh(SystemSolver* syst_solv, const int& nb_spot_steps, const int& nb_time_steps, const double& initial_spot);
	const int& get_nb_time_steps() const;
	const int& get_nb_spot_steps() const;
	const double& get_initial_spot() const;

	double comp_dx() const;
	double comp_dt() const;

	Eigen::MatrixXd comp_mesh() const;

	Eigen::MatrixXd mesh_maturity() const;
	Eigen::MatrixXd log_spot_prices() const;

	double extract_price(const Eigen::MatrixXd& full_mesh, const Eigen::MatrixXd& log_spots) const;

private:

	int nb_time_steps_;
	int nb_spot_steps_;
	double initial_spot_;
	
	
	
};

