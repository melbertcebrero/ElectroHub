#include <iomanip>
#include <iostream>

#include "ActivityStack.h"
#include "BrandCarousel.h"
#include "CheckoutQueue.h"
#include "Color.h"
#include "Console.h"
#include "Inventory.h"
#include "ProductBST.h"
#include "Sorting.h"
#include "Utils.h"

using namespace std;

namespace {
void showMenu() {
    cout << Color::BOLD << Color::CYAN << "\n========== ELECTROHUB INVENTORY SYSTEM ==========\n" << Color::RESET << Color::YELLOW
         << " 1. Display all products\n"
         << " 2. Add a product\n"
         << " 3. Search product by name\n"
         << " 4. Filter products by brand\n"
         << " 5. Search product ID using BST\n"
         << " 6. Sort products\n"
         << " 7. Update product stock\n"
         << " 8. Remove a product\n"
         << " 9. Add checkout request\n"
         << "10. View checkout queue\n"
         << "11. Process checkout queue\n"
         << "12. View recent activity stack\n"
         << "13. Browse next featured brand (circular list)\n"
         << " 0. Exit\n"
         << Color::CYAN << "=================================================\n" << Color::RESET;
}

void handleSort(const Inventory& inventory) {
    int count = 0;
    Product* products = inventory.toArray(count);
    if (products == nullptr) {
        cout << Color::RED << "No products to sort.\n" << Color::RESET;
        return;
    }

    cout << "\nSort by:\n1. Price (lowest to highest)\n2. Name (A-Z)\n";
    int choice = getInt("Choose: ");
    if (choice != 1 && choice != 2) {
        cout << Color::RED << "Invalid sorting option.\n" << Color::RESET;
        delete[] products;
        return;
    }

    mergeSort(products, count, choice == 1 ? SortKey::Price : SortKey::Name);

    cout << Color::GREEN << "\nSorted products:\n" << Color::RESET;
    printProductHeader();
    for (int index = 0; index < count; index++) printProductRow(products[index]);

    delete[] products;
}

void processCheckout(CheckoutQueue& queue, Inventory& inventory, ActivityStack& activity) {
    if (queue.isEmpty()) {
        cout << Color::RED << "There are no checkout requests to process.\n" << Color::RESET;
        return;
    }

    Order order;
    while (queue.dequeue(order)) {
        Product* product = inventory.findById(order.productId);

        if (product == nullptr) {
            cout << Color::RED << "Order rejected: Product ID " << order.productId << " does not exist.\n" << Color::RESET;
        } else if (order.quantity <= 0) {
            cout << Color::RED << "Order rejected: Quantity must be greater than zero.\n" << Color::RESET;
        } else if (product->stockQuantity < order.quantity) {
            cout << Color::RED << "Order rejected: Not enough stock for " << product->name << ".\n" << Color::RESET;
        } else {
            product->stockQuantity -= order.quantity;
            double total = product->price * order.quantity;
            cout << Color::GREEN << "Order completed: " << product->name << " x " << order.quantity
                 << " | Total: PHP " << fixed << setprecision(2) << total << Color::RESET << '\n';
            activity.push("Checkout completed for " + product->name);
        }
    }
}
}

