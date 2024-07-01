#-------------------------------------------------------------------------------
# Ptree Loader
#-------------------------------------------------------------------------------
# Examples for Ptree Loader
#-------------------------------------------------------------------------------

# Add source to this project's executable.
add_executable (PtreeLoader
    "main.cpp"
)

target_include_directories(PtreeLoader PUBLIC
    ".."
    ${Boost_INCLUDE_DIR})
target_link_libraries(PtreeLoader ${Boost_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PtreeLoader PROPERTY CXX_STANDARD 23)
endif()

# Copy "load.xxx.cmd" scripts to /out dir
file(GLOB PTREE_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/*.in")
foreach(SRC ${PTREE_SCRIPTS})
    get_filename_component(DST ${SRC} NAME_WLE)
    configure_file("${SRC}" "${CMAKE_SOURCE_DIR}/out/${DST}")
endforeach()

file(GLOB_RECURSE PTREE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Ptrees/*")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${PTREE_FILES}")

# Copy Configs/* to /out dir
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Ptrees" DESTINATION "${CMAKE_SOURCE_DIR}/out/")
