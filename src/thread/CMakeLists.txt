# Create the util library
add_library(thread
    CurrentThread.cpp
)

# Include the current directory for header files
target_include_directories(thread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set the output directory for the library
set_target_properties(thread PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"  # For static libraries (.a)
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"  # For shared libraries (.so)
)