#include "SymbolTable.h"

enum KEYWORD {
    INSERT, ASSIGN, BEGIN, END, LOOKUP, PRINT, ERROR
};

enum TYPE {
    NUMBER, STRING, FUNCTION, ID, FUNCTION_CALL, INVALID
};

KEYWORD getKey (string str) {
    if (str == "INSERT") return INSERT;
    else if (str == "ASSIGN") return ASSIGN;
    else if (str == "BEGIN") return BEGIN;
    else if (str == "END") return END;
    else if (str == "LOOKUP") return LOOKUP;
    else if (str == "PRINT") return PRINT;
    return ERROR;
}

bool checkID (string id) {
    regex regexp("[a-z][a-zA-Z0-9_]*");
    smatch m;
    if (regex_match(id, m, regexp)) 
        return true;
    return false;
}

bool isString(string value) {
    regex regexp("\\'[a-zA-Z0-9 ]*\\'");
    smatch m;   
    if (regex_match(value, m, regexp)) 
        return true;
    return false;    
}

bool isNumber(string value) {
    regex regexp("[0-9]+");
    smatch m;
    if (regex_match(value, m, regexp)) 
        return true;
    return false;
}

bool isFunction(string value) {
    regex regexp ("\\(((number,|string,)*(number|string)|())\\)\\-\\>(number|string)");
    if (regex_match(value, regexp)) return true;
    return false;
}

bool isFunctionCalling(string value) {
    regex regexp("[a-z][a-zA-Z0-9_]*\\((([0-9]+,|'[a-zA-Z0-9 ]*',|[a-z][a-zA-Z0-9_]*,)*([0-9]+|'[a-zA-Z0-9 ]*'|[a-z][a-zA-Z0-9_]*)|())\\)");
    if (regex_match(value, regexp)) return true;
    return false;
}

TYPE checkTypeOfValue(string value) {
    if (isNumber(value)) return NUMBER;
    else if (isString(value)) return STRING;
    else if (isFunction(value)) return FUNCTION;
    else if (checkID(value)) return ID;
    else if (isFunctionCalling(value)) return FUNCTION_CALL;
    else return INVALID;
}

void tokenize (string str, string& keyword, string& ID, string& value, string& staticc) {
    int pos = (int)str.find(" ");
    if (pos == -1) {
        keyword = str;
        ID = "";
        value = "";
        staticc = "";
        return;
    }

    keyword = str.substr(0, pos);
    str.erase(0, pos + 1);
    pos = (int)str.find(" ");
    if (pos == -1) {
        ID = str;
        value = "";
        staticc = "";
        return;
    }
    ID = str.substr(0, pos);

    str.erase(0, pos + 1);
    pos = (int)str.find(" ");
    if (pos == -1 || keyword == "ASSIGN") {
        value = str;
        staticc = "";
        return;
    }
    value = str.substr(0, pos);
    
    str.erase(0, pos + 1);
    staticc = str;
    return;
}

void SymbolTable::run(string filename)
{
    SymbolTable table;
    ifstream myfile;
    myfile.open(filename, ios::in);
    string line;
    int scope = 0; int order = 1;
    string keyword, ID, value, staticc;
    int num_comp, num_splay;

    while (myfile.good()) {
        getline(myfile, line);
        tokenize(line, keyword, ID, value, staticc);
        KEYWORD input = getKey(keyword);
        STATUS output;
        num_comp = 0; num_splay = 0;
        int countDelNode = 0;
        switch (input)
        {
        case INSERT:
            if (staticc != "true" && staticc != "false")
                throw InvalidInstruction(line);

            if (staticc == "true")
                output = table.insert(ID, value, 0, order, num_comp, num_splay);

            else output = table.insert(ID, value, scope, order, num_comp, num_splay); 

            if (output == REDECLARED)
                throw Redeclared(line);
            else if (output == INVALID_DECLARATION)
                throw InvalidDeclaration(line);
            else if (output == INVALID_INSTRUCTION)
                throw InvalidInstruction(line);

            cout << num_comp << " " << num_splay << endl;
            order++;    
            break;
        
        case ASSIGN:
            output = table.assign(ID, value, scope, num_comp, num_splay);
            if (output == INVALID_INSTRUCTION)
                throw InvalidInstruction(line);
            else if (output == UNDECLARED)
                throw Undeclared(line);
            else if (output == TYPE_MISMATCH)
                throw TypeMismatch(line);

            cout << num_comp << " " << num_splay << endl;            
            break;

        case BEGIN:
            if (!ID.empty()) throw InvalidInstruction(line);
            scope++;
            break;

        case END:
            if (!ID.empty()) throw InvalidInstruction(line);
            if (scope == 0) throw UnknownBlock();
            for (int i = 1; i < order; i++) {
                bool flag = table.remove(table.root, scope, i);
                if (flag == true) countDelNode++;
            }
            order -= countDelNode;
            scope--;
            break;

        case LOOKUP:
            if (!value.empty()) throw InvalidInstruction(line);
            output = table.lookup(ID, scope, num_comp, num_splay);
            if (output == UNDECLARED) throw Undeclared(line);
            cout << table.root->scope << endl;
            break;       

        case PRINT:
            if (!ID.empty()) throw InvalidInstruction(line);
            if (table.root == nullptr) cout << "";
            else cout << table.preOrder() << endl;
            break;

        default:
            throw InvalidInstruction(line);
            break;
        }
    }
    myfile.close();
    if (scope > 0) throw UnclosedBlock(scope);

}

