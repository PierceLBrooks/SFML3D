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
#include <SFML3D/Window/OSX/WindowImplCocoa.hpp>
#include <SFML3D/Window/VideoMode.hpp>
#include <SFML3D/Window/WindowHandle.hpp>
#include <SFML3D/Window/WindowStyle.hpp>
#include <SFML3D/System/Err.hpp>
#include <ApplicationServices/ApplicationServices.h>

#import <SFML3D/Window/OSX/SFWindowController.h>
#import <SFML3D/Window/OSX/SFApplication.h>
#import <SFML3D/Window/OSX/SFOpenGLView.h>
#import <SFML3D/Window/OSX/SFWindow.h>
#import <OpenGL/OpenGL.h>

////////////////////////////////////////////////////////////
/// SFWindowController class : Privates Methods Declaration
///
////////////////////////////////////////////////////////////
@interface SFWindowController ()

////////////////////////////////////////////////////////////
/// Retrieves the screen height.
///
////////////////////////////////////////////////////////////
-(float)screenHeight;

////////////////////////////////////////////////////////////
/// Retrives the title bar height.
///
////////////////////////////////////////////////////////////
-(float)titlebarHeight;

@end

@implementation SFWindowController

#pragma mark
#pragma mark SFWindowController's methods

////////////////////////////////////////////////////////
-(id)initWithWindow:(NSWindow *)window
{
    if ((self = [super init])) {
        m_requester = 0;
        m_fullscreenMode = new sf3d::VideoMode();

        // Retain the window for our own use.
        m_window = [window retain];

        if (m_window == nil) {

            sf3d::err()
            << "No window was given to initWithWindow:."
            << std::endl;

            return self;
        }

        // Create the view.
        m_oglView = [[SFOpenGLView alloc] initWithFrame:[[m_window contentView] frame]];

        if (m_oglView == nil) {

            sf3d::err()
            << "Could not create an instance of NSOpenGLView "
            << "in (SFWindowController -initWithWindow:)."
            << std::endl;

            return self;
        }

        // Set the view to the window as its content view.
        [m_window setContentView:m_oglView];
    }

    return self;
}


////////////////////////////////////////////////////////
-(id)initWithMode:(sf3d::VideoMode const &)mode andStyle:(unsigned long)style
{
    // If we are not on the main thread we stop here and advice the user.
    if ([NSThread currentThread] != [NSThread mainThread]) {
        /*
         * See http://lists.apple.com/archives/cocoa-dev/2011/Feb/msg00460.html
         * for more information.
         */
        sf3d::err()
        << "Cannot create a window from a worker thread. (OS X limitation)"
        << std::endl;

        return nil;
    }

    if ((self = [super init])) {
        m_requester = 0;
        m_fullscreenMode = new sf3d::VideoMode();

        // Create our window size.
        NSRect rect = NSZeroRect;
        if (style & sf3d::Style::Fullscreen && mode != sf3d::VideoMode::getDesktopMode()) {
            // We use desktop mode to size the window
            // but we set the back buffer size to 'mode' in applyContext method.

            *m_fullscreenMode = mode;

            sf3d::VideoMode dm = sf3d::VideoMode::getDesktopMode();
            rect = NSMakeRect(0, 0, dm.width, dm.height);

        } else { // no fullscreen requested.
            rect = NSMakeRect(0, 0, mode.width, mode.height);
        }

        // Convert the SFML3D window style to Cocoa window style.
        unsigned int nsStyle = NSBorderlessWindowMask;
        if (!(style & sf3d::Style::Fullscreen)) { // if fullscrean we keep our NSBorderlessWindowMask.

            if (style & sf3d::Style::Titlebar) nsStyle |= NSTitledWindowMask | NSMiniaturizableWindowMask;

            if (style & sf3d::Style::Resize)   nsStyle |= NSResizableWindowMask;

            if (style & sf3d::Style::Close)    nsStyle |= NSClosableWindowMask;

        }

        // Create the window.
        m_window = [[SFWindow alloc] initWithContentRect:rect
                                               styleMask:nsStyle
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO]; // Don't defer it!
        /*
         "YES" produces some "invalid drawable".
         See http://www.cocoabuilder.com/archive/cocoa/152482-nsviews-and-nsopenglcontext-invalid-drawable-error.html

         [...]
         As best as I can figure, this is happening because the NSWindow (and
         hence my view) are not visible onscreen yet, and the system doesn't like that.
         [...]
         */

        if (m_window == nil) {
            sf3d::err()
            << "Could not create an instance of NSWindow "
            << "in (SFWindowController -initWithMode:andStyle:)."
            << std::endl;

            return self;
        }

        // Apply special feature for fullscreen window.
        if (style & sf3d::Style::Fullscreen) {
            // We place the window above everything else.
            [m_window setOpaque:YES];
            [m_window setHidesOnDeactivate:YES];
            [m_window setLevel:NSMainMenuWindowLevel+1];

            // And hide the menu bar
            [NSMenu setMenuBarVisible:NO];

            /* ---------------------------
             * | Note for future version |
             * ---------------------------
             *
             * starting with OS 10.6 NSView provides
             * a new method -enterFullScreenMode:withOptions:
             * which could be a good alternative.
             */
        }

        // Center the window to be cool =)
        [m_window center];

        // Create the view.
        m_oglView = [[SFOpenGLView alloc] initWithFrame:[[m_window contentView] frame]];

        if (m_oglView == nil) {
            sf3d::err()
            << "Could not create an instance of NSOpenGLView "
            << "in (SFWindowController -initWithMode:andStyle:)."
            << std::endl;

            return self;
        }

        // If a fullscreen window was requested...
        if (style & sf3d::Style::Fullscreen) {
            /// ... we tell the OpenGL view
            [m_oglView enterFullscreen];

            // ... and if the resolution is not the default one...
            if (mode != sf3d::VideoMode::getDesktopMode()) {
                // ... we set the "real size" of the view (that is the back buffer size).
                [m_oglView setRealSize:NSMakeSize(m_fullscreenMode->width, m_fullscreenMode->height)];
            }
        }

        // Set the view to the window as its content view.
        [m_window setContentView:m_oglView];

        // Register for event.
        [m_window setDelegate:self];
        [m_window setAcceptsMouseMovedEvents:YES];
        [m_window setIgnoresMouseEvents:NO];

        // And some other things...
        [m_window setAutodisplay:YES];
        [m_window setReleasedWhenClosed:NO];
    } // if super init ok

    return self;
}

