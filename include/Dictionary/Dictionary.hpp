#ifndef DICTIONARY_DICTIONARY_HPP
#define DICTIONARY_DICTIONARY_HPP

#include <cstdint>
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
    // Collision chain at a leaf node.
    struct Entry {
        std::string key;
        std::string value;
        Entry* next;

        Entry(const std::string& k, const std::string& v, Entry* n);
    };

    // A nibble (4-bit) trie node. Children at each level are stored as a
    // singly-linked list of siblings (one per nibble value that exists).
    struct TrieNode {
        std::uint8_t nibble;       // 0..15
        TrieNode* nextSibling;     // next node at the same depth
        TrieNode* child;           // first child for the next nibble
        Entry* entries;            // non-null only at the leaf depth

        TrieNode(std::uint8_t nib, TrieNode* next);
    };

    // First trie level (depth 0) sibling list.
    TrieNode* root_;

    // Helpers for deep copy / deletion and trie navigation.
    static Entry* cloneEntries(const Entry* head);
    static void deleteEntries(Entry* head);
    static TrieNode* cloneTrie(const TrieNode* node);
    static void deleteTrie(TrieNode* node);
    static TrieNode* findSibling(TrieNode* siblings, std::uint8_t nibble);
    static TrieNode* findOrCreateSibling(TrieNode*& siblings, std::uint8_t nibble);

    void clear();
    void copyFrom(const Dictionary& other);
};

#endif // DICTIONARY_DICTIONARY_HPP

