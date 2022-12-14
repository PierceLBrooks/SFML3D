
cmake_minimum_required(VERSION 2.8)

# define a macro that helps defining an option
macro(sfml3d_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# set a default build type if none was provided
# this has to be done before the project() instruction!
sfml3d_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# project name
project(SFML3D)

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# setup version numbers
set(VERSION_MAJOR 2)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# add the SFML3D header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add an option for choosing the build type (shared or static)
sfml3d_set_option(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build SFML3D as shared libraries, FALSE to build it as static libraries")

# add an option for building the examples
sfml3d_set_option(SFML3D_BUILD_EXAMPLES FALSE BOOL "TRUE to build the SFML3D examples, FALSE to ignore them")

# add an option for building the API documentation
sfml3d_set_option(SFML3D_BUILD_DOC FALSE BOOL "TRUE to generate the API documentation, FALSE to ignore it")

# add an option for forcing usage of legacy OpenGL
sfml3d_set_option(SFML3D_LEGACY_GL FALSE BOOL "TRUE to force SFML3D to use legacy OpenGL, FALSE to let SFML3D automatically use non-legacy OpenGL if supported")

# Mac OS X specific options
if(SFML3D_OS_MACOSX)
    # add an option to build frameworks instead of dylibs (release only)
    sfml3d_set_option(SFML3D_BUILD_FRAMEWORKS FALSE BOOL "TRUE to build SFML3D as frameworks libraries (release only), FALSE to build according to BUILD_SHARED_LIBS")
    
    # add an option to let the user specify a custom directory for frameworks installation (SFML3D, sndfile, ...)
    sfml3d_set_option(CMAKE_INSTALL_FRAMEWORK_PREFIX "/Library/Frameworks" STRING "Frameworks installation directory")

    # add an option to automatically install Xcode 4 templates
    sfml3d_set_option(SFML3D_INSTALL_XCODE4_TEMPLATES FALSE BOOL "TRUE to automatically install the Xcode 4 templates, FALSE to do nothing about it")
endif()

# define SFML3D_STATIC if the build type is not set to 'shared'
if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DSFML3D_STATIC)
endif()

# define SFML3D_LEGACY_GL if requested
if(SFML3D_LEGACY_GL)
    add_definitions(-DSFML3D_LEGACY_GL)
endif()

# remove SL security warnings with Visual C++
if(SFML3D_COMPILER_MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

# define an option for choosing between static and dynamic C runtime (Windows only)
if(SFML3D_OS_WINDOWS)
    sfml3d_set_option(SFML3D_USE_STATIC_STD_LIBS FALSE BOOL "TRUE to statically link to the standard libraries, FALSE to use them as DLLs")

    # the following combination of flags is not valid
    if (BUILD_SHARED_LIBS AND SFML3D_USE_STATIC_STD_LIBS)
        message(FATAL_ERROR "BUILD_SHARED_LIBS and SFML3D_USE_STATIC_STD_LIBS cannot be used together")
    endif()

    # for VC++, we can apply it globally by modifying the compiler flags
    if(SFML3D_COMPILER_MSVC AND SFML3D_USE_STATIC_STD_LIBS)
        foreach(flag
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if(${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
    endif()
endif()

# disable the rpath stuff
set(CMAKE_SKIP_BUILD_RPATH TRUE)

# setup Mac OS X stuff
if(SFML3D_OS_MACOSX)
    # SFML3D_BUILD_FRAMEWORKS needs two things :
    # first, it's available only for release
    #    (because cmake currently doesn't allow specifying a custom framework name so XXX-d is not possible)
    # secondly, it works only with BUILD_SHARED_LIBS enabled
    if(SFML3D_BUILD_FRAMEWORKS)
        # requirement #1
        if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
            message(FATAL_ERROR "CMAKE_BUILD_TYPE should be \"Release\" when SFML3D_BUILD_FRAMEWORKS is TRUE")
            return()
        endif()

        # requirement #2
        if(NOT BUILD_SHARED_LIBS)
            message(FATAL_ERROR "BUILD_SHARED_LIBS should be TRUE when SFML3D_BUILD_FRAMEWORKS is TRUE")
            return()
        endif()
    endif()
endif()

if(SFML3D_OS_LINUX OR SFML3D_OS_FREEBSD)
    if(BUILD_SHARED_LIBS)
        sfml3d_set_option(SFML3D_INSTALL_PKGCONFIG_FILES FALSE BOOL "TRUE to automatically install pkg-config files so other projects can find SFML3D")
        if(SFML3D_INSTALL_PKGCONFIG_FILES)
            foreach(sfml3d_module IN ITEMS all system window graphics audio network)
                CONFIGURE_FILE(
                    "tools/pkg-config/sfml3d-${sfml3d_module}.pc.in"
                    "tools/pkg-config/sfml3d-${sfml3d_module}.pc"
                    @ONLY)
	    INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/tools/pkg-config/sfml3d-${sfml3d_module}.pc"
                    DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig")
            endforeach()
        endif()
    else()
        if(SFML3D_INSTALL_PKGCONFIG_FILES)
            message(WARNING "No pkg-config files are provided for the static SFML3D libraries (SFML3D_INSTALL_PKGCONFIG_FILES will be ignored).")
        endif()
    endif()
endif()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add the subdirectories
add_subdirectory(src/SFML3D)
if(SFML3D_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(SFML3D_BUILD_DOC)
    add_subdirectory(doc)
endif()

# setup the install rules
if(NOT SFML3D_BUILD_FRAMEWORKS)
    install(DIRECTORY include
            DESTINATION .
            COMPONENT devel
            PATTERN ".svn" EXCLUDE)
else()
    # find only "root" headers
    file(GLOB SFML3D_HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/SFML3D/*")

    # in fact we have to fool cmake to copy all the headers in subdirectories
    # to do that we have to add the "root" headers to the PUBLIC_HEADER
    # then we can run a post script to copy the remaining headers

    # we need a dummy file in order to compile the framework
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp
                       COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

    set(SFML3D_SOURCES ${SFML3D_HEADERS})
    list(APPEND SFML3D_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

    # create SFML3D.framework
    add_library(SFML3D ${SFML3D_SOURCES})

    # edit target properties
    set_target_properties(SFML3D PROPERTIES 
                          FRAMEWORK TRUE
                          FRAMEWORK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          MACOSX_FRAMEWORK_IDENTIFIER org.sfml3d-dev.SFML3D
                          MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                          PUBLIC_HEADER "${SFML3D_HEADERS}")

    # add the remaining headers
    add_custom_command(TARGET SFML3D 
                       POST_BUILD
                       COMMAND cp -r ${PROJECT_SOURCE_DIR}/include/SFML3D/* SFML3D.framework/Versions/${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}/Headers)

    # adapt install directory to allow distributing dylibs/frameworks in user’s frameworks/application bundle
    # NOTE : it's not required to link agains SFML3D.framework
    set_target_properties(SFML3D PROPERTIES 
                          BUILD_WITH_INSTALL_RPATH 1 
                          INSTALL_NAME_DIR "@executable_path/../Frameworks")

    # install rule
    install(TARGETS SFML3D
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX}
            COMPONENT devel)
endif()

install(FILES cmake/Modules/FindSFML3D.cmake DESTINATION ${INSTALL_MISC_DIR}/cmake/Modules)
install(FILES license.txt DESTINATION ${INSTALL_MISC_DIR})
install(FILES readme.txt DESTINATION ${INSTALL_MISC_DIR})

# install 3rd-party libraries and tools on Windows and OS X
if(SFML3D_OS_WINDOWS)
    if(ARCH_32BITS)
        install(DIRECTORY extlibs/bin/x86/ DESTINATION bin)
        if(SFML3D_COMPILER_MSVC)
            install(DIRECTORY extlibs/libs-msvc/x86/ DESTINATION lib)
        else()
            install(DIRECTORY extlibs/libs-mingw/x86/ DESTINATION lib)
        endif()
    elseif(ARCH_64BITS)
        install(DIRECTORY extlibs/bin/x64/ DESTINATION bin)
        if(SFML3D_COMPILER_MSVC)
            install(DIRECTORY extlibs/libs-msvc/x64/ DESTINATION lib)
        else()
            install(DIRECTORY extlibs/libs-mingw/x64/ DESTINATION lib)
        endif()
    endif()
elseif(SFML3D_OS_MACOSX)
    install(DIRECTORY extlibs/libs-osx/Frameworks/sndfile.framework DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX})
    install(DIRECTORY extlibs/libs-osx/Frameworks/freetype.framework DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX})

    if(SFML3D_INSTALL_XCODE4_TEMPLATES)
        install(DIRECTORY tools/xcode/templates/SFML3D DESTINATION /Library/Developer/Xcode/Templates)
    endif()
endif()
