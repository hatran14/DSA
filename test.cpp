#include <iostream>
#include <string>

using namespace std;

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

//zigggggggg
Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (x->left != nullptr) x->left->parent = x;
    y->right = x;
    y->parent = x->parent;
    y->right->parent = y;
    if (y->parent == nullptr)
        return y;
    if (y->scope < y->parent->scope)
        y->parent->left = y;
    else if (y->scope > y->parent->scope)
        y->parent->right = y;
    else {
        if (y->id.compare(y->parent->id) < 0)
            y->parent->left = y;
        else if (y->id.compare(y->parent->id) > 0)
            y->parent->right = y;
    }        
    return y;
}

//zagggggggggg
Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (x->right != nullptr) x->right->parent = x;
    y->left = x;
    y->parent = x->parent;
    y->left->parent = y;
    if (y->parent == nullptr)
        return y;
    if (y->scope < y->parent->scope)
        y->parent->left = y;
    else if (y->scope > y->parent->scope)
        y->parent->right = y;
    else {
        if (y->id.compare(y->parent->id) < 0)
            y->parent->left = y;
        else if (y->id.compare(y->parent->id) > 0)
            y->parent->right = y;
    }   
    return y;
}

Node* zig_zig(Node* grand) {
    Node* parent =  rightRotate(grand);
    Node* child =  rightRotate(parent);
    return child;
}

Node* zig_zag(Node* grand) {
    Node* parent = grand->left;
    grand =  leftRotate(parent)->parent;
    Node* child =  rightRotate(grand);
    return child;
}

Node* zag_zag(Node* grand) {
    Node* parent =  leftRotate(grand);
    Node* child =  leftRotate(parent);
    return child;
}

Node* zag_zig(Node* grand) {
    Node* parent = grand->right;
    grand =  rightRotate(parent)->parent;
    Node* child =  leftRotate(grand);
    return child;
}

Node* findNode(Node* root, string id, int scope, int &num_comp) {
    if (root == nullptr) return NULL;
    if (scope < root->scope) {
        num_comp++;
        return findNode(root->left, id, scope, num_comp);
    }    
    else if (scope > root->scope) {
        num_comp++;
        return findNode(root->right, id, scope, num_comp);
    }    
    else {
        num_comp++;
        if (id.compare(root->id) < 0)
            return findNode(root->left, id, scope, num_comp);
        else if (id.compare(root->id) > 0)
            return findNode(root->right, id, scope, num_comp);
        else
            return root;    
    }
}

Node* splayTree(Node* root, Node* splay) {
    Node* child = splay;
    Node* parent;

    while (true) {
        parent = child->parent;
        if (parent == nullptr) return child;
        Node* grand = parent->parent;
        if (grand == nullptr) {
            if (child->scope < parent->scope)
                child =  rightRotate(parent);
            else if (child->scope > parent->scope)
                child =  leftRotate(parent);
            else {
                if (child->id.compare(parent->id) < 0)
                    child =  rightRotate(parent);
                else if (child->id.compare(parent->id) > 0)
                    child =  leftRotate(parent);    
            }
        }
        else {
            if (child->scope < parent->scope) {
                //num_comp++;
                if (parent->scope < grand->scope) {
                    //num_comp++;
                    child =  zig_zig(grand);
                }
                else if (parent->scope > grand->scope) {
                    //num_comp++;
                    child =  zag_zig(grand);
                }
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                    //num_comp++;
                    child =  zig_zig(grand);
                } 
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                    //num_comp++;
                    child =  zag_zig(grand);
                }
            }
            else if (child->scope > parent->scope) {
                //num_comp++;
                if (parent->scope > grand->scope) {
                    //num_comp++;
                    child =  zag_zag(grand);
                }
                else if (parent->scope < grand->scope) {
                    //num_comp++;
                    child =  zig_zag(grand);
                }
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                    //num_comp++;
                    child =  zag_zag(grand);
                } 
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                    //num_comp++;
                    child =  zig_zag(grand);
                }
            }
            else if (child->scope == parent->scope) {
                if (child->id.compare(parent->id) < 0) {
                    //num_comp++;
                    if (parent->scope < grand->scope) {
                        //num_comp++;
                        child =  zig_zig(grand);
                    }
                    else if (parent->scope > grand->scope) {
                        //num_comp++;
                        child =  zag_zig(grand);
                    }
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                        //num_comp++;
                        child =  zig_zig(grand);
                    } 
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                        //num_comp++;
                        child =  zag_zig(grand);
                    }
                }
                else if (child->id.compare(parent->id) > 0) {
                    //num_comp++;
                    if (parent->scope > grand->scope) {
                        //num_comp++;
                        child =  zag_zag(grand);
                    }
                    else if (parent->scope < grand->scope) {
                        //num_comp++;
                        child =  zig_zag(grand);
                    }
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                        //num_comp++;
                        child =  zag_zag(grand);
                    } 
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                        //num_comp++;
                        child =  zig_zag(grand);
                    }
                }
            }
        }        
    }
}

bool search(Node* root, string id, int scope) {
    if (root == nullptr) return false;
    else if (root->scope == scope && id.compare(root->id) == 0)
        return true;
    if (scope < root->scope)
        return search(root->left, id, scope);
    if (scope > root->scope)
        return search(root->right, id, scope);
    if (scope == root->scope) {
        if (id.compare(root->id) < 0)
            return search(root->left, id, scope);
        if (id.compare(root->id) > 0)
            return search(root->right, id, scope);
    }
    return false;
}

