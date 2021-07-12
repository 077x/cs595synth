/*
  ==============================================================================

    FilterData.cpp

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	filter.prepare(spec);
	isPrepared = true;

}

void FilterData::updateFilterParameters(const int filterType, const float frequency, const float resonance, const float modulator)
{
	switch (filterType)
	{
	case 0:
		filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
		break;
	
	case 1:
		filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
		break;

	case 2:
		filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
		break;

	default:
		filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
		break;
	}

	float modulatedFreq = frequency * modulator;
	modulatedFreq = std::fmax(std::fmin(modulatedFreq, 20000.0f), 20.0f);

	filter.setCutoffFrequency(modulatedFreq);
	filter.setResonance(resonance);
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
	jassert(isPrepared);
	juce::dsp::AudioBlock<float> block{ buffer };
	filter.process(juce::dsp::ProcessContextReplacing<float>{ block });
}

void FilterData::resetFilter()
{
	filter.reset();
}