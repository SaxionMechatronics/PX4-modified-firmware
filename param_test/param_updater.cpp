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

	// Creating an object of CSVWriter
	CSVReader reader("pixhawk4.params");
  ofstream output_file("pixhawk4_updated.params");
	// Get the data from CSV File
	vector<vector<string> > dataList = reader.getData();

	// Print the content of row by row on screen
	for(vector<string> vec : dataList)
	{
      for(string data : vec)
      {
        ostringstream oss;
        oss << "CAL_ACC" << sensor_nr << "_ALGN_X";

        if(data.compare(oss.str()) == 0){
              cout << data << " " << vec[3] << endl;

              for(int i = 0; i < (vec.size() - 1); i++){
                output_file << vec[i] << "	";
              }
              output_file << vec[vec.size() - 1] << endl; //skip space for last element

        }
      }
      rownr++;
	}
	return 0;

}
