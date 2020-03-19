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
vector<vector<float>> read_matlab_csv(int *nr_of_accelerometers_p)
{
  CSVReader csv_reader("henk_csv_multiple_senors.csv", ",");
	vector<vector<string> > csv_dataList = csv_reader.getData();
	vector<vector<float> > param_data_array;
	int accel_on_row  = 0;
	int nr_of_accelerometers;

	for(int i = 0; i < csv_dataList.size(); i++){
		if(csv_dataList[i][0].compare("Accelerometer calibration parameters:") == 0){
			//Found on row in the csv file
			accel_on_row = i;
		}

		if(csv_dataList[i][0].compare("Gyroscope calibration parameters:") == 0){
			//rownr of accelerometer - rownr gyro -2 definition rows = nr of sensors
			nr_of_accelerometers = (i - accel_on_row - 2);
		}
	}

	*nr_of_accelerometers_p = nr_of_accelerometers;

	for(int i = 0; i < (csv_dataList.size() - 2); i++){
		cout << i << endl;
		if(csv_dataList[i][0].compare("Accelerometer calibration parameters:") == 0){
			cout<<"HIT!" << endl;
			for(int j = 0; j < nr_of_accelerometers; j++){
				cout << "I: " << i << endl;
				cout << "J: " << j << endl;
				for(int k = 0; k < csv_dataList[i + 2].size(); k++){
					cout << "K: " << k << "endl";
					param_data_array[0].push_back(stof(csv_dataList[i + 2][k]));
				}
			}
		}
	}

	cout << "leaving.." << endl;
	return param_data_array;
}

//choose which parameters to modify
vector<vector<string> > parameter_selection(int nr_of_sensors)
{
	vector<vector<string> > selected_params;
	ostringstream oss;
	//fill in CALL_ACC<sensor_nr>_D<row><col>
	for(int x = 0; x < nr_of_sensors; x++){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				oss.str("");
				oss.clear();

				oss << "CAL_ACC" << x << "_D" << i << j;
				selected_params[x].push_back(oss.str());
			}
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
  int nr_of_accelerometers = 0;


  //actual csv file with params from matlab
	vector<vector<float>> param_data_array = read_matlab_csv(&nr_of_accelerometers);
	cout << "Number of accelerometers : " << nr_of_accelerometers << endl;

	//define the parameters which need to be set
	vector<vector<string>> selected_params = parameter_selection(nr_of_accelerometers);

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

	for(int x = 0; x < nr_of_accelerometers; x++){
		for(int i = 0; i < selected_params.size(); i++){
			ostringstream oss;
			oss << "1	1	"<< selected_params[x][i] << "	" << param_data_array[x][i] << "	9" << endl;
			output_file << oss.str();
		}
	}

 	return 0;
}
