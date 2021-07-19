// 20127163
// TRAN THANH HOA

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

struct Dictionary
{
    string word;
    string meaning;
};

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

void load(Dictionary *&D, int &size)
{
    fstream f;
    string s;
    f.open("Oxford English Dictionary.txt", ios::in);
    while (!f.eof())
    {
        getline(f, s);
        if (s.size() > 3)
            ++size;
    }
    f.clear();
    f.seekg(0L, ios::beg);

    D = new Dictionary[size];
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
    quickSort(D, 0, size - 1);
}

int binarySearch(string key, Dictionary *D, int size)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = (left + right) / 2;

        if (key == D[mid].word)
            return mid;

        if (key < D[mid].word)
            right = mid - 1;

        else
            left = mid + 1;
    }
    return -1;
}

void search(Dictionary *D, int size)
{
    string key;
    int keyValue;

    cout << "Enter a word to translate meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;
    keyValue = binarySearch(key, D, size);
    if (keyValue != -1)
        cout << D[keyValue].word + ": " + D[keyValue].meaning + "\n";
    else
        cout << "Can't find the word.\n";
}

void addNewWord(Dictionary *&D, int &size)
{
    Dictionary *d;
    int newSize;

    d = new Dictionary[size + 1];
    for (int i = 0; i < size; ++i)
    {
        d[i].word = D[i].word;
        d[i].meaning = D[i].meaning;
    }
    delete[] D;
    D = d;

    cout << "Enter the word for adding: ";
    getline(cin, D[size].word);
    if (D[size].word[0] >= 'a' && D[size].word[0] <= 'z')
        D[size].word[0] -= 32;
    cout << "Enter the meaning of word: ";
    getline(cin, D[size].meaning);
    ++size;
    quickSort(D, 0, size - 1);
    cout << "Successfully added...\n";
}

void editMeaning(Dictionary *D, int size)
{
    string key;
    int keyValue;

    cout << "Enter a word to edit meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    keyValue = binarySearch(key, D, size);
    if (keyValue != 1)
    {
        cout << D[keyValue].word + ": " + D[keyValue].meaning + '\n';
        cout << "Enter new meaning for editing: ";
        getline(cin, key);
        D[keyValue].meaning = key;
        cout << D[keyValue].word + ": " + D[keyValue].meaning + "\n";
    }
    else
        cout << "Can't find the word.\n";
}

void delAWord(Dictionary *D, int &size)
{
    string key;
    int keyValue;

    cout << "Enter a word to delete: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    keyValue = binarySearch(key, D, size);
    if (keyValue != 1)
    {
        D[keyValue].word = D[size - 1].word;
        D[keyValue].meaning = D[size - 1].meaning;
        --size;
        quickSort(D, 0, size - 1);
        cout << "Successfully deleted...\n";
    }
    else
        cout << "Can't find the word.\n";
}

void save(Dictionary *D, int size)
{
    fstream f;
    string fn;

    cout << "Enter file name to save: ";
    getline(cin, fn);

    f.open(fn + ".txt", ios::out);
    for (int i = 0; i < size; ++i)
        f << D[i].word + "  " + D[i].meaning + "\n";
    f.close();
}

void menu(Dictionary *&D, int &size)
{
    load(D, size);
    bool f = true;

    while (f)
    {
        system("cls");
        cout << "=== Dictionary ===\n";
        cout << "    = Arrays =\n";
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
            search(D, size);
            break;
        case 2:
            addNewWord(D, size);
            break;
        case 3:
            editMeaning(D, size);
            break;
        case 4:
            delAWord(D, size);
            break;
        }
        system("pause");
    }
    save(D, size);
}

int main()
{
    Dictionary *D;
    int size(0);

    menu(D, size);

    return 0;
}