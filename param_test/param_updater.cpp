#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <sstream>
#include <boost/algorithm/string.hpp>

/*
 * A class to read data from a csv file.
 */
class CSVReader
{
	std::string fileName;
	std::string delimeter;

public:
	CSVReader(std::string filename, std::string delm = "	") :
			fileName(filename), delimeter(delm)
	{ }

	// Function to fetch data from a CSV File
	std::vector<std::vector<std::string> > getData();
};

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);

	std::vector<std::vector<std::string> > dataList;

	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
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
  //sensor selection
  std::cout << "Type the sensor number: ";
  std::cin >> sensor_nr;

	// Creating an object of CSVWriter
	CSVReader reader("pixhawk4.params");
	// Get the data from CSV File
	std::vector<std::vector<std::string> > dataList = reader.getData();

	// Print the content of row by row on screen
	for(std::vector<std::string> vec : dataList)
	{
      for(std::string data : vec)
      {
        std::ostringstream oss;
        oss << "CAL_ACC" << sensor_nr << "_ALGN_X";
        if(data.compare(oss.str()) == 0){
              std::cout<<data << std::endl;
        }
      }
	}
	return 0;

}
