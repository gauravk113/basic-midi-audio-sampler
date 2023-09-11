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
class HoundAudioInstinctAudioProcessorEditor  : public juce::AudioProcessorEditor



{
public:
    HoundAudioInstinctAudioProcessorEditor (HoundAudioInstinctAudioProcessor&);
    ~HoundAudioInstinctAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void SliderValueChanged (juce::Slider* slider);
    

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    juce::MidiKeyboardComponent keyboardComponent;
    
    juce::TextButton mLoadButton {"Load Sample"};
    
    HoundAudioInstinctAudioProcessor& audioProcessor;
    
    juce::ImageComponent mImageComponent;
    
    juce::ImageComponent mImageComponentBG;
    
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HoundAudioInstinctAudioProcessorEditor)
};
