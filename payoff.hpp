#pragma once


class Payoff {

public:

	virtual ~Payoff() = default; // virtual destructor
	//Payoff();

	virtual double operator()(const double& s) const = 0; //pure virtual function, we overload the operator () for PayOff to be a functor.
	virtual Payoff* clone() const = 0;
	

protected:

	const double& get_K() const;
	Payoff(const double& K);

private:

	double K_; //option strike price
};


class CallPayoff : public Payoff {

public:

	virtual ~CallPayoff() = default;
	CallPayoff(const double& K);

	virtual double operator()(const double& s) const;
	Payoff* clone() const override;

};


class PutPayoff : public Payoff {

public:

	virtual ~PutPayoff() = default;
	PutPayoff(const double& K);

	virtual double operator()(const double& s) const;
	Payoff* clone() const override;

};

