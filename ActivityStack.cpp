#include "ActivityStack.h"

#include <iostream>

#include "Color.h"

ActivityStack::ActivityStack() : topNode(nullptr) {}

ActivityStack::~ActivityStack() {
    while (topNode != nullptr) {
        StackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
}

void ActivityStack::push(const std::string& message) {
    topNode = new StackNode{message, topNode};
}

void ActivityStack::showLatest() const {
    if (topNode == nullptr) {
        std::cout << Color::RED << "No activity recorded yet.\n" << Color::RESET;
        return;
    }
    std::cout << Color::YELLOW << "\nMost recent activity (stack / LIFO):\n" << Color::RESET;
    int shown = 0;
    for (StackNode* current = topNode; current != nullptr && shown < 10;
         current = current->next, shown++)
        std::cout << "- " << current->message << '\n';
}
