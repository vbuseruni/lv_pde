#ifndef OPTION_HPP
#define OPTION_HPP




class Option {

public:

	virtual ~Option() = default;
	explicit Option(const double& K, const double& vol, const double& T, const double& rate);
	double m_K; 
	double m_vol;
	double m_T;
	double m_rate;

protected:

private:



};

class VanillaOption:public Option {

public:

	explicit VanillaOption(const double& K, const double& vol, const double& T, const double& rate);

protected:

private:


};

#endif