////////////////////////////////////////////////////////
-(void)dealloc
{
    [self closeWindow];
    [NSMenu setMenuBarVisible:YES];

    [m_window release];
    [m_oglView release];

    delete m_fullscreenMode;

    [super dealloc];
}


#pragma mark
#pragma mark WindowImplDelegateProtocol's methods


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
    return m_window;
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
    // First, get the top left corner of the view in its own base system
    NSPoint const origin = [m_oglView frame].origin;
    NSSize  const size = [m_oglView frame].size;
    NSPoint const topLeftCornerOfView = NSMakePoint(origin.x, origin.y + size.height);
    NSPoint const positionInView = [m_oglView convertPointToBase:topLeftCornerOfView];

    // Then, convert it to window base system
    NSPoint const positionInWindow = [m_oglView convertPoint:positionInView toView:nil];
    // here nil denotes the window containing the view

    // Next, convert it to the screen base system
    NSPoint const positionInScreen = [[m_oglView window] convertBaseToScreen:positionInWindow];

    // Finally, flip for SFML3D window coordinate system
    // Don't forget to discard the title bar !
    NSPoint const positionInSFML3D = NSMakePoint(positionInScreen.x,
                                               ([self screenHeight] - [self titlebarHeight]) - positionInScreen.y);

    return positionInSFML3D;
}


////////////////////////////////////////////////////////.
-(void)setWindowPositionToX:(int)x Y:(int)y
{
    NSPoint point = NSMakePoint(x, y);

    // Flip for SFML3D window coordinate system.
    point.y = [self screenHeight] - point.y;

    // Place the window.
    [m_window setFrameTopLeftPoint:point];
}


////////////////////////////////////////////////////////
-(NSSize)size
{
    if (*m_fullscreenMode == sf3d::VideoMode()) {
        return [m_oglView frame].size;
    } else {
        return NSMakeSize(m_fullscreenMode->width, m_fullscreenMode->height);
    }
}


