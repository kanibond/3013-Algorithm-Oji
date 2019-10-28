///////////////////////////////////////////////////////////////////////////////
//
// Date:             28 October 2019
// Title:            Program 3 - Graphs
// Semester:         Spring 2018
// Course:           CMPS 3013 
// Assignment:       A05
// Author:           Kanayo Oji
// Email:            ojibond150@gmail.com
// Files:            graph_builder.cpp, cities.json, heap.hpp, json.hpp, 
//                   jsonFacade.hpp
// Description:
//     This program finds the closest distance between 2 or more locations/cities.
//     Using a Heap, values are stored in a struct as Edges and these
//     values are used to find the shortest distance between a certain
//     number of locations (cities). User determines the number of location using
//     the cmd
///////////////////////////////////////////////////////////////////////////////// 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include "Heap.hpp"
#include "JsonFacade.hpp"
#include "json.hpp"
#include "Geo.hpp"


using namespace std;

using json = nlohmann::json;

struct City {
	float longitude; //used to calculate distance
	float latitude;
	long double Priority; //used to check for priority
	string Name;

	City() 
	{
		Priority = 0;
		Name = "";
	}
	City(json obj) {
		Name = obj["city"];
		latitude = obj["latitude"];
		longitude = obj["longitude"];
		Priority = 0; //priority is most with min distance
	}
};
struct path {
	int type;
	double Distance;

	path() 
	{
		type = 0;
		Distance = 0.0;
	}
	path(double D) {
		Distance = D;
	}
};

int main(int argc, char *argv[]) {
	json obj;
	City** Cities;
	City* temp; //create a temp value pointing to city
	string filename = "cities.json";
	JsonFacade J(filename);
	int degree;
	int tag = 0; //keeps count of different cities
	int inner_tag = 0;
	Heap <City> H(1000, false);

	ofstream out;
		out.open("output.txt");	

cout<<"Enter a number of cities to be checked: ";
cin >> degree;

	int size = J.getSize();
	Cities = new City *[size];

	// loads arrays of cities
	for (int i = 0; i < size; i++) 
	{
		obj = J.getNext();
		Cities[i] = new City(obj);
	}

	//Loop through cities and calculate the distance
	for (int i = 0; i < size; i++)
	{
		tag++;
		if (i < 10 || i >= size - 10)
		{
			out << setfill('0') << setw(4) << tag << ":" << Cities[i]->Name << endl;

			for (int j = 0; j < size; j++)
			{
				Coordinate p1(Cities[i]->latitude, Cities[i]->longitude);
				Coordinate p2(Cities[j]->latitude, Cities[j]->longitude);
				//Set priority of cities
				Cities[j]->Priority = HaversineDistance(p1, p2);
				H.Insert(Cities[j]);
			}
			//When done, empty the Heap
			for (int x = 0; x < size; x++)
			{
				City* temp = H.Extract();
				if (x > 0 && x < degree + 1)
				{
					inner_tag++;
					out << setfill(' ') << setw(2) << " " << inner_tag << "\) "<< temp->Name << " " << temp->Priority << endl;				
				}	
			}
			inner_tag = 0;

			out << endl << endl;
		}
	}
	return 0;
}
