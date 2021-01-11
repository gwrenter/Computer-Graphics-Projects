//---------------------------------------
//	CSCE 4813 Programming Project #1
//	Author: Gregory Renteria
//	Code: CreateChart.cpp
//	Goal: Create a textfile that contains 
//        different graphics commands
//---------------------------------------

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

//Template to set up the number of
//data values from the text file
template <typename T>
int countElements(istream& is){
	istream_iterator<T> start(is), end;
	return distance(start,end);
}

//Function to set up the number of tickmarks in the x-axis
//depending on the number of data values entered.
//Parameters passed: filename(textfile) and dataTxt
//(series of new graphics commands).
void tickMarks(char* filename, const char* dataTxt){
	//Reading in the data file
	ifstream is(filename);
	int numValues = countElements<double>(is);
	int constNumValues = numValues;
	int index = 0;
	int avgDisplayX;
	double displayX;
	int scaleValuesTicks[100];
	while(numValues != 0){
		displayX = (numValues * 474);
		displayX = (displayX / constNumValues);
		avgDisplayX = round(displayX);
		scaleValuesTicks[index] = avgDisplayX;
		index++;
		numValues--;
	}
	
	int keepValue = constNumValues;
	
	if(constNumValues > 0){
		int keepValue = constNumValues;
		int index = keepValue-1;
		
		ofstream dout(dataTxt, ios::app);
		if(dout.is_open()){
			do{
				dout << "draw_line 1 " << scaleValuesTicks[index] << " " << "10 " 
				   	 << scaleValuesTicks[index] << " " << "16" <<  endl;
				index--;
				constNumValues--;
			}while(constNumValues != 0);
		}
		dout.close();
	}
}


//Function to create graphics commands for the column chart.
//Parameters passed: filename(textfile), arrY(y-coordinate array),
//scaleX(array created to scale some x-coordinates for the draw_polygon commands),
//scaleXCoord(array created to scale some x-coordinates for the draw_polygon commands)
void createColumnChart(char* filename, int arrY[], int scaleX[], int scaleXCoord[]){
	//default commands for background
	ofstream dout("column.txt");
	if(dout.is_open()){
		dout << "set_color 1.0 1.0 1.0" << endl;  //white color
		dout << "draw_line 1 10 16 490 16" << endl; //x axis line
		dout << "draw_line 1 16 10 16 490" << endl; //y axis line
		//evenly spaced horizontal lines
		dout << "draw_line 1 10 62 490  62" << endl;
		dout << "draw_line 1 10 108 490 108" << endl;
		dout << "draw_line 1 10 154 490 154" << endl; 
		dout << "draw_line 1 10 200 490 200" << endl;
		dout << "draw_line 1 10 246 490 246" << endl;
		dout << "draw_line 1 10 292 490 292" << endl;
		dout << "draw_line 1 10 338 490 338" << endl;
		dout << "draw_line 1 10 384 490 384" << endl;
		dout << "draw_line 1 10 430 490 430" << endl;
		dout << "draw_line 1 10 476 490 476" << endl;
	}
	else{
		cout << "Cannot open column.txt for writing" << endl;
	}
	dout.close();
	
	const char* dataTxt = "column.txt";
	
	tickMarks(filename, dataTxt);

	//to calculate the number of data values
	ifstream is(filename);
	int numValues = countElements<double>(is);
	
	int saveValue = numValues;
	
	//Writing the graphics commands for the polygons
	if(numValues > 0){
		int saveValue = numValues;
		
		ofstream dout("column.txt", ios::app);
		if(dout.is_open()){
			dout << "set_color 0.5 0.5 1.0" << endl;  //set color to blue
			do{
				dout << "draw_polygon 4 " << scaleX[numValues-1] << " " << arrY[numValues] 
				<< " " << scaleXCoord[numValues-1] << " " << arrY[numValues] << " " 
				<< scaleXCoord[numValues-1] << " 16 " << scaleX[numValues-1] << " 16" << endl;
				numValues--;
			}while(numValues != 0);
		}
		dout.close();
	}

}


//Function to create graphics commands to draw a point chart.
//Parameters passed: filename(textfile), arrX(some x-coordinates),
//arrY(some y-coordinates)
void createPointChart(char* filename, int arrX[], int arrY[]){
	//default commands for background
	ofstream dout("point.txt");
	if(dout.is_open()){
		dout << "set_color 1.0 1.0 1.0" << endl;  //white color
		dout << "draw_line 1 10 16 490 16" << endl; //x axis line
		dout << "draw_line 1 16 10 16 490" << endl; //y axis line
		//evenly spaced horizontal lines
		dout << "draw_line 1 10 62 490  62" << endl;
		dout << "draw_line 1 10 108 490 108" << endl;
		dout << "draw_line 1 10 154 490 154" << endl; 
		dout << "draw_line 1 10 200 490 200" << endl;
		dout << "draw_line 1 10 246 490 246" << endl;
		dout << "draw_line 1 10 292 490 292" << endl;
		dout << "draw_line 1 10 338 490 338" << endl;
		dout << "draw_line 1 10 384 490 384" << endl;
		dout << "draw_line 1 10 430 490 430" << endl;
		dout << "draw_line 1 10 476 490 476" << endl;
	}
	else{
		cout << "Cannot open point.txt for writing" << endl;
	}
	dout.close();
	
	const char* dataTxt = "point.txt";
	
	tickMarks(filename, dataTxt);
	
	//to calculate the number of data values
	ifstream is(filename);
	int numValues = countElements<double>(is);
	
	int saveValue = numValues;
	
	//Writing the graphics commands for the points
	if(numValues > 0){
		int saveValue = numValues;
		
		ofstream dout("point.txt", ios::app);
		if(dout.is_open()){
			dout << "set_color 0.5 1.0 0.5" << endl; //set color to green
			do{
				dout << "draw_point 6 " << arrX[numValues] << " " << arrY[numValues] << endl;
				numValues--;
			}while(numValues != 0);
		}
		dout.close();
	}
}

