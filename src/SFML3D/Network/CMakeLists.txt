
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML3D/Network)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML3D/Network)

# all source files
set(SRC
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Ftp.cpp
    ${INCROOT}/Ftp.hpp
    ${SRCROOT}/Http.cpp
    ${INCROOT}/Http.hpp
    ${SRCROOT}/IpAddress.cpp
    ${INCROOT}/IpAddress.hpp
    ${SRCROOT}/Packet.cpp
    ${INCROOT}/Packet.hpp
    ${SRCROOT}/Socket.cpp
    ${INCROOT}/Socket.hpp
    ${SRCROOT}/SocketImpl.hpp
    ${INCROOT}/SocketHandle.hpp
    ${SRCROOT}/SocketSelector.cpp
    ${INCROOT}/SocketSelector.hpp
    ${SRCROOT}/TcpListener.cpp
    ${INCROOT}/TcpListener.hpp
    ${SRCROOT}/TcpSocket.cpp
    ${INCROOT}/TcpSocket.hpp
    ${SRCROOT}/UdpSocket.cpp
    ${INCROOT}/UdpSocket.hpp
)

# add platform specific sources
if(SFML3D_OS_WINDOWS)
    set(SRC
        ${SRC}
        ${SRCROOT}/Win32/SocketImpl.cpp
        ${SRCROOT}/Win32/SocketImpl.hpp
    )
else()
    set(SRC
        ${SRC}
        ${SRCROOT}/Unix/SocketImpl.cpp
        ${SRCROOT}/Unix/SocketImpl.hpp
    )
endif()

source_group("" FILES ${SRC})

# build the list of external libraries to link
set(NETWORK_EXT_LIBS)
if(SFML3D_OS_WINDOWS)
    set(NETWORK_EXT_LIBS ${NETWORK_EXT_LIBS} ws2_32)
endif()

# define the sfml3d-network target
sfml3d_add_library(sfml3d-network
                 SOURCES ${SRC}
                 DEPENDS sfml3d-system
                 EXTERNAL_LIBS ${NETWORK_EXT_LIBS})
