#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>

const int right_overweight = 2;
const int left_overweight = -2;
const int indent = 0;

typedef struct Node {
    int key;
    unsigned char height;
    Node* left;
    Node* right;

    Node(int k) {
        key = k;
        left = right = 0;
        height = 1;
    }
}node;

unsigned char height(Node* p) {
    return p ? p->height : 0;
}

int bFactor(Node* p) {
    return height(p->right) - height(p->left);
}

void fixHeight(Node* p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

Node* rotateRight(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

Node* rotateLeft(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeight(q);
    fixHeight(p);
    return p;
}

Node* balance(Node* p) {
    fixHeight(p);
    if (bFactor(p) == right_overweight) {
        if (bFactor(p->right) < 0)
            p->right = rotateRight(p->right);
        return rotateLeft(p);
    }
    if (bFactor(p) == left_overweight) {
        if (bFactor(p->left) > 0)
            p->left = rotateLeft(p->left);
        return rotateRight(p);
    }
    return p; 
}

node* findmin(node* p) {
    return p->left ? findmin(p->left) : p;
}

node* removemin(node* p) 
{
    if (p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

Node* insert(Node* p, int k) {
    if (!p) return new Node(k);
    if (k < p->key)
        p->left = insert(p->left, k);
    else
        p->right = insert(p->right, k);
    return balance(p);
}

void printAVLTree(Node* p, int maxHeight) {

    for (int i = 0; i < maxHeight; ++i) {
        std::cout << ' ';
    }
    std::cout << p->key;
    std::cout << "\n";
    if (p->right) {
        printAVLTree(p->right, maxHeight + 1);
    }
    if (p->left) {
        printAVLTree(p->left, maxHeight + 1);
    }

}

void deleteTree(Node* p) {
    if (p->left) {
        deleteTree(p->left);
    }
    if (p->right) {
        deleteTree(p->right);
    }
    delete p;
}

bool checkExpression(std::string& name, int& my_length) {
    int length = name.length();
    for (int j = 0; j < length; ++j) {
        if (!isdigit(name[j]) && name[j] != ',') {
            return false;
        }
        if (name[j] == ',') {
            name[j] = ' ';
            (my_length)++;
        }
    }

    return true;
}


node* mainCheck(std::string name) {

    int my_lenght = 0;
    if (checkExpression(name, my_lenght)) {
        std::istringstream iss;
        std::string strvalues = name;
        iss.str(strvalues);
        int now;
        iss >> now;
        node* myTree = new Node(now);
        for (int i = 0; i < my_lenght; i++) {
            int val;
            iss >> val;
            myTree = insert(myTree, val);
        }
        std::cout << '\n';
        myTree = balance(myTree);
        return myTree;
    }
    else {
        std::cout << "Unwanted characters\n";
    }
}

std::string generateData() {
    srand(time(0));
    int size = rand() % 5 + 10;
    std::string res = "";
    for (int i = 0; i < size; i++) {
        int elem = rand() % 19 + 1;
        if (elem > 9) {
            res += char(elem / 10 + 48);
            res += char(elem % 10 + 48);
        }
        else {
            res += char(elem + 48);
        }
        if (i != size - 1) res += ",";
    }
    return res;
}

node* Adding(node* p, int k) {
    if (!p) return new Node(k);
    if (k < p->key)
        p->left = Adding(p->left, k);
    else
        p->right = Adding(p->right, k);
    return p;
}

int seeAdd(node* tree, node* treeptr) {
    srand(time(0));
    int added = rand() % 19 + 1;
    std::cout << "We need to add " << added << " into the tree.";
    std::cout << "Firstly, we need to find place like a normal binary tree.\n" <<
        "If elem larger than root key, we go to right branch, else to left.\n" <<
        "We do it until we reach end of tree.";
    std::cout << "Result of this action is:\n";
    Adding(tree, added);
    printAVLTree(tree, 0);
    std::cout << "Now we need to verify balance factor on main root and on the branch\n"<<
        "Balance factor is difference beetwen left hieght and right height.\n"<<
        "If absolute value of balance factor is 2 we need to balance tree.\n"<<
        "Result is: \n";
    insert(treeptr, added);
    printAVLTree(treeptr, 0);
    return added;
}

node* remove(node* p, int k) // удаление ключа k из дерева p
{
    if (!p) return 0;
    if (k < p->key)
        p->left = remove(p->left, k);
    else if (k > p->key)
        p->right = remove(p->right, k);
    else //  k == p->key 
    {
        node* q = p->left;
        node* r = p->right;
        delete p;
        if (!r) return q;
        node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

void seeRemove(node* tree, int to_remove) {
    std::cout << "Now let remove " << to_remove << " from tree.\n" << "Once the key is found: remember the roots the left and right subtrees of the node p; delete the node p;\n if the right subtree is empty, then return the pointer to the left subtree;\n if the right subtree is not empty, then we find the minimum element \"min\" there, then we extract it from there, hang q from the left to min, and what turned out from r to the right, return min after balancing it.\n\n";
    remove(tree, to_remove);
    printAVLTree(tree, indent);
}

int main() {

    int your_choose = 0;
    node* myTree;
    node* tree_ptr;
    std::cout << "Source data \n";
    std::string str;
    str = generateData();
    std::cout << str << '\n';
    std::cout << "Source tree:";
    myTree = mainCheck(str);
    tree_ptr = mainCheck(str);
    printAVLTree(myTree, indent);
    int elem = seeAdd(myTree, tree_ptr);
    seeRemove(myTree, elem);
    deleteTree(myTree);
    deleteTree(tree_ptr);
    return 0;

}
