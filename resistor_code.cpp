/*
 File Name: resistor_code.cpp
 Date Created: 10/3/2016
 Created By: Tanner
 Description: Determines resistor color 
		code from resistance or determines
		resistance from color code
*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<vector>
#include<math.h>
// #include<iomanip>
#include<locale>

using namespace std;
const unsigned char Ohm = 234; //Ω
// #define DEBUG /* For debugging */

// Check if Input is valid
bool InvalidInput(int menu, char inOpt){
	if(menu==0){
		return(inOpt != 'c' && inOpt != 'r' && inOpt != 'q');
	}
	return(true);
}

// Generic Prompt
int prompt(void){
	char option;
	option = 'q';
	cout << endl << "Choose option below:";
	do{
		cout << endl << "Get [c]olor bands from known resistance,\ncompute [r]esistance from resistor, or\n[q]uit: "; // << endl;
		cin >> option;
		if(InvalidInput(0,option)){
			cout << "Error:Invalid Input" << endl;
		}
	}while(InvalidInput(0,option));
	
	return(option);
}

// Allow Strings for Resistance input
double get_resistance(void){
	string str_R;
	cout << "Enter resistance in ohms: ";
	cin >> str_R;
	stringstream ss;
	double dbl_R;
	if(str_R.find('k') != string::npos){
		replace(str_R.begin(),str_R.end(),'k','.');
		ss.str(str_R);
		ss >> dbl_R;
		dbl_R *= 1e3;
	} else if(str_R.find('M') != string::npos){
		replace(str_R.begin(),str_R.end(),'M','.');
		ss.str(str_R);
		ss >> dbl_R;
		dbl_R *= 1e6;
	} else {
		ss.str(str_R);
		ss >> dbl_R;
	}
	return(dbl_R);
}

// Get Color bands from Resistance
void Get_Color(void){
	double resistance;
	resistance = get_resistance();
	#ifdef DEBUG
	cout << endl << resistance << endl;
	#endif // DEBUG
	double tolerance;
	cout << "Enter tolerance (absolute value): ";
	cin >> tolerance;
	int bands;
	if(tolerance <= 1){
		bands = 5;
	} else {
		bands = 4;
	}
	// Convert if necessary
	vector<string> code(bands);
	int mag = log10(resistance);
	#ifdef DEBUG
	cout << "Mag: " << mag << endl;
	#endif // DEBUG
	
	// Get Colors
	resistance /= pow(10,mag);	
	#ifdef DEBUG
	cout << resistance << endl;
	cout << floor(3300) << endl;
	cout << floor(3.300) << endl;
	#endif // DEBUG
	++mag;
	int aVal;
	bool ok_so_far = true;
	for(int i=0; i<bands; ++i, resistance -= aVal, resistance *= 10){
		aVal = floor(resistance+0.005); // +0.005...cuz floor is weird
		string aBand;
		#ifdef DEBUG
		cout << i << ":" << aVal << endl;
		#endif // DEBUG
		if(i < 2 || (i == 2 && bands == 5)){
			if(aVal == 0){
				aBand = "black";
			} else if(aVal == 1){
				aBand = "brown";
			} else if(aVal == 2){
				aBand = "red";
			} else if(aVal == 3){
				aBand = "orange";
			} else if(aVal == 4){
				aBand = "yellow";
			} else if(aVal == 5){
				aBand = "green";
			} else if(aVal == 6){
				aBand = "blue";
			} else if(aVal == 7){
					aBand = "violet";
			} else if(aVal == 8){
					aBand = "grey";
			} else if(aVal == 9){
					aBand = "white";
			} else {
				cout << "Invalid: Value" << endl;
				ok_so_far = false;
				break;
			}
			--mag;
		} else if(i == bands - 2){
			#ifdef DEBUG
			cout << "Multiplier: " << mag << endl;
			aBand = "HOLD";
			#endif // DEBUG
			if(mag == 0){
				aBand = "black";
			} else if(mag == 1){
				aBand = "brown";
			} else if(mag == 2){
				aBand = "red";
			} else if(mag == 3){
				aBand = "orange";
			} else if(mag == 4){
				aBand = "yellow";
			} else if(mag == 5){
				aBand = "green";
			} else if(mag == 6){
				aBand = "blue";
			} else if(mag == 7){
				aBand = "violet";
			} else if(mag == -1){
				aBand = "gold";
			} else if(mag == -2){
				aBand = "silver";
			} else {
				cout << "Invalid: Multiplier" << endl;
				ok_so_far = false;
				break;
			}
		} else if(i == bands - 1){
			if(bands == 5){
				if(tolerance==0.05){
					aBand = "grey";
				} else if(tolerance == 0.10){
					aBand = "violet";
				} else if(tolerance == 0.25){
					aBand = "blue";
				} else if(tolerance == 0.50){
					aBand = "green";
				} else if(tolerance == 1){
					aBand = "brown";
				} else {
					cout << "Invalid: Tolerance" << endl;
					ok_so_far = false;
					break;
				}
			} else if(bands == 4){
				if(tolerance==2){
					aBand = "red";
				} else if(tolerance == 5){
					aBand = "gold";
				} else if(tolerance == 10){
					aBand = "silver";
				} else {
					cout << "Invalid: Tolerance" << endl;
					ok_so_far = false;
					break;
				}
			}
		}
		code.push_back(aBand);
	}
	
	
	// Display Colors
	if(ok_so_far){
		cout << "Colors: ";
		for(size_t i=0; i<code.size(); ++i){
			cout << code.at(i) << " ";
		}
	} 
	cout << endl;
}

