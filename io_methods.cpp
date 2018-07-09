/****************************
  File: io_methods.cpp
  Date: 4-28-2017
  Created by: Tanner

  Methods for reading reading input
  and files
****************************/
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<vector>
#include<iomanip>

using namespace std;

#define SHOW_CONTENTS

// Prototypes
void display_text(vector<string>*);
bool InvalidInput(char,const string );
int prompt(const string, const string);
int load_file(vector<string>* , const string);
void write_text(vector<string>* );
int write_file(vector<string>*,const string );
void string2ascii(vector<int>* , const string );
string ascii2string(vector<int>* );

// Functions
// Convert String to Ascii (Int Vector)
void string2ascii(vector<int>* outA, const string inText){
	for (int i=0; i<inText.length(); ++i){
		int a;
		a = int(inText.at(i));
		outA->push_back(a);
	}
}

// Convert Ascii (Int Vector) to String
string ascii2string(vector<int>* inA){
	string out;
	out = "";
	for(int i=0; i<inA->size(); ++i){
		out += char(inA->at(i));

	}	
	return(out);
}

// Check if Input is valid
bool InvalidInput(char inOpt, const string options){
	for(int i=0; i<options.length(); ++i){
		if(inOpt == options.at(i)) { return(false); }
	}
	return(true);
}

// Generic Prompt
int prompt(const string qry, const string options){
	char opt;
	opt = 'q';
	do{
		cout << endl << qry;
		cin >> opt;
		opt= tolower(opt);
		if(InvalidInput(opt, options)){
			cout << "Error:Invalid Input" << endl;
		}
	}while(InvalidInput(opt,options));
	return(opt);
}

// Load text into vector pointer. Return success
int load_file(vector<string>* inText, const string filename){
	int nlines;
	nlines = 0;
	string line;
	ifstream infile(filename.c_str());
	if(!infile){
		cout << endl << "Error reading: \"" << filename << "\"" << endl;
	} else {
		cout << endl << "Loading: \"" << filename << "\"..." << endl;
		while(getline(infile,line)){
			inText->push_back(line);
			//inText->push_back(line + "\r\n");
			#ifdef SHOW_CONTENTS
			cout << line << endl;
			#endif // SHOW_CONTENTS
			++nlines;
		}
		cout << endl << "Done!" << endl;
	}
	return(nlines);
}

// Double overload 
int load_file(vector<double>* data, const string filename){
	int nlines;
	nlines = 0;
	double a;
	ifstream infile(filename.c_str());
	if(!infile){
		cout << endl << "Error reading: \"" << filename << "\"" << endl;
	} else {
		cout << endl << "Loading: \"" << filename << "\"..." << endl;
		while(infile >> a){
			data->push_back(a);
			++nlines;
		}
		cout << endl << "Done!" << endl;
	}
	return(nlines);
}




// Write text into vector pointer
void write_text(vector<string>* inText){
	cout << "Type:" << endl;
	string line = "dummy";
	while(line.length() != 0){
		getline(cin,line);
		inText->push_back(line);
	}
	#ifdef SHOW_CONTENTS
	cout << endl << "Contents:" << endl;
	display_text(inText);
	#endif // SHOW_CONTENTS

}

// Displays text from a vector<string> input
void display_text(vector<string>* inText){
	for(int i=0; i<inText->size(); ++i){
		cout << inText->at(i) << endl;
	}
}

void display_dblvector(vector<double>* inData){
	for(int i=0; i<inData->size(); ++i){
		cout << inData->at(i) << " ";// << endl;
	}
	cout << endl;
}

// Writes file out to filename
int write_file(vector<string>* inText, const string filename){

	ofstream outFile;
	outFile.open(filename.c_str());
	cout << endl << "Writing to:\"" << filename << "\"..." << endl;
	for(int i=0; i<inText->size(); ++i){
		outFile << inText->at(i);// << endl;
	}
	cout << endl << "Done!" << endl;
	outFile.close();
	return(1);

}

// Writes file out to filename Double overload
int write_file(vector<double>* data, const string filename){

	ofstream outFile;
	outFile.open(filename.c_str());
	cout << endl << "Writing to:\"" << filename << "\"..." << endl;
	for(int i=0; i<data->size(); ++i){
		outFile << data->at(i) << endl;
	}
	cout << endl << "Done!" << endl;
	outFile.close();
	return(1);

}

