#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

enum STATUS {
    SUCCESS, UNDECLARED, REDECLARED, INVALID_DECLARATION, INVALID_INSTRUCTION, TYPE_MISMATCH, UNCLOSED_BLOCK, UNKNOWN_BLOCK
};

class Node
{
public:
    string id, type;
    int scope;
    int order;
    Node* left;
    Node* right;
    Node* parent;  
    Node (string id = "", string type = "", int scope = 0, int order = 0, Node* parent = NULL, Node* left = NULL, Node* right = NULL) {
        this->id = id;
        this->type = type;
        this->scope = scope;
        this->order = order;
        this->parent = parent;
        this->left = left;
        this->right = right;
    }
}; 

class SymbolTable
{
public:
    Node* root;
    int size;    
public:
    SymbolTable() {
        this->root = NULL;
        this->size = 0;
    }
    void run(string filename);
    Node* rightRotate(Node* x);
    Node* leftRotate(Node* x);
    Node* zig_zig(Node* grand);
    Node* zig_zag(Node* grand);
    Node* zag_zag(Node* grand);
    Node* zag_zig(Node* grand);
    Node* findNode(Node* root, string id, int scope, int &num_comp);
    Node* splayTree(Node* root, Node* splay);
    bool search(Node* root, string id, int scope);
    Node* insertRecursive(Node* root, string id, string type, int scope, int order);
    STATUS insert(string id, string type, int scope, int order, int &num_comp, int &num_splay);
    bool findID(Node *&root, string id, int scope, int &num_comp, int &num_splay);
    STATUS validate_function(Node* &root, string strA, string &returnedValue, int scope, int &num_comp, int &num_splay);
    STATUS assign(string id, string value, int scope, int &num_comp, int &num_splay);
    Node* findOrder(Node* root, int scope, int order);
    Node* findMaxLeft(Node* root);
    Node* deleteMaxLeft(Node* root);
    bool remove(Node *&root, int scope, int order);
    STATUS lookup(string id, int scope, int num_comp, int num_splay);

    string preOrderRecursive(Node* root);
    string preOrder();
};
#endif