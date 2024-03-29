////////////////////////////////////////////////////////////
//
// SFML3D - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML3D_JOYSTICKIMPLLINUX_HPP
#define SFML3D_JOYSTICKIMPLLINUX_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <linux/joystick.h>
#include <fcntl.h>
#include <string>


namespace sf3d
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Linux implementation of joysticks
///
////////////////////////////////////////////////////////////
class JoystickImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global initialization of the joystick module
    ///
    ////////////////////////////////////////////////////////////
    static void initialize();

    ////////////////////////////////////////////////////////////
    /// \brief Perform the global cleanup of the joystick module
    ///
    ////////////////////////////////////////////////////////////
    static void cleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Check if a joystick is currently connected
    ///
    /// \param index Index of the joystick to check
    ///
    /// \return True if the joystick is connected, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isConnected(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Open the joystick
    ///
    /// \param index Index assigned to the joystick
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool open(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// \brief Close the joystick
    ///
    ////////////////////////////////////////////////////////////
    void close();

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick capabilities
    ///
    /// \return Joystick capabilities
    ///
    ////////////////////////////////////////////////////////////
    JoystickCaps getCapabilities() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the joystick identification
    ///
    /// \return Joystick identification
    ///
    ////////////////////////////////////////////////////////////
    Joystick::Identification getIdentification() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the joystick and get its new state
    ///
    /// \return Joystick state
    ///
    ////////////////////////////////////////////////////////////
    JoystickState update();

private :

    ////////////////////////////////////////////////////////////
    /// Get the joystick name
    ///
    /// \param index Index of the joystick
    ///
    /// \return Joystick name
    ///
    ////////////////////////////////////////////////////////////
    std::string getJoystickName(unsigned int index);

    ////////////////////////////////////////////////////////////
    /// Get a system attribute from udev as an unsigned int
    ///
    /// \param index Index of the joystick
    /// \param attributeName Name of the attribute to retrieve
    ///
    /// \return Attribute value as unsigned int
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getUdevAttributeUint(unsigned int index, std::string attributeName);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int           m_file;                          ///< File descriptor of the joystick
    char          m_mapping[ABS_MAX + 1];          ///< Axes mapping (index to axis id)
    JoystickState m_state;                         ///< Current state of the joystick
    sf3d::Joystick::Identification m_identification; ///< Identification of the joystick
};

} // namespace priv

} // namespace sf3d


#endif // SFML3D_JOYSTICKIMPLLINUX_HPP
