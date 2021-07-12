/*
  ==============================================================================

    OscComponent.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String fmFreqID, juce::String fmDepthID);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	juce::ComboBox oscWaveSelector;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;

	juce::Slider fmFrequencySlider;
	juce::Slider fmDepthSlider;

	using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	std::unique_ptr<Attachment> fmFrequencyAttachment;
	std::unique_ptr<Attachment> fmDepthAttachment;
	
	juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };
	juce::Label fmFrequencyLabel{ "FM Frequency", "FM Frequency" };
	juce::Label fmDepthLabel{ "FM Depth", "FM Depth" };

	void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>&  attachment);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
