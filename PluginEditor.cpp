/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HoundAudioInstinctAudioProcessorEditor::HoundAudioInstinctAudioProcessorEditor (HoundAudioInstinctAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor(p),
keyboardComponent(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    
    setSize(600, 600);
    
    mLoadButton.onClick = [&](){audioProcessor.loadFile();};
    addAndMakeVisible(mLoadButton);
    mLoadButton.setSize (100, 100);
    //mLoadButton.setBoundsRelative(0.3f, 0.3f, 0.2f, 0.2f);
    
    auto tapImage = juce::ImageCache::getFromMemory(BinaryData::Hound_resized_png, BinaryData::Hound_resized_pngSize);
    
    if(!tapImage.isNull())
    mImageComponent.setImage(tapImage,juce::RectanglePlacement::onlyReduceInSize);
    else
        jassert(!tapImage.isNull());
    
    addAndMakeVisible(mImageComponent);
    mImageComponent.setSize(100, 100);
    
    addAndMakeVisible (keyboardComponent);
            keyboardComponent.setSize (getWidth(), 100);
    
    //Attack Slider
    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 30, 20);
    //mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    //mAttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::g)
    
    addAndMakeVisible(mAttackSlider);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", mAttackSlider);
    
    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", juce::dontSendNotification);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
    
    //Decay Slider
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 30, 20);
    //mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DECAY", mDecaySlider);
    
    addAndMakeVisible(mDecaySlider);
    
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::dontSendNotification);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);

    
    //Sustain Slider
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 30, 20);
    //mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "SUSTAIN", mSustainSlider);
    
    addAndMakeVisible(mSustainSlider);
    
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::dontSendNotification);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);

    //ReleaseSlider
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 30, 20);
    //mReleaseSlider.setRange(0.0f, 1.0f, 0.01f);
    
    attackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", mReleaseSlider);
    
    addAndMakeVisible(mReleaseSlider);
    
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::dontSendNotification);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);

    

}

HoundAudioInstinctAudioProcessorEditor::~HoundAudioInstinctAudioProcessorEditor()
{
}

//==============================================================================
void HoundAudioInstinctAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    auto mImageComponentBG = juce::ImageCache::getFromMemory (BinaryData::Instinct_3_png, BinaryData::Instinct_3_pngSize);
    
       g.drawImageAt(mImageComponentBG, 0, 0);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void HoundAudioInstinctAudioProcessorEditor::resized()
{
    mLoadButton.setBounds(getWidth()/2 - 50, getHeight()/ 2 - 20, 80, 80);
    mImageComponent.setBoundsRelative(0.0f, 0.0f, 0.2f, 0.2f);
    keyboardComponent.setBoundsRelative(0.0f, 0.55f, getWidth(), 40);
    
    
    mAttackSlider.setBoundsRelative(0.0f, 0.2f, 0.1f, 0.1f);
    mDecaySlider.setBoundsRelative(0.07f, 0.2f, 0.1f, 0.1f);
    mSustainSlider.setBoundsRelative(0.25f, 0.2f, 0.1f, 0.1f);
    mReleaseSlider.setBoundsRelative(0.32f, 0.2f, 0.1f, 0.1f);
    
}

void HoundAudioInstinctAudioProcessorEditor::SliderValueChanged(juce::Slider *slider)

{
 
}


//void HoundAudioInstinctAudioProcessorEditor::

//SliderValueChanged(juce::Slider *slider)
//{
 // if (slider == &mAttackSlider)
 //{
     // processor.attack
 // }
   // else if (slider == &mDecaySlider)
    //{
        
   // }
//else if (slider == &mSustainSlider)
//{
//}
    //else if (slider == &mReleaseSlider)
   // {
    //
   // }
    