// Not sure why std::transform isn't working
string make_lower(string str){
	locale loc;
	string out;
	out.resize(str.length());
	for(size_t i=0; i<str.length(); ++i)
		out[i] =  tolower(str[i],loc);
	return(out);
}

// Get Resistance from Color Bands
void Get_Resistance(void){
	string code;
	cout << "Enter colors in order: ";
	cin.ignore();
	getline(cin,code);
	code = make_lower(code);
	// transform(code.begin(), code.end(), code.begin(), tolower);
	
	// Compute Resistance
	stringstream ss(code);
	string aColor;
	double R = 0;
	#ifdef DEBUG
	cout << "Colors:";
	#endif // DEBUG
	
	// Method 1
	vector<string> colors;
	while(ss >> aColor){
		colors.push_back(aColor);
	}
	#ifdef DEBUG
	cout << " size:" << colors.size() << endl;
	#endif // DEBUG
	bool ok_so_far = true;
	if(colors.size() == 4 || colors.size() == 5){
		string tolerance;
		for(size_t i=0; i<colors.size(); ++i){
			double val;
			aColor = colors.at(i);
			if(i < 2 || (i == 2 && colors.size() == 5)){
				R *= 10;
				if(aColor == "black"){
					val = 0;
				} else if(aColor == "brown"){
					val = 1;
				} else if(aColor == "red"){
					val = 2;
				} else if(aColor == "orange"){
					val = 3;
				} else if(aColor == "yellow"){
					val = 4;
				} else if(aColor == "green"){
					val = 5;
				} else if(aColor == "blue"){
					val = 6;
				} else if(aColor == "violet"){
					val = 7;
				} else if(aColor == "grey"){
					val = 8;
				} else if(aColor == "white"){
					val = 9;
				} else {
					cout << "Invalid: Value Band" << endl;
					ok_so_far = false;
					break;
				}
				R += val;
			} else if(i == colors.size() - 2){
				if(aColor == "black"){
					val = 0;
				} else if(aColor == "brown"){
					val = 1;
				} else if(aColor == "red"){
					val = 2;
				} else if(aColor == "orange"){
					val = 3;
				} else if(aColor == "yellow"){
					val = 4;
				} else if(aColor == "green"){
					val = 5;
				} else if(aColor == "blue"){
					val = 6;
				} else if(aColor == "violet"){
					val = 7;
				// } else if(aColor == "grey"){
					// val = 8;
				// } else if(aColor == "white"){
					// val = 9;
				} else if(aColor == "gold"){
					val = -1;
				} else if(aColor == "silver"){
					val = -2;
				} else {
					cout << "Invalid: Multiplier Band" << endl;
					ok_so_far = false;
					break;
				}
				R *= pow(10,val);
			} else if(i == colors.size() - 1){
				if(colors.size() == 5){
					if(aColor == "grey"){
						val = 0.05;
					} else if(aColor == "violet"){
						val = 0.10;
					} else if(aColor == "blue"){
						val = 0.25;
					} else if(aColor == "green"){
						val = 0.5;
					} else if(aColor == "brown"){ 
						val = 1;
					} else {
						cout << "Invalid: Tolerance Band" << endl;
						ok_so_far = false;
						break;
					}
				} else {
					if(aColor == "red"){
						val = 2;
					} else if(aColor == "gold"){
						val = 5;
					} else if(aColor == "silver"){
						val = 10;
					} else {
						cout << "Invalid: Tolerance Band" << endl;
						ok_so_far = false;
						break;
					}
				}
				ostringstream str;
				str << val;
				tolerance = char(241) + str.str() + "%";
			}
			#ifdef DEBUG
				cout << i << ":" << val << ", R:" << R << endl;
			#endif // DEBUG
		}
		// Display Resistance
		if(ok_so_far)
			cout << "Value: " << R << Ohm << " " << tolerance << endl;
		// cin.ignore();
	} else {
		cout << "Invalid" << endl;
	}
	cout << endl;
}

// Global Functions
int main(){
	char inOpt;
	
	cout << "Caution:\n This program does not check whether a resistor exists\n it only computes the theoretical color band or resistance" << endl;
	cout << endl << "Program accepts 'k' and 'M' modifiers\nfor [k]ilo and [M]ega respectively. \nE.g. 4k7 = 4.7e3" << endl;
	do{
		inOpt = prompt();
		if(inOpt == 'c'){
			Get_Color();
		} else if(inOpt == 'r'){
			Get_Resistance();
		} else if(inOpt == 'q'){
			cout << "Bye" << endl;
		}
	
	}while(inOpt != 'q');
		
	return(0);
}