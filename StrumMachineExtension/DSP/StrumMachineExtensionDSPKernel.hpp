//
//  StrumMachineExtensionDSPKernel.hpp
//  StrumMachineExtension
//
//  Created by Austin Kang on 3/15/23.
//

#pragma once

#import <AudioToolbox/AudioToolbox.h>
#import <algorithm>
#import <vector>
#import <span>

#import "StrumMachineExtension-Swift.h"
#import "StrumMachineExtensionParameterAddresses.h"

/*
 StrumMachineExtensionDSPKernel
 As a non-ObjC class, this is safe to use from render thread.
 */
class StrumMachineExtensionDSPKernel {
public:
    void initialize(int inputChannelCount, int outputChannelCount, double inSampleRate) {
        mSampleRate = inSampleRate;
    }
    
    void deInitialize() {
    }
    
    // MARK: - Bypass
    bool isBypassed() {
        return mBypassed;
    }
    
    void setBypass(bool shouldBypass) {
        mBypassed = shouldBypass;
    }
    
    // MARK: - Parameter Getter / Setter
    void setParameter(AUParameterAddress address, AUValue value) {
        switch (address) {
            case StrumMachineExtensionParameterAddress::gain:
                mGain = value;
                break;
                // Add a case for each parameter in StrumMachineExtensionParameterAddresses.h
            case StrumMachineExtensionParameterAddress::strum:
                mStrum = value;
                break;
        }
    }
    
    AUValue getParameter(AUParameterAddress address) {
        // Return the goal. It is not thread safe to return the ramping value.
        
        switch (address) {
            case StrumMachineExtensionParameterAddress::gain:
                return (AUValue)mGain;
                
            case StrumMachineExtensionParameterAddress::strum:
                return (AUValue)mStrum;
                
            default: return 0.f;
        }
    }
    
    // MARK: - Max Frames
    AUAudioFrameCount maximumFramesToRender() const {
        return mMaxFramesToRender;
    }
    
    void setMaximumFramesToRender(const AUAudioFrameCount &maxFrames) {
        mMaxFramesToRender = maxFrames;
    }
    
    // MARK: - Musical Context
    void setMusicalContextBlock(AUHostMusicalContextBlock contextBlock) {
        mMusicalContextBlock = contextBlock;
    }
    
    /**
     MARK: - Internal Process
     
     This function does the core siginal processing.
     Do your custom DSP here.
     */
    void process(std::span<float const*> inputBuffers, std::span<float *> outputBuffers, AUEventSampleTime bufferStartTime, AUAudioFrameCount frameCount) {
        /*
         Note: For an Audio Unit with 'n' input channels to 'n' output channels, remove the assert below and
         modify the check in [StrumMachineExtensionAudioUnit allocateRenderResourcesAndReturnError]
         */
        assert(inputBuffers.size() == outputBuffers.size());
        
        if (mBypassed) {
            // Pass the samples through
            for (UInt32 channel = 0; channel < inputBuffers.size(); ++channel) {
                std::copy_n(inputBuffers[channel], frameCount, outputBuffers[channel]);
            }
            return;
        }
        
        double samplesPerGap = mSampleRate * mStrum;
        double samplesPerChunk = samplesPerGap + mSampleRate; // One chunk is audio + gap

        
        // Use this to get Musical context info from the Plugin Host,
        // Replace nullptr with &memberVariable according to the AUHostMusicalContextBlock function signature
        /*
         if (mMusicalContextBlock) {
         mMusicalContextBlock(nullptr, 	// currentTempo
         nullptr, 	// timeSignatureNumerator
         nullptr, 	// timeSignatureDenominator
         nullptr, 	// currentBeatPosition
         nullptr, 	// sampleOffsetToNextBeat
         nullptr);	// currentMeasureDownbeatPosition
         }
         */
        
        // Perform per sample dsp on the incoming float in before assigning it to out
        for (UInt32 channel = 0; channel < inputBuffers.size(); ++channel) {
            for (UInt32 frameIndex = 0; frameIndex < frameCount; ++frameIndex) {
                double currentPositionInChunk = fmod(mCurrentPosition, samplesPerChunk);

                if (currentPositionInChunk < mSampleRate) {
                    // Normal audio playback
                    outputBuffers[channel][frameIndex] = inputBuffers[channel][frameIndex] * mGain;
                } else {
                    // Gap in the audio
                    outputBuffers[channel][frameIndex] = 0.0f;
                }
                
                mCurrentPosition++;
            }
        }
    }
    
    void handleOneEvent(AUEventSampleTime now, AURenderEvent const *event) {
        switch (event->head.eventType) {
            case AURenderEventParameter: {
                handleParameterEvent(now, event->parameter);
                break;
            }
                
            default:
                break;
        }
    }
    
    void handleParameterEvent(AUEventSampleTime now, AUParameterEvent const& parameterEvent) {
        // Implement handling incoming Parameter events as needed
    }
    
    // MARK: Member Variables
    AUHostMusicalContextBlock mMusicalContextBlock;
    
    double mSampleRate = 44100.0;
    double mGain = 1.0;
    double mStrum = 0.0;
    bool mBypassed = false;
    double mCurrentPosition = 0.0;

    AUAudioFrameCount mMaxFramesToRender = 1024;
};
