/**
-----------------------------------------
-----------------------------------------
A program implementing Prims Algorithm
in order to find the shortest path between
between all cities
Author: Christian Davidson
Date: November 5th 2011
-----------------------------------------
----------------------------------------- 
*/


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <cmath>
using namespace std;

struct city
{
	string name;
	double longi;
	double lat;
	bool key;
	int parentIndex;
        city(string tempname = "", double templong = 0, double templat = 0, bool tempKey = false)
        {
		name = tempname;
		longi = templong;
		lat = templat;
		key = tempKey;
		parentIndex = -1;
        }
	
};
         
        
int main (int argc, const char * argv[])
{
//Reading in various
//user information
         
        cout << "What is the name of the cities file?" << endl;
        string input;
        cin >> input;
                

//Initializing variables
//and reading information in from the file         
  
	fstream inFile(input.c_str());
        char tempChar;
	string tempString = "";
	string structString = "";
        double structLong = 0;
	double structLat = 0;
	int index = 0;
	city cityArray[1000];	
	if (!inFile)
	{
        	cout << "Unable to open file.";
        	exit(1);
        }
        while(inFile.peek() != EOF)
        {

//Getting
//the String
//for city
		tempChar = inFile.get();		
                while(tempChar != '\t')
		{
			tempString += tempChar;
			tempChar = inFile.get();
		}
		structString = tempString;
		tempString = "";
//Getting
//the Longitude
		tempChar = inFile.get();
                while(tempChar != '\t')     
                {
                        tempString += tempChar;
                        tempChar = inFile.get();
                }
		std::istringstream stm;
		stm.str(tempString);
		stm >> structLong;
//Getting
//the Latitude
		tempString = "";
		tempChar = inFile.get();
                while(tempChar != '\n')     
                {
                        tempString += tempChar;
                        tempChar = inFile.get();
                }
		stm.clear();		
		stm.str(tempString);
		stm >> structLat;
//Creating the
//city structure
//and putting it
//in an array

		cityArray[index] = city(structString, structLong, structLat, false);
		index++;
		tempString = "";
        }
        inFile.close();

//Creating a matrix
//to hold the distance values
//of a complete graph between all cities

	double distArray[index][index];
	for(int j = 0; j < index; j++)
	{
		city tempCity1 = cityArray[j];
		for(int i = 0; i < index; i++)
		{
			city tempCity2 = cityArray[i];
			distArray[j][i] = sqrt(pow(cityArray[i].longi - cityArray[j].longi, 2) + pow(cityArray[i].lat - cityArray[j].lat, 2));
		}
	}	

//Creating and initializing
//our Adjacency Matrix	
	int AdjMatrix[index][index];
	for(int j = 0; j < index; j++)
	{
		for(int i = 0; i < index; i++)
			AdjMatrix[j][i] = 0;
	}



//Starting
//with the
//first city

	int numEdges = 0;
	int tempIndex1 = 0;
	int tempIndex2 = 0;
	cityArray[0].key = true;

//Creating a completed graph
//implementing Prims algorithm

	while(numEdges < index - 1)
	{
		double min = 9999;
		for(int j = 0; j < index; j++)
		{
			if(cityArray[j].key != false)
			{
				for(int i = 0; i < index; i++)
				{
					if(distArray[j][i] < min && distArray[j][i] != 0 && cityArray[i].key != true)
					{
						min = distArray[j][i];
						tempIndex1 = j;
						tempIndex2 = i;
					}
				}
			}
		}

//Changing the key
//value to true and
//removing the edge used from
//the weights and adding it to the
//Adjacency Matrix

		cityArray[tempIndex2].parentIndex = tempIndex1;
		cityArray[tempIndex2].key = true;
		distArray[tempIndex1][tempIndex2] = 0;
		distArray[tempIndex2][tempIndex1] = 0;
		AdjMatrix[tempIndex1][tempIndex2] = 1;
		AdjMatrix[tempIndex2][tempIndex1] = 1;
		numEdges++;
	}
	
//Printing out
//the Adjacency Matrix
//into a file named PrimsOut

	ofstream outFile;
	outFile.open("PrimsOutAdj", ios::out | ios::binary);
        for(int j = 0; j < index; j++)
        {
                for(int i = 0; i < index; i++)
                        outFile << AdjMatrix[j][i] << " ";
		outFile << endl;  
        }
	outFile.close();

	double totalLength = 0;
	outFile.open("PrimsOutCit", ios::out | ios::binary);
	int tempIndex = -1;
        for(int j = 0; j < index; j++)
        {
		tempIndex = cityArray[j].parentIndex;
		outFile << "Original City: ";
		if(tempIndex == -1)
			outFile << "None" << endl;
                else
			outFile << cityArray[tempIndex].name << "(" << cityArray[tempIndex].lat << ", " << cityArray[tempIndex].longi << ")" << endl;
		outFile << "Now at: ";
		outFile << cityArray[j].name << "(" << cityArray[j].lat << ", " << cityArray[j].longi << ")" << endl;
		outFile << "Going to: ";
         	for(int i = 0; i < index; i++)
		{
			if(AdjMatrix[j][i] == 1)
			{
                		outFile << cityArray[i].name << "(" << cityArray[i].lat << ", " << cityArray[i].longi << ")";
                		totalLength = sqrt(pow(cityArray[j].longi - cityArray[i].longi, 2) + pow(cityArray[j].lat - cityArray[i].lat, 2));
				outFile << " with distance: " << totalLength << endl;
				outFile << "          ";
			}
		}
		outFile << endl;
        }
	outFile.close();
		

		
}
