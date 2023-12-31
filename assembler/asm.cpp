#include<iostream>
#include<fstream>
#include<cstring>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include <boost/lexical_cast.hpp>
using namespace std;

string logo = R"(
   ===========================================================================================================
   ===========================================================================================================
   =       ___           _______.     _______. _______ .___  ___. .______    __       _______ .______        =  
   =      /   \         /       |    /       ||   ____||   \/   | |   _  \  |  |     |   ____||   _  \       =
   =     /  ^  \       |   (----`   |   (----`|  |__   |  \  /  | |  |_)  | |  |     |  |__   |  |_)  |      =
   =    /  /_\  \       \   \        \   \    |   __|  |  |\/|  | |   _  <  |  |     |   __|  |      /       =
   =   /  _____  \  .----)   |   .----)   |   |  |____ |  |  |  | |  |_)  | |  `----.|  |____ |  |\  \----.  =
   =  /__/     \__\ |_______/    |_______/    |_______||__|  |__| |______/  |_______||_______|| _| `._____|  =
   =                                                                                                         =
   ===========================================================================================================
   ===========================================================================================================

This assembler is intended only for processing programs written in the "Hack Assembly Language" which was developed by
 professor Shimon Schocken & professor Noam Nissim at the Hebrew University of Jerusalem. The assembler was designed
      and built by Eliyahu Hawila, and is available as open source for training and education purposes.
=====================================================================================================================                            
)";


//predefined symbols table and its iterator
map<string, int> predefined = {{"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3}, {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}, {"R8", 8}, {"R9", 9},
                               {"R10", 10}, {"R11", 11}, {"R12", 12}, {"R13", 13}, {"R14", 14}, {"R15", 15}, {"SCREEN", 16384}, {"KBD", 24576},
                               {"SP", 0}, {"LCL", 1}, {"ARG", 2}, {"THIS", 3}, {"THAT", 4}};
map<string,int>::iterator it = predefined.begin();





//compute table for a = 0 and its iterator
map<string, string> comp0 = {{"0", "101010"}, {"1", "111111"}, {"-1", "111010"}, {"D", "001100"}, {"A", "110000"}, {"!D", "001101"}, {"!A", "110001"},
                             {"-D", "001111"}, {"-A", "110011"}, {"D+1", "011111"}, {"A+1", "110111"}, {"D-1", "001110"}, {"A-1", "110010"}, {"D+A", "000010"},
                             {"D-A", "010011"}, {"A-D", "000111"}, {"D&A", "000000"}, {"D|A", "010101"}};
map<string,string>::iterator comp0_it = comp0.begin();




//compute table for a = 1 and its iterator
map<string, string> comp1 = {{"M", "110000"}, {"!M", "110001"}, {"-M", "110011"}, {"M+1", "110111"}, {"M-1", "110010"}, {"D+M", "000010"}, {"D-M", "010011"}, 
                             {"M-D", "000111"}, {"D&M", "000000"}, {"D|M", "010101"}};
map<string,string>::iterator comp1_it = comp1.begin();




//destination table and its iterator
map<string, string> dest = {{"M", "001"}, {"D", "010"}, {"DM", "011"}, {"A", "100"}, {"AM", "101"}, {"AD", "110"}, {"ADM", "111"}};
map<string,string>::iterator dest_it = dest.begin();




//jump table and its iterator
map<string, string> jump = {{"JGT", "001"}, {"JEQ", "010"}, {"JGE", "011"}, {"JLT", "100"}, {"JNE", "101"}, {"JLE", "110"}, {"JMP", "111"}};

map<string,string>::iterator jump_it = jump.begin();

//memory address of the first detected assembly variable is declared here as a global variable
int data_memory = 16;


//decimal to binary converter
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

//in-line white space cleaner and comment remover
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

//empty line remover
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
        }
    }
    ss.close();
    return grabber;
}

//first pass to process labels
string firstPass(string line, int index) {
    if (line.at(0) == 40) {
        line.erase(line.begin());
        line.erase(line.end() - 1);
        predefined.insert( it, pair<string, int>(line, index));
        line = "";
    }
    return line;
}

//second pass to process variables
string secondPass(string line, int index) {
    string symbol;
    //int data_memory = 16;
    if (line != "" && line.at(0) == 64 && !isdigit(line.at(1))) {
        for (int i = 1; i < line.length(); i++) {
            symbol.push_back(line.at(i));
        }
        if (!(predefined.find(symbol) != predefined.end())) {
            predefined.insert( it, pair<string, int>(symbol, data_memory));
            data_memory++;
        }
    }
    return line;
}