//Function that create graphics commands for the line chart
//Parameters passed: filename(textfile), arrX(some x-coordinate values),
//arrY(some y-coordinate values)
void createLineChart(char* filename, int arrX[], int arrY[]){
	//default commands for background
	ofstream dout("line.txt");
	if(dout.is_open()){
		dout << "set_color 1.0 1.0 1.0" << endl;  //white color
		dout << "draw_line 1 10 16 490 16" << endl; //x axis line
		dout << "draw_line 1 16 10 16 490" << endl; //y axis line
		//evenly spaced horizontal lines
		dout << "draw_line 1 10 62 490 62" << endl;
		dout << "draw_line 1 10 108 490 108" << endl;
		dout << "draw_line 1 10 154 490 154" << endl; 
		dout << "draw_line 1 10 200 490 200" << endl;
		dout << "draw_line 1 10 246 490 246" << endl;
		dout << "draw_line 1 10 292 490 292" << endl;
		dout << "draw_line 1 10 338 490 338" << endl;
		dout << "draw_line 1 10 384 490 384" << endl;
		dout << "draw_line 1 10 430 490 430" << endl;
		dout << "draw_line 1 10 476 490 476" << endl;
	}
	else{
		cout << "Cannot open line.txt for writing" << endl;
	}
	dout.close();
	
	const char* dataTxt = "line.txt";
	
	tickMarks(filename, dataTxt);
	
	//to calculate the number of data values
	ifstream is(filename);
	int numValues = countElements<double>(is);
	
	int saveValue = numValues;
	
	//Writing the graphics commands for the points
	if(numValues > 0){
		int saveValue = numValues;
		
		ofstream dout("line.txt", ios::app);
		if(dout.is_open()){
			dout << "set_color 1.0 0.5 0.5" << endl;  //set color to light red
			do{
				dout << "draw_line 6 " << arrX[numValues] << " " << arrY[numValues] << " " << arrX[numValues-1]
					 << " " << arrY[numValues-1] << endl;
				numValues--;
			}while(numValues != 1);
		}
		dout.close();
	}
}

//Function that create graphics commands for the area chart
//Parameters passed: filename(textfile), arrX(some x-coord values),
//arrY(some y-coord values)
void createAreaChart(char* filename, int arrX[], int arrY[]){
	//default commands for background
	ofstream dout("area.txt");
	if(dout.is_open()){
		dout << "set_color 1.0 1.0 1.0" << endl;  //white color
		dout << "draw_line 1 10 16 490 16" << endl; //x axis line
		dout << "draw_line 1 16 10 16 490" << endl; //y axis line
		//evenly spaced horizontal lines
		dout << "draw_line 1 10 62 490 62" << endl;
		dout << "draw_line 1 10 108 490 108" << endl;
		dout << "draw_line 1 10 154 490 154" << endl; 
		dout << "draw_line 1 10 200 490 200" << endl;
		dout << "draw_line 1 10 246 490 246" << endl;
		dout << "draw_line 1 10 292 490 292" << endl;
		dout << "draw_line 1 10 338 490 338" << endl;
		dout << "draw_line 1 10 384 490 384" << endl;
		dout << "draw_line 1 10 430 490 430" << endl;
		dout << "draw_line 1 10 476 490 476" << endl;
	}
	else{
		cout << "Cannot open area.txt for writing" << endl;
	}
	dout.close();
	
	const char* dataTxt = "area.txt";
	
	tickMarks(filename,dataTxt);
	
	//to calculate the number of data values
	ifstream is(filename);
	int numValues = countElements<double>(is);
	
	int saveValue = numValues;
	
	//Writing the graphics commands for the polygons
	if(numValues > 0){
		int saveValue = numValues;
		
		ofstream dout("area.txt", ios::app);
		if(dout.is_open()){
			dout << "set_color 1.0 0.5 1.0" << endl;  //set color to pink
			do{
				dout << "draw_polygon 4 " << arrX[numValues] << " 16 " << arrX[numValues] << " "
				     << arrY[numValues] << " " << arrX[numValues-1] << " " << arrY[numValues-1] 
					 << " " << arrX[numValues-1] << " 16" << endl;
				numValues--;
			}while(numValues != 1);
		}
		dout.close();
	}
}

