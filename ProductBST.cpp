#include "ProductBST.h"

ProductBST::ProductBST() : root(nullptr) {}
ProductBST::~ProductBST() { destroy(root); }

BSTNode* ProductBST::insertNode(BSTNode* node, int id) {
    if (node == nullptr) return new BSTNode{id, nullptr, nullptr};
    if (id < node->id) node->left = insertNode(node->left, id);
    else if (id > node->id) node->right = insertNode(node->right, id);
    return node;
}

bool ProductBST::searchNode(BSTNode* node, int id) const {
    if (node == nullptr) return false;
    if (id == node->id) return true;
    return id < node->id ? searchNode(node->left, id)
                         : searchNode(node->right, id);
}

BSTNode* ProductBST::removeNode(BSTNode* node, int id) {
    if (node == nullptr) return nullptr;

    if (id < node->id) {
        node->left = removeNode(node->left, id);
    } else if (id > node->id) {
        node->right = removeNode(node->right, id);
    } else {
        if (node->left == nullptr) {
            BSTNode* right = node->right;
            delete node;
            return right;
        }
        if (node->right == nullptr) {
            BSTNode* left = node->left;
            delete node;
            return left;
        }
        BSTNode* successor = node->right;
        while (successor->left != nullptr) successor = successor->left;
        node->id = successor->id;
        node->right = removeNode(node->right, successor->id);
    }
    return node;
}

void ProductBST::destroy(BSTNode* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

void ProductBST::insert(int id) { root = insertNode(root, id); }
bool ProductBST::search(int id) const { return searchNode(root, id); }
void ProductBST::remove(int id) { root = removeNode(root, id); }
