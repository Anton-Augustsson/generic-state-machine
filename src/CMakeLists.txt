# Add library target
add_library(gsmLibrary INTERFACE)

# Specify include directories for the library
target_include_directories(gsmLibrary INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})