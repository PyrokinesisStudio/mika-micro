#include "Filter.h"

double TwoPole::Process(double dt, double input, double cutoff, double resonance)
{
	// f calculation
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > .99 ? .99 : f < .01 ? .01 : f;

	// feedback calculation
	auto feedback = resonance + resonance / (1.0 - f);
	feedback = fastAtan(feedback * .1) * 10.0;

	// main processing
	a += f * (input - a + feedback * (a - b));
	a = fastAtan(a * .1) * 10.0;
	b += f * (a - b);
	b = fastAtan(b * .1) * 10.0;

	return b;
}

double Svf::Process(double dt, double input, double cutoff, double resonance)
{
	// f calculation
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > 1.0 ? 1.0 : f < .01 ? .01 : f;

	// resonance rolloff
	auto maxResonance = 1.0 - f * f * f * f * f;
	resonance = resonance > maxResonance ? maxResonance : resonance;

	// main processing
	auto high = input - (low + band * (1.0 - resonance));
	band += f * high;
	low += f * band;
	low = fastAtan(low * .1) * 10.0;

	return low;
}

double Filter::Process(double dt, double input, double blend, double cutoff, double resonance)
{
	value = 0.0;
	if (blend < 1.0)
		value += twoPole.Process(dt, input, cutoff, resonance) * sqrt(1.0 - blend);
	if (blend > 0.0)
		value += svf.Process(dt, input, cutoff, resonance) * sqrt(blend);
	return value;
}
