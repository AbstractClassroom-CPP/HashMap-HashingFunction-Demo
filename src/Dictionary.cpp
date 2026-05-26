#include "Dictionary/Dictionary.hpp"

#include <simple_integer_hash/IntegerHash.hpp>

#include <stdexcept>
#include <utility>

namespace {

constexpr int kNibbleCount = 16; // 64 bits @ 4 bits per step

std::uint8_t nibbleAt(std::uint64_t h, int depth) {
    // Use most-significant nibble first.
    const int shift = 60 - (depth * 4);
    return static_cast<std::uint8_t>((h >> shift) & 0xFULL);
}

} // namespace

Dictionary::Entry* Dictionary::cloneEntries(const Dictionary::Entry* head) {
    Dictionary::Entry* newHead = nullptr;
    Dictionary::Entry* tail = nullptr;

    const Dictionary::Entry* current = head;
    while (current != nullptr) {
        auto* n = new Dictionary::Entry(current->key, current->value, nullptr);
        if (newHead == nullptr) {
            newHead = n;
            tail = n;
        } else {
            tail->next = n;
            tail = n;
        }
        current = current->next;
    }

    return newHead;
}

void Dictionary::deleteEntries(Dictionary::Entry* head) {
    while (head != nullptr) {
        Dictionary::Entry* toDelete = head;
        head = head->next;
        delete toDelete;
    }
}

Dictionary::TrieNode* Dictionary::cloneTrie(const Dictionary::TrieNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    auto* copy = new Dictionary::TrieNode(node->nibble, nullptr);
    copy->entries = cloneEntries(node->entries);
    copy->child = cloneTrie(node->child);
    copy->nextSibling = cloneTrie(node->nextSibling);
    return copy;
}

void Dictionary::deleteTrie(Dictionary::TrieNode* node) {
    if (node == nullptr) {
        return;
    }

    deleteTrie(node->child);
    deleteTrie(node->nextSibling);
    deleteEntries(node->entries);
    delete node;
}

Dictionary::TrieNode* Dictionary::findSibling(Dictionary::TrieNode* siblings, std::uint8_t nibble) {
    Dictionary::TrieNode* current = siblings;
    while (current != nullptr) {
        if (current->nibble == nibble) {
            return current;
        }
        current = current->nextSibling;
    }
    return nullptr;
}

Dictionary::TrieNode* Dictionary::findOrCreateSibling(Dictionary::TrieNode*& siblings, std::uint8_t nibble) {
    Dictionary::TrieNode* existing = findSibling(siblings, nibble);
    if (existing != nullptr) {
        return existing;
    }

    // Append to the end to keep sibling list stable/predictable.
    if (siblings == nullptr) {
        siblings = new Dictionary::TrieNode(nibble, nullptr);
        return siblings;
    }

    Dictionary::TrieNode* tail = siblings;
    while (tail->nextSibling != nullptr) {
        tail = tail->nextSibling;
    }
    tail->nextSibling = new Dictionary::TrieNode(nibble, nullptr);
    return tail->nextSibling;
}

Dictionary::Entry::Entry(const std::string& k, const std::string& v, Entry* n)
    : key(k), value(v), next(n) {}

Dictionary::TrieNode::TrieNode(std::uint8_t nib, TrieNode* next)
    : nibble(nib), nextSibling(next), child(nullptr), entries(nullptr) {}

Dictionary::Dictionary() : root_(nullptr) {}

Dictionary::~Dictionary() {
    clear();
}

Dictionary::Dictionary(const Dictionary& other) : root_(nullptr) {
    copyFrom(other);
}

Dictionary& Dictionary::operator=(const Dictionary& other) {
    if (this == &other) {
        return *this;
    }

    clear();
    copyFrom(other);
    return *this;
}

Dictionary::Dictionary(Dictionary&& other) noexcept : root_(other.root_) {
    other.root_ = nullptr;
}

Dictionary& Dictionary::operator=(Dictionary&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear();
    root_ = other.root_;
    other.root_ = nullptr;
    return *this;
}

void Dictionary::setValue(const std::string& key, const std::string& value) {
    const simple_integer_hash::IntegerHash hashedKey(key);
    const std::uint64_t h = hashedKey.hash();

    TrieNode** siblingsPtr = &root_;
    TrieNode* node = nullptr;

    for (int depth = 0; depth < kNibbleCount; ++depth) {
        const std::uint8_t nib = nibbleAt(h, depth);
        node = findOrCreateSibling(*siblingsPtr, nib);
        siblingsPtr = &node->child;
    }

    // Leaf: update/insert into the collision chain.
    Entry* e = node->entries;
    while (e != nullptr) {
        if (e->key == key) {
            e->value = value;
            return;
        }
        e = e->next;
    }

    node->entries = new Entry(key, value, node->entries);
}

std::string Dictionary::getValue(const std::string& key) const {
    const simple_integer_hash::IntegerHash hashedKey(key);
    const std::uint64_t h = hashedKey.hash();

    TrieNode* siblings = root_;
    TrieNode* node = nullptr;

    for (int depth = 0; depth < kNibbleCount; ++depth) {
        const std::uint8_t nib = nibbleAt(h, depth);
        node = findSibling(siblings, nib);
        if (node == nullptr) {
            throw std::out_of_range("Dictionary key not found: " + key);
        }
        siblings = node->child;
    }

    Entry* e = node->entries;
    while (e != nullptr) {
        if (e->key == key) {
            return e->value;
        }
        e = e->next;
    }

    throw std::out_of_range("Dictionary key not found: " + key);
}

void Dictionary::clear() {
    deleteTrie(root_);
    root_ = nullptr;
}

void Dictionary::copyFrom(const Dictionary& other) {
    root_ = cloneTrie(other.root_);
}

