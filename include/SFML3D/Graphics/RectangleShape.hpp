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

#ifndef SFML3D_RECTANGLESHAPE_HPP
#define SFML3D_RECTANGLESHAPE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/Graphics/Export.hpp>
#include <SFML3D/Graphics/Shape.hpp>


namespace sf3d
{
////////////////////////////////////////////////////////////
/// \brief Specialized shape representing a rectangle
///
////////////////////////////////////////////////////////////
class SFML3D_GRAPHICS_API RectangleShape : public Shape
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param size Size of the rectangle
    ///
    ////////////////////////////////////////////////////////////
    explicit RectangleShape(const Vector2f& size = Vector2f(0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Set the size of the rectangle
    ///
    /// \param size New size of the rectangle
    ///
    /// \see getSize
    ///
    ////////////////////////////////////////////////////////////
    void setSize(const Vector2f& size);

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the rectangle
    ///
    /// \return Size of the rectangle
    ///
    /// \see setSize
    ///
    ////////////////////////////////////////////////////////////
    const Vector2f& getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of points defining the shape
    ///
    /// \return Number of points of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual unsigned int getPointCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a point of the shape
    ///
    /// The result is undefined if \a index is out of the valid range.
    ///
    /// \param index Index of the point to get, in range [0 .. getPointCount() - 1]
    ///
    /// \return Index-th point of the shape
    ///
    ////////////////////////////////////////////////////////////
    virtual Vector3f getPoint(unsigned int index) const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2f m_size; ///< Size of the rectangle
};

} // namespace sf3d


#endif // SFML3D_RECTANGLESHAPE_HPP


////////////////////////////////////////////////////////////
/// \class sf3d::RectangleShape
/// \ingroup graphics
///
/// This class inherits all the functions of sf3d::Transformable
/// (position, rotation, scale, bounds, ...) as well as the
/// functions of sf3d::Shape (outline, color, texture, ...).
///
/// Usage example:
/// \code
/// sf3d::RectangleShape rectangle;
/// rectangle.setSize(sf3d::Vector2f(100, 50));
/// rectangle.setOutlineColor(sf3d::Color::Red);
/// rectangle.setOutlineThickness(5);
/// rectangle.setPosition(10, 20);
/// ...
/// window.draw(rectangle);
/// \endcode
///
/// \see sf3d::Shape, sf3d::CircleShape, sf3d::ConvexShape
///
////////////////////////////////////////////////////////////
