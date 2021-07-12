/*
  ==============================================================================

    SynthVoice.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/OscData.h"
#include "Data/ADSRData.h"
#include "Data/FilterData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
	bool canPlaySound(juce::SynthesiserSound*) override;
	void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
	void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
	void updateFilter(const int filterType, const float cutoff, const float resonance);

	OscData& getOscillator() { return osc; }
	ADSRData& getADSR() { return adsr; }
	FilterData& getFilter() { return filter; }
	ADSRData& getFilterADSR() { return filterADSR; }

private:
	juce::AudioBuffer<float> synthBuffer;
	OscData osc;
	ADSRData adsr;
	FilterData filter;
	ADSRData filterADSR;

	bool isPrepared{ false };

};