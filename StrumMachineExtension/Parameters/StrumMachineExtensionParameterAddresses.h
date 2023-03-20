//
//  StrumMachineExtensionParameterAddresses.h
//  StrumMachineExtension
//
//  Created by Austin Kang on 3/15/23.
//

#pragma once

#include <AudioToolbox/AUParameters.h>

#ifdef __cplusplus
namespace StrumMachineExtensionParameterAddress {
#endif

typedef NS_ENUM(AUParameterAddress, StrumMachineExtensionParameterAddress) {
    gain = 0,
    strum = 1
};

#ifdef __cplusplus
}
#endif
