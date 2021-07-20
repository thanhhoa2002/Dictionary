#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Dictionary
{
    string word;
    string meaning;
};

struct Node
{
    Dictionary D;
    Node *left;
    Node *right;
};
typedef Node *Tree;

Node *createNode(Dictionary D)
{
    Node *p = new Node;
    p->D = D;
    p->left = NULL;
    p->right = NULL;
    return p;
}

void deleteTree(Tree &t)
{
    if (t)
    {
        deleteTree(t->left);
        deleteTree(t->right);
        delete t;
    }
}

void addNode(Tree &t, Node *node)
{
    if (t)
    {
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

Node *minValueNode(Node *node)
{
    Node *p = node;

    while (p && p->left != NULL)
        p = p->left;
    return p;
}
Node *deleteNode(Node *node, string s)
{
    if (node == NULL)
        return node;

    if (s < node->D.word)
        node->left = deleteNode(node->left, s);
    else if (s > node->D.word)
        node->right = deleteNode(node->right, s);
    else
    {
        if(node->left == NULL && node->right == NULL){
            delete node;
            return NULL;
        }
        else if (node->left == NULL){
            Node* p = node->right;
            delete node;
            return p;
        }
        else if (node->right == NULL){
            Node* p = node->left;
            delete node;
            return p;
        }

        Node* p = minValueNode(node->right);
        node->D = p->D;
        node->right = deleteNode(p, node->D.word);
    }    

    return node;
}

void processingUsage(Dictionary *D, int &size)
{
    for (int i = 0; i < size; ++i)
    {
        if (D[i].word == "Usage")
        {
            if (D[i].meaning[0] != 'n' || D[i].meaning[1] != '.')
            {
                D[i - 1].meaning += " " + D[i].word + ": " + D[i].meaning;
                D[i].word = D[size - 1].word;
                D[i].meaning = D[size - 1].meaning;
                --size;
            }
        }
    }
}
void delRepeat(Dictionary *D, int &size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            if (D[j].meaning == D[i].meaning)
            {
                D[j].word = D[size - 1].word;
                D[j].meaning = D[size - 1].meaning;
                --size;
                break;
            }
        }
    }
}
void load(Tree &t)
{
    fstream f;
    string s;
    int size(0);

    f.open("Oxford English Dictionary.txt", ios::in);
    while (!f.eof())
    {
        getline(f, s);
        if (s.size() > 3)
            ++size;
    }
    f.clear();
    f.seekg(0L, ios::beg);

    Dictionary *D = new Dictionary[size];
    for (int i = 0; i < size; ++i)
    {
        do
        {
            getline(f, s);
        } while (s.size() < 3);

        stringstream ss(s);
        string s1;

        getline(ss, D[i].word, ' ');
        getline(ss, s1, ' ');
        if (s1.size() != 0)
        {
            D[i].word += " " + s1;
            getline(ss, s1, ' ');
            if (s1.size() != 0)
            {
                D[i].word += " " + s1;
                getline(ss, s1, ' ');
            }
        }
        getline(ss, D[i].meaning, '\n');
    }
    f.close();
    delRepeat(D, size);
    processingUsage(D, size);

    for (int i = 0; i < size; ++i)
    {
        int temp = rand() % size + 0;
        swap(D[i], D[temp]);
    }

    for (int i = 0; i < size; ++i)
        addNode(t, createNode(D[i]));

    delete[] D;
}

Node *find(Tree t, string s)
{
    if (t)
    {
        if (t->D.word == s)
            return t;
        if (t->D.word > s)
            return find(t->left, s);
        else
            return find(t->right, s);
    }
    else
        return NULL;
}
void search(Tree t)
{
    Node *result;
    string key;

    cout << "Enter a word to translate meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(t, key);
    if (result == NULL)
        cout << "Can't find the word.\n";
    else
        cout << result->D.word + ": " + result->D.meaning + "\n";
}

void addNewWord(Tree &t)
{
    Dictionary D;

    cout << "Enter the word for adding: ";
    getline(cin, D.word);
    if (D.word[0] >= 'a' && D.word[0] <= 'z')
        D.word[0] -= 32;
    cout << "Enter the meaning of word: ";
    getline(cin, D.meaning);
    addNode(t, createNode(D));
    cout << "Successfully added...\n";
}

void editMeaning(Tree t)
{
    string key;
    Node *result;

    cout << "Enter a word to edit meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(t, key);
    if (result == NULL)
        cout << "Can't find the word.\n";
    else
    {
        cout << result->D.word + ": " + result->D.meaning + '\n';
        cout << "Enter new meaning for editing: ";
        getline(cin, key);
        result->D.meaning = key;
        cout << result->D.word + ": " + result->D.meaning + "\n";
    }
}

void delAWord(Tree& t)
{
    string key;
    Node* result;

    cout << "Enter a word to delete: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = deleteNode(t, key);
    if (result == NULL)
        cout << "Can't find the word.\n";
    else
        cout << "Successfully deleted...\n";
}

void outputTree(Tree t, fstream &f)
{
    if (t == NULL)
        return;

    outputTree(t->left, f);
    f << t->D.word + "  " + t->D.meaning + "\n\n";
    outputTree(t->right, f);
}
void save(Tree t)
{
    fstream f;
    string fn;

    cout << "Enter file name to save: ";
    getline(cin, fn);

    f.open(fn + ".txt", ios::out);
    outputTree(t, f);
}

void menu(Tree t){
    load(t);
    bool f = true;

    while (f)
    {
        system("cls");
        cout << "=== Dictionary ===\n";
        cout << "  = BST Struct =\n";
        cout << "1. Search\n";
        cout << "2. Add\n";
        cout << "3. Edit meaning\n";
        cout << "4. Delete\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";

        int temp;
        cin >> temp;
        cin.ignore();
        cout << endl;

        switch (temp)
        {
        case 0:
            f = false;
            break;
        case 1:
            search(t);
            break;
        case 2:
            addNewWord(t);
            break;
        case 3:
            editMeaning(t);
            break;
        case 4:
            delAWord(t);
            break;
        }
        system("pause");
    }
    save(t);
}

int main()
{
    srand(time(NULL));
    Tree t = NULL;

    menu(t);

    return 0;
}