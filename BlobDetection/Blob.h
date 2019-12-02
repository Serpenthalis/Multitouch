#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "TuioServer.h"
//#include "TuioTime.h"
//#include "TuioCursor.h"
#include <cmath>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define WAIT_KEY 20
#define BLOB_MAX_SIZE 20
#define TEXT_SIZE 0.65
#define BLOB_MAX_DISTANCE 30
#define BLOB_DEFAULT_DISTANCE 9999
#define BLOB_LOST_TOLERANCE 2

using namespace std;
using namespace cv;

class Blob
{
private:
	int id;
	Point position;
	double distance;
	bool active;
	int lostFor;

public:
	Blob();
	~Blob();
	int getID(void);
	void setID(int);
	Point getPosition(void);
	void setPosition(Point);
	double getDistance(void);
	void setDistance(double);
	bool getActive(void);
	void setActive(bool);
	int getLostFor(void);
	void setLostFor(int);
};
