/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AndrewHp05compressorAudioProcessorEditor::AndrewHp05compressorAudioProcessorEditor (AndrewHp05compressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (420, 250);

    auto& params = processor.getParameters();

    ///////////////////////////////////////////////////////////////////////////////////////
    AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
    thresholdSlider.setBounds(350, 30, 50, 200);
    thresholdSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    thresholdSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    thresholdSlider.setRange(audioParam->range.start, audioParam->range.end);
    thresholdSlider.setValue(*audioParam);
    thresholdSlider.setTextValueSuffix(" dB");
    thresholdSlider.addListener(this);
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(threshLabel);
    threshLabel.setText("Threshold", juce::dontSendNotification);
    threshLabel.setJustificationType(juce::Justification::centred);
    threshLabel.attachToComponent(&thresholdSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(1);
    ratioSlider.setBounds(10, 100, 100, 100);
    ratioSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    ratioSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    ratioSlider.setRange(audioParam->range.start, audioParam->range.end);
    ratioSlider.setValue(*audioParam);
    //ratioSlider.setTextValueSuffix(" dB");
    ratioSlider.addListener(this);
    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(ratioLabel);
    ratioLabel.setText("Ratio", juce::dontSendNotification);
    ratioLabel.setJustificationType(juce::Justification::centred);
    ratioLabel.attachToComponent(&ratioSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(2);
    attackSlider.setBounds(120, 100, 100, 100);
    attackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    attackSlider.setRange(audioParam->range.start, audioParam->range.end);
    attackSlider.setValue(*audioParam);
    attackSlider.setTextValueSuffix(" sec");
    attackSlider.addListener(this);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.setJustificationType(juce::Justification::centred);
    attackLabel.attachToComponent(&attackSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(3);
    releaseSlider.setBounds(230, 100, 100, 100);
    releaseSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    releaseSlider.setRange(audioParam->range.start, audioParam->range.end);
    releaseSlider.setValue(*audioParam);
    releaseSlider.setTextValueSuffix(" sec");
    releaseSlider.addListener(this);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::centred);
    releaseLabel.attachToComponent(&releaseSlider, false);
}

AndrewHp05compressorAudioProcessorEditor::~AndrewHp05compressorAudioProcessorEditor()
{
}

//==============================================================================
void AndrewHp05compressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Compressor!", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AndrewHp05compressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AndrewHp05compressorAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
    auto& proccessorParams = processor.getParameters();
    
    // B) Compare the pointer to the slider that was changed with the address of our slider
    if (slider == &thresholdSlider)
    {
        juce::AudioParameterFloat* threshParam = (juce::AudioParameterFloat*)proccessorParams.getUnchecked(0);
        // D) Use the value from our slider to set the parameter in the Processor
        float sliderValue = thresholdSlider.getValue();
        *threshParam = sliderValue;
    }
    else if (slider == &ratioSlider)
    {
        juce::AudioParameterFloat* ratioParam = (juce::AudioParameterFloat*)proccessorParams.getUnchecked(1);
        // D) Use the value from our slider to set the parameter in the Processor
        float sliderValue = ratioSlider.getValue();
        *ratioParam = sliderValue;
    }
    else if (slider == &attackSlider)
    {
        juce::AudioParameterFloat* attackParam = (juce::AudioParameterFloat*)proccessorParams.getUnchecked(2);
        // D) Use the value from our slider to set the parameter in the Processor
        float sliderValue = attackSlider.getValue();
        *attackParam = sliderValue;
    }
    else if (slider == &releaseSlider)
    {
        juce::AudioParameterFloat* releaseParam = (juce::AudioParameterFloat*)proccessorParams.getUnchecked(3);
        // D) Use the value from our slider to set the parameter in the Processor
        float sliderValue = releaseSlider.getValue();
        *releaseParam = sliderValue;
    }
}