Node* insertRecursive(Node* root, string id, string type, int scope, int order) {
    if (root == nullptr) return new Node(id, type, scope, order);
    if (scope < root->scope) {
        if (root->left == nullptr) {
            Node* newNode = new Node(id, type, scope, order);
            root->left = newNode;
            newNode->parent = root;
            return root;
        }
        else root->left = insertRecursive(root->left, id, type, scope, order);
    }    
    else if (scope > root->scope) {
        if (root->right == nullptr) {
            Node* newNode = new Node(id, type, scope, order);
            root->right = newNode;
            newNode->parent = root;
            //return root;
        }
        else root->right = insertRecursive(root->right, id, type, scope, order);
    }
    else {
        if (id.compare(root->id) < 0) {
            if (root->left == nullptr) {
                Node* newNode = new Node(id, type, scope, order);
                root->left = newNode;
                newNode->parent = root;
                //return root;
            }
            else root->left = insertRecursive(root->left, id, type, scope, order);            
        }
        else if (id.compare(root->id) > 0) {
            if (root->right == nullptr) {
                Node* newNode = new Node(id, type, scope, order);
                root->right = newNode;
                newNode->parent = root;
                //return root;
            }
            else root->right = insertRecursive(root->right, id, type, scope, order);
        }
        else return root;
    }
    return root;
}

Node* insert(Node* root, string id, string type, int scope, int order) {
	int num_comp = 0;
    root = insertRecursive(root, id, type, scope, order);
    Node* splay = findNode(root, id, scope, num_comp);
    root = splayTree(root, splay);

    return root;
}

/* string preOrder(Node* root) {
    if (root == nullptr) return "";
    string output = root->id + " " + root->type + " " + to_string(root->scope) + "\n";
    output += (preOrder(root->left) + preOrder(root->right));
    return output;
} */

Node* findOrder(Node* root, int scope, int order) {
    if (root == nullptr) return nullptr;  
    if (root->scope == scope && root->order == order) return root;
    Node* found = findOrder(root->left, scope, order);
    if (found != nullptr) return found;
	return findOrder(root->right, scope, order);
}

Node* findMaxLeft(Node *root) {
    if (root->right == nullptr) return root;
    return findMaxLeft(root->right);
}

Node* deleteMaxLeft(Node* root) {
	if (root->right == nullptr) {
        if (root->left == nullptr)
            return nullptr;
        else {
            Node* temp = root->left;
            temp->parent = root->parent;
            temp->parent->right = temp;
            //delete root;
            return temp;
        }
    }
    //cout << root->right->right->id;    
	root->right = deleteMaxLeft(root->right);
	return root;
}

void preOrder (Node* root);
void remove(Node *&root, int scope, int order, int num_comp) {
    Node* del =  findOrder(root, scope, order);
    if (del == nullptr) return;
	//cout << del->id << endl;
    root = splayTree(root, del);
    //preOrder(root->left->right->left);
    if (root->left == nullptr) {
        Node* newRoot = root->right;
        newRoot->parent = nullptr;
        root = newRoot;
        return;
    }
    if (root->right == nullptr) {
        Node* newRoot = root->left;
        newRoot->parent = nullptr;
        root = newRoot;
        return;
    }
    Node* maxLeft = findMaxLeft(root->left);
	if (maxLeft->parent->parent == nullptr) {
		maxLeft->right = root->right;
		maxLeft->right->parent = maxLeft;
        maxLeft->parent = nullptr;
		root = maxLeft;
		return;
	}
    //preOrder(root->left);
    //cout << maxLeft->id << endl;
    Node* temp = root->left;
    //temp->parent = nullptr;
    temp = deleteMaxLeft(temp);
    maxLeft->parent = nullptr;
    maxLeft->right = root->right;
	maxLeft->right->parent = maxLeft;
    //cout << temp->id << endl;
    //cout << temp->right->right->id << endl;
	maxLeft->left = temp;
	maxLeft->left->parent = maxLeft;
	root = maxLeft;
    return;
}


void preOrder(Node* root) {
	if (root == nullptr) return;
	cout << root->id << " " << root->type << " " << root->scope << " " << root->order << endl;
	preOrder(root->left);
	preOrder(root->right);
}

int main()
{
    Node* root = nullptr;
    string type = "number";
    int num = 0;
    //root = insert(root, "f", type, 0, 1);
    root = insert(root, "d", type, 0, 2);
    root = insert(root, "a", type, 0, 3);
    root = insert(root, "e", type, 0, 4);
	root = insert(root, "h", type, 0, 5);
    root = insert(root, "c", type, 0, 6);
    root = insert(root, "b", type, 0, 7);
	root = insert(root, "f", type, 0, 8);
    //for (int i = 1; i <= 8; i++) {
        //remove(root, 1, 7, 0);
        //preOrder(root);
        //cout << root->id << endl;
        //cout << root->parent->id;
    //root = splayTree(root, "g", 1, num);    
    remove(root, 0, 8, 0);
    cout << root->id << endl;
    preOrder(root);
    //cout << root->id << endl;
    return 0;
    /* int main() {
    SymbolTable table;
    string type = "number";
    int num1 = 0;
    int num2 = 0;
    table.insert("f", type, 0, 1, num1, num2);
    cout << table.root->id;
    table.lookup("f",0,num1,num2);
    cout << table.root->id;
} */
}