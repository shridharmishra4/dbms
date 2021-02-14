#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <math.h>
#include <iterator> 

using namespace std;

struct Employee {
    int id;
    string name;
    string bio;
    int managerId;
};

struct Block {
    int index=0;
    vector<Employee> employeeBlock;
};

vector<Employee> employeedata; 
map<int,Block *> HashMap;
int globalIndex = 1;
int maxIndex = 1;
int nextpt = 0;
int recordCount = 0;

int getid;
int getmanagerid;
string getname;
string getbio;

void LoadCsv() {
    ifstream fin;
    fin.open("Employees.csv", ios::in);

    vector<Employee> row; 
    string line, word, temp; 
    
    while (!fin.eof()) {
        getline(fin, line); 

        stringstream s(line);

        Employee employee;
        int count = 0;
        while (getline(s, word, ',')) {

            if(count == 0) {
                employee.id = stoi(word);
//                cout << word <<"\n";

                count++;
            } else if(count == 1) {
                employee.name = word;
//                cout << word <<"\n";
                count++;
            } else if(count == 2) {
                employee.bio = word;
                count++;
            } else if(count == 3) {
                employee.managerId = stoi(word);
                count++;
            } else{
                return;
            }

        }

        row.push_back(employee);
    }
    
    employeedata = row;
    fin.close();
}

int hashingFunction(int id, int i) {


    double div = pow(2, globalIndex);
    // cout<<"global index= "<<globalIndex<<"\n";
    // cout<<"divider prior= "<<div<<"\n";



    if(i!=0){
        cout<<"inside nasty loop i= "<<i<<"\n";
        div = pow(2, i);
    }
    cout<<"Id  and divider key = "<<id <<" " <<div <<" "<<id % (int)div<<"\n";
    int hash = id % (int)div;

    if (hash < HashMap.size())
        {
        return hash;
        }
    else
        {
            return ((int)hash ^ (1<<globalIndex-1));

    };

    // return id % (int)div;
}

// unsigned int hashingFunction(int id, int i) {
        
//         unsigned int mod = (1 << i);
//         return (unsigned int)(id % mod + mod) % (HashMap.size()-1);
// };



int isOverflow() {
    int usage = (recordCount*100) / (HashMap.size()*5);
//     cout<<usage<<"\n";
// //    cout<< HashMap.size()<<"\n";
// //    cout<<recordCount<<"\n";
//     cout<<"-----------------\n";

    return usage > 85;
}

void printOutput() {
    ofstream outFile;
    outFile.open("EmployeeIndex.txt");
    
    for (int i = 0; i < HashMap.size() ; i++)
    {
        outFile << i << "| ";
        for(int j= 0; j< HashMap[i]->employeeBlock.size(); j++) {
            outFile << HashMap[i]->employeeBlock[j].id << ',';
            outFile << HashMap[i]->employeeBlock[j].managerId << ',';
            outFile << HashMap[i]->employeeBlock[j].name << ',';
            outFile << HashMap[i]->employeeBlock[j].bio << "||";
        }
        outFile << '\n';
    }

    outFile.close();
}

void writeBackToEmpTable(Employee emp){
    ofstream outFile;
    outFile.open("Employee.csv");
    for (int i = 0; i < HashMap.size() ; i++)
    // for (int i = 0; i < 18 ; i++)

    {
        for(int j= 0; j< HashMap[i]->employeeBlock.size(); j++) {
            outFile << HashMap[i]->employeeBlock[j].id << ',';
            outFile << HashMap[i]->employeeBlock[j].managerId << ',';
            outFile << HashMap[i]->employeeBlock[j].name << ',';
            outFile << HashMap[i]->employeeBlock[j].bio;
            outFile << '\n';
        }
    }
    outFile << emp.id << ',';
    outFile << emp.managerId << ',';
    outFile << emp.name << ',';
    outFile << emp.bio;
    outFile.close();
}