//zigggggggg
Node* SymbolTable::rightRotate(Node* x) {
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
Node* SymbolTable::leftRotate(Node* x) {
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

Node* SymbolTable::zig_zig(Node* grand) {
    Node* parent = this->rightRotate(grand);
    Node* child = this->rightRotate(parent);
    return child;
}

Node* SymbolTable::zig_zag(Node* grand) {
    Node* parent = grand->left;
    grand = this->leftRotate(parent)->parent;
    Node* child = this->rightRotate(grand);
    return child;
}

Node* SymbolTable::zag_zag(Node* grand) {
    Node* parent = this->leftRotate(grand);
    Node* child = this->leftRotate(parent);
    return child;
}

Node* SymbolTable::zag_zig(Node* grand) {
    Node* parent = grand->right;
    grand = this->rightRotate(parent)->parent;
    Node* child = this->leftRotate(grand);
    return child;
}

Node* SymbolTable::findNode(Node* root, string id, int scope, int &num_comp) {
    if (root == nullptr) return NULL;
    if (scope < root->scope) {
        num_comp++;
        return this->findNode(root->left, id, scope, num_comp);
    }    
    else if (scope > root->scope) {
        num_comp++;
        return this->findNode(root->right, id, scope, num_comp);
    }    
    else {
        num_comp++;
        if (id.compare(root->id) < 0)
            return this->findNode(root->left, id, scope, num_comp);
        else if (id.compare(root->id) > 0)
            return this->findNode(root->right, id, scope, num_comp);
        else
            return root;    
    }
}

Node* SymbolTable::splayTree(Node* root, Node* splay) {
    Node* child = splay;

    while (true) {
        Node* parent = child->parent;
        if (parent == nullptr) return child;
        Node* grand = parent->parent;
        if (grand == nullptr) {
            if (child->scope < parent->scope) {
                child = this->rightRotate(parent);
            }    
            else if (child->scope > parent->scope) {
                child = this->leftRotate(parent);
            }    
            else {
                if (child->id.compare(parent->id) < 0)
                    child = this->rightRotate(parent);
                else if (child->id.compare(parent->id) > 0)
                    child = this->leftRotate(parent);    
            }
        }
        else {
            if (child->scope < parent->scope) {
                if (parent->scope < grand->scope) {
                    child = this->zig_zig(grand);
                }
                else if (parent->scope > grand->scope) {
                    child = this->zag_zig(grand);
                }
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                    child = this->zig_zig(grand);
                } 
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                    child = this->zag_zig(grand);
                }
            }
            else if (child->scope > parent->scope) {
                if (parent->scope > grand->scope) {
                    child = this->zag_zag(grand);
                }
                else if (parent->scope < grand->scope) {  
                    child = this->zig_zag(grand);
                }
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                    child = this->zag_zag(grand);
                } 
                else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                    child = this->zig_zag(grand);
                }
            }
            else if (child->scope == parent->scope) {
                if (child->id.compare(parent->id) < 0) {
                    if (parent->scope < grand->scope) {
                        child = this->zig_zig(grand);
                    }
                    else if (parent->scope > grand->scope) {
                        child = this->zag_zig(grand);
                    }
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                        child = this->zig_zig(grand);
                    } 
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                        child = this->zag_zig(grand);
                    }
                }
                else if (child->id.compare(parent->id) > 0) {
                    if (parent->scope > grand->scope) {
                        child = this->zag_zag(grand);
                    }
                    else if (parent->scope < grand->scope) {
                        child = this->zig_zag(grand);
                    }
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) > 0) {
                        child = this->zag_zag(grand);
                    } 
                    else if (parent->scope == grand->scope && parent->id.compare(grand->id) < 0) {
                        child = this->zig_zag(grand);
                    }
                }
            }
        }        
    }
}

