#pragma once

struct Order {
    int productId;
    int quantity;
};

struct QueueNode {
    Order data;
    QueueNode* next;
};

class CheckoutQueue {
private:
    QueueNode* frontNode;
    QueueNode* rearNode;

public:
    CheckoutQueue();
    ~CheckoutQueue();
    CheckoutQueue(const CheckoutQueue&) = delete;
    CheckoutQueue& operator=(const CheckoutQueue&) = delete;

    bool isEmpty() const;
    void enqueue(const Order& order);
    bool dequeue(Order& order);
    void display() const;
};
