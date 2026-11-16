#pragma once

#include <string>

struct StackNode {
    std::string message;
    StackNode* next;
};

class ActivityStack {
private:
    StackNode* topNode;

public:
    ActivityStack();
    ~ActivityStack();
    ActivityStack(const ActivityStack&) = delete;
    ActivityStack& operator=(const ActivityStack&) = delete;

    void push(const std::string& message);
    void showLatest() const;
};
