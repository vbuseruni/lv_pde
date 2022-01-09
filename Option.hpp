#ifndef OPTION_HPP
#define OPTION_HPP
#include "payoff.hpp"



class Option {

public:

	virtual ~Option() = default;
	explicit Option(Payoff* payoff, const double& K, const double& vol, const double& T, const double& rate);

	double get_K() const;
	double get_vol() const;
	double get_T() const;
	double get_rate() const;


	Payoff* m_payoff;
private:	
	
	double m_K; 
	double m_vol;
	double m_T;
	double m_rate;

protected:

	
	




};

class VanillaOption:public Option {

public:

	explicit VanillaOption(Payoff* payoff,const double& K, const double& vol, const double& T, const double& rate);

protected:

private:


};

#endif

