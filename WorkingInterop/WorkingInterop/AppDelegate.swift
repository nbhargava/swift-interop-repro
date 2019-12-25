//
//  AppDelegate.swift
//  WorkingInterop
//
//  Created by Nikhil Bhargava on 12/25/19.
//  Copyright © 2019 Nikhil Bhargava. All rights reserved.
//

import Cocoa
import SwiftUI

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {

    var window: NSWindow!


    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Create the SwiftUI view that provides the window contents.
        let contentView = ContentView()

        // Create the window and set the content view. 
        window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 480, height: 300),
            styleMask: [.titled, .closable, .miniaturizable, .resizable, .fullSizeContentView],
            backing: .buffered, defer: false)
        window.center()
        window.setFrameAutosaveName("Main Window")
        window.contentView = NSHostingView(rootView: contentView)
        window.makeKeyAndOrderFront(nil)
        
        registerForChanges(testCB)
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
}



public typealias Callback = @convention(c) (UnsafePointer<Int8>?) -> Void

public func registerForChanges(_ callback: @escaping Callback) {
    let arg = "arg"
    callback(UnsafePointer<CChar>((arg as NSString).utf8String)!)
}
