#include<iostream>
#include<fstream>
#include<cstring>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include <boost/lexical_cast.hpp>
using namespace std;

map<string, int> predefined = {{"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
                               {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}, {"SCREEN", 16384}, {"KBD", 24576},
                               {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4}};

map<string,int>::iterator it = predefined.begin();


map<string, string> comp1 = {};
map<string, string> comp2 = {};
map<string, string> dest = {};
map<string, string> jump = {};

string decTo15BitBinary(int n) {
	string address;
	// Size of an integer is assumed to be 15 bits
	for (int i = 15; i >= 0; i--) {
		int k = n >> i;
		if (k & 1) {
			address.push_back(49);
		}
		else {
			address.push_back(48);
		}
	}
	return address;
}

string cleaner(string line) {
	//erasing all whitespace in the line
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
	//ascii value of /
	char slash = 47;
	//detecting the beginning of a comment and erasing
	for (int i = 0; i < line.length(); i++) {
		if(line.at(i) == slash) {
			line.erase(i);
			break;
		}
	}
	return line;
}

vector<string> parser(string filename) {
	vector<string> grabber;
	string line;
	string clean_line;
	ifstream ss;
	ss.open(filename);
	while (getline(ss >> ws, line)) {
		clean_line = cleaner(line);
		//skipping blank lines
		if (!clean_line.empty()) {
			//appending lines to grabber for element by element processing
			grabber.push_back(clean_line);
            cout << clean_line << endl;
        }
    }
	ss.close();
	return grabber;
}


string firstPass(string line, int index) {
	if (line.at(0) == 40) {
		line.erase(line.begin());
		line.erase(line.end());
		predefined.insert( it, pair<string, int>(line, index + 1));
	}
	return line;
}


string secondPass(string line, int index) {
	string symbol;
	int data_memory = 16;
	if (line.at(0) == 64 && !isdigit(line.at(1))) {
		for (int i = 1; i < line.length(); i++) {
			symbol.push_back(line.at(i));
		}
		if (predefined.find(symbol) != predefined.end()) {
			predefined.insert( it, pair<string, int>(symbol, data_memory));
			data_memory++;
		}
	}
	return symbol;
}

string thirdPass(string line, int index) {
	string address;
	string address_str;
	int address_int;
	int predefined_address;
	if (line.at(0) == 64 && isdigit(line.at(1))) {
		for (int i = 1; i < line.length(); i++) {
			address_str.push_back(line.at(i));
		}
		address_int = stoi(address_str);
		address = decTo15BitBinary(address_int);
		line = boost::lexical_cast<string>(address);

	}
	else if (line.at(0) == 64 && !isdigit(line.at(1))) {
		for (int i = 1; i < line.length(); i++) {
			address_str.push_back(line.at(i));
		}
		predefined_address = predefined[address_str];
		address = decTo15BitBinary(predefined_address);
		line = boost::lexical_cast<string>(address);
	}

	return line;
}

string cInstruction(string line) {
	string init = "111";
	string a;
	string c;
	string d;
	string j;
	return line;
}

int main() {
	cout << endl << "file to assemble >> ";
	string filename;
	cin >> filename;
	//vector<string> grabber;
	//grabber = parser(filename);

	return 0;
}