#include "Option.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <math.h> 

Option::Option(const double& K, const double& vol, const double& T, const double& rate) : m_k(K),m_vol(vol),m_T(T),m_rate(rate){}


VanillaOption::VanillaOption(const double& K, const double& vol, const double& T, const double& rate) : Option(const double& K, const double& vol, const double& T, const double& rate){}

