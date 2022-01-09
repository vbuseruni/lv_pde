#include "FinancialPDE.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <math.h> 

BS_PDE::BS_PDE(Option* _Opt) : Opt(_Opt) {

};

double BS_PDE::coeff_a() const{
	double sigma=Opt->vol
	return -0.5 * pow(sigma, 2);
}
double BS_PDE::coeff_b() const {
	double sigma = Opt->vol;
	double r = Opt->rate;
	return 0.5 * sigma - r;
}
double BS_PDE::coeff_c()const {
	double r = Opt->rate;
	return r;
}
double BS_PDE::coeff_d()const {
	return 0;
}
