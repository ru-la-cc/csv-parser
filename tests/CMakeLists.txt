include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(csvparser_test 
    csv_parser_test.cpp
)

target_link_libraries(csvparser_test
    PRIVATE
    csvparser
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(csvparser_test) 