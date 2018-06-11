#pragma once

#include <cmath>
#include "Util.h"

class TwoPole
{
public:
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double a = 0.0;
	double b = 0.0;
};

class Svf
{
public:
	double Process(double dt, double input, double cutoff, double resonance);

private:
	double band = 0.0;
	double low = 0.0;
};

class Filter
{
public:
	bool IsSilent() { return value == 0.0; }
	double Process(double dt, double input, double blend, double cutoff, double resonance);

private:
	TwoPole twoPole;
	Svf svf;
	double value = 0.0;
};

