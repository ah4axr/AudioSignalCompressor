/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AndrewHp05compressorAudioProcessor::AndrewHp05compressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mThresholdParam = new juce::AudioParameterFloat("Threshold",
        "Threshold",
        -50.0f,
        0.0f,
        -15.0f
    ));
    addParameter(mRatioParam = new juce::AudioParameterFloat("Ratio",
        "Ratio",
        1.0f,
        20.0f,
        5.0f
    ));
    addParameter(mAttackParam = new juce::AudioParameterFloat("Attack Time",
        "Attack",
        0.1f,
        10.0f,
        0.20f
    ));
    addParameter(mReleaseParam = new juce::AudioParameterFloat("Release Time",
        "Release",
        0.1f,
        30.0f,
        0.20f
    ));
}

AndrewHp05compressorAudioProcessor::~AndrewHp05compressorAudioProcessor()
{
}

//==============================================================================
const juce::String AndrewHp05compressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AndrewHp05compressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHp05compressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHp05compressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AndrewHp05compressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AndrewHp05compressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AndrewHp05compressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AndrewHp05compressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AndrewHp05compressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void AndrewHp05compressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AndrewHp05compressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
    mEnvB0 = 1 - exp(-1 / (mEnvTau * mFs));
}

void AndrewHp05compressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AndrewHp05compressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void AndrewHp05compressorAudioProcessor::calcAlgorithmParams() 
{
    threshold = mThresholdParam->get();
    ratio = mRatioParam->get();
    tauAttack = mAttackParam->get();
    attackCoeff = 1 - exp(-1 / ((.1 * tauAttack) * mFs));
    tauRelease = mReleaseParam->get();
    releaseCoeff = 1 - exp(-1 / ((.1 * tauRelease) * mFs));
}

void AndrewHp05compressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);

    calcAlgorithmParams();

    float tempDiff = 1;
    float cDb;
    //float gOut;
    float gainCoeff;

    float tempL, tempR, tempEnv, tempEnvDb, outL, outR;
    float cDbToLin;

    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
        outL = channelDataLeft[samp];
        outR = channelDataRight[samp];
        //DBG(outL);
        ///////////////////////////////////////////////////////
        //Level detector
        //Square input for RMS
        tempL = channelDataLeft[samp] * channelDataLeft[samp];
        tempR = channelDataRight[samp] * channelDataRight[samp];
        //mix input
        tempEnv = 0.5 * (tempL + tempR);
        //1-pole LPF (Mean)
        mEnvOut += mEnvB0 * (tempEnv - mEnvOut);

        //take root for RMS
        tempEnv = sqrt(mEnvOut);
        //convert to dB
        tempEnvDb = 20 * log10(tempEnv);
        //tempEnvDb = 10 * log(tempEnv);

        //DBG(tempEnvDb);

        ///////////////////////////////////////////////////////
        //Gain computer
        if (tempEnvDb > threshold)
        {
            cDb = (tempEnvDb - threshold) * ((1 / ratio) - 1);
            //DBG(cDb);
            cDbToLin = pow(10.0, cDb / 20);
            //DBG(cDbToLin);

            //outL = channelDataLeft[samp];
            //outR = 0.1 * cDbToLin;

            //DBG(outL);
            //gOut = cDb;
        }
        else
        {
            cDbToLin = 1;
        }

        if (cDbToLin <= gOut)
        {
            gainCoeff = attackCoeff;  
            //DBG("ATTACK");
        }
        else
        {
            gainCoeff = releaseCoeff;
            //DBG("RELEASE");
        }
        //DBG(gainCoeff);
        gOut += gainCoeff * (cDbToLin - gOut);
        //DBG(gOut);            
        outL = channelDataLeft[samp] * gOut;
        outR = channelDataRight[samp] * gOut;

        // Debug Envelope Follower
        channelDataLeft[samp] = outL; // write input to output
        channelDataRight[samp] = outR;       // write env to an audio output for debugging
    }
}

//==============================================================================
bool AndrewHp05compressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AndrewHp05compressorAudioProcessor::createEditor()
{
    return new AndrewHp05compressorAudioProcessorEditor (*this);
}

//==============================================================================
void AndrewHp05compressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AndrewHp05compressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AndrewHp05compressorAudioProcessor();
}
