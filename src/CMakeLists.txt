# Add library target
add_library(gsmLibrary gsm.cc expr.cc)

# Specify include directories for the library
target_include_directories(gsmLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})