/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AndrewHp05compressorAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener
{
public:
    AndrewHp05compressorAudioProcessorEditor (AndrewHp05compressorAudioProcessor&);
    ~AndrewHp05compressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AndrewHp05compressorAudioProcessor& audioProcessor;

    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;

    juce::Label threshLabel;
    juce::Label ratioLabel;
    juce::Label attackLabel;
    juce::Label releaseLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHp05compressorAudioProcessorEditor)
};
