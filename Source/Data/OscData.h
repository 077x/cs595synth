/*
  ==============================================================================

    OscData.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
	void prepareToPlay(juce::dsp::ProcessSpec& spec);
	void setOscillatorWaveType(const int choice);
	void setOscillatorFrequency(const int midiNoteNumber);
	void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);
	void processFMOsc(juce::dsp::AudioBlock<float>& block);
	void setFMParams(const float frequency, const float depth);

private:
	juce::dsp::Oscillator<float> fmOsc{ [](float x) { return std::sin(x); } };
	float fmMod{ 0.0f };
	float fmDepth{ 0.0f };
	int lastMidiNote{ 0 };

};