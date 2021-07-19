#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
    int height;
};

int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

Node *createNode(Dictionary D)
{
    Node *p = new Node;
    p->D = D;
    p->left = NULL;
    p->right = NULL;
    p->height = 1;
    return p;
}

Node *rightRotate(Node *N)
{
    Node *Nl = N->left;
    Node *Nlr = Nl->right;

    Nl->right = N;
    N->left = Nlr;

    N->height = max(height(N->left), height(N->right)) + 1;
    Nl->height = max(height(Nl->left), height(Nl->right)) + 1;

    return Nl;
}

Node *leftRotate(Node *N)
{
    Node *Nr = N->right;
    Node *Nrl = Nr->left;

    Nr->left = N;
    N->right = Nrl;

    N->height = max(height(N->left), height(N->right)) + 1;
    Nr->height = max(height(Nr->left), height(Nr->right)) + 1;

    return Nr;
}

int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return (height(N->left) - height(N->right));
}

Node *insertNode(Node *N, Dictionary D)
{
    if (N == NULL)
        return createNode(D);

    if (D.word < N->D.word)
        N->left = insertNode(N->left, D);
    else if (D.word > N->D.word)
        N->right = insertNode(N->right, D);
    else
        return N;

    N->height = max(height(N->left), height(N->right)) + 1;

    int balance = getBalance(N);

    // LL & RR
    if (balance > 1 && D.word < N->D.word)
        return rightRotate(N);
    if (balance < -1 && D.word > N->D.word)
        return leftRotate(N);

    // LR & RL
    if (balance > 1 && D.word > N->D.word)
    {
        N->left = leftRotate(N->left);
        return rightRotate(N);
    }
    if (balance < -1 && D.word < N->D.word)
    {
        N->right = rightRotate(N->right);
        return leftRotate(N);
    }

    return N;
}

Node *minValueNode(Node *N)
{
    Node *p = N;

    while (p->left != NULL)
        p = p->left;
    return p;
}
Node *deleteNode(Node *N, string s)
{
    if (N == NULL)
        return N;

    if (s < N->D.word)
        N->left = deleteNode(N->left, s);
    else if (s > N->D.word)
        N->right = deleteNode(N->right, s);
    else
    {
        if (N->left == NULL || N->right == NULL)
        {
            Node *p = N->left ? N->left : N->right;
            if (p == NULL)
            {
                p = N;
                N = NULL;
            }
            else
                N->D = p->D;
            delete p;
        }
        else
        {
            Node *p = minValueNode(N->right);
            N->D = p->D;
            N->right = deleteNode(N->right, p->D.word);
        }
    }

    if (N == NULL)
        return N;

    N->height = max(height(N->left), height(N->right)) + 1;

    int balance = getBalance(N);

    if (balance > 1 && getBalance(N->left) >= 0)
        return rightRotate(N);
    if (balance < -1 && getBalance(N->right) <= 0)
        return leftRotate(N);

    if (balance > 1 && getBalance(N->left) < 0)
    {
        N->left = leftRotate(N->left);
        N = rightRotate(N);
    }
    if (balance < -1 && getBalance(N->right) > 0)
    {
        N->right = rightRotate(N->right);
        N = leftRotate(N);
    }

    return N;
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
void quickSort(Dictionary *D, int l, int r)
{
    if (l > r)
        return;

    string s = D[(l + r) / 2].word;

    int i = l;
    int j = r;

    while (i <= j)
    {
        while (D[i].word < s)
            ++i;
        while (D[j].word > s)
            --j;

        if (i <= j)
        {
            swap(D[i], D[j]);
            ++i;
            --j;
        }
    }

    if (l < j)
        quickSort(D, l, j);
    if (r > i)
        quickSort(D, i, r);
}
void load(Node *&N)
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
        N = insertNode(N, D[i]);

    delete[] D;
}

Node *find(Node *N, string s)
{
    if (N == NULL)
        return NULL;

    if (N->D.word == s)
        return N;

    if (N->D.word > s)
        return find(N->left, s);
    return find(N->right, s);
}
void search(Node *N)
{
    string key;

    cout << "Enter a word to translate meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    Node *p = find(N, key);
    if (p != NULL)
        cout << p->D.word + ": " + p->D.meaning + "\n";
    else
        cout << "Can't find the word.\n";
}

void addNewWord(Node *N)
{
    Dictionary D;

    cout << "Enter the word for adding: ";
    getline(cin, D.word);
    if (D.word[0] >= 'a' && D.word[0] <= 'z')
        D.word[0] -= 32;
    cout << "Enter the meaning of word: ";
    getline(cin, D.meaning);

    N = insertNode(N, D);
    cout << "Successfully added...\n";
}

void delAWord(Node* N){
    string key;

    cout << "Enter a word to delete: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    N = deleteNode(N, key);

    cout << "Successfully deleted...\n";
}

void editMeaning(Node *N)
{
    string key;
    Node *result;

    cout << "Enter a word to edit meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(N, key);
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

void outputTree(Node *N, fstream &f)
{
    if (N == NULL)
        return;

    outputTree(N->left, f);
    f << N->D.word + "  " + N->D.meaning + "\n\n";
    outputTree(N->right, f);
}
void save(Node* N)
{
    fstream f;
    string fn;

    cout << "Enter file name to save: ";
    getline(cin, fn);

    f.open(fn + ".txt", ios::out);
    outputTree(N, f);
}

void menu(Node* N){
    load(N);
    bool f = true;

    while (f)
    {
        system("cls");
        cout << "=== Dictionary ===\n";
        cout << "   = AVL Tree =\n";
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
            search(N);
            break;
        case 2:
            addNewWord(N);
            break;
        case 3:
            editMeaning(N);
            break;
        case 4:
            delAWord(N);
            break;
        }
        system("pause");
    }
    save(N);
}

int main()
{
    Node *N = NULL;
    
    menu(N);

    return 0;
}
