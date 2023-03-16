//
//  StrumMachineApp.swift
//  StrumMachine
//
//  Created by Austin Kang on 3/15/23.
//

import CoreMIDI
import SwiftUI

@main
class StrumMachineApp: App {
    @ObservedObject private var hostModel = AudioUnitHostModel()

    required init() {}

    var body: some Scene {
        WindowGroup {
            ContentView(hostModel: hostModel)
        }
    }
}
