#include "Inventory.h"

#include <iostream>

#include "Color.h"
#include "Utils.h"

Inventory::Inventory() : head(nullptr), tail(nullptr), count(0) {}

Inventory::~Inventory() { clear(); }

void Inventory::clear() {
    while (head != nullptr) {
        ProductNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    count = 0;
}

bool Inventory::isEmpty() const { return head == nullptr; }
int Inventory::size() const { return count; }

bool Inventory::idExists(int id) const {
    for (ProductNode* current = head; current != nullptr; current = current->next)
        if (current->data.id == id) return true;
    return false;
}

bool Inventory::addProduct(const Product& product) {
    if (idExists(product.id)) return false;

    ProductNode* node = new ProductNode{product, nullptr};
    if (head == nullptr) head = tail = node;
    else { tail->next = node; tail = node; }
    count++;
    return true;
}

Product* Inventory::findById(int id) {
    for (ProductNode* current = head; current != nullptr; current = current->next)
        if (current->data.id == id) return &current->data;
    return nullptr;
}

bool Inventory::removeProduct(int id) {
    ProductNode* current = head;
    ProductNode* previous = nullptr;
    while (current != nullptr && current->data.id != id) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) return false;

    if (previous == nullptr) head = current->next;
    else previous->next = current->next;
    if (current == tail) tail = previous;

    delete current;
    count--;
    return true;
}

bool Inventory::updateStock(int id, int newStock) {
    Product* product = findById(id);
    if (product == nullptr || newStock < 0) return false;
    product->stockQuantity = newStock;
    return true;
}

void Inventory::displayAll() const {
    if (isEmpty()) {
        std::cout << Color::RED << "No products in the inventory.\n" << Color::RESET;
        return;
    }
    printProductHeader();
    for (ProductNode* current = head; current != nullptr; current = current->next)
        printProductRow(current->data);
}

void Inventory::filterByBrand(const std::string& brand) const {
    bool found = false;
    std::string key = toLower(brand);
    std::cout << Color::YELLOW << "\nProducts under brand: " << brand << Color::RESET << "\n";
    for (ProductNode* current = head; current != nullptr; current = current->next) {
        if (toLower(current->data.brand) == key) {
            if (!found) printProductHeader();
            printProductRow(current->data);
            found = true;
        }
    }
    if (!found) std::cout << "No products found for that brand.\n";
}

void Inventory::searchByName(const std::string& keyword) const {
    bool found = false;
    std::string key = toLower(keyword);
    for (ProductNode* current = head; current != nullptr; current = current->next) {
        if (toLower(current->data.name).find(key) != std::string::npos) {
            if (!found) {
                std::cout << Color::YELLOW << "\nSearch results:\n" << Color::RESET;
                printProductHeader();
            }
            printProductRow(current->data);
            found = true;
        }
    }
    if (!found) std::cout << "No matching product found.\n";
}

Product* Inventory::toArray(int& outCount) const {
    outCount = count;
    if (count == 0) return nullptr;
    Product* products = new Product[count];
    int index = 0;
    for (ProductNode* current = head; current != nullptr; current = current->next)
        products[index++] = current->data;
    return products;
}
