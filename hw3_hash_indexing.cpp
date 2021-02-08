#include <iostream>
#include <list>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <ostream>
#include <unordered_map>
using namespace std;
unordered_map<string, vector<string> > umap;

class Hash
{
    int Container;

    list<int> *table;

    public:
        Hash(int V);

        int hashFunction(int x)
        {
            return (x % Container);
        }

        void insertItem(int key)
        {
        int index = hashFunction(key);
        table[index].push_back(key);
        }

        void displayHash()
        {
            ofstream inputFile;
            inputFile.open("EmployeeIndex.txt");
            for (int i = 0; i < Container; i++)
            {
                cout << i;
                inputFile << i;
                for (auto x : table[i])
                {
                    cout << " --> " << x;
                    inputFile << " --> " << x;
                    string c = to_string(x);
                    for (int i = 0; i < umap[c].size(); i++)
                    {
                        cout << " ";
                        cout << umap[c][i];
                        inputFile << " " << umap[c][i];
                    }
                }
                inputFile << "\n";

                cout << endl;
            }
            inputFile.close();
        }
};

Hash::Hash(int b)
{
    this->Container = b;
    table = new list<int>[Container];
}


void retrieveRecord(string c)
{
    for (int i = 0; i < umap[c].size(); i++)
    {
        cout << umap[c][i] << " ";
    }
    cout << "\n";
}

void CreateIndex()
{
    std::ifstream data("Employee.csv");
    std::string line;
    std::vector<std::vector<std::string> > ParseCSV;
    unordered_map<string, vector<std::string> >::iterator itr;
    int count = 0;
    while (std::getline(data, line))
    {
        std::stringstream LStream(line);
        std::string cell;
        std::vector<std::string> row_parse;
        while (std::getline(LStream, cell, ','))
        {
            row_parse.push_back(cell);
        }
        count++;
        ParseCSV.push_back(row_parse);
    }
    Hash h(count / 2);
    for (auto row : ParseCSV)
    {
        string id;
        for (auto col : row)
        {
            id = col;
            umap[id] = row;
            int i = stoi(id);
            h.insertItem(i);
            break;
        }
    }

    h.displayHash();
}

int main()
{
    char input;

    while (true)
    {
        string c;
        int flag = 0;
        cout << "Enter command l:fetch record, c:create index , e:exit"
             << "\n";
        cin >> input;
        switch (input)
        {
        case 'c':
        case 'C':
            CreateIndex();
            cout << "Index Generated!"
                 << "\n";
            break;
        case 'l':
        case 'L':
            cout << "Enter employee ID"
                 << "\n";
            cin >> c;
            retrieveRecord(c);
            break;
        case 'e':
            flag = 1;
            break;
        default:
            break;
        }
        if (flag == 1)
        {
            break;
        }
    }

    return 0;
}