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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#import <AppKit/AppKit.h>

////////////////////////////////////////////////////////////
/// \brief Here we redefine some methods to allow grabing fullscreen events.
///
////////////////////////////////////////////////////////////
@interface SFWindow : NSWindow

////////////////////////////////////////////////////////////
/// These two methods must return YES to grab fullscreen events.
/// See http://stackoverflow.com/questions/999464/fullscreen-key-down-actions
/// for more informations
///
////////////////////////////////////////////////////////////
-(BOOL)acceptsFirstResponder;
-(BOOL)canBecomeKeyWindow;

////////////////////////////////////////////////////////////
/// Override default implementation of keyDown: to prevent
/// system alert
///
////////////////////////////////////////////////////////////
-(void)keyDown:(NSEvent *)theEvent;

@end


@interface NSWindow (SFML3D)

////////////////////////////////////////////////////////////
/// Proxy for performClose: for the app delegate
///
/// Always return nil
///
////////////////////////////////////////////////////////////
-(id)sfClose;

@end
