#pragma once

#include <string>

#include "Product.h"

void clearInput();
int getInt(const std::string& prompt);
double getDouble(const std::string& prompt);
std::string getLine(const std::string& prompt);
std::string getNonEmptyLine(const std::string& prompt);

std::string toLower(std::string value);
void printProductHeader();
void printProductRow(const Product& product);