//Function that handles the creation of the values for the y-coordinates.
//Parameters passed: filename(textfile), xCoord(some values for the x-coordinate),
//scaleX(scaled coordinates in case of drawing polygons), scaleXCoord(scaled coordinates in case
//of rawing polygons)
void yCoordinate(char* filename, int xCoord[], int scaleX[], int scaleXCoord[]){
	//Reading in the text file
	ifstream din;
	din.open("data.txt");
	if(!din){
		cerr << "Unable to open file data.txt" << endl;
		exit(1);
	}
	
	//Using linear interpolation formula to scale
	//the y coordinate points into the screen
	ifstream is(filename);
	int numValues = countElements<double>(is);
	int inputY;
	double displayY;
	int avgDisplayY;
	int scaleValuesY[100];
	while(din >> inputY){
		displayY = (inputY * 474);
		displayY = (displayY / 50);
		avgDisplayY = round(displayY);
		scaleValuesY[numValues] = avgDisplayY;
		numValues--;
	}
	
	//After creation of y-coordinates it calls all creation charts to write
	//the y-coordinate values into them.
	createPointChart(filename, xCoord, scaleValuesY);
	createLineChart(filename, xCoord, scaleValuesY);
	createAreaChart(filename, xCoord, scaleValuesY);
	createColumnChart(filename, scaleValuesY, scaleX, scaleXCoord);
}


//Function that handles the creation of the x-coordinate values.
//Parameters passed: filename(textfile)
void xCoordinate(char* filename){
	ifstream din;
	din.open("data.txt");
	if(!din){
		cerr << "Unable to open file data.txt" << endl;
		exit(1);
	}
	
	//to get the xCoordinates
	ifstream is(filename);
	int numValues = countElements<double>(is);
	int index = 1;
	int inputX;
	double displayX;
	int avgDisplayX;
	int constNumValues = numValues;
	int scaleX[500];
	while(numValues >= 0){
		displayX = (numValues * 474);
		displayX = (displayX / constNumValues);
		avgDisplayX = round(displayX);
		scaleX[index] = avgDisplayX;
		index++;
		numValues--;
	}
	
	//to get the xCoordinates scaled for the column graph (-11)
	int keepValue = constNumValues;
	int indexXCoord = 0;
	double displayXCoord;
	int avgDisplayXCoord;
	int scaleXCoord[100];
	while(constNumValues >= 0){
		displayXCoord = (constNumValues * 474);
		displayXCoord = (displayXCoord / keepValue);
		avgDisplayXCoord = round(displayXCoord) - 11;
		scaleXCoord[indexXCoord] = avgDisplayXCoord;
		indexXCoord++;
		constNumValues--;
	}
	
	//to get the xCoordinates scaled for the column graph(+11)
	int saveValue = keepValue;
	int indexXCoord1 = 0;
	double displayXCoord1;
	int avgDisplayXCoord1;
	int scaleXCoord1[100];
	while(keepValue >= 0){
		displayXCoord1 = (keepValue * 474);
		displayXCoord1 = (displayXCoord1 / saveValue);
		avgDisplayXCoord1 = round(displayXCoord1) + 11;
		scaleXCoord1[indexXCoord1] = avgDisplayXCoord1;
		indexXCoord1++;
		keepValue--;
	}
	
	//After creating the x-coordinate values, this function calls
	//yCoordinate function to write to append this values into data files.
	yCoordinate(filename, scaleX, scaleXCoord, scaleXCoord1);
}

//Main program
int main() {
	//User interaction to input the name of the text file
	char filename[50];
	ifstream din;
	cout << "Welcome to Graph World!" << endl;
	cout << "Please enter the name of the text file:" << endl;
	cin.getline(filename,50);
	din.open(filename);
	cout << endl;
	
	//Error checking to see if the file is valid
	while(!din.is_open()){
		cout << "Error! No such text file, please try again" << endl;
		cin.getline(filename,50);
		din.open(filename);
	}
	
	//User interaction to input the name of the chart type
	string chartChoice;
	cout << "Now, name the type of chart you would like to create:" << endl;
	cout << "(column, point, line, area)" << endl;
	cin >> chartChoice;
	
	//Error checking to see if the chart chosen is valid
	while(chartChoice != "column" && chartChoice != "point" && chartChoice != "line" && chartChoice != "area"){
		cout << "Error! Not a type of graph, please try again:" << endl;
		cin >> chartChoice;
	}
	
	//Displaying message confirmation of the type of graph selected
	if(chartChoice == "column"){
		cout << "Column graph selected" << endl;
		char* textFile = filename;
		xCoordinate(filename);
	}
	if(chartChoice == "line"){
		cout << "Line graph selected" << endl;
		char* textFile = filename;
		xCoordinate(filename);
	}
	if(chartChoice == "point"){
		cout << "Point graph selected" << endl;
		char* textFile = filename;
		xCoordinate(filename);
	}
	if(chartChoice == "area"){
		cout << "Area graph selected" << endl;
		char* textFile = filename;
		xCoordinate(filename);
	}
	
	return 0;
}
