# Add the net library
add_library(net
    Channel.cpp
    InetAddress.cpp
)

# Include directories for the net library
target_include_directories(net PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the util library to the net library
target_link_libraries(net PRIVATE util)

# Set output directory for the net library
set_target_properties(net PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)
