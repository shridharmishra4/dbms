#include <string>
#include <ostream>
#include <list>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;
unordered_map<string, vector<string> > IndexMap;

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

        void writetoFile()
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
                    for (int i = 0; i < IndexMap[c].size(); i++)
                    {
                        cout << " ";
                        cout << IndexMap[c][i];
                        inputFile << " " << IndexMap[c][i];
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
    for (int i = 0; i < IndexMap[c].size(); i++)
    {
        cout << IndexMap[c][i] << " ";
    }
    cout << "\n";
}

void CreateIndex()
{
    std::ifstream data("Employees.csv");
    std::string line;
    std::vector<std::vector<std::string> > parseCSV;
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
        parseCSV.push_back(row_parse);
    }
    Hash half(count / 2);
    for (auto row : parseCSV)
    {
        string id;
        for (auto col : row)
        {
            id = col;
            IndexMap[id] = row;
            int i = stoi(id);
            half.insertItem(i);
            break;
        }
    }

    half.writetoFile();
}

int main()
{
    char input;

    while (true)
    {
        string inputStr;
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
            cin >> inputStr;
            retrieveRecord(inputStr);
            break;
        case 'E':
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