//third pass to change the results of the affected line in the first and second pass into binary
string thirdPass(string line, int index) {
    string address;
    string address_str;
    int address_int;
    int predefined_address;
    if (line != "" && line.at(0) == 64 && isdigit(line.at(1))) {
        for (int i = 1; i < line.length(); i++) {
            address_str.push_back(line.at(i));
        }
        address_int = stoi(address_str);
        address = decTo15BitBinary(address_int);
        line = boost::lexical_cast<string>(address);

    }
    else if (line != "" && line.at(0) == 64 && !isdigit(line.at(1))) {
        for (int i = 1; i < line.length(); i++) {
            address_str.push_back(line.at(i));
        }
        predefined_address = predefined[address_str];
        address = decTo15BitBinary(predefined_address);
        line = boost::lexical_cast<string>(address);
    }

    return line;
}


//c-instruction handler from instruction to binary
string cInstruction(string line) {
    string init = "111";
    string a;
    string c;
    string d;
    string j;
    char j_char = 59;
    char e_char = 61;
    auto jmp = find(line.begin(), line.end(), j_char);
    auto equal = find(line.begin(), line.end(), e_char);
    if(line != "" && !isdigit(line.at(0))) {
        if (!(jmp != line.end())) {
            j = "000";
            for (int id = 0; id < (equal - line.begin()); id++) {
                d.push_back(line.at(id));
            }
            if (dest.find(d) != dest.end()) {
                d = dest[d];
            }
            for (int  ic = (equal - line.begin()) + 1; ic < line.length(); ic++) {
                c.push_back(line.at(ic));
            }
            if (comp0.find(c) != comp0.end()) {
                c = comp0[c];
                a = "0";
            }
            else if (comp1.find(c) != comp1.end()) {
                c = comp1[c];
                a = "1";
            }
        }
        else {
            if (equal != line.end()) {
                for (int id = 0; id < (equal - line.begin()); id++) {
                    d.push_back(line.at(id));
                }
                d = dest[d];
                for (int ic = (equal - line.begin()) + 1; ic < (jmp - line.begin()); ic++) {
                    c.push_back(line.at(ic));
                }
                for (int ij = (jmp - line.begin()) + 1; ij < line.length(); ij++) {
                    j.push_back(line.at(ij));
                }
                j = jump[j];
                if (comp0.find(c) != comp0.end()) {
                    c = comp0[c];
                    a = "0";
                }
                else if (comp1.find(c) != comp1.end()) {
                    c = comp1[c];
                    a = "1";
                }
            }
            else {
                d = "000";
                for (int ic = 0; ic < (jmp - line.begin()); ic++) {
                    c.push_back(line.at(ic));
                }
                for (int ij = (jmp - line.begin()) + 1; ij < line.length(); ij++) {
                    j.push_back(line.at(ij));
                }
                j = jump[j];
                if (comp0.find(c) != comp0.end()) {
                    c = comp0[c];
                    a = "0";
                }
                else if (comp1.find(c) != comp1.end()) {
                    c = comp1[c];
                    a = "1";
                }

            }
        }
        line = init + a + c + d + j;
    }
    //uncondition jump format exception
    else if (line == "0;JMP") {
        line = "1110101010000111";
    }
    return line;
}


void releaser(vector<string> &binary, string filename) {
    filename.pop_back();
    filename.pop_back();
    filename.pop_back();
    filename.push_back(104);
    filename.push_back(97);
    filename.push_back(99);
    filename.push_back(107);
    ofstream ss;
    ss.open(filename);
    for (int i = 0; i < binary.size(); i++) {
        ss << binary[i] << endl;
    }
    ss.close();
    cout << "file assembled to >> " << filename << endl;
}

//main function packager
int main() {
    cout << logo << endl;
    cout << endl << "file to assemble >> ";
    string filename;
    cin >> filename;
    string line;
    vector<string> grabber;
    vector<string> binary;
    grabber = parser(filename);
    for (int i = 0; i < grabber.size(); i++) {
        line = grabber[i];
        line = firstPass(line, i);
        line = secondPass(line, i);
        line = thirdPass(line, i);
        line = cInstruction(line);
        if (line != "") {binary.push_back(line);}
    }
    releaser(binary, filename);
    return 0;
}
