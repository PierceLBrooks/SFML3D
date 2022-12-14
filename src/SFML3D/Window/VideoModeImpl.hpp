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

#ifndef SFML3D_VIDEOMODEIMPL_HPP
#define SFML3D_VIDEOMODEIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/Window/VideoMode.hpp>


namespace sf3d
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OS-specific implementation of video modes functions
///
////////////////////////////////////////////////////////////
class VideoModeImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the list of all the supported fullscreen video modes
    ///
    /// \return Array filled with the fullscreen video modes
    ///
    ////////////////////////////////////////////////////////////
    static std::vector<VideoMode> getFullscreenModes();

    ////////////////////////////////////////////////////////////
    /// \brief Get the current desktop video mode
    ///
    /// \return Current desktop video mode
    ///
    ////////////////////////////////////////////////////////////
    static VideoMode getDesktopMode();
};

} // namespace priv

} // namespace sf3d


#endif // SFML3D_VIDEOMODEIMPL_HPP
