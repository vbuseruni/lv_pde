#ifndef FINANCIAL_PDE_HPP
#define FINANCIAL_PDE_HPP

#include "Option.hpp"


class Financial_PDE {

public:

	virtual ~Financial_PDE() = default;
	
	Financial_PDE(Option* Opt);
	// Or "Option* Opt;" ??

	//=== coefficients
	virtual double coeff_a() const; // ou (double t, double x)
	virtual double coeff_b() const;  
	virtual double coeff_c() const;
	virtual double coeff_d() const; 
	

protected:
	Option* Opt_;

private:



};

class BS_PDE:public Financial_PDE{

public:
	
	BS_PDE(Option* Opt);


	//=== BS coefficients
	
	
	double coeff_a() const; // ou (double t, double x)
	double coeff_b() const;
	double coeff_c() const;  
	double coeff_d() const; 

protected:

private:


};

#endif

