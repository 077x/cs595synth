/*
  ==============================================================================

    FilterData.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData
{
public:
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void updateFilterParameters(const int filterType, const float frequency, const float resonance, const float modulator);
	void process(juce::AudioBuffer<float>& buffer);
	void resetFilter();

private:
	juce::dsp::StateVariableTPTFilter<float> filter;
	bool isPrepared{ false };

};