void insertByCommand() {
    recordCount = recordCount+1;
    int key = hashingFunction(getid, 0);
    Employee empTmp;
    empTmp.id = getid;
    empTmp.name = getname;
    empTmp.managerId = getmanagerid;
    empTmp.bio = getbio;

    writeBackToEmpTable(empTmp);
    
    if(!isOverflow()) {
        Block* matchedBlock = HashMap[key];
        
        if(matchedBlock->index == 0) {
            matchedBlock->index = globalIndex;
        }

        matchedBlock->employeeBlock.push_back(empTmp);
    } else {
        HashMap[HashMap.size()] = new Block;

        HashMap[nextpt]->index = HashMap[nextpt]->index + 1;
        maxIndex = HashMap[nextpt]->index;
        int firstId = HashMap[nextpt]->employeeBlock[0].id;

        int hashResultAfter = hashingFunction(empTmp.id, 0);
        HashMap[hashResultAfter]->employeeBlock.push_back(empTmp);
        

        vector<Employee> temp_vector = HashMap[nextpt]->employeeBlock;
        HashMap[nextpt]->employeeBlock.erase(HashMap[nextpt]->employeeBlock.begin(),HashMap[nextpt]->employeeBlock.end());

        



        for (int i = 0; i < temp_vector.size() ; i++){
             int hashResult = hashingFunction(temp_vector[i].id,0);

            HashMap[hashResult]->employeeBlock.push_back(HashMap[nextpt]->employeeBlock[0]);
            HashMap[hashResult]->index = HashMap[nextpt]->index;
            HashMap[nextpt]->employeeBlock.erase(HashMap[nextpt]->employeeBlock.begin());

        }

        // do {

        //     int hashResult = hashingFunction(HashMap[nextpt]->employeeBlock[0].id, HashMap[nextpt]->index);

        //     HashMap[hashResult]->employeeBlock.push_back(HashMap[nextpt]->employeeBlock[0]);
        //     HashMap[hashResult]->index = HashMap[nextpt]->index;
        //     HashMap[nextpt]->employeeBlock.erase(HashMap[nextpt]->employeeBlock.begin());

        // } while (HashMap[nextpt]->employeeBlock[0].id != firstId && HashMap[nextpt]->employeeBlock[0].id != empTmp.id);

        nextpt++;

        int compare = (int)(pow(2, globalIndex));
        
        if(nextpt == compare) {
            nextpt = 0;
            globalIndex++;
        }
    }
    printOutput(); 
}

