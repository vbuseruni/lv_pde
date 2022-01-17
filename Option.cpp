#include "Option.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <math.h> 

Option::Option(Payoff* payoff, const double& K, const double& vol, const double& T, const double& rate) : m_payoff(payoff), m_K(K), m_vol(vol), m_T(T), m_rate(rate) {}


VanillaOption::VanillaOption(Payoff* payoff, const double& K, const double& vol, const double& T, const double& rate) : Option(payoff, K, vol, T, rate) {}

double Option::get_K() const
{
	return m_K;
}
double Option::get_vol() const
{
	return m_vol;
}
double Option::get_T() const
{
	return m_T;
}
double Option::get_rate() const
{
	return m_rate;
}