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

#ifndef SFML3D_HIDINPUTMANAGER_HPP
#define SFML3D_HIDINPUTMANAGER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/System/NonCopyable.hpp>
#include <SFML3D/Window/JoystickImpl.hpp>
#include <SFML3D/Window/Keyboard.hpp>
#include <SFML3D/Window/Mouse.hpp>
#include <Carbon/Carbon.h>
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDDevice.h>
#include <vector>

namespace sf
{
namespace priv
{

typedef std::vector<IOHIDElementRef> IOHIDElements;

////////////////////////////////////////////////////////////
/// \brief This class manage as a singleton instance the
/// keyboard and mouse states. It's only purpose is
/// to help sf3d::priv::InputImpl class.
///
////////////////////////////////////////////////////////////
class HIDInputManager : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the unique instance of the class
    ///
    /// \note Private use only
    ///
    /// \return Reference to the HIDInputManager instance
    ///
    ////////////////////////////////////////////////////////////
    static HIDInputManager& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key Key to check
    ///
    /// \return True if the key is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool isKeyPressed(Keyboard::Key key);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a mouse button is pressed
    ///
    /// \param button Button to check
    ///
    /// \return True if the button is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool isMouseButtonPressed(Mouse::Button button);

public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the usb location ID of a given device
    ///
    /// This location ID is unique and can be used a usb port identifier
    ///
    /// \param device HID device to query
    /// \return the device's location ID or 0 if something went wrong
    ///
    ////////////////////////////////////////////////////////////
    static long getLocationID(IOHIDDeviceRef device);

    ////////////////////////////////////////////////////////////
    /// \brief Create a mask (dictionary) for an IOHIDManager
    ///
    /// \param page  HID page
    /// \param usage HID usage page
    /// \return a retained CFDictionaryRef
    ///
    ////////////////////////////////////////////////////////////
    static CFDictionaryRef copyDevicesMask(UInt32 page, UInt32 usage);

    ////////////////////////////////////////////////////////////
    /// Try to convert a character into a SFML3D key code.
    ///
    /// Return sf3d::Keyboard::Unknown if it doesn't match any 'localized' keys.
    ///
    /// By 'localized' I mean keys that depend on the keyboard layout
    /// and might not be the same as the US keycode in some country
    /// (e.g. the keys 'Y' and 'Z' are switched on QWERTZ keyboard and
    /// US keyboard layouts.)
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Key localizedKeys(UniChar ch);

    ////////////////////////////////////////////////////////////
    /// Try to convert a virtual keycode into a SFML3D key code.
    ///
    /// Return sf3d::Keyboard::Unknown if the keycode is unknown.
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Key nonLocalizedKeys(UniChar virtualKeycode);

private :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    HIDInputManager();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~HIDInputManager();

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the keyboard part of this class
    ///
    /// If something went wrong freeUp is called
    ///
    ////////////////////////////////////////////////////////////
    void initializeKeyboard();

    ////////////////////////////////////////////////////////////
    /// \brief Initialize the mouse part of this class
    ///
    /// If something went wrong freeUp is called
    ///
    ////////////////////////////////////////////////////////////
    void initializeMouse();

    ////////////////////////////////////////////////////////////
    /// \brief Load the given keyboard into m_keys
    ///
    /// If the given keyboard has no key this function simply
    /// returns. freeUp is _not_ called because this is not fatal.
    ///
    /// \param keyboard Keyboard to load
    ///
    ////////////////////////////////////////////////////////////
    void loadKeyboard(IOHIDDeviceRef keyboard);

    ////////////////////////////////////////////////////////////
    /// \brief Load the given mouse into m_buttons
    ///
    /// If the given mouse has no button this function simply
    /// returns. freeUp is _not_ called because this is not fatal.
    ///
    /// \param mouse Mouse to load
    ///
    ////////////////////////////////////////////////////////////
    void loadMouse(IOHIDDeviceRef mouse);

    ////////////////////////////////////////////////////////////
    /// \brief Load the given key into m_keys
    ///
    /// freeUp is _not_ called by this function.
    ///
    /// \param key Key to load
    ///
    ////////////////////////////////////////////////////////////
    void loadKey(IOHIDElementRef key);

    ////////////////////////////////////////////////////////////
    /// \brief Load the given button into m_buttons
    ///
    /// freeUp is _not_ called by this function.
    ///
    /// \param button Button to load
    ///
    ////////////////////////////////////////////////////////////
    void loadButton(IOHIDElementRef button);

    ////////////////////////////////////////////////////////////
    /// \brief Release all resources
    ///
    /// Close all connections to any devices, if required
    /// Set m_isValid to false
    ///
    ////////////////////////////////////////////////////////////
    void freeUp();

    ////////////////////////////////////////////////////////////
    /// \brief Filter the devices and return them.
    ///
    /// freeUp is _not_ called by this function.
    ///
    /// \param page  HID page like kHIDPage_GenericDesktop
    /// \param usage HID usage page like kHIDUsage_GD_Keyboard or kHIDUsage_GD_Mouse
    /// \return a retained CFSetRef of IOHIDDeviceRef or NULL
    ///
    ////////////////////////////////////////////////////////////
    CFSetRef copyDevices(UInt32 page, UInt32 usage);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a key / mouse button is pressed
    ///
    /// \param elements HID elements mapping to this key / mouse button
    ///
    /// \return True if the key / button is pressed, false otherwise
    ///
    /// \see isKeyPressed, isMouseButtonPressed
    ///
    ////////////////////////////////////////////////////////////
    bool isPressed(IOHIDElements& elements);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a HID key usage to its corresponding virtual code
    ///
    /// See IOHIDUsageTables.h
    ///
    /// \param usage Any kHIDUsage_Keyboard* usage
    /// \return the virtual code associate with the given HID key usage
    ///         or 0xff if it is associate with no virtual code
    ///
    ////////////////////////////////////////////////////////////
    static UInt8 usageToVirtualCode(UInt32 usage);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    bool              m_isValid;                    ///< If any error occurs this variable is false
    CFDataRef         m_layoutData;                 ///< CFData containing the layout
    UCKeyboardLayout* m_layout;                     ///< Current Keyboard Layout
    IOHIDManagerRef   m_manager;                    ///< HID Manager

    IOHIDElements     m_keys[Keyboard::KeyCount];   ///< All the keys on any connected keyboard
    IOHIDElements     m_buttons[Mouse::ButtonCount];///< All the buttons on any connected mouse

    ////////////////////////////////////////////////////////////
    /// m_keys' index corresponds to sf3d::Keyboard::Key enum.
    /// if no key is assigned with key XYZ then m_keys[XYZ].size() == 0.
    /// if there are several keyboards connected and several HID keys associate
    /// with the same sf3d::Keyboard::Key then m_keys[XYZ] contains all these
    /// HID keys.
    ///
    /// m_buttons works the same way.
    ///
    ////////////////////////////////////////////////////////////
};

} // namespace priv

} // namespace sf

#endif
