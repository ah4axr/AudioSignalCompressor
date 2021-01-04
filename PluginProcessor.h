/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StkLite-4.6.1/BiQuad.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "Mu45LFO/Mu45LFO.h"

//==============================================================================
/**
*/
class AndrewHp05compressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AndrewHp05compressorAudioProcessor();
    ~AndrewHp05compressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
//Functions
    //==============================================================================
    void calcAlgorithmParams();
    // convert MIDI Note Number to H
    float calcMiditoHz(float midiNote) { return 440.0 * powf(2, (midiNote - 60.0) / 12.0); }
    // convert Hz to MIDI Note Number
    float calcHzToMidi(float Hz){return 12 * log2f(Hz/440.0)+60.0;}

    //User
    AudioParameterFloat* mThresholdParam;
    AudioParameterFloat* mRatioParam;
    AudioParameterFloat* mAttackParam;
    AudioParameterFloat* mReleaseParam;

    // Algorithm Parameters
    float mFs;       // Sampling rate
    float mEnvOut = 0.0;    //Output/Delay mem for RMS Envelope follower
    float mEnvB0;           //Filter Coeffs for RMS Envelope follower
    float mEnvTau = 0.0020;  //RMS Envelope follower integration time in seconds

    float threshold;
    float ratio;

    float tauAttack;
    float tauRelease;
    float attackCoeff;
    float releaseCoeff;

    float gOut;

    //==============================================================================
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHp05compressorAudioProcessor)
};
