/*
  ==============================================================================

    OscData.cpp

  ==============================================================================
*/

#include "OscData.h"

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec)
{
	fmOsc.prepare(spec);
	prepare(spec);
}

void OscData::setOscillatorWaveType(const int choice)
{
	switch (choice)
	{
	case 0:
		//Sine Wave
		initialise([](float x) { return std::sin(x); });
		break;
	
	case 1:
		//Saw Wave
		initialise([](float x) { return x / juce::MathConstants<float>::pi; });
		break;
	
	case 2:
		//Square Wave
		initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
		break;

	case 3:
		//White noise
		initialise([](float x) {return juce::Random::getSystemRandom().nextFloat() * x - x / 2; });
		break;

	default:
		jassertfalse;
		break;
	}
}

void OscData::setOscillatorFrequency(const int midiNoteNumber)
{
	setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
	lastMidiNote = midiNoteNumber;
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block)
{
	for (int channel = 0; channel < block.getNumChannels(); ++channel)
	{
		for (int sample = 0; sample < block.getNumSamples(); ++sample)
		{
			fmMod = fmOsc.processSample(block.getSample(channel, sample)) * fmDepth;
		}
	}

	process(juce::dsp::ProcessContextReplacing<float>(block));
}

void OscData::setFMParams(const float frequency, const float depth)
{
	fmOsc.setFrequency(frequency);
	fmDepth = depth;
	auto currentFrequency = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmMod;
	setFrequency(currentFrequency >= 0 ? currentFrequency : currentFrequency* -1.0f);
}