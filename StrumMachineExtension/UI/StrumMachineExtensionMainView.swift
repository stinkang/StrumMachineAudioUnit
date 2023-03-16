//
//  StrumMachineExtensionMainView.swift
//  StrumMachineExtension
//
//  Created by Austin Kang on 3/15/23.
//

import SwiftUI

struct StrumMachineExtensionMainView: View {
    var parameterTree: ObservableAUParameterGroup
    
    var body: some View {
        ParameterSlider(param: parameterTree.global.gain)
    }
}
