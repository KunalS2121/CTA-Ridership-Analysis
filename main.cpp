//
// CTA "L" Ridership Analysis 2: main program file
//
// Kunal Shah
// U. of Illinois, Chicago
// CS341, Spring 2016
// HW #3
//
#include <iostream>
#include <string>

#include<iomanip>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>

using namespace::std;

using std::sort;
using std::string;
using std::fstream;
using std::vector;
using std::stringstream;
using std::getline;
using std::cout;
using std::endl;
using std::cin;




//Class will keep track of each line's several stops
class Stop
{
public:

	string stopName; //Name of the stop

	string stopDirection; //Direction that the stop is running
	string stopHandicap; //Boolean int to see if the stop is handicap accessable


	vector<string> stopService; //Will hold the stops that are serviced.


								//Constructor for the Stop class
	Stop(string name, string direction, string handicap, vector<string>service)
		: stopName(name), stopDirection(direction), stopService(service)
	{
		//Setting handicap access
		if (handicap.compare("TRUE"))
		{
			stopHandicap = "Handicap Access";
		}
		else
		{
			stopHandicap = "*No* Handicap Access";
		}


	}
};

//Class will keep track of the seperate stations
class Station
{
public:
	int stationID; //ID of the station
	string stationName; //Name of the station

	int U; //Ridership for the station on Sunday/Holidays
	int A; //Ridership for the station on Saturday
	int W; //Ridership for the station on Weekdays

	vector<Stop> stationStops; //List of stops for the specific station



							   //Constructor for the Station class which only initilizes the ID and the day type
	Station(int id, string name)
		: stationID(id), stationName(name), U(0), A(0), W(0)
	{}


};


