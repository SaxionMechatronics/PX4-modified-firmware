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
vector<float > read_matlab_csv()
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

	for(int i = 0; i < param_data_array.size(); i++){
		cout << param_data_array[i] << endl;
	}

	return param_data_array;
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

  // //actual csv file with params from matlab
	vector<float> param_data_array = read_matlab_csv();


	// Creating an object of CSVWriter
	CSVReader param_reader("pixhawk4.params", "	");
	vector<vector<string> > param_dataList = param_reader.getData();

  //output file
  ofstream output_file("pixhawk4_updated.params");
	// Get the data from CSV File


	for(vector<string> vec : param_dataList)
	{
		ostringstream oss;
		oss << "CAL_ACC" << sensor_nr << "_XSCALE";
		// if(vec[2].compare(oss.str()) == 0) //value is X_SCALE
		// {
		// 	for(int i = 0; i < (vec.size() - 1); i++)
		// 	{
		// 		if(i != 3){
		// 			output_file << vec[i] << "	" ;
		// 		}else{
		// 			// output_file << csv_dataList[8][0] << "	";
		// 		}
		// 	}
			output_file << vec[vec.size() - 1] << endl; //skip space for last element
		{
			for(int i = 0; i < (vec.size() - 1); i++)
			{
				output_file << vec[i] << "	" ;
			}
			output_file << vec[vec.size() - 1] << endl; //skip space for last element
		}
 }
 	return 0;
}
