
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML3D/Audio)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML3D/Audio)

# all source files
set(SRC
    ${SRCROOT}/ALCheck.cpp
    ${SRCROOT}/ALCheck.hpp
    ${SRCROOT}/AudioDevice.cpp
    ${SRCROOT}/AudioDevice.hpp
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Listener.cpp
    ${INCROOT}/Listener.hpp
    ${SRCROOT}/Music.cpp
    ${INCROOT}/Music.hpp
    ${SRCROOT}/Sound.cpp
    ${INCROOT}/Sound.hpp
    ${SRCROOT}/SoundBuffer.cpp
    ${INCROOT}/SoundBuffer.hpp
    ${SRCROOT}/SoundBufferRecorder.cpp
    ${INCROOT}/SoundBufferRecorder.hpp
    ${SRCROOT}/SoundFile.cpp
    ${SRCROOT}/SoundFile.hpp
    ${SRCROOT}/SoundRecorder.cpp
    ${INCROOT}/SoundRecorder.hpp
    ${SRCROOT}/SoundSource.cpp
    ${INCROOT}/SoundSource.hpp
    ${SRCROOT}/SoundStream.cpp
    ${INCROOT}/SoundStream.hpp
)
source_group("" FILES ${SRC})

# let CMake know about our additional audio libraries paths (on Windows and OSX)
if(SFML3D_OS_WINDOWS)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/AL")
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/libsndfile/windows")
elseif(SFML3D_OS_MACOSX)
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/headers/libsndfile/osx")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/libs-osx/Frameworks")
endif()

# find external libraries
find_package(OpenAL REQUIRED)
find_package(Sndfile REQUIRED)

# add include paths of external libraries
include_directories(${OPENAL_INCLUDE_DIR} ${SNDFILE_INCLUDE_DIR})

# define the sfml3d-audio target
sfml3d_add_library(sfml3d-audio
                 SOURCES ${SRC}
                 DEPENDS sfml3d-system
                 EXTERNAL_LIBS ${OPENAL_LIBRARY} ${SNDFILE_LIBRARY})
