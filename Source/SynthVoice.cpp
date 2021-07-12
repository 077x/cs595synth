/*
  ==============================================================================

    SynthVoice.cpp

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
	return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
	osc.setOscillatorFrequency(midiNoteNumber);
	adsr.noteOn();
	filterADSR.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
	adsr.noteOff();
	filterADSR.noteOff();

	if (!allowTailOff || !adsr.isActive())
		clearCurrentNote();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
	adsr.setSampleRate(sampleRate);

	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = outputChannels;

	osc.prepareToPlay(spec);
	adsr.setSampleRate(sampleRate);
	filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);
	filterADSR.setSampleRate(sampleRate);

	isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
	jassert(isPrepared);

	if (!isVoiceActive())
		return;

	synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
	
	synthBuffer.clear();

	juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
	osc.getNextAudioBlock(audioBlock);
	adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
	filter.process(synthBuffer);
	filterADSR.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);

	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

		if (!adsr.isActive())
			clearCurrentNote();
	}
}

void SynthVoice::updateFilter(const int filterType, const float frequency, const float resonance)
{
	auto modulator = filterADSR.getNextSample();
	filter.updateFilterParameters(filterType, frequency, resonance, modulator);
}