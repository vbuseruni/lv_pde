#pragma once
#include <iostream>
#include <vector>
#include "Financial_PDE.hpp"

class BoundaryConditions
{
public:
	virtual ~BoundaryConditions() = default;
	//BoundaryConditions();


protected:

	Financial_PDE* pde_;

	const double& get_dx() const;
	const double& get_dt() const;
	const double& get_theta() const;
	BoundaryConditions(Financial_PDE* pde, const double& dx, const double& dt, const double& theta);

	/*
	
	*/
	virtual std::vector<double> coeff_fn() const = 0;
	virtual std::vector<double> coeff_fn1() const = 0;

private:

	double dx_;
	double dt_;
	double theta_;

};

class Boundaryx0: public BoundaryConditions 
{

public:

	Boundaryx0(Financial_PDE* pde, const double& dx, const double& dt, const double& theta);

protected:

	virtual std::vector<double> coeff_fn() const override;
	virtual std::vector<double> coeff_fn1() const override;
	

private:

	/*
	double gamma_x0(double& dx) const;
	double vega_x0(double& dx) const;
	double mu_x0(double& dx) const;
	*/
	double gamma_x0() const;
	double vega_x0() const;
	double mu_x0() const;
};

class BoundaryxN : public BoundaryConditions
{

public:

	BoundaryxN(Financial_PDE* pde, const double& dx, const double& dt, const double& theta);

protected:

	virtual std::vector<double> coeff_fn() const override;
	virtual std::vector<double> coeff_fn1() const override;

private:

	/*
	double gamma_x0(double& dx) const;
	double vega_x0(double& dx) const;
	double mu_x0(double& dx) const;
	*/
	double gamma_xN() const;
	double vega_xN() const;
	double mu_xN() const;
};


/*
3 classes filles à faire:
	- Boundaryx0: gauche de la mesh quand log(s)=x_0
	- BoundaryxN: droite de la mesh quand log(s)=x_N
	- BoundaryT: haut de la mesh quand t=T (payoff à maturité selon exo(log(s)))
qui correspondent aux trois boundaries connues.

Il faudrait que ça hérite d'un pointeur de type option encore, pour avoir les données de r, sigma, T, et du payoff.

EN FAIT, juste besoin d'hériter d'un pointeur de type FinancialPDE. Puis, on pourra récupérer l'option, le payoff etc et la valeur
des coefficients au temps t. Mettre un paramètre t pour les fonctions sur les coefficients peut-être je pense dans FinancialPDE, dans un 
souci de généralité.

Les paramètres nécessaires autre que ce qui est déjà contenu dans le pointeur vers un objet de type Financial_PDE sont simplement dx et dt.
Il faut créer des fonctions qui ressortent des vecteurs de dimension trois en vrai avec vect=(A_0, B_0, C_0) par exemple pour x0.
On va avoir un vecteur qui correspond à f^n et un à f^(n+1).
Donc, seulement besoin de deux fonctions en vrai.

ATTENTION, faire plus de fonctions en fait. Il faudrait faire des sous fonctions qui calculent ce que j'ai noté gamma, vega, et mu sur la 
feuille.

ATTENTION, il y a en plus besoin du paramètre Theta qui doit pouvoir être modifié par l'utilisateur je pense.
*/