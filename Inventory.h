#pragma once

#include <string>

#include "Product.h"

struct ProductNode {
    Product data;
    ProductNode* next;
};

class Inventory {
private:
    ProductNode* head;
    ProductNode* tail;
    int count;

public:
    Inventory();
    ~Inventory();

    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;

    void clear();
    bool isEmpty() const;
    int size() const;

    bool idExists(int id) const;
    bool addProduct(const Product& product);
    Product* findById(int id);
    bool removeProduct(int id);
    bool updateStock(int id, int newStock);

    void displayAll() const;
    void filterByBrand(const std::string& brand) const;
    void searchByName(const std::string& keyword) const;

    Product* toArray(int& outCount) const;
};
