/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class Cs595SynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Cs595SynthAudioProcessorEditor (Cs595SynthAudioProcessor&);
    ~Cs595SynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
	Cs595SynthAudioProcessor& audioProcessor;
	OscComponent osc;
	ADSRComponent adsr;
	FilterComponent filter;
	ADSRComponent filterADSR;

	//==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Cs595SynthAudioProcessorEditor)
};
