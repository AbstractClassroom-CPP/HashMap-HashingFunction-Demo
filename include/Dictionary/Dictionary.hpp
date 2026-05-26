#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <string>

class Dictionary {
public:
    Dictionary();
    ~Dictionary();

    Dictionary(const Dictionary& other);
    Dictionary& operator=(const Dictionary& other);

    Dictionary(Dictionary&& other) noexcept;
    Dictionary& operator=(Dictionary&& other) noexcept;

    // Inserts or updates (key -> value).
    void setValue(const std::string& key, const std::string& value);

    // Returns value or throws std::out_of_range if the key is not found.
    std::string getValue(const std::string& key) const;

private:
    struct Node {
        std::string key;
        std::string value;
        Node* next;

        Node(const std::string& k, const std::string& v, Node* n);
    };

    Node* head_;

    void clear();
    void copyFrom(const Dictionary& other);
};

#endif // DICTIONARY_DICTIONARY_HPP

