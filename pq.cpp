#include <queue>
#include <string>
#include <iostream>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Record {
    //Emp (eid (integer), ename (string), age (integer), salary (double))
	private:
         int eid;
		string ename;
		 int age;
		 int salary;

	public:
		Record(  int, string,  int,  int );
		void print() const;



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

void print_records(vector<Record> r){
    for(int k = 0; k < r.size(); k++) {
        r[k].print();
        
        
    }
}

int main() {
	int const N = 4;
	// Record array[N] = {
	// 	Record( 1, "Harder", 0, 1 ),
	// 	Record( 2, "Cushing", 1, 2 ),
    //     Record( 12, "Cushing", 4, 44 ),
    //     Record( 22, "Cushing", 4, 23 )
	// };
    vector<Record> tuples = LoadCsv();
    // print_records(tuples);
	priority_queue< Record, vector< Record >, Record::eid_sort > pq_number;

	for ( int i = 0; i < tuples.size(); ++i ) {
		pq_number.push( tuples[i] );
	}


	cout << endl << endl << "ID# order:" << endl;

	// for ( int i = 0; i < tuples.size(); ++i ) {
	// 	pq_number.top().print();
	// 	pq_number.pop();
	// }
    pq_number.push(Record(1,"me",123,23232));
	pq_number.top().print();

	return 0;
}