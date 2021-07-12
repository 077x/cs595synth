/*
  ==============================================================================

    FilterComponent.cpp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& apvts, juce::String filterTypeSelectorID, juce::String filterFreqID, juce::String filterResID)
{
	juce::StringArray filterChoices{ "Low-Pass", "Band-Pass", "Hi-Pass" };
	filterTypeSelector.addItemList(filterChoices, 1);
	addAndMakeVisible(filterTypeSelector);

	filterTypeSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, filterTypeSelectorID, filterTypeSelector);

	filterTypeSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
	filterTypeSelectorLabel.setFont(15.0f);
	filterTypeSelectorLabel.setJustificationType(juce::Justification::left);
	addAndMakeVisible(filterTypeSelectorLabel);

	setSliderWithLabel(filterFreqSlider, filterFreqLabel, apvts, filterFreqID, filterFreqAttachment);
	setSliderWithLabel(filterResSlider, filterResLabel, apvts, filterResID, filterResAttachment);

}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
	auto bounds = getLocalBounds().reduced(5);
	auto labelSpace = bounds.removeFromTop(25.0f);

	g.fillAll(juce::Colours::black);
	g.setColour(juce::Colours::white);
	g.setFont(20.0f);
	g.drawText("Filter", labelSpace.withX(5), juce::Justification::left);
	g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void FilterComponent::resized()
{
	const auto startY = 55;;
	const auto sliderWidth = 100;
	const auto sliderHeight = 90;
	const auto labelYOffset = 20;
	const auto labelHeight = 20;

	filterTypeSelector.setBounds(10, startY + 5, 90, 20);
	filterTypeSelectorLabel.setBounds(10, startY - labelYOffset, 90, labelHeight);
	
	filterFreqSlider.setBounds(filterTypeSelector.getRight(), startY, sliderWidth, sliderHeight);
	filterFreqLabel.setBounds(filterFreqSlider.getX(), filterFreqSlider.getY() - labelYOffset, filterFreqSlider.getWidth(), labelHeight);

	filterResSlider.setBounds(filterFreqSlider.getRight(), startY, sliderWidth, sliderHeight);
	filterResLabel.setBounds(filterResSlider.getX(), filterResSlider.getY() - labelYOffset, filterResSlider.getWidth(), labelHeight);

}

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>&  attachment)
{
	slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
	addAndMakeVisible(slider);

	attachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, paramID, slider);

	label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
	label.setFont(15.0f);
	label.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(label);

}