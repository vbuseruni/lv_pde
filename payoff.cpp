#include "payoff.hpp"

#include <iostream>
#include <algorithm>

// BASE CLASSE:

Payoff::Payoff() {}

const double& Payoff::get_K() const 
{
	return K_;
}

Payoff::Payoff(const double& K) : K_(K)
{

}

// CALL PAYOFF
CallPayoff::CallPayoff(const double& K): Payoff(K)
{
	
}

double CallPayoff::operator()(const double& s) const 
{
	return std::max(s - get_K(), 0.0);
}

Payoff* CallPayoff::clone() const
{
	return new CallPayoff(*this);
}


// Put PAYOFF
PutPayoff::PutPayoff(const double& K) : Payoff(K)
{

}

double PutPayoff::operator()(const double& s) const
{
	return std::max(s - get_K(), 0.0);
}

Payoff* PutPayoff::clone() const
{
	return new PutPayoff(*this);
}
