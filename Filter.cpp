/*************************************

   File Name: Filter.cpp
   Date Created: 03-27-18
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

/************* Functions *************/
// Infinite Impulse Response Filter
// Currently, initial condition = 0, may change later
void filter(vector<double>* Y, vector<double>* B,vector<double>* A, vector<double>* X){
	int N = B->size();
	for(int i=0; i < X->size(); ++i){
		double s = 0;
		if(i >= N-1){
			for(int b=0; b < N; ++b){
				s += B->at(b)*X->at(i-b);
			}
			for(int a=1; a < N; ++a){
				s -= A->at(a)*Y->at(i-a);
			}
			s /= A->at(0);
		} 
		Y->push_back(s);
	}
	
}	

// Zero-phase Linear IIR Filter (same as Matlab filtfilt())
void filtfilt(vector<double>* Y, vector<double>* B,vector<double>* A, vector<double>* X){
	vector<double>YY;
	filter(&YY,B,A,X);
	reverse(YY.begin(),YY.end());
	filter(Y,B,A,&YY);
}


void movavg(vector<double>*Y, int W, vector<double>* X){
	vector<double> A;
	vector<double> B;
	for(int i=0; i<W; ++i){
		B.push_back(1/W);
		A.push_back(1);
	}
	filter(Y,&B,&A,X);
}
