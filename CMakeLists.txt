cmake_minimum_required(VERSION 3.14)
project(csvparser VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(csvparser INTERFACE)
target_include_directories(csvparser INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

option(BUILD_SAMPLE "Build sample program" ON)
if(BUILD_SAMPLE)
    add_executable(csvparser_sample 
        src/sample.cpp
    )
    target_link_libraries(csvparser_sample
        PRIVATE
        csvparser
    )
endif()

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS csvparser
    EXPORT csvparserTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT csvparserTargets
    FILE csvparserTargets.cmake
    NAMESPACE csvparser::
    DESTINATION lib/cmake/csvparser
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/csvparserConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/csvparserConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/csvparserConfig.cmake"
    INSTALL_DESTINATION lib/cmake/csvparser
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/csvparserConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/csvparserConfigVersion.cmake"
    DESTINATION lib/cmake/csvparser
) 