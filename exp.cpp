#include <iostream>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <cmath>
using namespace std;

unsigned int numBuffers, bufferSize;

struct Employee {
    int id;
    string name;
    string bio;
    int managerId;
};

vector<Employee> employeedata; 


class Block {
    // each block has at max (bufferSize/4) numbers and the possibility of putting one overflow block
    // vector<int> records;
    vector<Employee> records;

    Block *overflow;

    public:
    Block() {
        overflow = NULL;
        records.clear();
    }

    bool isPresent(int x) {
        Block *node = this;
        while(node) {
            for(unsigned int i = 0; i < node->records.size(); i++) {
                if(node->records[i].id == x) {
                    cout << "Lookup result: "<< node->records[i].id << ", " << node->records[i].name << '\n';
                    return true;
                }
            }
            node = node->overflow;
        }
        return false;
    }

    void add(Employee x) {
        // as sizeof(int) is 4
        if(records.size() < (bufferSize / 4)) {
            records.push_back(x);
        }
        else {
            if(overflow == NULL) {
                overflow = new Block();
            }
            overflow->add(x);
        }
    }

    void clearElements(vector<Employee> &v) {
        for(unsigned int i = 0; i < records.size(); i++) {
            v.push_back(records[i]);
        }
        records.clear();
        if(overflow) {
            overflow->clearElements(v);
            delete overflow;
            overflow = NULL;
        }
    }

    void print() {
        Block *node = this;
        while(node) {
            for(unsigned int i = 0; i < node->records.size(); i++) {
                cout << node->records[i].id << ' ';
            }
            node = node->overflow;
        }
        cout << '\n';
    }

};

class HashTable {
    int numRecords, numBits;
    vector<Block *> buckets;

    public:
    HashTable() {
        // initial configuration of Hash table
        numRecords = 0;
        numBits = 1;
        buckets.push_back(new Block());
        buckets.push_back(new Block());
    }

    unsigned int hash(int x) {
        unsigned int mod = (1 << numBits);
        return (unsigned int)(x % mod + mod) % mod;
    }

    int occupancy() {
        // as sizeof(int) is 4
        double ratio = 1.0 * numRecords / buckets.size();
        return (int)(100 * (ratio / (bufferSize / 4)));
    }

    bool isPresent(int x) {
        unsigned int k = hash(x);
        if(k >= buckets.size()) {
            k -= (1 << (numBits - 1));
        }
        if(buckets[k]->isPresent(x)) {
            return true;
        }
        return false;
    }

    void insert(int x) {
        unsigned int k = hash(x);
        if(k >= buckets.size()) {
            k -= (1 << (numBits - 1));
        }
        buckets[k]->add(x);
        numRecords++;
        while(occupancy() >= 75) {
            buckets.push_back(new Block());
            numBits = ceil(log2((double)buckets.size()));
            // split old bucket and rehash
            k = buckets.size() - 1 - (1 << (numBits - 1));
            vector<Employee> v;
            buckets[k]->clearElements(v);
            for(unsigned int i = 0; i < v.size(); i++) {
                buckets[hash(v[i])]->add(v[i]);
            }
        }
        //print();
    }

    void print() {
        for(unsigned int i = 0; i < buckets.size(); i++) {
            cout << i << " -> ";
            buckets[i]->print();
        }
        cout << '\n';
    }

};



HashTable h;

// Buffered IO
// queue<int> inputBuffer, outputBuffer;
// unsigned int inputSize, outputSize;

// void clearOutput() {
//     int ox;
//     while(!outputBuffer.empty()) {
//         ox = outputBuffer.front();
//         outputBuffer.pop();
//         cout << ox << '\n';
//     }
// }

// void clearInput() {
//     int ix;
//     while(!inputBuffer.empty()) {
//         ix = inputBuffer.front();
//         inputBuffer.pop();
//         if(!h.isPresent(ix)) {
//             h.insert(ix);
//             if(outputBuffer.size() == outputSize) {
//                 clearOutput();
//             }
//             outputBuffer.push(ix);
//         }
//     }
// }

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

int main(int argc, char *argv[]) {



    LoadCsv();

    for(int i=0;i<employeedata.size();i++) {
        // recordCount = i+1;

        if(!h.isPresent(employeedata[i])) {
            h.insert(employeedata[i].id);
    // if(argc < 4) {
    //     cout << "Usage : ./a.out <filename> <numBuffers> <bufferSize>\n";
    //     exit(-1);
    // }
    // ifstream input(argv[1]);
    // numBuffers = atoi(argv[2]); //Must be >=2
    // bufferSize = atoi(argv[3]); // Must be >=4
    // if(numBuffers < 2 || bufferSize < 4) {
    //     cout << "Ensure that M>=2 and B>=4\n";
    //     exit(-1);
    // }

    // // as sizeof(int) is 4
    // inputSize = (numBuffers - 1) * (bufferSize / 4);
    // outputSize = bufferSize / 4;
    // int x;
    // while(input >> x) {
    //     if(inputBuffer.size() < inputSize) {
    //         inputBuffer.push(x);
    //     }
    //     else {
    //         clearInput();
    //         inputBuffer.push(x);
    //     }
    // }
    // clearInput();
    // clearOutput();
    // return 0;




}