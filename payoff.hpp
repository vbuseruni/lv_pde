#pragma once


class Payoff {

public:

	virtual ~Payoff() = default; // virtual destructor
	Payoff();

	virtual double operator()(const double& s) const = 0; //pure virtual function, we overload the operator () for PayOff to be a functor.

private:

	double K_; //option strike price
};


class CallPayoff : public Payoff {

public:

	virtual ~CallPayoff() = default;
	CallPayoff(const double& K);

	virtual double operator()(const double& s) const;

private:

	double K_;

};

class PutPayoff : public Payoff {

public:

	virtual ~PutPayoff() = default;
	PutPayoff(const double& K);

	virtual double operator()(const double& s) const;

private:

	double K_;

};