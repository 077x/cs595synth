/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Cs595SynthAudioProcessorEditor::Cs595SynthAudioProcessorEditor(Cs595SynthAudioProcessor& p)
: AudioProcessorEditor(&p), 
audioProcessor(p), 
osc(audioProcessor.apvts, "OSC1", "OSC1FMFREQUENCY", "OSC1FMDEPTH"), 
adsr("Amp Envelope", audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE"), 
filter(audioProcessor.apvts, "FILTERTYPE", "FILTERFREQ", "FILTERRES"), 
filterADSR("Filter Envelope", audioProcessor.apvts, "FILTERATTACK", "FILTERDECAY", "FILTERSUSTAIN", "FILTERRELEASE")
{
	
	setSize(800, 525);

	addAndMakeVisible(osc);
	addAndMakeVisible(adsr);
	addAndMakeVisible(filter);
	addAndMakeVisible(filterADSR);
}

Cs595SynthAudioProcessorEditor::~Cs595SynthAudioProcessorEditor()
{
}

//==============================================================================
void Cs595SynthAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	//DEFAULT GRAPHICS:
	/*g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

	g.setColour (juce::Colours::white);
	g.setFont (15.0f);
	g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);*/

	g.fillAll(juce::Colours::black);
}

void Cs595SynthAudioProcessorEditor::resized()
{
	const auto paddingX = 5;
	const auto paddingY = 35;
	const auto width = 300;
	const auto height = 200;

	osc.setBounds(osc.getRight(), paddingY, width, height);
	adsr.setBounds(osc.getRight(), paddingY, width, height);
	filter.setBounds(paddingX, 240, width, height);
	filterADSR.setBounds(filter.getRight(), 240, width, height);
}