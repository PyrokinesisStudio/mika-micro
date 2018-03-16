#include "Oscillator.h"

// http://www.kvraudio.com/forum/viewtopic.php?t=375517
double Oscillator::Blep(double t, double dt)
{
	if (t < dt)
	{
		t /= dt;
		return t + t - t * t - 1.0;
	}
	else if (t > 1.0 - dt)
	{
		t = (t - 1.0) / dt;
		return t * t + t + t + 1.0;
	}
	return 0.0;
}

double Oscillator::GeneratePulse(double width, double phaseIncrement)
{
	double v = phase < width ? 1.0 : -1.0;
	v += Blep(phase, phaseIncrement);
	v -= Blep(fmod(phase + (1.0 - width), 1.0), phaseIncrement);
	return v;
}

double Oscillator::Next(double frequency, EWaveforms waveform)
{
	double phaseIncrement = frequency * dt;
	phase += phaseIncrement;
	while (phase >= 1) phase -= 1;
	
	switch (waveform)
	{
	case kWaveformSine:
		return sin(phase * twoPi);
	case kWaveformTriangle:
		triLast = triCurrent;
		triCurrent = phaseIncrement * GeneratePulse(.5, phaseIncrement) + (1 - phaseIncrement) * triLast;
		return triCurrent * 5;
	case kWaveformSaw:
		return 1 - 2 * phase + Blep(phase, phaseIncrement);
	case kWaveformSquare:
		return GeneratePulse(.5, phaseIncrement);
	case kWaveformPulse:
		return GeneratePulse(.75, phaseIncrement);
	}
}
