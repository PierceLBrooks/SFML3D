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
#import <SFML3D/Window/OSX/SFViewController.h>
#import <SFML3D/Window/OSX/SFOpenGLView.h>
#import <SFML3D/Window/OSX/SFApplication.h>
#include <SFML3D/System/Err.hpp>
#include <SFML3D/Window/OSX/WindowImplCocoa.hpp>

@implementation SFViewController


////////////////////////////////////////////////////////
-(id)initWithView:(NSView *)view
{
    if ((self = [super init])) {
        m_requester = 0;

        // Retain the view for our own use.
        m_view = [view retain];

        if (m_view == nil) {

            sf3d::err()
            << "No view was given to initWithWindow:."
            << std::endl;

            return self;
        }

        // Create the view.
        NSRect frame = [m_view frame];
        frame.origin.x = 0;
        frame.origin.y = 0;
        m_oglView = [[SFOpenGLView alloc] initWithFrame:frame];

        if (m_oglView == nil) {

            sf3d::err()
            << "Could not create an instance of NSOpenGLView "
            << "in (SFViewController -initWithView:)."
            << std::endl;

            return self;
        }

        // Set the (OGL) view to the view as its "content" view.
        [m_view addSubview:m_oglView];

        [m_oglView setAutoresizingMask:[m_view autoresizingMask]];
    }

    return self;
}


////////////////////////////////////////////////////////
-(void)dealloc
{
    [self closeWindow];

    [m_view release];
    [m_oglView release];

    [super dealloc];
}


////////////////////////////////////////////////////////
-(void)setRequesterTo:(sf3d::priv::WindowImplCocoa *)requester
{
    // Forward to the view.
    [m_oglView setRequesterTo:requester];
    m_requester = requester;
}


////////////////////////////////////////////////////////
-(sf3d::WindowHandle)getSystemHandle
{
    return m_view;
}


////////////////////////////////////////////////////////
-(void)hideMouseCursor
{
    [NSCursor hide];
}


////////////////////////////////////////////////////////
-(void)showMouseCursor
{
    [NSCursor unhide];
}


////////////////////////////////////////////////////////////
-(NSPoint)position
{
    // Origin is bottom-left corner of the window
    return [m_view convertPoint:NSMakePoint(0, 0) toView:nil]; // nil means window
}


////////////////////////////////////////////////////////.
-(void)setWindowPositionToX:(int)x Y:(int)y
{
    sf3d::err() << "Cannot move SFML3D area when SFML3D is integrated in a NSView. Use the view hanlder directly instead." << std::endl;
}


////////////////////////////////////////////////////////////
-(NSSize)size
{
    return [m_oglView frame].size;
}


////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height
{
    NSRect frame = NSMakeRect([m_view frame].origin.x,
                              [m_view frame].origin.y,
                              width,
                              height);

    [m_view setFrame:frame];
    [m_oglView setFrame:frame];
}


////////////////////////////////////////////////////////
-(void)changeTitle:(NSString *)title
{
    sf3d::err() << "Cannot change the title of the SFML3D area when SFML3D is integrated in a NSView." << std::endl;
}


////////////////////////////////////////////////////////
-(void)hideWindow
{
    [m_view setHidden:YES];
}


////////////////////////////////////////////////////////
-(void)showWindow
{
    [m_view setHidden:NO];
}


////////////////////////////////////////////////////////
-(void)closeWindow
{
    sf3d::err() << "Cannot close SFML3D area when SFML3D is integrated in a NSView." << std::endl;
}


////////////////////////////////////////////////////////
-(void)enableKeyRepeat
{
    [m_oglView enableKeyRepeat];
}


////////////////////////////////////////////////////////
-(void)disableKeyRepeat
{
    [m_oglView disableKeyRepeat];
}


////////////////////////////////////////////////////////
-(void)setIconTo:(unsigned int)width
              by:(unsigned int)height
            with:(sf3d::Uint8 const *)pixels
{
    sf3d::err() << "Cannot set an icon when SFML3D is integrated in a NSView." << std::endl;
}


////////////////////////////////////////////////////////
-(void)processEvent
{
    // If we are not on the main thread we stop here and advice the user.
    if ([NSThread currentThread] != [NSThread mainThread]) {
        /*
         * See http://lists.apple.com/archives/cocoa-dev/2011/Feb/msg00460.html
         * for more information.
         */
        sf3d::err()
        << "Cannot fetch event from a worker thread. (OS X restriction)"
        << std::endl;

        return;
    }

    // If we don't have a requester we don't fetch event.
    if (m_requester != 0) {
        [SFApplication processEvent];
    }
}


////////////////////////////////////////////////////////
-(void)applyContext:(NSOpenGLContext *)context
{
    [m_oglView setOpenGLContext:context];
    [context setView:m_oglView];
}


@end
