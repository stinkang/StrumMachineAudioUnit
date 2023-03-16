//
//  StrumMachineExtensionAudioUnit.h
//  StrumMachineExtension
//
//  Created by Austin Kang on 3/15/23.
//

#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

@interface StrumMachineExtensionAudioUnit : AUAudioUnit
- (void)setupParameterTree:(AUParameterTree *)parameterTree;
@end
