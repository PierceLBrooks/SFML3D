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

#ifndef SFML3D_DRAWABLE_HPP
#define SFML3D_DRAWABLE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/Graphics/Export.hpp>
#include <SFML3D/Graphics/RenderStates.hpp>


namespace sf3d
{
class RenderTarget;

////////////////////////////////////////////////////////////
/// \brief Abstract base class for objects that can be drawn
///        to a render target
///
////////////////////////////////////////////////////////////
class SFML3D_GRAPHICS_API Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Drawable() {}

protected :

    friend class RenderTarget;

    ////////////////////////////////////////////////////////////
    /// \brief Draw the object to a render target
    ///
    /// This is a pure virtual function that has to be implemented
    /// by the derived class to define how the drawable should be
    /// drawn.
    ///
    /// \param target Render target to draw to
    /// \param states Current render states
    ///
    ////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};

} // namespace sf3d


#endif // SFML3D_DRAWABLE_HPP


////////////////////////////////////////////////////////////
/// \class sf3d::Drawable
/// \ingroup graphics
///
/// sf3d::Drawable is a very simple base class that allows objects
/// of derived classes to be drawn to a sf3d::RenderTarget.
///
/// All you have to do in your derived class is to override the
/// draw virtual function.
///
/// Note that inheriting from sf3d::Drawable is not mandatory,
/// but it allows this nice syntax "window.draw(object)" rather
/// than "object.draw(window)", which is more consistent with other
/// SFML3D classes.
///
/// Example:
/// \code
/// class MyDrawable : public sf3d::Drawable
/// {
/// public :
///
///    ...
///
/// private :
///
///     virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
///     {
///         // You can draw other high-level objects
///         target.draw(m_sprite, states);
///
///         // ... or use the low-level API
///         states.texture = &m_texture;
///         target.draw(m_vertices, states);
///
///         // ... or draw with OpenGL directly
///         glBegin(GL_QUADS);
///         ...
///         glEnd();
///     }
///
///     sf3d::Sprite m_sprite;
///     sf3d::Texture m_texture;
///     sf3d::VertexContainer m_vertices;
/// };
/// \endcode
///
/// \see sf3d::RenderTarget
///
////////////////////////////////////////////////////////////
