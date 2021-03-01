#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;
#define CHUNK_SIZE 22
long long FILE_SIZE; 
// #include "helper.h"




bool EmpSortFunction(const vector<string>& p1, const vector<string>& p2)
{
    stringstream ss1(p1[0]);
    int compare1 = 0;
    ss1 >> compare1;

    stringstream ss2(p2[0]);
    int compare2 = 0;
    ss2 >> compare2;

    return compare1 < compare2;
}

long long getFileSize(string ipFile){
    ifstream file(ipFile);
    string data = "";
    long long count = 0;
    while(getline(file, data,'\n')) count++;
    return count;
}

void SavePartition(const char*  temp_filename,vector<vector<string> > print_str)
{
    string Tuple;

    for(int k = 0; k < print_str.size(); k++) {
        for (int i=0;i<print_str[k].size();i++)
        {
        Tuple += print_str[k][i];
        Tuple += ",";
        }
        Tuple+="\n";
        
        
    }
    // cout << Tuple;
    ofstream csv(temp_filename, ios_base::app | ios_base::out);
    csv << Tuple;
    csv.close();

}


int sortPartition(const char* filename)
{
    vector<vector<string> > Records;
    vector<string> Tuple;
    string line;
    string iterator;
    FILE_SIZE = getFileSize(filename);
    cout<<endl<<FILE_SIZE<<endl;

    int NO_OF_CHUNKS = FILE_SIZE/CHUNK_SIZE;
    if(FILE_SIZE % CHUNK_SIZE != 0) NO_OF_CHUNKS += 1;
    int count = 0;
    int fnptr = 1;


    ifstream csv(filename);
    while(getline(csv, line)) {
        stringstream ss(line);
        while(getline(ss, iterator, ',')){
            Tuple.push_back(iterator);
            
        }
        count++;
        Records.push_back(Tuple);
        Tuple.clear();
        // cout<<count<<endl;
        if(count == CHUNK_SIZE){
            string chunkFile = "temp_emp_" + to_string(fnptr) +".csv";
            sort(Records.begin(), Records.end(), EmpSortFunction);
            SavePartition(chunkFile.c_str(),Records);
            Records.clear();
            fnptr++;
            count = 0;
        }
    }

    if(Records.size()>0){

        string chunkFile = "temp_emp_" + to_string(fnptr) +".csv";
        sort(Records.begin(), Records.end(), EmpSortFunction);
        SavePartition(chunkFile.c_str(),Records);
        Records.clear();
        fnptr++;
        count = 0;
    }
    csv.close();
    fnptr--;
    //cout<<fnptr;
    return fnptr;
}

class Record {
    //Emp (eid (integer), ename (string), age (integer), salary (double))


	public:
        int eid;
		string ename;
		int age;
		int salary;
		Record(  int, string,  int,  int );
        Record() = default;
		void print() const;
        string Stringify() const;



		class eid_sort {
			public:
				bool operator() ( Record const &,
			             	Record const & );
		};


	friend class eid_sort;
};

Record::Record(  int a, string b,  int c,  int d ):
eid( a ),
ename( b ),
age( c ),
salary( d ) {
	// Does nothing
}

void Record::print() const {
	cout << eid << ", " << ename << "," << age <<  "," << salary << ","<< endl;

}

string Record::Stringify() const {
    string s;
	// cout << eid << ", " << ename << "," << age <<  "," << salary << ","<< endl;
    s = to_string(eid) + "," + ename + "," + to_string(age) + "," + to_string(salary) + "\n";
    return s;

}



bool Record::eid_sort::operator() ( Record const &a, Record const &b ) {
	return a.eid > b.eid;
}

vector<Record> LoadCsv() {
    ifstream fin;
    fin.open("Emp.csv", ios::in);

    vector<Record> Rows;
    string line, word, temp;
    

    while (getline(fin, line)) {
        // getline(fin, line);

        stringstream s(line);

        int count = 0;
        int eid,age,salary;
        string ename;
        while (getline(s, word, ',')) {

            if(count == 0) {
                eid = stoi(word);
                count++;
            } else if(count == 1) {
                ename = word;
                count++;
            } else if(count == 2) {
                age = stoi(word);
                count++;
            } else if(count == 3) {
                salary = stoi(word);
                count++;
            }             

        }
        // cout << eid << ", " << ename << "," << age <<  "," << salary << ","<< endl;
        Rows.push_back(Record(eid,ename,age,salary));
        
    }
    fin.close();
    return Rows;
}



