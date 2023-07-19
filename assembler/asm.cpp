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


map<string, string> comp0 = {{"0", "101010"}, {"1", "111111"}, {"-1", "111010"}, {"D", "001100"}, {"A", "110000"}, {"!D", "001101"}, {"!A", "110001"},
                             {"-D", "001111"}, {"-A", "110011"}, {"D+1", "011111"}, {"A+1", "110111"}, {"D-1", "001110"}, {"A-1", "110010"}, {"D+A", "000010"},
                             {"D-A", "010011"}, {"A-D", "000111"}, {"D&A", "000000"}, {"D|A", "010101"}};

map<string, string> comp1 = {{"M", "110000"}, {"!M", "110001"}, {"-M", "110011"}, {"M+1", "110111"}, {"M-1", "110010"}, {"D+M", "000010"}, {"D-M", "010011"}, 
                             {"M-D", "000111"}, {"D&M", "000000"}, {"D|M", "010101"}};

map<string, string> dest = {{"M", "001"}, {"D", "010"}, {"DM", "011"}, {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"ADM", "111"}};

map<string, string> jump = {{"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};

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