void insertHash() {
    if(HashMap.size() == 0) {
        HashMap[0] = new Block;
        HashMap[1] = new Block;
    }

    for(int i=0;i<employeedata.size();i++) {
        recordCount = i+1;
        int key = hashingFunction(employeedata[i].id, 0);

        if(!isOverflow()) {
            Block* matchedBlock = HashMap[key];
            cout<<"-------EmpID "<<employeedata[i].id<<"-----index num--------------    "<<HashMap[0]->index<<"\n";

            Employee empTmp = employeedata[i];

            if(matchedBlock->index == 0) {
                matchedBlock->index = globalIndex;
            }

            matchedBlock->employeeBlock.push_back(empTmp);
        } 
        
        else {
            for(int i=0;i<HashMap.size();i++){
                cout<<"index of "<< i <<" "<<HashMap[i]->index<<"\n";
            }

            cout << "overflow.......\n";
            HashMap[HashMap.size()] = new Block;
            cout<<"hashmap size "<<HashMap.size()<<endl;
            cout<<"---------nextptr------  "<<nextpt<<"  ----index-------    "<<HashMap[nextpt]->index<<"\n";

            HashMap[nextpt]->index = HashMap[nextpt]->index + 1;
            maxIndex = HashMap[nextpt]->index;
            int firstId = HashMap[nextpt]->employeeBlock[0].id;

            int hashResultAfter = hashingFunction(employeedata[i].id, 0);

            HashMap[hashResultAfter]->employeeBlock.push_back(employeedata[i]);

            vector<Employee> temp_vector = HashMap[nextpt]->employeeBlock;
            HashMap[nextpt]->employeeBlock.erase(HashMap[nextpt]->employeeBlock.begin(),HashMap[nextpt]->employeeBlock.end());

        



            for (int i = 0; i < temp_vector.size() ; i++){
                int hashResult = hashingFunction(temp_vector[i].id,0);
                HashMap[hashResult]->employeeBlock.push_back(temp_vector[i]);
                HashMap[hashResult]->index = 0;

            }

            
            // do {
            //     // cout<<"aawara  blockmap "<<nextpt<<"\n";
            //     cout<<"aawara hashmapIndex "<<HashMap[nextpt]->index<<"\n";
            //     int hashResult = hashingFunction(HashMap[nextpt]->employeeBlock[0].id, HashMap[nextpt]->index);
            //     cout<<"aawara "<<hashResult<<endl;
            //     cout<<"aawara before accessigng hashResult hashmapIndex "<<HashMap[hashResult]->index<<"\n";


            //     HashMap[hashResult]->employeeBlock.push_back(HashMap[nextpt]->employeeBlock[0]);
            //     HashMap[hashResult]->index = HashMap[nextpt]->index;
            //     HashMap[nextpt]->employeeBlock.erase(HashMap[nextpt]->employeeBlock.begin());

            // } while (HashMap[nextpt]->employeeBlock[0].id != firstId && HashMap[nextpt]->employeeBlock[0].id != employeedata[i].id);


            nextpt++;
            
            cout<<"-------------after while ptr->index = -------------    "<<HashMap[nextpt]->index<<"\n";


            int compare = (int)(pow(2, globalIndex));
            
            if(nextpt == compare) {
                nextpt = 0;
                globalIndex++;
            }
            cout<<"-------------Inc global index = -------------    "<<globalIndex<<"\n";

        }
    }
    printOutput(); 
}

int foundLookupResult;
int findattempt;

bool lookupData(int index) {
    findattempt = findattempt+1;
    int hash = hashingFunction(getid, index);
    if(HashMap.size() <= hash ) {
        hash = hashingFunction(getid, 0);
    }
    
    if(HashMap[hash]){
        vector<Employee> empList = HashMap[hash]->employeeBlock;
        for(int i = 0; i< empList.size(); i++) {
            if(empList[i].id == getid) {
                foundLookupResult = 1;
                cout << "Lookup result: "<< empList[i].id << ", " << empList[i].name << '\n';
                return true;
            }
        }
        if(foundLookupResult == 0 && findattempt == 2) {
            cout << "No lookup result\n";
        }
    }
    return false;
}

bool attemptLookup() {
    bool found;
    if(!HashMap.empty()) {
        found = lookupData(0);
        if(!found) {
            found = lookupData(maxIndex);
        }
    }
    return found;
}

int main(int argc, char* argv[]) {
    LoadCsv();
    insertHash();

    if(argc > 1 && argc <= 6) {
        // cout << "  " << argv[2] << "  " << argv[3] << "  " << argv[4] << "  " << argv[4];
        if(strcmp(argv[1], "L") == 0) {
            cout << "Lookup!!\n";
            getid = stoi(argv[2]);

            attemptLookup();
        }

        if(strcmp(argv[1], "C") == 0) {
            cout << "Create index!!\n";
            getid = stoi(argv[2]);
            getmanagerid = stoi(argv[3]);
            getname = argv[4];
            getbio = argv[5];
            
            if(!attemptLookup()) {
                insertByCommand();
            }
        }
    }

    cout << "\n  RESULT: " << "\n";

    cout << "block size: " << HashMap.size() << '\n';

    cout << "Global index: " << globalIndex << '\n';

    for(int i = 0; i< HashMap.size(); i++) {
        cout << "--------------------index: " << i << ": " << HashMap[i]->index << '\n';
        for(int j= 0; j< HashMap[i]->employeeBlock.size(); j++) {
            cout << "result: " << i << ": " << HashMap[i]->employeeBlock[j].id << '\n';
        }
    } 
}