#include "CheckoutQueue.h"

#include <iostream>

#include "Color.h"

CheckoutQueue::CheckoutQueue() : frontNode(nullptr), rearNode(nullptr) {}

CheckoutQueue::~CheckoutQueue() {
    while (frontNode != nullptr) {
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
    }
}

bool CheckoutQueue::isEmpty() const { return frontNode == nullptr; }

void CheckoutQueue::enqueue(const Order& order) {
    QueueNode* node = new QueueNode{order, nullptr};
    if (rearNode == nullptr) frontNode = rearNode = node;
    else { rearNode->next = node; rearNode = node; }
}

bool CheckoutQueue::dequeue(Order& order) {
    if (isEmpty()) return false;
    QueueNode* temp = frontNode;
    order = temp->data;
    frontNode = frontNode->next;
    if (frontNode == nullptr) rearNode = nullptr;
    delete temp;
    return true;
}

void CheckoutQueue::display() const {
    if (isEmpty()) {
        std::cout << Color::RED << "Checkout queue is empty.\n" << Color::RESET;
        return;
    }
    std::cout << Color::YELLOW << "\nPending checkout requests (FIFO):\n" << Color::RESET;
    int number = 1;
    for (QueueNode* current = frontNode; current != nullptr; current = current->next)
        std::cout << number++ << ". Product ID: " << current->data.productId
                  << ", Quantity: " << current->data.quantity << '\n';
}
