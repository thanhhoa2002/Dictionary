#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

struct Dictionary{
    string word;
    string meaning;
};

struct Node{
    Dictionary D;
    Node* left;
    Node* right;
};
typedef Node* Tree;

Node* createNode(Dictionary D){
    Node* p = new Node;
    p->D = D;
    p->left = NULL;
    p->right = NULL;
    return p;
}

void initTree(Tree& t){
    t = NULL;
}

void deleteTree(Tree& t){
    if (t){
        deleteTree(t->left);
        deleteTree(t->right);
        delete t;
    }
}

void addNode(Tree& t, Node* node){
    if (t){
        if (t->D.word == node->D.word)
            return;
        if (t->D.word > node->D.word)
            addNode(t->left, node);
        else
            addNode(t->right, node);
    }    
    else
        t = node;
}

int main(){
    
    return 0;
}