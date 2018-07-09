/*************************************

   File Name: mymath.cpp
   Date Created: 03-28-18
   Created By: Tanner Buel

*************************************/



/************* Libraries *************/
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<vector>
#include<math.h>
#include<iomanip>
#include<locale>


// using namespace std;

// Global and Const Variables
const double PI = 3.14159;

/************* Functions *************/

// Root mean square
double rms(std::vector<double>* X){
	double s = 0;
	for(int i=0; i < X->size(); ++i){
		s += pow(X->at(i),2.0);
	}
	return(sqrt(s/X->size()));
}


double mag2db(double x){
	return(20*log10(x));
}

double pow2db(double p){
	return(10*log10(p));
}

double db2pow(double d){
	return(pow(10,d/10));
}	
