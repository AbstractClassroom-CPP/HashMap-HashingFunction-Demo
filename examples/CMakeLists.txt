add_executable(dictionary_example
    basic.cpp
)

target_link_libraries(dictionary_example PRIVATE Dictionary::Dictionary)
target_compile_features(dictionary_example PRIVATE cxx_std_17)

add_executable(profile_dictionary
    profile_dictionary.cpp
)

target_link_libraries(profile_dictionary PRIVATE Dictionary::Dictionary)
target_compile_features(profile_dictionary PRIVATE cxx_std_17)


