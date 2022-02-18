#include <iostream>
#include <string>
#include <regex>

using namespace std;

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

bool isFunctionCalling(string value) {
    regex regexp("[a-z][a-zA-Z0-9_]*\\((([0-9]+,|'[a-zA-Z0-9 ]*',|[a-z][a-zA-Z0-9_]*,)*([0-9]+|'[a-zA-Z0-9 ]*'|[a-z][a-zA-Z0-9_]*)|())\\)");
    if (regex_match(value, regexp)) return true;
    return false;
}

int main() {
/*     string str = "ASSIGN x sum('abc ab',12)";
    string keyword, id, value, staticc;
    tokenize(str, keyword, id, value, staticc);
    cout << keyword << endl;
    cout << id << endl;
    cout << value << endl;
    cout << staticc << endl; */
    string value = "sum(x1,2,'abc cb')";
    if (isFunctionCalling(value)) cout << "true" << endl;
    else cout << "false\n";
    return 0;
}