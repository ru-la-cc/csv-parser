include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Windows固有のGoogleTest設定
if(WIN32)
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
endif()

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