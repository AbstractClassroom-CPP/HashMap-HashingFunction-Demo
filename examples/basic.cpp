#include "Dictionary/Dictionary.hpp"

#include <iostream>
#include <stdexcept>

int main() {
    Dictionary dict;

    dict.setValue("apple", "red");
    dict.setValue("banana", "yellow");

    std::cout << "apple -> " << dict.getValue("apple") << "\n";

    // Update existing key
    dict.setValue("apple", "green");
    std::cout << "apple -> " << dict.getValue("apple") << "\n";

    // Demonstrate exception for missing keys
    try {
        std::cout << dict.getValue("missing") << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Error: " << e.what() << "\n";
    }

    return 0;
}

