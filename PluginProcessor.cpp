/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HoundAudioInstinctAudioProcessor::HoundAudioInstinctAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())


#endif
{
    
    mFormatManager.registerBasicFormats();
    
    for (int i=0; i<mNumVoices; i++)
    { mSampler.addVoice(new juce::SamplerVoice());

    }
    
    
    
}

HoundAudioInstinctAudioProcessor::~HoundAudioInstinctAudioProcessor()
{
    
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String HoundAudioInstinctAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HoundAudioInstinctAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HoundAudioInstinctAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HoundAudioInstinctAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HoundAudioInstinctAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HoundAudioInstinctAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HoundAudioInstinctAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HoundAudioInstinctAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HoundAudioInstinctAudioProcessor::getProgramName (int index)
{
    return {};
}

void HoundAudioInstinctAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HoundAudioInstinctAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    
    updateADSR();
   
}
    // Use this method as the place to do any pre-playback
    // initialisation that you need..


void HoundAudioInstinctAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HoundAudioInstinctAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void HoundAudioInstinctAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    //getADSRValue();


    //mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    midiMessages.clear();
    
    auto attackValue = apvts.getRawParameterValue("ATTACK")->load();
    
    auto decayValue = apvts.getRawParameterValue("DECAY")->load();
    
    auto sustainValue = apvts.getRawParameterValue("SUSTAIN")->load();
    
    auto releaseValue = apvts.getRawParameterValue("RELEASE")->load();
    
    mADSRParams.attack = attackValue;
    mADSRParams.decay = decayValue;
    mADSRParams.sustain = sustainValue;
    mADSRParams.release = releaseValue;
    
}
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    

        // ..do something to the data...
    

//==============================================================================
bool HoundAudioInstinctAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HoundAudioInstinctAudioProcessor::createEditor()
{
    
    return new HoundAudioInstinctAudioProcessorEditor (*this);
    
}
//==============================================================================
void HoundAudioInstinctAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}



void HoundAudioInstinctAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void HoundAudioInstinctAudioProcessor::loadFile()
{
    juce::FileChooser chooser{"Please load sample"};
    
    if(chooser.browseForFileToOpen())
    {
        
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    
}
    
    juce::BigInteger range;
    range.setRange(0, 128, true);
    
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10.0) );
}

//void HoundAudioInstinctAudioProcessor::getADSRValue()
//{
   //DBG("Attack: " << attack << "Decay: " << decay << "Sustain: " << sustain << "Release: " << release);
    
//}

juce::AudioProcessorValueTreeState::ParameterLayout HoundAudioInstinctAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 5.0f, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.1f, 1.0f, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.1f, 1.0f, 0.0f));

    return { params.begin(), params.end()};
}

void HoundAudioInstinctAudioProcessor::updateADSR()
{
    for (int i=0; i < mSampler.getNumSounds(); ++i)
    
    {
       if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))
           
       {
           sound->setEnvelopeParameters(mADSRParams);
           
       }
        
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HoundAudioInstinctAudioProcessor();
}
