#include "Financial_PDE.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <math.h> 



Financial_PDE::Financial_PDE(Option* Opt) : Opt_(Opt)
{

}

BS_PDE::BS_PDE(Option* Opt) : Financial_PDE(Opt) 
{
}

double BS_PDE::coeff_a() const{
	double sigma = Opt_ -> get_vol();
	return -0.5 * pow(sigma, 2);
}
double BS_PDE::coeff_b() const {
	double sigma = Opt_->get_vol();
	double r = Opt_->get_rate();
	return 0.5 * sigma - r;
}
double BS_PDE::coeff_c()const {
	double r = Opt_->get_rate();
	return r;
}
double BS_PDE::coeff_d()const {
	return 0;
}