bool SymbolTable::search(Node* root, string id, int scope) {
    if (root == nullptr) return false;
    else if (root->scope == scope && id.compare(root->id) == 0)
        return true;
    if (scope < root->scope)
        return this->search(root->left, id, scope);
    if (scope > root->scope)
        return this->search(root->right, id, scope);
    if (scope == root->scope) {
        if (id.compare(root->id) < 0)
            return this->search(root->left, id, scope);
        if (id.compare(root->id) > 0)
            return this->search(root->right, id, scope);
    }
    return false;
}

Node* SymbolTable::insertRecursive(Node* root, string id, string type, int scope, int order) {
    if (root == nullptr) return new Node(id, type, scope, order);
    if (scope < root->scope) {
        if (root->left == nullptr) {
            Node* newNode = new Node(id, type, scope, order);
            root->left = newNode;
            newNode->parent = root;
        }
        else root->left = insertRecursive(root->left, id, type, scope, order);
    }    
    else if (scope > root->scope) {
        if (root->right == nullptr) {
            Node* newNode = new Node(id, type, scope, order);
            root->right = newNode;
            newNode->parent = root;
        }
        else root->right = insertRecursive(root->right, id, type, scope, order);
    }
    else {
        if (id.compare(root->id) < 0) {
            if (root->left == nullptr) {
                Node* newNode = new Node(id, type, scope, order);
                root->left = newNode;
                newNode->parent = root;
            }
            else root->left = insertRecursive(root->left, id, type, scope, order);            
        }
        else if (id.compare(root->id) > 0) {
            if (root->right == nullptr) {
                Node* newNode = new Node(id, type, scope, order);
                root->right = newNode;
                newNode->parent = root;
            }
            else root->right = insertRecursive(root->right, id, type, scope, order);
        }
        else return root;
    }
    return root;
}

STATUS SymbolTable::insert(string id, string type, int scope, int order, int &num_comp, int &num_splay) {
    if (checkTypeOfValue(type) == FUNCTION && scope != 0)
        return INVALID_DECLARATION;
    if (type != "string" && type != "number" && checkTypeOfValue(type) != FUNCTION)
        return INVALID_INSTRUCTION;
    if (checkID(id) == false)
        return INVALID_INSTRUCTION;    
    if (this->search(this->root, id, scope) == true)
        return REDECLARED;
    
    this->root = this->insertRecursive(this->root, id, type, scope, order);
    Node* splay = this->findNode(this->root, id, scope, num_comp);
    this->size++;
    this->root = this->splayTree(this->root, splay);
    if (this->size == 1) {
        num_comp = 0;
        num_splay = 0;
    }    
    else {
        num_comp--;
        num_splay++;
    }
    return SUCCESS;
}

bool SymbolTable::findID(Node *&root, string id, int scope, int &num_comp, int &num_splay) {
    Node* found;
    while (scope >= 0) {
        found = this->findNode(root, id, scope, num_comp);
        if (found != nullptr) break;
        else scope--;
    }
    if (found == nullptr) return false;
    if (root->id == found->id && root->scope == found->scope && root->type == found->type) {
        num_splay = 0;
    }    
    else {
        root = this->splayTree(root, found);
        num_splay++;
    }
    return true;
}

STATUS SymbolTable::validate_function(Node *&root, string strA, string &returnedValue, int scope, int &num_comp, int &num_splay) {
    //strA là đầu vào; strB là value cần assign
    int pos1 = (int)strA.find("(");
    int pos2 = (int)strA.find(")");
    string value = strA.substr(0,pos1);
    if (this->findID(root, value, scope, num_comp, num_splay) == false) return UNDECLARED;
    strA = strA.substr(pos1 + 1, pos2 - (pos1 + 1)) + ",";     //strA có dạng para,para,para,
    string strB = root->type;
    pos1 = (int)strB.find(")");
    pos2 = (int)strB.find(">");
    returnedValue = strB.substr(pos2 + 1);
    strB = strB.substr(1, pos1 - 1) + ",";                //strB có dạng type,type,type,

    while(true) {
        pos1 = (int)strA.find(",");
        pos2 = (int)strB.find(","); 
        string para = strA.substr(0, pos1);
        string type = strB.substr(0, pos2);
        TYPE checkTypeOfPara = checkTypeOfValue(para);
        if (checkTypeOfPara == STRING && type != "string") return TYPE_MISMATCH;
        else if (checkTypeOfPara == NUMBER && type != "number") return TYPE_MISMATCH;
        else if (checkTypeOfPara == ID) {
            bool check = this->findID(root, para, scope, num_comp, num_splay);
            if (check == false) return UNDECLARED;
            else if (root->type != type) return TYPE_MISMATCH;
        }
        if (strA.substr(pos1) == "," && strB.substr(pos2) != ",")
            return TYPE_MISMATCH;
        else if (strA.substr(pos1) != "," && strB.substr(pos2) == ",")   
            return TYPE_MISMATCH;
        else if (strA.substr(pos1) == "," && strB.substr(pos2) == ",")
            break;      
        strA = strA.substr(pos1 + 1);
        strB = strB.substr(pos2 + 1);
    }
    return SUCCESS;
}

