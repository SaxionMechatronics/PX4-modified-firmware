
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <math.h>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include <boost/algorithm/string.hpp>

#define VERBOSE 1

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
	vector<vector<string> > params_with_val;

	public:
	void ReadMatlabCSV();
	void FillParameter(vector<vector<string> > csv_dataList, int sensor_on_row, int nr_of_sensors, string param_str);
	void WriteParamToFile();

};

//id's with corresponding param nr
vector<vector<int>> id_param_nr = {{4260618, 1},
																		{3866634, 0},
																		{3932170, 1},
																		{4325898, 0},
																		{396825, 1},
																		{396809, 0}};

/*
* Reads csv file from henk with the parameters coming from matlab
*/
void ParameterData::ReadMatlabCSV()
{
  CSVReader csv_reader("henk_csv.csv", ",");
	vector<vector<string> > csv_dataList = csv_reader.getData();
	int accel_on_row  = 0;
	int gyro_on_row = 0;
	int mag_on_row = 0;

	int nr_of_accelerometers;
	int nr_of_gyroscopes;
	int nr_of_magnetometers;

	for(int i = 0; i < csv_dataList.size(); i++){
		if(csv_dataList[i][0].compare("Accelerometer calibration parameters:") == 0){
			//Found on row in the csv file
			accel_on_row = i;
		}

		if(csv_dataList[i][0].compare("Gyroscope calibration parameters:") == 0){
			//rownr of accelerometer - rownr accel -2 definition rows = nr of sensors
			gyro_on_row = i;
			nr_of_accelerometers = (i - accel_on_row - 2);
		}

		if(csv_dataList[i][0].compare("Magnetometer calibration parameters:") == 0){
			//rownr of gyroscope - rownr gyro -2 definition rows = nr of sensors
			mag_on_row = i;
			nr_of_gyroscopes = (i - gyro_on_row - 2);
		}

		if(csv_dataList[i][0].compare("Kinematics:") == 0){
			//rownr of gyroscope - rownr gyro -2 definition rows = nr of sensors
			nr_of_magnetometers = (i - mag_on_row - 2);
		}
	}

	FillParameter(csv_dataList ,accel_on_row, nr_of_accelerometers, "CAL_ACC");
	FillParameter(csv_dataList ,gyro_on_row, nr_of_gyroscopes, "CAL_GYR");
	FillParameter(csv_dataList ,mag_on_row, nr_of_magnetometers, "CAL_MAG");

	if(VERBOSE){
		for(int i = 0; i < params_with_val.size(); i++){
			cout << params_with_val[i][0] << ", Value: " << params_with_val[i][1] << endl;
		}
	}
}

//choose which parameters to modify
void ParameterData::FillParameter(vector<vector<string> > csv_dataList, int sensor_on_row, int nr_of_sensors, string param_str)
{
	//creates an vector array per accellerometer and stores them in a vector of vectors
	for(int csv_row = sensor_on_row + 2; csv_row < sensor_on_row + nr_of_sensors + 2; csv_row++){
		int sensor_nr;

		//find id with corresponding parameter number
		for(int id_param_index = 0; id_param_index < id_param_nr.size(); id_param_index++){
			if(id_param_nr[id_param_index][0] == stof(csv_dataList[csv_row][0])){
				sensor_nr = id_param_nr[id_param_index][1];
			}
		}

		//Fill in parameters values and determine the correct parameter string with it
		//Dmatrix only
		for(int csv_col = 1 ; csv_col < 9; csv_col++){
			ostringstream oss;

			//divide the column index by 3 and round it down to get the row in de Dmatrix
			//Take the modulo 3 to get the column index of the Dmatrix
			oss << param_str << sensor_nr << "_D" << floor((csv_col-1)/3)
			<< (csv_col-1) % 3;

			//parse string to float and back to string to remove whitespaces(dirty solution)
			vector<string> tmp_entry = {oss.str(), to_string(stof(csv_dataList[csv_row][csv_col]))};
			params_with_val.push_back(tmp_entry);
		}
		//adding bias to list
		ostringstream oss;

		oss << param_str << sensor_nr << "_XOFF";
		params_with_val.push_back({oss.str(),
			to_string(stof(csv_dataList[csv_row][9]))});

		oss.str("");
		oss << param_str << sensor_nr << "_YOFF";
		params_with_val.push_back({oss.str(),
			 to_string(stof(csv_dataList[csv_row][10]))});

		oss.str("");
		oss << param_str << sensor_nr << "_ZOFF";
		params_with_val.push_back({oss.str(),
		to_string(stof(csv_dataList[csv_row][11]))});
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
	for(int i = 0; i < params_with_val.size(); i++){
		ostringstream oss;
		oss << "1	1	" << params_with_val[i][0] << "	" << params_with_val[i][1] << "	9" << endl;
		output_file << oss.str();
		if(VERBOSE)cout << oss.str();
	}
	if(VERBOSE)cout << endl;
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
	parameter_data.WriteParamToFile();

 	return 0;
}