struct MinHeapNode
{
    // The tuple to be stored
    Record tuple;
 
    // array index from which the element is taken
    int i;
};

struct comp
{
    bool operator()(const MinHeapNode &lhs, const MinHeapNode &rhs) const {
        return lhs.tuple.eid > rhs.tuple.eid;
    }
};

void read_first_lines(int k){

    // int k=4;
    ifstream chunkFiles[k];
    ofstream csv("join.csv", ios_base::app | ios_base::out);
    MinHeapNode harr[k];
    // priority_queue< Record, vector< Record >, Record::eid_sort > pq_number;
    priority_queue<MinHeapNode, vector<MinHeapNode>, comp> pq;

    for(int i=0; i<k; i++){

        ostringstream fn ;
        fn<< "temp_emp_" <<to_string(i+1) <<".csv";
        chunkFiles[i].open(fn.str().c_str(),ios::in);
    }

    for(int i=0; i<k; i++){
        string iterator;
        string line, word, temp;
        // getline(chunkFiles[i], line);
        if (!getline(chunkFiles[i], line).eof()){
            stringstream s(line);

            int count = 0;
            int eid,age,salary;
            string ename;
            while (getline(s, word, ',')) {

                if(count == 0) {
                    eid = stoi(word);
                    count++;
                } else if(count == 1) {
                    ename = word;
                    count++;
                } else if(count == 2) {
                    age = stoi(word);
                    count++;
                } else if(count == 3) {
                    salary = stoi(word);
                    count++;
                }             

            }
            // cout << eid << ", " << ename << "," << age <<  "," << salary << ","<< endl;
            harr[i].tuple = Record(eid,ename,age,salary);
            harr[i].i = i;
            pq.push(harr[i]);
        }



    }

 
    // One by one, get the minimum element from the min-heap and replace
    // it with the next element. Run till all filled input files reach EOF.
    int ct = 1;
    int fc = 1;
    while(fc!=k)
    {
        // Get the minimum element and store it in the output file
        MinHeapNode root = pq.top();
        pq.pop();
        root.tuple.print();
        if(ct==FILE_SIZE)
            csv << root.tuple.Stringify();
        else{
            csv << root.tuple.Stringify();
            }
        ct++;

 
        // Find the next element that should replace the current root of the heap.
        // The next element belongs to the same input file as the current
        // minimum element.
        string iterator;
        string line, word, temp;
        // getline(chunkFiles[root.i], line);
        if (!getline(chunkFiles[root.i], line).eof()){
            stringstream s(line);

            int count = 0;
            int eid,age,salary;
            string ename;
            while (getline(s, word, ',')) {

                if(count == 0) {
                    eid = stoi(word);
                    count++;
                } else if(count == 1) {
                    ename = word;
                    count++;
                } else if(count == 2) {
                    age = stoi(word);
                    count++;
                } else if(count == 3) {
                    salary = stoi(word);
                    count++;
                }             

            }
            // cout << eid << ", " << ename << "," << age <<  "," << salary << ","<< endl;
            harr[root.i].tuple = Record(eid,ename,age,salary);
            harr[root.i].i = root.i;
            pq.push(harr[root.i]);
        }
        else fc += 1;
 
        // Replace the root with the next element of the input file
        // pq.push(root);
    }
    for (int i = 0; i < k; i++) {
        chunkFiles[i].close();
    }
 
    csv.close();
 
    cout<<"-----------------"<<ct<<endl;
    // pq_number.top().print();
    // pq_number.po

}


int main()
{   
    FILE_SIZE = getFileSize("temp.csv");
    int no_of_partitions = 0;
    no_of_partitions = sortPartition("temp.csv");
    read_first_lines(no_of_partitions);

}