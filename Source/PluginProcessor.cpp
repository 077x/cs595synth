/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Cs595SynthAudioProcessor::Cs595SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
	synth.addSound(new SynthSound());

	for (int i = 0; i < 5; i++)
	{
		synth.addVoice(new SynthVoice());
	}
}

Cs595SynthAudioProcessor::~Cs595SynthAudioProcessor()
{

}

//==============================================================================
const juce::String Cs595SynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Cs595SynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Cs595SynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Cs595SynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Cs595SynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Cs595SynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Cs595SynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Cs595SynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Cs595SynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void Cs595SynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Cs595SynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);

	for (int i = 0; i < synth.getNumVoices(); i++)
	{
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
		}
	}

	juce::dsp::ProcessSpec spec;
	spec.maximumBlockSize = samplesPerBlock;
	spec.sampleRate = sampleRate;
	spec.numChannels = getTotalNumOutputChannels();

}

void Cs595SynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Cs595SynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Cs595SynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	for (int i = 0; i < synth.getNumVoices(); ++i)
	{
		if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
		{
			auto& oscWaveChoice = *apvts.getRawParameterValue("OSC1");

			auto& fmFrequency = *apvts.getRawParameterValue("OSC1FMFREQUENCY");
			auto& fmDepth = *apvts.getRawParameterValue("OSC1FMDEPTH");
			
			//Amp ADSR
			auto& attack = *apvts.getRawParameterValue("ATTACK");
			auto& decay = *apvts.getRawParameterValue("DECAY");
			auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
			auto& release = *apvts.getRawParameterValue("RELEASE");

			//Filter
			auto& filterType = *apvts.getRawParameterValue("FILTERTYPE");
			auto& filterFrequency = *apvts.getRawParameterValue("FILTERFREQ");
			auto& resonance = *apvts.getRawParameterValue("FILTERRES");

			//Modulator/filter ADSR
			auto& filterAttack = *apvts.getRawParameterValue("FILTERATTACK");
			auto& filterDecay = *apvts.getRawParameterValue("FILTERDECAY");
			auto& filterSustain = *apvts.getRawParameterValue("FILTERSUSTAIN");
			auto& filterRelease = *apvts.getRawParameterValue("FILTERRELEASE");

			voice->getOscillator().setOscillatorWaveType(oscWaveChoice);

			voice->getOscillator().setFMParams(fmFrequency, fmDepth);

			voice->getADSR().updateADSR(attack.load(), decay.load(), sustain.load(), release.load());

			voice->updateFilter(filterType.load(), filterFrequency.load(), resonance.load());

			voice->getFilterADSR().updateADSR(filterAttack, filterDecay, filterSustain, filterRelease);
		}
	}

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Cs595SynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Cs595SynthAudioProcessor::createEditor()
{
    return new Cs595SynthAudioProcessorEditor (*this);
}

//==============================================================================
void Cs595SynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Cs595SynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Cs595SynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout Cs595SynthAudioProcessor::createParams()
{
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

	//ComboBox: switch oscillators
	params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square", "White Noise" }, 0));

	//Frequency Modulation (FM)
	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FMFREQUENCY", "Osc1 FM Frequency", juce::NormalisableRange<float>{ 0.0f, 1000.0f, 0.01f, 0.6f }, 0.0f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1FMDEPTH", "Osc1 FM Depth", juce::NormalisableRange<float>{0.0f, 1000.0f, 0.01f, 0.6f}, 0.0f));

	//ADSR
	params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f }, 0.06f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{0.01f, 1.0f, 0.01f }, 1.0f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f }, 1.0f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>{0.01f, 5.0f, 0.01f }, 0.25f));

	//Filter
	params.push_back(std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray{ "Low-Pass", "Band-Pass", "Hi-Pass" }, 0));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERFREQ", "Filter Frequency", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.01f, 0.6f }, 20000.0f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>{1.0f, 10.0f, 0.01f }, 1.0f));

	//Filter ADSR
	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERATTACK", "Filter Attack", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f }, 0.06f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERDECAY", "Filter Decay", juce::NormalisableRange<float>{0.01f, 1.0f, 0.01f }, 0.1f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERSUSTAIN", "Filter Sustain", juce::NormalisableRange<float>{0.0f, 1.0f, 0.01f }, 1.0f));

	params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTERRELEASE", "Filter Release", juce::NormalisableRange<float>{0.01f, 5.0f, 0.01f }, 0.25f));

	return { params.begin(), params.end() };
}