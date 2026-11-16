#pragma once

struct BSTNode {
    int id;
    BSTNode* left;
    BSTNode* right;
};

class ProductBST {
private:
    BSTNode* root;

    BSTNode* insertNode(BSTNode* node, int id);
    bool searchNode(BSTNode* node, int id) const;
    BSTNode* removeNode(BSTNode* node, int id);
    void destroy(BSTNode* node);

public:
    ProductBST();
    ~ProductBST();
    ProductBST(const ProductBST&) = delete;
    ProductBST& operator=(const ProductBST&) = delete;

    void insert(int id);
    bool search(int id) const;
    void remove(int id);
};
