#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include <boost/algorithm/string.hpp>

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

/*
* Reads csv file from henk with the parameters coming from matlab
*/
vector<float> read_matlab_csv()
{
  CSVReader csv_reader("henk_csv.csv", ",");
	vector<vector<string> > csv_dataList = csv_reader.getData();
	vector<float> param_data_array;
	int rownr = 0;

	for(int i = 0; i < csv_dataList.size(); i++){
		if(csv_dataList[i][0].compare("Accelerometer calibration parameters:") == 0){
			for(int j = 0; j < csv_dataList[i + 2].size(); j++){
				param_data_array.push_back(stof(csv_dataList[i + 2][j]));
			}
		}
	}
	return param_data_array;
}

//choose which parameters to modify
vector<string> parameter_selection(int sensor_nr)
{
	vector<string> selected_params;
	ostringstream oss;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			oss.str("");
			oss.clear();

			oss << "CAL_ACC" << sensor_nr << "_D" << i << j;
			selected_params.push_back(oss.str());
		}
	}
	return selected_params;
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
  int sensor_nr;
  int rownr;
  //sensor selection
  cout << "Type the sensor number: ";
  cin >> sensor_nr;

  //actual csv file with params from matlab
	vector<float> param_data_array = read_matlab_csv();

	//define the parameters which need to be set
	vector<string> selected_params = parameter_selection(sensor_nr);

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

	for(int i = 0; i < selected_params.size(); i++){
		ostringstream oss;
		oss << "1	1	"<< selected_params[i] << "	" << param_data_array[i] << "	9" << endl;
		output_file << oss.str();
	}

 	return 0;
}
