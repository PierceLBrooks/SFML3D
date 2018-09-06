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

#ifndef SFML3D_CONTEXT_HPP
#define SFML3D_CONTEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/Window/Export.hpp>
#include <SFML3D/Window/GlResource.hpp>
#include <SFML3D/Window/ContextSettings.hpp>
#include <SFML3D/System/NonCopyable.hpp>


namespace sf3d
{
namespace priv
{
    class GlContext;
}

////////////////////////////////////////////////////////////
/// \brief Class holding a valid drawing context
///
////////////////////////////////////////////////////////////
class SFML3D_WINDOW_API Context : GlResource, NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// The constructor creates and activates the context
    ///
    ////////////////////////////////////////////////////////////
    Context();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// The desctructor deactivates and destroys the context
    ///
    ////////////////////////////////////////////////////////////
    ~Context();

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate explicitely the context
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool setActive(bool active);

public :

    ////////////////////////////////////////////////////////////
    /// \brief Construct a in-memory context
    ///
    /// This constructor is for internal use, you don't need
    /// to bother with it.
    ///
    /// \param settings Creation parameters
    /// \param width    Back buffer width
    /// \param height   Back buffer height
    ///
    ////////////////////////////////////////////////////////////
    Context(const ContextSettings& settings, unsigned int width, unsigned int height);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::GlContext* m_context; ///< Internal OpenGL context
};

} // namespace sf3d


#endif // SFML3D_CONTEXT_HPP

////////////////////////////////////////////////////////////
/// \class sf3d::Context
/// \ingroup window
///
/// If you need to make OpenGL calls without having an
/// active window (like in a thread), you can use an
/// instance of this class to get a valid context.
///
/// Having a valid context is necessary for *every* OpenGL call.
///
/// Note that a context is only active in its current thread,
/// if you create a new thread it will have no valid context
/// by default.
///
/// To use a sf3d::Context instance, just construct it and let it
/// live as long as you need a valid context. No explicit activation
/// is needed, all it has to do is to exist. Its destructor
/// will take care of deactivating and freeing all the attached
/// resources.
///
/// Usage example:
/// \code
/// void threadFunction(void*)
/// {
///    sf3d::Context context;
///    // from now on, you have a valid context
///
///    // you can make OpenGL calls
///    glClear(GL_DEPTH_BUFFER_BIT);
/// }
/// // the context is automatically deactivated and destroyed
/// // by the sf3d::Context destructor
/// \endcode
///
////////////////////////////////////////////////////////////
