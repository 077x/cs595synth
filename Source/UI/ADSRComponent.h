/*
  ==============================================================================

    ADSRComponent.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(juce::String name, juce::AudioProcessorValueTreeState& apvts, juce::String attackID, juce::String decayID, juce::String sustainID, juce::String releaseID);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
	void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String parameterID, std::unique_ptr<Attachment>& sliderAttachment);;

	juce::String componentName = "" ;

	juce::Slider attackSlider;
	juce::Slider decaySlider;
	juce::Slider sustainSlider;
	juce::Slider releaseSlider;

	juce::Label attackLabel{ "Attack", "A" };
	juce::Label decayLabel{ "Decay", "D" };
	juce::Label sustainLabel{ "Sustain", "S" };
	juce::Label releaseLabel{ "Release", "R" };

	std::unique_ptr<Attachment> attackAttachment;
	std::unique_ptr<Attachment> decayAttachment;
	std::unique_ptr<Attachment> sustainAttachment;
	std::unique_ptr<Attachment> releaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