STATUS SymbolTable::assign(string id, string value, int scope, int &num_comp, int &num_splay) {
    if (checkID(id) == false)
        return INVALID_INSTRUCTION;
    if (checkTypeOfValue(value) == INVALID || checkTypeOfValue(value) == FUNCTION)
        return INVALID_INSTRUCTION;

    TYPE checkType = checkTypeOfValue(value);

    if (checkType == NUMBER || checkType == STRING) {
        if (this->findID(this->root, id, scope, num_comp, num_splay) == false) return UNDECLARED;
        Node* temp = this->root;
        if (temp->type == "number" && checkType == STRING) return TYPE_MISMATCH;
        else if (temp->type == "string" && checkType == NUMBER) return TYPE_MISMATCH;
    }

    else if (checkType == ID) {
        bool checkConsistionOfValue = this->findID(this->root, value, scope, num_comp, num_splay);
        if (checkConsistionOfValue == false) return UNDECLARED;
        Node* temp1 = this->root;
        bool checkConsistionOfID = this->findID(this->root, id, scope, num_comp, num_splay);
        if (checkConsistionOfID == false) return UNDECLARED;
        Node* temp2 = this->root;
        if (temp1->type != temp2->type) return TYPE_MISMATCH;
    }

    else if (checkType == FUNCTION_CALL) {
        string returnedValue = "";
        STATUS forFuncCalling = this->validate_function(this->root, value, returnedValue, scope, num_comp, num_splay);
        if (forFuncCalling == UNDECLARED) return UNDECLARED;
        else if (forFuncCalling == TYPE_MISMATCH) return TYPE_MISMATCH;
        if (this->findID(this->root, id, scope, num_comp, num_splay) == false) return UNDECLARED;
        if (root->type != returnedValue) return TYPE_MISMATCH;
    }
    return SUCCESS;
}

Node* SymbolTable::findOrder(Node* root, int scope, int order) {
    if (root == nullptr) return nullptr;  
    if (root->scope == scope && root->order == order) return root;
    Node* found = findOrder(root->left, scope, order);
    if (found != nullptr) return found;
	return findOrder(root->right, scope, order);
}

Node* SymbolTable::findMaxLeft(Node *root) {
    if (root->right == nullptr) return root;
    return findMaxLeft(root->right);
}

Node* SymbolTable::deleteMaxLeft(Node* root) {
	if (root->right == nullptr) {
        if (root->left == nullptr)
            return nullptr;
        else {
            Node* temp = root->left;
            temp->parent = root->parent;
            temp->parent->right = temp;
            return temp;
        }
    }    
	root->right = deleteMaxLeft(root->right);
	return root;
}

bool SymbolTable::remove(Node *&root, int scope, int order) {
    Node* del = findOrder(root, scope, order);
    if (del == nullptr) return false;
    root = splayTree(root, del);
    if (root->left == nullptr) {
        Node* newRoot = root->right;
        newRoot->parent = nullptr;
        root = newRoot;
        return true;
    }
    if (root->right == nullptr) {
        Node* newRoot = root->left;
        newRoot->parent = nullptr;
        root = newRoot;
        return true;
    }
    Node* maxLeft = findMaxLeft(root->left);
	if (maxLeft->parent->parent == nullptr) {
		maxLeft->right = root->right;
		maxLeft->right->parent = maxLeft;
        maxLeft->parent = nullptr;
		root = maxLeft;
		return true;
	}
    Node* temp = root->left;
    temp = deleteMaxLeft(temp);
    maxLeft->parent = nullptr;
    maxLeft->right = root->right;
	maxLeft->right->parent = maxLeft;
	maxLeft->left = temp;
	maxLeft->left->parent = maxLeft;
	root = maxLeft;
    return true;
}

STATUS SymbolTable::lookup(string id, int scope, int num_comp, int num_splay) {
    bool check = this->findID(this->root, id, scope, num_comp, num_splay);
    if (check == false) return UNDECLARED;
    return SUCCESS;
}

string SymbolTable::preOrderRecursive(Node* root) {
    if (root == nullptr) return "";
    string output = root->id + "//" + to_string(root->scope) +  " ";
    output += (preOrderRecursive(root->left) + preOrderRecursive(root->right));
    return output;
}

string SymbolTable::preOrder() {
    string output = preOrderRecursive(this->root);
    return output.erase(output.length() - 1);
}


