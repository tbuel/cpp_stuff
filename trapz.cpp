/*
 File Name: trapz.cpp
 Date Created: 9/27/2016
 Created By: Tanner
 Description: Trapezoidal Discrete Integration
*/

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

double mov_avg(vector<double>* outAvg, vector<double>* inData, int N){
	
	double max_val = 0;
	for(int i=0; i<(inData->size() - N); ++i){
		double sum = 0;
		for(int n=0; n<N; ++n){
			sum += (inData->at(i+n));
		}
		double val = sum/N;
		max_val = abs(max_val) > abs(val) ? max_val : val;
		// cout << i << ":" << sum << endl;
		outAvg->push_back(val);
		// cout << outAvg->at(i) << endl;
	
	}
	return max_val;
}

void downsample(vector<double>* out, vector<double>* in, int N){
	
	for(int i=0; i< in->size(); i += N){
		double val = in->at(i);
		// cout << i << ":" << val << endl;
		out->push_back(val);
	}

}

// void max_one_sec(vector<double>* out, vector<double>* in){
	// for(int i=0; i < (in->size() - fs); i += fs){
		// double val = *max_element((in + i), (in + i + fs - 1));
		// out->push_back(val);
	
	// }


// }


// Global Functions
int main(){
	
	string filename;
	vector<double> data;
	int fs;
	
	// Input
	cout << "Enter filename:";
	cin >> filename;
	cout << "Sample Rate: ";
	cin >> fs;
	ifstream inFile;
	inFile.open(filename.c_str());
	if(inFile.is_open()){
		double a;
		cout << filename << " is open..." << endl << "Parsing...";
		while(inFile >> a){
			// cout << a << endl;
			data.push_back(a);	
			// data_avg.push_back(a);
		}
	
	} else {
		cout << "error";
	}
	cout << "Done" << endl;
	inFile.close();
	cout << "Lines: " << data.size() << endl;
	cout << "Max: " << *max_element(data.begin(),data.end()) << endl;
	cout << "Min: " << *min_element(data.begin(),data.end()) << endl;
	
	// Calculations
	double ans = 0;
	for(int i=1; i<data.size(); ++i){
		ans += (data.at(i) + data.at(i-1))/(fs*2);
	}
	// vector<double> data_ds;
	// int fds = 0;
	// do{
		// cout << "Enter Downsample Rate: ";
		// cin >> fds;
		// if(fds > fs || fs == 0){
			// cout << "invalid" << endl;
		// }
	// }while(fds > fs || fs == 0);
	// downsample(&data_ds,&data,fs/fds);
	// // max_one_sec(&data_ds,&data);
	// cout << "New lines: " << data_ds.size() << endl;
	
	// int Window;
	// cout << "Window size for averaging [seconds]: " << endl;
	// cin >> Window;
	// vector<double> data_avg;
	// double max_avg;
	// cout << "Computing..." << endl;
	// max_avg = mov_avg(&data_avg,&data_ds,Window*fds);
	// cout << "Max moving average: " << max_avg << endl;
	
	// Write out
	// string outfilename;
	// outfilename = filename.substr(0,filename.length()-4) + "_movavg.txt";
	// ofstream outFile;
	// outFile.open(outfilename.c_str());
	
	// cout << "Writing to " <<  outfilename << "..." <<  endl;
	// for(int i=0; i<data_avg.size(); ++i){
		// outFile << data_avg.at(i) << endl;
	// }
	cout << "integral(data)dt = " << ans << endl;
	cout << "Done" << endl;
	// outFile.close();
	cout << "Bye" << endl;
	
	return(0);
}