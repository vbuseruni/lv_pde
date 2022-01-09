#ifndef FINANCIAL_PDE_HPP
#define FINANCIAL_PDE_HPP

#include "Option.hpp"


class Financial_PDE {

public:

	virtual ~Financial_PDE() = default;
	// Or "Option* Opt;" ??

	//=== coefficients
	virtual double coeff_a() const; // ou (double t, double x)
	virtual double coeff_b() const;  
	virtual double coeff_c() const;
	virtual double coeff_d() const; 

protected:

private:



};

class BS_PDE:public Financial_PDE{

public:
	Option* Opt;
	BS_PDE(const Option& _Opt);


	//=== BS coefficients
	
	
	double coeff_a() const; // ou (double t, double x)
	double coeff_b() const;
	double coeff_c() const;  
	double coeff_d() const; 

protected:

private:


};

#endif

