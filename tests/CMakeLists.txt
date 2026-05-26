include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

# For Windows: prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Don't install gtest/gmock when installing this project.
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(dictionary_tests
    DictionaryTests.cpp
)

target_link_libraries(dictionary_tests
    PRIVATE
        Dictionary::Dictionary
        GTest::gtest_main
)

target_compile_features(dictionary_tests PRIVATE cxx_std_17)

include(GoogleTest)
gtest_discover_tests(dictionary_tests)


