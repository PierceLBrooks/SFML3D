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

#ifndef SFML3D_SOUNDBUFFERRECORDER_HPP
#define SFML3D_SOUNDBUFFERRECORDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML3D/Audio/Export.hpp>
#include <SFML3D/Audio/SoundBuffer.hpp>
#include <SFML3D/Audio/SoundRecorder.hpp>
#include <vector>


namespace sf3d
{
////////////////////////////////////////////////////////////
/// \brief Specialized SoundRecorder which stores the captured
///        audio data into a sound buffer
///
////////////////////////////////////////////////////////////
class SFML3D_AUDIO_API SoundBufferRecorder : public SoundRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the sound buffer containing the captured audio data
    ///
    /// The sound buffer is valid only after the capture has ended.
    /// This function provides a read-only access to the internal
    /// sound buffer, but it can be copied if you need to
    /// make any modification to it.
    ///
    /// \return Read-only access to the sound buffer
    ///
    ////////////////////////////////////////////////////////////
    const SoundBuffer& getBuffer() const;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Start capturing audio data
    ///
    /// \return True to start the capture, or false to abort it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onStart();

    ////////////////////////////////////////////////////////////
    /// \brief Process a new chunk of recorded samples
    ///
    /// \param samples     Pointer to the new chunk of recorded samples
    /// \param sampleCount Number of samples pointed by \a samples
    ///
    /// \return True to continue the capture, or false to stop it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onProcessSamples(const Int16* samples, std::size_t sampleCount);

    ////////////////////////////////////////////////////////////
    /// \brief Stop capturing audio data
    ///
    ////////////////////////////////////////////////////////////
    virtual void onStop();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Int16> m_samples; ///< Temporary sample buffer to hold the recorded data
    SoundBuffer        m_buffer;  ///< Sound buffer that will contain the recorded data
};

} // namespace sf3d

#endif // SFML3D_SOUNDBUFFERRECORDER_HPP


////////////////////////////////////////////////////////////
/// \class sf3d::SoundBufferRecorder
/// \ingroup audio
///
/// sf3d::SoundBufferRecorder allows to access a recorded sound
/// through a sf3d::SoundBuffer, so that it can be played, saved
/// to a file, etc.
///
/// It has the same simple interface as its base class (start(), stop())
/// and adds a function to retrieve the recorded sound buffer
/// (getBuffer()).
///
/// As usual, don't forget to call the isAvailable() function
/// before using this class (see sf3d::SoundRecorder for more details
/// about this).
///
/// Usage example:
/// \code
/// if (sf3d::SoundBufferRecorder::isAvailable())
/// {
///     // Record some audio data
///     sf3d::SoundBufferRecorder recorder;
///     recorder.start();
///     ...
///     recorder.stop();
///
///     // Get the buffer containing the captured audio data
///     const sf3d::SoundBuffer& buffer = recorder.getBuffer();
///
///     // Save it to a file (for example...)
///     buffer.saveToFile("my_record.ogg");
/// }
/// \endcode
///
/// \see sf3d::SoundRecorder
///
////////////////////////////////////////////////////////////
