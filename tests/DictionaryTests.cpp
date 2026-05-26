#include "Dictionary/Dictionary.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

TEST(Dictionary, SetAndGet) {
    Dictionary dict;
    dict.setValue("cat", "meow");
    EXPECT_EQ(dict.getValue("cat"), "meow");
}

TEST(Dictionary, UpdateExistingKey) {
    Dictionary dict;
    dict.setValue("a", "1");
    dict.setValue("a", "2");
    EXPECT_EQ(dict.getValue("a"), "2");
}

TEST(Dictionary, MissingKeyThrows) {
    Dictionary dict;
    EXPECT_THROW(dict.getValue("does_not_exist"), std::out_of_range);
}

TEST(Dictionary, CopyIsDeep) {
    Dictionary original;
    original.setValue("x", "1");

    Dictionary copy = original;
    original.setValue("x", "2");

    EXPECT_EQ(copy.getValue("x"), "1");
    EXPECT_EQ(original.getValue("x"), "2");
}


