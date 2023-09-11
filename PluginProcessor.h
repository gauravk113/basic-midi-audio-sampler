/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class HoundAudioInstinctAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    HoundAudioInstinctAudioProcessor();
    ~HoundAudioInstinctAudioProcessor() override;
    

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

    void loadFile();
    juce::MidiKeyboardState keyboardState;
    
    
    //void getADSRValue();
    void updateADSR();
    
    //float attack {0.0};
    //float decay {0.0} ;
    //float sustain {0.0};
    //float release {0.0};
    
    juce::AudioProcessorValueTreeState apvts;

    juce::ADSR::Parameters& getADSRParams() {return mADSRParams;}
    
private:
   
    //juce::AudioProcessorValueTreeState treeState;
    
    juce::Synthesiser mSampler;
    const int mNumVoices { 3 };
    
    juce::AudioFormatManager mFormatManager;
    juce::AudioFormatReader* mFormatReader {nullptr};
    
    juce::ADSR::Parameters mADSRParams;
    
    //foleys::MagicProcessorState magicState {*this, treeState};
    
   
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoundAudioInstinctAudioProcessor)
};
