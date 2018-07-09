/*************************************

   File Name: SPL2DBA.cpp
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

// My sources
#include"../lib/io_methods.cpp"
#include"../lib/Filter.cpp"
#include"../lib/mymath.cpp"
#include"DBA.h"

using namespace std;

// Global and Const Variables
// const double PI = 3.14159;
const double Pref = 0.00002; //Reference for converting Pa to dB (SPL) -- already converted to dB (see spl2db())
#define spl2db(x) (mag2db(x) + 93.9794)

const double fs=25000; // Sample rate
const double Nyq=12500; // Nyquist Frequency

// const string Header[11] = {'Time, s','Band 1, dBA','Band 2, dBA','Band 3, dBA','Band 4, dBA','Band 5, dBA','Band 6, dBA','Band 7, dBA','Band 8, dBA','Band 9, dBA','Composite, dBA'};
const string statsLbl[] = {"Min, dBA","Max, dBA","Mean, dBA"};


/****** Debugging Macros ******/
#define WRITE_FILES
#define GET_STATS
// #define SHOW_PROGRESS
// #define SLOWONLY



/************* Functions *************/
// Actual Conversion of Pa to dBA
double convert(vector<double>*Y, vector<double>*X, int respMode){
	int N = X->size();
	int Wsize;
	if(respMode == kFast){
		Wsize = Wfast;
	} else {
		Wsize = Wslow;
	}
	int K = floor(N/Wsize);
	double SPL_A[K][9];

	// vector<double> filt1;
	vector<double> filt;
	// For each band
	cout << "Converting..." << endl;
	// cout << K << "rows of " << Wsize << "sized windows"<< endl;
	// Close enough....
	// vector<double> A(Acoeff[4],Acoeff[4] + 5);
	// vector<double> B(Bcoeff[4],Bcoeff[4] + 5);
	// filter(Y,&B,&A,X);
	
	for(int band=0; band < 9; ++band){
		double progress = 0;
	#ifdef SHOW_PROGRESS
		cout << endl << "Band " << band << ":" << endl;
	#endif // SHOW_PROGRESS
		vector<double> A(Acoeff[band],Acoeff[band] + 5);
		vector<double> B(Bcoeff[band],Bcoeff[band] + 5);
		// cout << "Bcoeff: ";
		// display_dblvector(&B);
		// cout << "Acoeff: ";
		// display_dblvector(&A);
		filt.clear();
		filter(&filt,&B,&A,X);
		// cout << "Filtered Data Size: " << filt.size() << endl;
	#ifdef SHOW_PROGRESS
		cout << "Filt Data: min=" << *min_element(filt.begin(),filt.end()) << " max=" << *max_element(filt.begin(),filt.end()) << endl ;
	#endif // SHOW_PROGRESS
		// For each Window
	#ifdef SHOW_PROGRESS
		if(band == 3){
			cout << "Bcoeff: ";
			display_dblvector(&B);
			cout << "Acoeff: ";
			display_dblvector(&A);
			write_file(&filt,"filtoutb4.txt");
		}
		cout << "Progress:" << endl;
	#endif // SHOW_PROGRESS
		for(int k=0; k<K; ++k){
			vector<double> W(&filt.at(Wsize*k),(Wsize*(k+1) < (N-1) ? &filt.at(Wsize*(k + 1)) : &filt.at(N-1)));			
			double rms1w = rms(&W);
			double db_SPL = spl2db(rms1w);
			double aVal = db_SPL + A_Wts[band];
			SPL_A[k][band] = (aVal > 0 && aVal < 140) ? db2pow(aVal) : 1;
	#ifdef SHOW_PROGRESS
			if(Wsize == 25000 && band == 3){
				cout << "Start:" << Wsize*k << " Stop:" << Wsize*(k + 1);
				cout << " rms:" << rms1w;
				cout << " db_SPL:" << db_SPL << endl;
				// cout << "dbA: " << pow2db(SPL_A[k][band]) << endl;
			}
	#endif // SHOW_PROGRESS
			W.clear();
		} // For each Window
	} // For each band
	
	
	double mean = 0;
	for(int k=0; k<K; ++k){
		double compAk = 0;
		for(int b=0; b<9; ++b){
			compAk += SPL_A[k][b];
		}
		mean += compAk;
		Y->push_back(pow2db(compAk));
	}
	mean /= K;	
	cout << "Done" << endl;
	return(pow2db(mean)); // Return the mean - you can get min and max directly
}


