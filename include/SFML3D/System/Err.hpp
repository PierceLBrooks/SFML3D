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

#ifndef SFML3D_ERR_HPP
#define SFML3D_ERR_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/System/Export.hpp>
#include <ostream>


namespace sf3d
{
////////////////////////////////////////////////////////////
/// \brief Standard stream used by SFML3D to output warnings and errors
///
////////////////////////////////////////////////////////////
SFML3D_SYSTEM_API std::ostream& err();

} // namespace sf3d


#endif // SFML3D_ERR_HPP


////////////////////////////////////////////////////////////
/// \fn sf3d::err
/// \ingroup system
///
/// By default, sf3d::err() outputs to the same location as std::cerr,
/// (-> the stderr descriptor) which is the console if there's
/// one available.
///
/// It is a standard std::ostream instance, so it supports all the
/// insertion operations defined by the STL
/// (operator <<, manipulators, etc.).
///
/// sf3d::err() can be redirected to write to another output, independently
/// of std::cerr, by using the rdbuf() function provided by the
/// std::ostream class.
///
/// Example:
/// \code
/// // Redirect to a file
/// std::ofstream file("sfml3d-log.txt");
/// std::streambuf* previous = sf3d::err().rdbuf(file.rdbuf());
///
/// // Redirect to nothing
/// sf3d::err().rdbuf(NULL);
///
/// // Restore the original output
/// sf3d::err().rdbuf(previous);
/// \endcode
///
/// \return Reference to std::ostream representing the SFML3D error stream
///
////////////////////////////////////////////////////////////
