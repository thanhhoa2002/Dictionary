// 20127163
// TRAN THANH HOA

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

#define TBS 10000

struct Dictionary
{
    string word;
    string meaning;
};

struct Node
{
    Dictionary D;
    Node *next;
};
typedef Node *HashTB[TBS];

void initHashTB(HashTB &H)
{
    for (int i = 0; i < TBS; ++i)
        H[i] = NULL;
}

int hashStr(string s)
{
    long sum = 0, mul = 1;
    for (int i = 0; i < s.length(); i++)
    {
        mul = (i % 4 == 0) ? 1 : mul * 256;
        sum += s.at(i) * mul;
    }
    return (int)(abs(sum) % TBS);
}

void insertNode(HashTB &H, Dictionary D)
{
    int k = hashStr(D.word);

    Node *p = new Node{D, NULL};
    if (H[k] == NULL)
    {
        H[k] = p;
    }
    else
    {
        Node *q = H[k];
        while (q->next != NULL)
            q = q->next;
        q->next = p;
    }
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
void load(HashTB &H)
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
        insertNode(H, D[i]);
    }

    delete[] D;
}

Node *find(HashTB H, string s)
{
    int k = hashStr(s);
    Node *p = H[k];
    while (p != NULL && p->D.word != s)
        p = p->next;
    if (p == NULL)
        return NULL;
    return p;
}
void search(HashTB H)
{
    Node *result;
    string key;

    cout << "Enter a word to translate meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(H, key);
    if (result == NULL)
        cout << "Can't find the word.\n";
    else
        cout << result->D.word + ": " + result->D.meaning + "\n";
}

void addNewWord(HashTB &H)
{
    Dictionary D;

    cout << "Enter the word for adding: ";
    getline(cin, D.word);
    if (D.word[0] >= 'a' && D.word[0] <= 'z')
        D.word[0] -= 32;
    cout << "Enter the meaning of word: ";
    getline(cin, D.meaning);
    insertNode(H, D);
    cout << "Successfully added...\n";
}

void editMeaning(HashTB &H)
{
    string key;
    Node *result;

    cout << "Enter a word to edit meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(H, key);
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

void delAWord(HashTB &H)
{
    string key;

    cout << "Enter a word to delete: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    int k = hashStr(key);
    Node *p = H[k];
    Node *q = p;

    while (p != NULL && p->D.word != key)
    {
        q = p;
        p = p->next;
    }
    if (p == NULL)
    {
        cout << "Can't find the word.\n";
        return;
    }
    else if (p == H[k])
    {
        H[k] = H[k]->next;
        delete p;
    }
    else
    {
        q->next = p->next;
        delete p;
    }
    cout << "Successfully deleted...\n";
}

void save(HashTB H)
{
    fstream f;
    string fn;

    cout << "Enter file name to save: ";
    getline(cin, fn);

    f.open(fn + ".txt", ios::out);
    for (int i = 0; i < TBS; ++i)
    {
        Node *p = H[i];
        while (p != NULL)
        {
            f << p->D.word + "  " + p->D.meaning + "\n";
            p = p->next;
        }
    }
}

void menu(HashTB &H)
{
    load(H);
    bool f = true;

    while (f)
    {
        system("cls");
        cout << "=== Dictionary ===\n";
        cout << "  = Hash Table =\n";
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
            search(H);
            break;
        case 2:
            addNewWord(H);
            break;
        case 3:
            editMeaning(H);
            break;
        case 4:
            delAWord(H);
            break;
        }
        system("pause");
    }
    save(H);
}

int main()
{
    HashTB H;

    initHashTB(H);

    menu(H);

    return 0;
}