# Create the util library
add_library(util
    Logger.cpp
    Timestamp.cpp
)

# Include the current directory for header files
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set the output directory for the library
set_target_properties(util PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"  # For static libraries (.a)
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"  # For shared libraries (.so)
)