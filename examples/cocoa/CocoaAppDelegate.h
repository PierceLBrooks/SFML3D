////////////////////////////////////////////////////////////
//
// SFML3D - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent.gom@gmail.com),
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#import <Cocoa/Cocoa.h>
#import <SFML3D/Graphics.hpp>

/*
 * NB : We need pointers for C++ objects fields in Obj-C interface !
 *      The recomanded way is to use PIMP idiom.
 *
 *      It's elegant. Moreover, we do no constrain
 *      other file including this one to be Obj-C++.
 */

struct SFML3DmainWindow;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1060
@interface CocoaAppDelegate : NSObject <NSApplicationDelegate> {
#else
@interface CocoaAppDelegate : NSObject {
#endif
@private
    NSWindow        *m_window;
    NSView          *m_sfml3dView;
    NSTextField     *m_textField;
    SFML3DmainWindow  *m_mainWindow;
    NSTimer         *m_renderTimer;
    BOOL             m_visible;
    BOOL             m_initialized;
}

@property (retain) IBOutlet NSWindow    *window;
@property (assign) IBOutlet NSView      *sfml3dView;
@property (assign) IBOutlet NSTextField *textField;

-(IBAction)colorChanged:(NSPopUpButton *)sender;
-(IBAction)rotationChanged:(NSSlider *)sender;
-(IBAction)visibleChanged:(NSButton *)sender;
-(IBAction)textChanged:(NSTextField *)sender;
-(IBAction)updateText:(NSButton *)sender;

@end

/*
 * This interface is used to prevent the system alert produced when the SFML3D view
 * has the focus and the user press a key.
 */
@interface SilentWindow : NSWindow

-(void)keyDown:(NSEvent *)theEvent;

@end
