cmake_minimum_required(VERSION 3.20)

project(Dictionary VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CTest)

set(_dictionary_default_examples ${PROJECT_IS_TOP_LEVEL})
set(_dictionary_default_tests ${PROJECT_IS_TOP_LEVEL})
if(NOT BUILD_TESTING)
    set(_dictionary_default_tests OFF)
endif()

option(DICTIONARY_BUILD_EXAMPLES "Build examples" ${_dictionary_default_examples})
option(DICTIONARY_BUILD_TESTS "Build unit tests" ${_dictionary_default_tests})

add_library(Dictionary)
add_library(Dictionary::Dictionary ALIAS Dictionary)

target_sources(Dictionary
    PRIVATE
        src/Dictionary.cpp
)

target_include_directories(Dictionary
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(Dictionary PUBLIC cxx_std_17)

target_compile_options(Dictionary PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# ----------------------
# Installation + CMake package export
# ----------------------

install(
    TARGETS Dictionary
    EXPORT DictionaryTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
    EXPORT DictionaryTargets
    FILE DictionaryTargets.cmake
    NAMESPACE Dictionary::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dictionary
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DictionaryConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DictionaryConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dictionary
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/DictionaryConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/DictionaryConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/DictionaryConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Dictionary
)

# Useful for in-tree consumption via FetchContent/add_subdirectory
export(
    EXPORT DictionaryTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/DictionaryTargets.cmake
    NAMESPACE Dictionary::
)

# ----------------------
# Subdirectories
# ----------------------

if(DICTIONARY_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(DICTIONARY_BUILD_TESTS)
    add_subdirectory(tests)
endif()


