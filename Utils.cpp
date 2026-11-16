#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>

#include "Color.h"

namespace {
void exitIfInputClosed() {
    if (std::cin.eof()) {
        std::cout << "\nInput closed. Exiting.\n";
        std::exit(0);
    }
}
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << Color::CYAN << prompt << Color::RESET;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        exitIfInputClosed();
        std::cout << Color::RED << "Please enter a valid whole number.\n" << Color::RESET;
        clearInput();
    }
}

double getDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << Color::CYAN << prompt << Color::RESET;
        if (std::cin >> value && value >= 0) {
            clearInput();
            return value;
        }
        exitIfInputClosed();
        std::cout << Color::RED << "Please enter a valid non-negative amount.\n" << Color::RESET;
        clearInput();
    }
}

std::string getLine(const std::string& prompt) {
    std::string value;
    std::cout << Color::CYAN << prompt << Color::RESET;
    std::getline(std::cin, value);
    exitIfInputClosed();
    return value;
}

std::string getNonEmptyLine(const std::string& prompt) {
    while (true) {
        std::string value = getLine(prompt);
        std::size_t start = value.find_first_not_of(" \t");
        if (start != std::string::npos) {
            std::size_t end = value.find_last_not_of(" \t");
            return value.substr(start, end - start + 1);
        }
        std::cout << Color::RED << "This field cannot be empty.\n" << Color::RESET;
    }
}

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char character) { return static_cast<char>(std::tolower(character)); });
    return value;
}

void printProductHeader() {
    std::cout << Color::BOLD << Color::CYAN << std::left << std::setw(6) << "ID" << std::setw(24) << "Product"
              << std::setw(16) << "Category" << std::setw(12) << "Brand"
              << std::right << std::setw(10) << "Price" << std::setw(9) << "Stock" << '\n';
    std::cout << std::string(77, '-') << Color::RESET << '\n';
}

void printProductRow(const Product& product) {
    std::string brand = toLower(product.brand);
    const char* brandColor = Color::WHITE;
    if (brand == "samsung") brandColor = Color::BLUE;
    else if (brand == "apple") brandColor = Color::MAGENTA;
    else if (brand == "asus") brandColor = Color::RED;
    else if (brand == "sony") brandColor = Color::CYAN;

    const char* stockColor = Color::GREEN;
    if (product.stockQuantity == 0) stockColor = Color::RED;
    else if (product.stockQuantity <= 5) stockColor = Color::YELLOW;

    std::cout << std::left << std::setw(6) << product.id
              << std::setw(24) << product.name
              << std::setw(16) << product.category
              << brandColor << std::setw(12) << product.brand << Color::RESET
              << std::right << std::setw(10) << std::fixed << std::setprecision(2) << product.price
              << stockColor << std::setw(9) << product.stockQuantity << Color::RESET << '\n';
}
