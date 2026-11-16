#include "BrandCarousel.h"

#include <iostream>

#include "Color.h"
#include "Utils.h"

BrandCarousel::BrandCarousel() : tail(nullptr), current(nullptr) {}

BrandCarousel::~BrandCarousel() {
    if (tail == nullptr) return;
    BrandNode* currentNode = tail->next;
    tail->next = nullptr;
    while (currentNode != nullptr) {
        BrandNode* temp = currentNode;
        currentNode = currentNode->next;
        delete temp;
    }
}

void BrandCarousel::add(const std::string& brand) {
    if (!canonical(brand).empty()) return;

    BrandNode* node = new BrandNode{brand, nullptr};
    if (tail == nullptr) {
        node->next = node;
        tail = node;
    } else {
        node->next = tail->next;
        tail->next = node;
        tail = node;
    }
}

std::string BrandCarousel::canonical(const std::string& brand) const {
    if (tail == nullptr) return "";
    std::string key = toLower(brand);
    BrandNode* currentNode = tail->next;
    do {
        if (toLower(currentNode->brand) == key) return currentNode->brand;
        currentNode = currentNode->next;
    } while (currentNode != tail->next);
    return "";
}

std::string BrandCarousel::next() {
    if (tail == nullptr) return "";
    if (current == nullptr) current = tail->next;
    std::string brand = current->brand;
    current = current->next;
    return brand;
}

void BrandCarousel::showCycle() const {
    if (tail == nullptr) {
        std::cout << Color::RED << "No partner brands registered.\n" << Color::RESET;
        return;
    }
    BrandNode* currentNode = tail->next;
    do {
        std::cout << currentNode->brand << " -> ";
        currentNode = currentNode->next;
    } while (currentNode != tail->next);
    std::cout << "(back to " << tail->next->brand << ")\n";
}