//Input data for "CTA-L-Stops"
void InputDataA(vector<Station> &stations, int &numStops, int &numStations)
{
	string filename = "CTA-L-Stops.csv";

	//Create a line for the L-Stops
	std::fstream file(filename.c_str());
	std::string line;

	if (!(file.good()))
	{
		throw std::invalid_argument("File '" + filename + "' is not found!");
	}

	std::getline(file, line); //Discard the first line

	while (std::getline(file, line))
	{
		std::stringstream ss(line);

		std::string ID, direction, stop_name, station_name, ada;
		std::string red, blue, grn, brn, purp, purXP, yel, pink, orng, junk;

		std::getline(ss, ID, ',');
		std::getline(ss, direction, ',');
		std::getline(ss, stop_name, ',');
		std::getline(ss, station_name, ',');
		std::getline(ss, ada, ',');

		//Get all the colors
		std::getline(ss, red, ','), std::getline(ss, blue, ','), std::getline(ss, grn, ',');
		std::getline(ss, brn, ','), std::getline(ss, purp, ','), std::getline(ss, purXP, ',');
		std::getline(ss, yel, ','), std::getline(ss, pink, ','), std::getline(ss, orng, ',');
		std::getline(ss, junk);


		int currentID = std::stoi(ID);

		//std::cout << currentID << ", " << stop_name << ", " << stop_name << endl;

		//Loop through to see if it is the first instance of this station or the second, or third, etc..
		auto result = std::find_if(stations.begin(), stations.end(),
			[&](Station &station)
		{

			if (station.stationID == currentID)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		);

		//If it is the first instance of this station...
		if (result == stations.end())
		{
			//Initilize a new station
			Station s(currentID, station_name);

			//Create list of serviced stops
			vector<string>services;

			services.push_back(red), services.push_back(blue), services.push_back(grn);
			services.push_back(brn), services.push_back(purp), services.push_back(purXP);
			services.push_back(yel), services.push_back(pink), services.push_back(orng);


			//Add stop to the stop list
			Stop stop(stop_name, direction, ada, services);
			s.stationStops.push_back(stop);

			//Add the station to stations vector
			stations.push_back(s);
			//Increment numStations and numStops
			numStations++;
			numStops++;


		}
		else//If it is not the first instance, obtain station via iterator and add a stop to the station list
		{

			//Obtain the correct station from iterator
			Station s = *result;


			//Create list of serviced stops
			vector<string>services;

			services.push_back(red), services.push_back(blue), services.push_back(grn);
			services.push_back(brn), services.push_back(purp), services.push_back(purXP);
			services.push_back(yel), services.push_back(pink), services.push_back(orng);



			//Add stop to the stop list
			Stop stop(stop_name, direction, ada, services);
			s.stationStops.push_back(stop);

			//Set the iterator reference back to s
			*result = s;

			Station checker = *result;

			//cout << checker.stationName << ", " << checker.stationID << endl;

			//Increment only num stops
			numStops++;
		}


	}//end of while
}//end of function



 //Inputs data into vector via CTA-L-Ridership-Daily
void InputDataB(vector<Station> &stations)
{

	string filename = "CTA-L-Ridership-Daily.csv";

	//Create a line for the daily ridership
	std::fstream file(filename.c_str());
	std::string	line;


	if (!(file.good()))
	{
		throw std::invalid_argument("File '" + filename + "' is not found!");
	}

	std::getline(file, line);

	while (std::getline(file, line))
	{
		//Initilize stream stream for B
		std::stringstream ss(line);

		std::string ID_2, junk4, dayType, numRides, junk5;

		std::getline(ss, ID_2, ','); //Will get the station ID***
		std::getline(ss, junk4, ','); //Will get the date, which is not needed
		std::getline(ss, dayType, ',');//Will get the type of day***
		std::getline(ss, numRides, ',');//Will get number of rides***
		std::getline(ss, junk5);//Will get the rest of the line, just in case.

		int currentID = std::stoi(ID_2); //Convert string of ID to int
		int rides = std::stoi(numRides); //Convert string of numRides to int

										 //Update total riderhsip



		for (Station &s : stations)
		{
			int id = s.stationID;

			//If the station's ID is the same as the current ID...
			if (id == currentID)
			{
				//Start comparing the dayType string with each of the possible days to figure out which type of data it is for the station.
				if (!(dayType.compare("U"))) //Returns 0 if they are equal
				{
					s.U += rides; //Add number of rides to the Sunday/Holiday catigory of ridership
				}

				if (!(dayType.compare("A"))) //Returns 0 if they are equal
				{
					s.A += rides; //Add number of rides to the Saturday catigory of ridership
				}

				if (!(dayType.compare("W"))) //Returns 0 if they are equal
				{
					s.W += rides; //Add number of rides to the Weekday catigory of ridership
				}
			}


		}//For loop

	}//while loop


}//end of functoin


 //Checks if the input color string is valid
int checkInputColor(string color)
{
	if (color.compare("Blue") == 0)
	{
		return 1;
	}
	if (color.compare("Red") == 0)
	{
		return 1;
	}
	if (color.compare("Green") == 0)
	{
		return 1;
	}
	if (color.compare("Brown") == 0)
	{
		return 1;
	}
	if (color.compare("Purple") == 0)
	{
		return 1;
	}
	if (color.compare("Purple Exp") == 0)
	{
		return 1;
	}
	if (color.compare("Yellow") == 0)
	{
		return 1;
	}
	if (color.compare("Pink") == 0)
	{
		return 1;
	}
	if (color.compare("Orange") == 0)
	{
		return 1;
	}

	return 0;
}


//Calculates the line data to be outputted before the line info
void calculateLineData(int &weekday, int &saturday, int &sunday
	, string &filename, vector<Station> &stationList
	, long long int &total)
{
	fstream file(filename.c_str());
	string line;

	double temp = 0.00;
	//Go through the line.txt file
	while (getline(file, line))
	{
		stringstream ss(line);
		string ID;

		getline(ss, ID);

		int currentID = stoi(ID);

		//Loop through the stations and add up the ridership for saturday, weekday, and sunday
		for (Station &s : stationList)
		{
			if (s.stationID == currentID)
			{
				weekday += s.W;
				saturday += s.A;
				sunday += s.U;
			}
		}

	}


	//percent = (double)((temp / total));

}

//Print out the lines serviced by each station
//Prints alphabetically
void printServiced(vector<string> &stopList)
{
	//A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z		
	if (stopList[1].compare("TRUE") == 0)
	{
		cout << ", Blue";
	}
	if (stopList[3].compare("TRUE") == 0)
	{
		cout << ", Brown";
	}
	if (stopList[2].compare("TRUE") == 0)
	{
		cout << ", Green";
	}
	if (stopList[8].compare("TRUE") == 0)
	{
		cout << ", Orange";
	}
	if (stopList[7].compare("TRUE") == 0)
	{
		cout << ", Pink";
	}
	if (stopList[4].compare("TRUE") == 0)
	{
		cout << ", Purple";
	}
	if (stopList[5].compare("TRUE") == 0)
	{
		cout << ", Purple Express";
	}
	if (stopList[0].compare("TRUE") == 0)
	{
		cout << ", Red";
	}
	if (stopList[6].compare("TRUE") == 0)
	{
		cout << ", Yellow";
	}

}

//Count the number of stations within the specified line
int countNumStations(string &color)
{
	string filename = color + ".txt";

	fstream file(filename.c_str());
	string line;

	
	if (!(file.good()))
	{
		throw std::invalid_argument("File '" + filename + "' is not found!");
	}

	int counter = 0;
	while (getline(file, line))
	{
		counter++;
	}
	return counter;
}

//Outputs information about the stations and the stops within those statoins.
void lineInfo(string &color, vector<Station> &stationList, long long int &totalRidership)
{
	int numStations = 0;
	int weekdayRidership = 0;
	int saturdayRidership = 0;
	int sundayRidership = 0;
	double percentTotal = 0.00;

	string filename = color + ".txt";

	//Calculates the line data that will be displayed before the list of stations
	calculateLineData(weekdayRidership, saturdayRidership, sundayRidership, filename, stationList, totalRidership);
	percentTotal = ((double)(weekdayRidership + saturdayRidership + sundayRidership) / totalRidership) * 100;

	//Calculate the number fo stations
	numStations = countNumStations(color);
	
	cout << endl;
	cout << "** Line contains: " << numStations << " stations" << endl;
	cout << "** Weekday Ridership: " << '\t' << weekdayRidership << endl;
	cout << "** Saturday Ridership: " << '\t' << saturdayRidership << endl;
	cout << "** Sunday/Holiday Ridership: " << '\t' << sundayRidership << endl;
	cout << "** % of Total Ridership: " << '\t' << percentTotal << "%" << endl;




	//Create line for the info

	fstream file(filename.c_str());
	string line;

	if (!(file.good()))
	{
		throw std::invalid_argument("File '" + filename + "' is not found!");
	}





	int counter = 0;

	//Loops through the file containing the station IDs
	while (getline(file, line))
	{
		stringstream ss(line);

		string ID;

		getline(ss, ID); //Will get the id from the file

		int currentID = stoi(ID);

		auto result = std::find_if(stationList.begin(), stationList.end(),
			[&](Station station)

		{
			if (station.stationID == currentID)
			{
				return true;
			}
			else
			{
				return false;
			}


		}

		);

		//For some reason that the ID isn't found in the vector, then error.
		if (result == stationList.end())
		{
			cout << "**ERROR" << endl;
		}
		else //IF found
		{
			Station s = *result; //Obtain access to the station via the * operator on the iterator

			cout << endl;
			cout << counter + 1 << ". " << s.stationName << endl; //Output the station name
			for (Stop &stop : s.stationStops)//Loop through the stop vector contained within that station
			{
				cout << '\t' << stop.stopName << ": " << stop.stopDirection << ", " << stop.stopHandicap;
				printServiced(stop.stopService); //Print out the serviced stops
				cout << endl;
			}



		}

		counter++;
	}


}













int main()
{
	vector<Station> stationList;

	int numStops = 0;
	int numStations = 0;
	long long int totalRidership = 0;

	//Functions that will input desired data into the vector
	InputDataA(stationList, numStops, numStations);
	InputDataB(stationList);

	//Obtain total ridership total
	for (Station &s : stationList)
	{

		totalRidership = totalRidership + s.A + s.U + s.W;

	}


	//Begin printing out the format
	cout << "** CTA L RIDERSHIP ANALYSIS PROGRAM **" << endl << endl;

	cout << "**Inputting Data..." << endl;
	cout << '\t' << "Num Stations: " << numStations << endl;
	cout << '\t' << "Num Stops:    " << numStops << endl;
	cout << '\t' << "Total Ridership:  " << totalRidership << endl;


	cout << "** Ready" << endl;


	std::string color;


	cout << " <<Please enter an L line (eg. Blue), or ENTER to quit: ";
	std::getline(cin, color);
	//While there is no space input, keep looping
	while (color.compare("") != 0)
	{
		//If the inputcolor is not valid, then print error
		if (checkInputColor(color) != 1)
		{
			cout << "**Error unable to open '" << color << ".txt', incorrect L Line?" << endl;
		}

		//Print the line info for that color
		lineInfo(color, stationList, totalRidership);

		//Prompt again for input
		cout << " <<Please enter an L line (eg. Blue), or ENTER to quit: ";
		std::getline(cin, color);

		if (checkInputColor(color) != 1)
		{
			cout << "**Error unable to open '" << color << ".txt', incorrect L Line?" << endl;
		}
	}





}


