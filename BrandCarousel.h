#pragma once

#include <string>

struct BrandNode {
    std::string brand;
    BrandNode* next;
};

class BrandCarousel {
private:
    BrandNode* tail;
    BrandNode* current;

public:
    BrandCarousel();
    ~BrandCarousel();
    BrandCarousel(const BrandCarousel&) = delete;
    BrandCarousel& operator=(const BrandCarousel&) = delete;

    void add(const std::string& brand);
    std::string canonical(const std::string& brand) const;
    std::string next();
    void showCycle() const;
};
