#include "Sorting.h"

#include "Utils.h"

namespace {
bool inOrder(const Product& first, const Product& second, SortKey key) {
    if (key == SortKey::Price) return first.price <= second.price;
    return toLower(first.name) <= toLower(second.name);
}

void mergeHalves(Product products[], Product temp[], int left, int middle, int right, SortKey key) {
    int first = left;
    int second = middle + 1;
    int output = left;
    while (first <= middle && second <= right)
        temp[output++] = inOrder(products[first], products[second], key)
            ? products[first++] : products[second++];
    while (first <= middle) temp[output++] = products[first++];
    while (second <= right) temp[output++] = products[second++];
    for (int index = left; index <= right; index++) products[index] = temp[index];
}

void mergeSortRec(Product products[], Product temp[], int left, int right, SortKey key) {
    if (left >= right) return;
    int middle = left + (right - left) / 2;
    mergeSortRec(products, temp, left, middle, key);
    mergeSortRec(products, temp, middle + 1, right, key);
    mergeHalves(products, temp, left, middle, right, key);
}
}

void mergeSort(Product products[], int count, SortKey key) {
    if (count < 2) return;
    Product* temp = new Product[count];
    mergeSortRec(products, temp, 0, count - 1, key);
    delete[] temp;
}