////////////////////////////////////////////////////////
-(void)resizeTo:(unsigned int)width by:(unsigned int)height
{
    // Before resizing, remove resizeable mask to be able to resize
    // beyond the desktop boundaries.
    NSUInteger styleMask = [m_window styleMask];

    [m_window setStyleMask:styleMask ^ NSResizableWindowMask];

    // Add titlebar height.
    NSRect frame = NSMakeRect([m_window frame].origin.x,
                              [m_window frame].origin.y,
                              width,
                              height + [self titlebarHeight]);

    [m_window setFrame:frame display:YES];

    // And restore the mask
    [m_window setStyleMask:styleMask];
}


////////////////////////////////////////////////////////
-(void)changeTitle:(NSString *)title
{
    [m_window setTitle:title];
}


////////////////////////////////////////////////////////
-(void)hideWindow
{
    [m_window orderOut:nil];
}


////////////////////////////////////////////////////////
-(void)showWindow
{
    [m_window makeKeyAndOrderFront:nil];
}


////////////////////////////////////////////////////////
-(void)closeWindow
{
    [m_window close];
    [m_window setDelegate:nil];
    [self setRequesterTo:0];
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
    // Create an empty image representation.
    NSBitmapImageRep* bitmap =
    [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:0 // if 0 : only allocate memory
                                            pixelsWide:width
                                            pixelsHigh:height
                                         bitsPerSample:8 // The number of bits used to specify
                                                         // one pixel in a single component of the data.
                                       samplesPerPixel:4 // 3 if no alpha, 4 with it
                                              hasAlpha:YES
                                              isPlanar:NO   // I don't know what it is but it works
                                        colorSpaceName:NSCalibratedRGBColorSpace
                                           bytesPerRow:0    // 0 == determine automatically
                                          bitsPerPixel:0];  // 0 == determine automatically

    // Load data pixels.
#if MAC_OS_X_VERSION_MAX_ALLOWED < 1050 // We may need to define NSUInteger.
#define NSUInteger unsigned int
#endif
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x, pixels+=4) {
            NSUInteger pixel[4] = { pixels[0], pixels[1], pixels[2], pixels[3] };
            [bitmap setPixel:pixel
                         atX:x
                           y:y];
        }
    }

    // Create an image from the representation.
    NSImage* icon = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [icon addRepresentation:bitmap];

    // Set app icon.
    [[SFApplication sharedApplication] setApplicationIconImage:icon];

    // Free up.
    [icon release];
    [bitmap release];
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

    // If fullscreen was requested and the mode used to create the window
    // was not the desktop mode, we change the back buffer size of the
    // context.
    if (*m_fullscreenMode != sf3d::VideoMode()) {
        CGLContextObj cgcontext = (CGLContextObj)[context CGLContextObj];

        GLint dim[2] = {m_fullscreenMode->width, m_fullscreenMode->height};

        CGLSetParameter(cgcontext, kCGLCPSurfaceBackingSize, dim);
        CGLEnable(cgcontext, kCGLCESurfaceBackingSize);
    }
}


#pragma mark
#pragma mark NSWindowDelegate's methods


////////////////////////////////////////////////////////
-(BOOL)windowShouldClose:(id)sender
{
    if (m_requester == 0) return YES;

    m_requester->windowClosed();
    return NO;
}


////////////////////////////////////////////////////////
-(void)windowDidBecomeKey:(NSNotification *)notification
{
    // Send event.
    if (m_requester == 0) return;

    m_requester->windowGainedFocus();

    if (*m_fullscreenMode != sf3d::VideoMode()) {
        [m_oglView enterFullscreen];
    }
}


////////////////////////////////////////////////////////
-(void)windowDidResignKey:(NSNotification *)notification
{
    // Send event.
    if (m_requester == 0) return;

    m_requester->windowLostFocus();

    if (*m_fullscreenMode != sf3d::VideoMode()) {
        [m_oglView exitFullscreen];
    }
}


#pragma mark
#pragma mark Other methods

////////////////////////////////////////////////////////
-(float)screenHeight
{
    NSDictionary *deviceDescription = [[m_window screen] deviceDescription];
    NSNumber *screenNumber = [deviceDescription valueForKey:@"NSScreenNumber"];
    CGDirectDisplayID screenID = (CGDirectDisplayID)[screenNumber intValue];
    CGFloat height = CGDisplayPixelsHigh(screenID);
    return height;
}


////////////////////////////////////////////////////////
-(float)titlebarHeight
{
    return NSHeight([m_window frame]) - NSHeight([[m_window contentView] frame]);
}

@end