int main() {
    enableColors();
    Inventory inventory;
    ActivityStack activity;
    CheckoutQueue checkoutQueue;
    ProductBST productIndex;
    BrandCarousel brands;

    brands.add("Samsung");
    brands.add("Apple");
    brands.add("ASUS");
    brands.add("Sony");

    Product sampleProducts[] = {
        {101, "Galaxy A55",      "Smartphone", "Samsung", 19999.00, 10},
        {102, "iPhone 15",       "Smartphone", "Apple",   39990.00,  8},
        {103, "ASUS VivoBook",   "Laptop",     "ASUS",    32999.00,  5},
        {104, "WH-CH520",        "Headphones", "Sony",     2499.00, 15},
        {105, "Galaxy Tab S9",   "Tablet",     "Samsung", 34999.00,  4},
        {106, "MacBook Air",     "Laptop",     "Apple",   59990.00,  3},
        {107, "ASUS TUF Gaming", "Laptop",     "ASUS",    45999.00,  6},
        {108, "Sony SRS-XB100",  "Speaker",    "Sony",     2999.00, 12}
    };
    for (const Product& product : sampleProducts) {
        if (inventory.addProduct(product)) productIndex.insert(product.id);
    }

    int choice;
    do {
        showMenu();
        choice = getInt("Enter your choice: ");

        switch (choice) {
        case 1:
            inventory.displayAll();
            break;

        case 2: {
            Product product;
            product.id = getInt("Enter product ID: ");
            if (product.id <= 0) {
                cout << Color::RED << "Product ID must be a positive number.\n" << Color::RESET;
                break;
            }
            if (productIndex.search(product.id) || inventory.idExists(product.id)) {
                cout << Color::RED << "A product with that ID already exists.\n" << Color::RESET;
                break;
            }
            product.name = getNonEmptyLine("Enter product name: ");
            product.category = getNonEmptyLine("Enter category: ");

            cout << "Partner brands: ";
            brands.showCycle();
            string brand = brands.canonical(getNonEmptyLine("Enter brand: "));
            if (brand.empty()) {
                cout << Color::RED << "Not a partner brand. Product was not added.\n" << Color::RESET;
                break;
            }
            product.brand = brand;
            product.price = getDouble("Enter price (PHP): ");
            product.stockQuantity = getInt("Enter stock quantity: ");
            if (product.stockQuantity < 0) {
                cout << Color::RED << "Stock cannot be negative. Product was not added.\n" << Color::RESET;
                break;
            }

            if (inventory.addProduct(product)) {
                productIndex.insert(product.id);
                activity.push("Added product: " + product.name);
                cout << Color::GREEN << "Product added successfully.\n" << Color::RESET;
            }
            break;
        }

        case 3:
            inventory.searchByName(getNonEmptyLine("Enter product name or keyword: "));
            break;

        case 4: {
            string brand = brands.canonical(getNonEmptyLine("Enter brand (Samsung/Apple/ASUS/Sony): "));
            if (brand.empty()) cout << "Not a partner brand.\n";
            else inventory.filterByBrand(brand);
            break;
        }

        case 5: {
            int id = getInt("Enter product ID to search: ");
            Product* product = productIndex.search(id) ? inventory.findById(id) : nullptr;
            if (product != nullptr) {
                cout << "Product ID exists in the BST index.\n";
                printProductHeader();
                printProductRow(*product);
            } else {
                cout << Color::RED << "Product ID not found.\n" << Color::RESET;
            }
            break;
        }

        case 6:
            handleSort(inventory);
            break;

        case 7: {
            int id = getInt("Enter product ID: ");
            int stock = getInt("Enter new stock quantity: ");
            Product* product = inventory.findById(id);
            if (product == nullptr) {
                cout << Color::RED << "Product ID not found.\n" << Color::RESET;
            } else if (stock < 0) {
                cout << Color::RED << "Stock cannot be negative.\n" << Color::RESET;
            } else {
                string name = product->name;
                inventory.updateStock(id, stock);
                activity.push("Updated stock for " + name);
                cout << Color::GREEN << "Stock updated successfully.\n" << Color::RESET;
            }
            break;
        }

        case 8: {
            int id = getInt("Enter product ID to remove: ");
            Product* product = inventory.findById(id);
            if (product == nullptr) {
                cout << Color::RED << "Product ID not found.\n" << Color::RESET;
                break;
            }
            string name = product->name;
            inventory.removeProduct(id);
            productIndex.remove(id);
            activity.push("Removed product: " + name);
            cout << Color::GREEN << "Product removed successfully.\n" << Color::RESET;
            break;
        }

        case 9: {
            Order order;
            order.productId = getInt("Enter product ID to buy: ");
            if (inventory.findById(order.productId) == nullptr) {
                cout << Color::RED << "Product ID not found. Request not added.\n" << Color::RESET;
                break;
            }
            order.quantity = getInt("Enter quantity: ");
            if (order.quantity <= 0) {
                cout << Color::RED << "Quantity must be greater than zero. Request not added.\n" << Color::RESET;
                break;
            }
            checkoutQueue.enqueue(order);
            cout << Color::GREEN << "Checkout request added to the queue.\n" << Color::RESET;
            break;
        }

        case 10:
            checkoutQueue.display();
            break;

        case 11:
            processCheckout(checkoutQueue, inventory, activity);
            break;

        case 12:
            activity.showLatest();
            break;

        case 13: {
            cout << "\nBrand carousel: ";
            brands.showCycle();
            string featured = brands.next();
            cout << Color::BOLD << Color::GREEN << "Featured brand now: " << featured << Color::RESET << '\n';
            inventory.filterByBrand(featured);
            break;
        }

        case 0:
            cout << Color::GREEN << "Thank you for using ElectroHub. Goodbye!\n" << Color::RESET;
            break;

        default:
            cout << Color::RED << "Invalid choice. Please try again.\n" << Color::RESET;
        }
    } while (choice != 0);

    return 0;
}
