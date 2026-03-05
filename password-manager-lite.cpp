#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Entry 
{
    string site;
    string login;
    string password;
};

string generatePassword(int length) 
{
    const string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*()-_=+[]{};:,.<>?";

    string result;
    result.reserve(static_cast<size_t>(length));

    for (int i = 0; i < length; ++i) 
    {
        result += chars[rand() % chars.size()];
    }

    return result;
}

void saveToFile(const vector<Entry>& entries, const string& filename) 
{
    ofstream file(filename);
    if (!file) {
        cout << "Error: cannot open file for writing.\n";
        return;
    }

    for (const auto& e : entries) 
    {
        file << e.site << " " << e.login << " " << e.password << "\n";
    }
}

void loadFromFile(vector<Entry>& entries, const string& filename) 
{
    ifstream file(filename);
    if (!file) 
    {
        return;
    }

    Entry e;
    while (file >> e.site >> e.login >> e.password) 
    {
        entries.push_back(e);
    }
}

void addEntry(vector<Entry>& entries) 
{
    Entry e;
    int length;

    cout << "Enter site: ";
    cin >> e.site;

    cout << "Enter login: ";
    cin >> e.login;

    cout << "Enter password length to generate: ";
    cin >> length;

    if (!cin || length <= 0) 
    {
        cout << "Invalid length.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    e.password = generatePassword(length);
    entries.push_back(e);

    cout << "Password generated and entry added:\n";
    cout << "Site: " << e.site << ", Login: " << e.login
        << ", Password: " << e.password << "\n";
}

void showAll(const vector<Entry>& entries) 
{
    if (entries.empty()) 
    {
        cout << "No entries.\n";
        return;
    }

    cout << "Saved entries:\n";
    for (size_t i = 0; i < entries.size(); ++i) 
    {
        cout << i + 1 << ") "
            << "Site: " << entries[i].site
            << ", Login: " << entries[i].login
            << ", Password: " << entries[i].password << "\n";
    }
}

void findBySite(const vector<Entry>& entries) 
{
    if (entries.empty()) 
    {
        cout << "No entries.\n";
        return;
    }

    string site;
    cout << "Enter site to search: ";
    cin >> site;

    bool found = false;
    for (const auto& e : entries) 
    {
        if (e.site == site) 
        {
            cout << "Found: "
                << "Site: " << e.site
                << ", Login: " << e.login
                << ", Password: " << e.password << "\n";
            found = true;
        }
    }

    if (!found) 
    {
        cout << "No entries found for site: " << site << "\n";
    }
}

void deleteBySite(vector<Entry>& entries) 
{
    if (entries.empty()) 
    {
        cout << "No entries.\n";
        return;
    }

    string site;
    cout << "Enter site to delete: ";
    cin >> site;

    bool found = false;
    for (auto it = entries.begin(); it != entries.end(); ) 
    {
        if (it->site == site) 
        {
            it = entries.erase(it);
            found = true;
        }
        else 
        {
            ++it;
        }
    }

    if (found) 
    {
        cout << "Entries for site \"" << site << "\" deleted.\n";
    }
    else 
    {
        cout << "No entries found for site: " << site << "\n";
    }
}

void printMenu() 
{
    cout << "\nPassword Manager Lite\n";
    cout << "1) Add entry\n";
    cout << "2) Show all entries\n";
    cout << "3) Find by site\n";
    cout << "4) Delete by site\n";
    cout << "5) Save and exit\n";
    cout << "Choose option: ";
}

int main() 
{
    srand(static_cast<unsigned int>(time(nullptr)));

    const string filename = "passwords.txt";
    vector<Entry> entries;

    loadFromFile(entries, filename);

    while (true) 
    {
        printMenu();

        int choice;
        cin >> choice;

        if (!cin) 
        {
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) 
        {
        case 1:
            addEntry(entries);
            break;
        case 2:
            showAll(entries);
            break;
        case 3:
            findBySite(entries);
            break;
        case 4:
            deleteBySite(entries);
            break;
        case 5:
            saveToFile(entries, filename);
            cout << "Data saved. Exiting...\n";
            return 0;
        default:
            cout << "Unknown option.\n";
            break;
        }
    }

    return 0;
}