// Main
int main(int argc, char** argv){
	char inOpt;
	vector<double> inData;
	vector<double> outData;	

	double slowStats[3];	
	double fastStats[3];	
	inOpt = 'q';
	do{
		// inOpt = prompt("[T]ype file, [f]etch file, or [q]uit: ","tfq");
		// if(inOpt=='t'){
			// write_text(&inData);
			// encrypt(&outText,&inData,&myKey);
			// encrypt(&newText,&outText,&myKey);
		// } else if(inOpt == 'f'){
			inData.clear();
			string inFile;
			if(argc == 1){
				cout << "Enter filename:" << endl;
				cin >> inFile;
			} else {
				inFile = "NS_TTCI_MicTest_r8_m1.txt";
			}
			(void)load_file(&inData,inFile);
			cout << "Input Data Size:" << inData.size() << endl;
			

			string outfilename;

			// Fast Response
#ifndef SLOWONLY
			outData.clear();
			cout << endl << "Fast Response:" << endl;
			cout << "--------------" << endl;
#ifndef GET_STATS
			(void)convert(&outData,&inData,kFast);
#else // GET_STATS
			fastStats[2] = convert(&outData,&inData,kFast);
			cout << "Calculating Stats...";
			fastStats[0]  = *min_element(outData.begin(),outData.end());
			// cout << *min_element(outData.begin(),outData.end()) << endl;
			fastStats[1]  = *max_element(outData.begin(),outData.end());
			// cout << *max_element(outData.begin(),outData.end()) << endl;
			cout << fastStats[0] << " " << fastStats[1] << " " << fastStats[2] << endl;
			cout << "Done" << endl;
#endif // GET_STATS
			cout << "Data Size:" << outData.size() << endl;
#ifdef WRITE_FILES
			outfilename = inFile.substr(0,inFile.find(".txt")) + "_dba_fast.txt";
			write_file(&outData,outfilename);
			cout << "Data Size:" << outData.size() << endl;
#endif // WRITE_FILES
#endif // SLOWONLY
			
			// Slow Response
			outData.clear();
			cout << endl << "Slow Response:" << endl;
			cout << "--------------" << endl;
#ifndef GET_STATS
			(void)convert(&outData,&inData,kSlow);
#else // GET_STATS
			slowStats[2] = convert(&outData,&inData,kSlow);
			slowStats[0] = *min_element(outData.begin(),outData.end());
			// cout << *min_element(outData.begin(),outData.end()) << endl;
			slowStats[1] = *max_element(outData.begin(),outData.end());
			// cout << *max_element(outData.begin(),outData.end()) << endl;
			cout << slowStats[0] << " " << slowStats[1] << " " << slowStats[2] << endl;
#endif // GET_STATS
			cout << "Data Size:" << outData.size() << endl;
#ifdef WRITE_FILES
			outfilename = inFile.substr(0,inFile.find(".txt")) + "_dba_slow.txt";
			write_file(&outData,outfilename);
			cout << "Data Size:" << outData.size() << endl;
#endif // WRITE_FILES
			
			// Statistics
			outfilename = inFile.substr(0,inFile.find(".txt")) + "_dba_stats.txt";

#ifdef GET_STATS
			vector<string> outStatsV;
			outStatsV.push_back("\t\tFast\tSlow\n");
			for(int i=0; i<3; ++i){
				ostringstream oss;
				oss << statsLbl[i];
				// cout << i ;
				oss << "\t";
				oss << setprecision(1) << fixed << floor(10*fastStats[i])/10;
				oss << "\t";
				oss << setprecision(1) << fixed << floor(10*slowStats[i])/10;
				oss << "\n";
				// cout << endl;
				outStatsV.push_back(oss.str());
			}
#ifdef WRITE_FILES
			write_file(&outStatsV,outfilename);
#endif // WRITE_FILES
#endif // GET_STATS
		// }else if(inOpt=='q'){
			// cout << "End Program" << endl;
		// }
		// delete[] outData;
	}while(inOpt != 'q');

	return(0);
}

