#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "TuioServer.h"
#include "TuioTime.h"
#include "TuioCursor.h"
#include <TuioListener.h>
#include "TuioClient.h"
#include <cmath>
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;
using namespace TUIO;

class EventListener : public TuioListener
{
private:
	vector<TuioCursor*> tuioCursors;
	vector<TuioObject*> tuioObjects;

public:
	EventListener();
	~EventListener();
	void addTuioCursor(TuioCursor* tcur);
	void updateTuioCursor(TuioCursor* tcur);
	void removeTuioCursor(TuioCursor* tcur);
	void addTuioObject(TuioObject* tobj);
	void updateTuioObject(TuioObject* tobj);
	void removeTuioObject(TuioObject* tobj);
	void refresh(TuioTime ftime);
};

