
# sources
set(SRC
    ${PROJECT_SOURCE_DIR}/src/SFML3D/Main/SFML3D_Main.cpp
)
source_group("" FILES ${SRC})

# define the sfml3d-main target
add_library(sfml3d-main STATIC ${SRC})

# set the debug suffix
set_target_properties(sfml3d-main PROPERTIES DEBUG_POSTFIX -d)

# insert the major version number in the output filename
set_target_properties(sfml3d-main PROPERTIES OUTPUT_NAME "sfml3d-main")

# set the target's folder (for IDEs that support it, e.g. Visual Studio)
set_target_properties(sfml3d-main PROPERTIES FOLDER "SFML3D")

# setup the install rule
install(TARGETS sfml3d-main ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel)
