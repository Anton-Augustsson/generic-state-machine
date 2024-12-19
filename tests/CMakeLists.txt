# Add test executable
add_executable(test_gsm test_gsm.cc)

# Link the library to the test executable
target_link_libraries(test_gsm PRIVATE gsmLibrary)

# Add the test to CTest
add_test(NAME TestGSMLibrary COMMAND test_gsm)