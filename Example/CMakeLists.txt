#-------------------------------------------------------------------------------
# Ptree Loader
#-------------------------------------------------------------------------------
# Examples for Ptree Loader
#-------------------------------------------------------------------------------

if (MSVC)
  message("MSVC=${MSVC}")
  set (BOOST_ROOT "C:/Program Files/boost/boost_1_81_0/")
  find_package(Boost REQUIRED)
else()
  message("UNIX=${UNIX}")
  find_package(Boost 1.81)
endif()

# Add source to this project's executable.
add_executable (PtreeLoader "main.cpp")

target_include_directories(PtreeLoader PUBLIC
    "../PtreeLoader"
    ${Boost_INCLUDE_DIR})
target_link_libraries(PtreeLoader ${Boost_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PtreeLoader PROPERTY CXX_STANDARD 23)
endif()

# Copy scripts to /out dir
file(GLOB PTREE_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/Scripts/*.in")
foreach(SRC ${PTREE_SCRIPTS})
    get_filename_component(DST ${SRC} NAME_WLE)
    configure_file("${SRC}" "${CMAKE_SOURCE_DIR}/out/${DST}"
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                         GROUP_READ GROUP_WRITE GROUP_EXECUTE
                         WORLD_READ WORLD_EXECUTE)
endforeach()

# Copy ptree files to /out dir
file(GLOB_RECURSE PTREE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Ptrees/*")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${PTREE_FILES}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Ptrees" DESTINATION "${CMAKE_SOURCE_DIR}/out/")
