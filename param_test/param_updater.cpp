
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include <boost/algorithm/string.hpp>

#define VERBOSE 0

using namespace std;



/*
 * A class to read data from a csv file.
 */
class CSVReader
{
	string fileName;
	string delimeter;

public:
	CSVReader(string filename, string delm = "	") :
			fileName(filename), delimeter(delm)
	{ }

	// Function to fetch data from a CSV File
	vector<vector<string> > getData();
};

class ParameterData
{
	vector<vector<float> > csv_data_array;
	vector<vector<string> > selected_params;

	int nr_of_accelerometers;
	int nr_of_gyroscopes;


	public:
	void ReadMatlabCSV();
	void ParameterSelection();
	void WriteParamToFile();

};

/*
* Reads csv file from henk with the parameters coming from matlab
*/
void ParameterData::ReadMatlabCSV()
{
  CSVReader csv_reader("henk_csv_multiple_senors.csv", ",");
	vector<vector<string> > csv_dataList = csv_reader.getData();
	int accel_on_row  = 0;
	int gyro_on_row = 0;

	for(int i = 0; i < csv_dataList.size(); i++){
		if(csv_dataList[i][0].compare("Accelerometer calibration parameters:") == 0){
			//Found on row in the csv file
			accel_on_row = i;
		}

		if(csv_dataList[i][0].compare("Gyroscope calibration parameters:") == 0){
			gyro_on_row = i;
			//rownr of accelerometer - rownr accel -2 definition rows = nr of sensors
			nr_of_accelerometers = (i - accel_on_row - 2);
		}

		if(csv_dataList[i][0].compare("Magnetometer calibration parameters:") == 0){
			//rownr of accelerometer - rownr gyro -2 definition rows = nr of sensors
			nr_of_gyroscopes = (i - gyro_on_row - 2);
		}
	}


	//creates an vector array per accellerometer and stores them in a vector of vectors
	for(int i = accel_on_row + 2; i < accel_on_row + nr_of_accelerometers + 2; i++){
		vector<float> tmp_data_array;
		for(int j = 0 ; j < csv_dataList[i].size(); j++){
			tmp_data_array.push_back(stof(csv_dataList[i][j]));
		}
		csv_data_array.push_back(tmp_data_array);
	}

	//creates an vector array per accellerometer and stores them in a vector of vectors
	for(int i = gyro_on_row + 2; i < gyro_on_row + nr_of_gyroscopes + 2; i++){
		vector<float> tmp_data_array;
		for(int j = 0 ; j < csv_dataList[i].size(); j++){
			tmp_data_array.push_back(stof(csv_dataList[i][j]));
		}
		csv_data_array.push_back(tmp_data_array);
	}

	//debug
	if(VERBOSE){
		for(int i = 0; i < csv_data_array.size(); i ++){
			cout << " Data entry " << i << ", csv entries: " << endl;
			for(int j = 0; j < csv_data_array[i].size(); j++){
				cout << csv_data_array[i][j] << " ";
			}
			cout << endl << endl;
		}
	}

}

//choose which parameters to modify
void ParameterData::ParameterSelection()
{
	ostringstream oss;
	//fill in CALL_ACC<sensor_nr>_D<row><col>
	for(int i = 0; i < nr_of_accelerometers; i++){
		vector<string> tmp_param_array;
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 3; k++){
				oss.str("");
				oss.clear();

				oss << "CAL_ACC" << i << "_D" << j << k;
				tmp_param_array.push_back(oss.str());
			}
		}
		selected_params.push_back(tmp_param_array);
	}

	for(int i = 0; i < nr_of_gyroscopes; i++){
		vector<string> tmp_param_array;
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 3; k++){
				oss.str("");
				oss.clear();

				oss << "CAL_GYR" << i << "_D" << j << k;
				tmp_param_array.push_back(oss.str());
			}
		}
		selected_params.push_back(tmp_param_array);
	}

	//DEBUG
	if(VERBOSE){
		for(int i = 0; i < selected_params.size(); i ++){
			for(int j = 0; j < selected_params[i].size(); j++){
				cout << selected_params[i][j] << ", ";
			}
			cout << endl ;
		}
	}

}

void ParameterData::WriteParamToFile(){
	//output file
	ofstream output_file("pixhawk4_updated.params");

	output_file <<	"# Onboard parameters for Vehicle 1/n" << endl;
	output_file <<	"#" << endl;
	output_file <<	"# Stack: PX4 Pro/n" << endl;
	output_file <<	"# Vehicle: Multi-Rotor/n" << endl;
	output_file <<	"# Version: 1.11.0 dev" << endl;
	output_file <<	"# Git Revision: a2ba0b9982000000" << endl;
	output_file <<	"#" << endl;
	output_file <<	"# Vehicle-Id Component-Id Name Value Type" << endl;

	if(VERBOSE)cout << endl;

	for(int x = 0; x < selected_params.size(); x++){
		if(VERBOSE)cout << "Sensor nr : " << x << endl;
		for(int i = 0; i < selected_params[x].size(); i++){
			ostringstream oss;
			oss << "1	1	"<< selected_params[x][i] << "	" << csv_data_array[x][i] << "	9" << endl;
			output_file << oss.str();
			if(VERBOSE)cout << oss.str();
		}
		if(VERBOSE)cout << endl;
	}
}

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
vector<vector<string> > CSVReader::getData()
{
	ifstream file(fileName);

	vector<vector<string> > dataList;

	string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		vector<string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}


int main()
{
  //actual csv file with params from matlab
	ParameterData parameter_data;
	parameter_data.ReadMatlabCSV();
	parameter_data.ParameterSelection();
	parameter_data.WriteParamToFile();

 	return 0;
}
