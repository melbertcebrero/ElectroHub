#pragma once

#include "Product.h"

enum class SortKey { Price, Name };

void mergeSort(Product products[], int count, SortKey key);
