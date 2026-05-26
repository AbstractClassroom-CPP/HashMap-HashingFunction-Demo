#include "Dictionary/Dictionary.hpp"

#include <stdexcept>
#include <utility>

Dictionary::Node::Node(const std::string& k, const std::string& v, Node* n)
    : key(k), value(v), next(n) {}

Dictionary::Dictionary() : head_(nullptr) {}

Dictionary::~Dictionary() {
    clear();
}

Dictionary::Dictionary(const Dictionary& other) : head_(nullptr) {
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

Dictionary::Dictionary(Dictionary&& other) noexcept : head_(other.head_) {
    other.head_ = nullptr;
}

Dictionary& Dictionary::operator=(Dictionary&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear();
    head_ = other.head_;
    other.head_ = nullptr;
    return *this;
}

void Dictionary::setValue(const std::string& key, const std::string& value) {
    if (head_ == nullptr) {
        head_ = new Node(key, value, nullptr);
        return;
    }

    Node* current = head_;
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }

        if (current->next == nullptr) {
            break;
        }
        current = current->next;
    }

    // Key not found; append new node at the end.
    current->next = new Node(key, value, nullptr);
}

std::string Dictionary::getValue(const std::string& key) const {
    Node* current = head_;
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    throw std::out_of_range("Dictionary key not found: " + key);
}

void Dictionary::clear() {
    Node* current = head_;
    while (current != nullptr) {
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    head_ = nullptr;
}

void Dictionary::copyFrom(const Dictionary& other) {
    // Preserve order by appending.
    Node* otherCurrent = other.head_;
    Node* thisTail = nullptr;

    while (otherCurrent != nullptr) {
        Node* newNode = new Node(otherCurrent->key, otherCurrent->value, nullptr);

        if (head_ == nullptr) {
            head_ = newNode;
            thisTail = head_;
        } else {
            thisTail->next = newNode;
            thisTail = newNode;
        }

        otherCurrent = otherCurrent->next;
    }
}

