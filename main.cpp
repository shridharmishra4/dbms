// Assignment 3: CS540 DBMS: B+ tree with linear indexing
// Date: 13/02/2021

// Authors:
// Shridhar Mishra( mishrash@oreognstate.edu, mishrash)
// Irene Tematelewo(temateli@oregonstate.edu, temateli)

// Usage:  g++ main.cpp -std=c++0x
// ./a.out
// Index first
// Lookup

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <math.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Employee {
    int ID;
    string Name;
    string Bio;
    int ManagerID;
};

struct Block {
    int index=0;
    vector<Employee> EmpBlock;
};

vector<Employee> AllEmployees;
map<int,Block *> HashMap;

int NextPointer = 0;
int RecordCounter = 0;
int Global_Index = 1;
int MaxI = 1;


void LoadCsv() {
    ifstream fin;
    fin.open("Employee.csv", ios::in);

    vector<Employee> Rows;
    string line, word, temp;

    while (!fin.eof()) {
        getline(fin, line);

        stringstream s(line);

        Employee employee;
        int count = 0;
        while (getline(s, word, ',')) {

            if(count == 0) {
                employee.ID = stoi(word);

                count++;
            } else if(count == 1) {
                employee.Name = word;
                count++;
            } else if(count == 2) {
                employee.Bio = word;
                count++;
            } else if(count == 3) {
                employee.ManagerID = stoi(word);
                count++;
            } else{
                return;
            }

        }

        Rows.push_back(employee);
    }

    AllEmployees = Rows;
    fin.close();
}

int GenerateHash(int ID, int i) {
    
    double div = pow(2, Global_Index);
    // cout<<"global index= "<<Global_Index<<"\n";
    // cout<<"divider prior= "<<div<<"\n";
    
    if(i!=0){
        div = pow(2, i);
    }
    int hash = ID % (int)div;

    if (hash < HashMap.size())
    {
        return hash;
        // cout<<"ID  and divider key = "<<ID <<" " <<div <<" "<<hash<<"\n";
    }
    else
    { 
        // Bit Flip
        hash = (int)hash ^ (1<<(Global_Index-1));
//        cout<<"ID  and divider key = "<<ID <<" " <<div <<" "<<hash<<"\n";
        return (hash);

    };

}

// unsigned int GenerateHash(int ID, int i) {

//         unsigned int mod = (1 << i);
//         return (unsigned int)(ID % mod + mod) % (HashMap.size()-1);
// };

void WriteIndexToFile() {
    ofstream EmpIndexFile;
    EmpIndexFile.open("EmployeeIndex.txt");

    for (int i = 0; i < HashMap.size() ; i++)
    {
        EmpIndexFile << i << "| ";
        for(int j= 0; j< HashMap[i]->EmpBlock.size(); j++) {
            EmpIndexFile << HashMap[i]->EmpBlock[j].ID << ',';
            EmpIndexFile << HashMap[i]->EmpBlock[j].ManagerID << ',';
            EmpIndexFile << HashMap[i]->EmpBlock[j].Name << ',';
            EmpIndexFile << HashMap[i]->EmpBlock[j].Bio << "||";
        }
        EmpIndexFile << '\n';
    }

    EmpIndexFile.close();
}


int DidOverflow() {
    int usage = (RecordCounter*100) / (HashMap.size()*5);


    return usage > 85;
}

void CreateIndex() {
    if(HashMap.size() == 0) {
        HashMap[0] = new Block;
        HashMap[1] = new Block;
    }

    for(int i=0;i<AllEmployees.size();i++) {
        RecordCounter = i+1;
        int key = GenerateHash(AllEmployees[i].ID, 0);

        if(!DidOverflow()) {
            Block* MatchedBLK = HashMap[key];
//            cout<<"-------EmpID "<<AllEmployees[i].ID<<"-----index num--------------    "<<HashMap[0]->index<<"\n";

            Employee TempEmpData = AllEmployees[i];

            if(MatchedBLK->index == 0) {
                MatchedBLK->index = Global_Index;
            }

            MatchedBLK->EmpBlock.push_back(TempEmpData);
        }

        else {
//            for(int i=0;i<HashMap.size();i++){
//                cout<<"index of "<< i <<" "<<HashMap[i]->index<<"\n";
//            }

            HashMap[HashMap.size()] = new Block;
//            cout<<"hashmap size "<<HashMap.size()<<endl;
//            cout<<"---------nextptr------  "<<NextPointer<<"  ----index-------    "<<HashMap[NextPointer]->index<<"\n";

            HashMap[NextPointer]->index = HashMap[NextPointer]->index + 1;
            MaxI = HashMap[NextPointer]->index;
            int hashResultAfter = GenerateHash(AllEmployees[i].ID, 0);
            HashMap[hashResultAfter]->EmpBlock.push_back(AllEmployees[i]);
            
            vector<Employee> temp_vector = HashMap[NextPointer]->EmpBlock;
            HashMap[NextPointer]->EmpBlock.erase(HashMap[NextPointer]->EmpBlock.begin(),HashMap[NextPointer]->EmpBlock.end());
            
            //copy form old bukcket to new
            for (int i = 0; i < temp_vector.size() ; i++){
                int hashResult = GenerateHash(temp_vector[i].ID,0);
                HashMap[hashResult]->EmpBlock.push_back(temp_vector[i]);
                HashMap[hashResult]->index = 0;
            }
            NextPointer++;

//            cout<<"-------------after while ptr->index = -------------    "<<HashMap[NextPointer]->index<<"\n";

            int cmp = (int)(pow(2, Global_Index));

            if(NextPointer == cmp) {
                NextPointer = 0;
                Global_Index++;
            }
//            cout<<"-------------Inc global index = -------------    "<<Global_Index<<"\n";

        }
    }
    WriteIndexToFile();
}


bool LookupData(int index) {
    int hash = GenerateHash(index, 0);

    if(HashMap[hash]){
        vector<Employee> EmployeeList = HashMap[hash]->EmpBlock;
        for(int i = 0; i< EmployeeList.size(); i++) {
            if(EmployeeList[i].ID == index) {
                cout << "Lookup result: "<< EmployeeList[i].ID <<endl;
                cout << EmployeeList[i].Name <<  endl;
                cout << EmployeeList[i].Bio<<  endl;
                cout << EmployeeList[i].ManagerID<<endl;
                return true;
            }
        }

    }
    return false;
}



int main() {
    char input;

    while (true)
    {
        string inputStr;
        int flag = 0;
        cout << "Enter command l:fetch record, c:create index s: show hashmap, e:exit"
             << "\n";
        cin >> input;
        LoadCsv();

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
                LookupData(stoi(inputStr));
                break;
            case 'E':
            case 'e':
                flag = 1;
                break;
            case 's':
                cout << "\n  RESULT: " << "\n";

                cout << "block size: " << HashMap.size() << '\n';

                cout << "Global index: " << Global_Index << '\n';

                for(int i = 0; i< HashMap.size(); i++) {
                    for(int j= 0; j< HashMap[i]->EmpBlock.size(); j++) {
                        cout << "result: " << i << ": " << HashMap[i]->EmpBlock[j].ID << '\n';
                    }
                }

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