#------------------------------------------------------------------------------
# Ptree Loader
#------------------------------------------------------------------------------
# CMake project for Ptree Loader
#------------------------------------------------------------------------------

if (MSVC)
  message("MSVC=${MSVC}")
  set (BOOST_ROOT "C:/Program Files/boost/boost_1_81_0/")
  find_package(Boost REQUIRED)
else()
  message("UNIX=${UNIX}")
  find_package(Boost 1.70 REQUIRED COMPONENTS property_tree)
endif()

# Example
add_subdirectory("Example")

