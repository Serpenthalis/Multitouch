#include "EventListener.h"

EventListener::EventListener()
{
}

EventListener::~EventListener()
{
}

void EventListener::addTuioCursor(TuioCursor* tcur)
{
	tuioCursors.push_back(tcur);
	cout << "cursor added" << endl;
}

void EventListener::updateTuioCursor(TuioCursor* tcur)
{
	int tcurID = tcur->getCursorID();

	for (unsigned int i = 0; i < tuioCursors.size(); i++)
	{
		if (tuioCursors.at(i)->getCursorID() == tcurID)
		{
			replace(tuioCursors.begin() + i, tuioCursors.begin() + i, tuioCursors.at(i), tcur);
			cout << "cursor updated" << endl;
			i = tuioCursors.size();
		}
	}
}

void EventListener::removeTuioCursor(TuioCursor* tcur)
{
	int tcurID = tcur->getCursorID();

	for (unsigned int i = 0; i < tuioCursors.size(); i++)
	{
		if (tuioCursors.at(i)->getCursorID() == tcurID)
		{
			tuioCursors.erase(tuioCursors.begin() + i);
			cout << "cursor removed" << endl;
			i = tuioCursors.size();
		}
	}
}

void EventListener::addTuioObject(TuioObject* tobj)
{
	tuioObjects.push_back(tobj);
	cout << "object added" << endl;
}

void EventListener::updateTuioObject(TuioObject* tobj)
{
	int tobjID = tobj->getSymbolID();

	for (unsigned int i = 0; i < tuioObjects.size(); i++)
	{
		if (tuioObjects.at(i)->getSymbolID() == tobjID)
		{
			replace(tuioObjects.begin() + i, tuioObjects.begin() + i, tuioObjects.at(i), tobj);
			cout << "object updated" << endl;
			i = tuioObjects.size();
		}
	}
}

void EventListener::removeTuioObject(TuioObject* tobj)
{
	int tobjID = tobj->getSymbolID();

	for (unsigned int i = 0; i < tuioObjects.size(); i++)
	{
		if (tuioObjects.at(i)->getSymbolID() == tobjID)
		{
			tuioObjects.erase(tuioObjects.begin() + i);
			cout << "object removed" << endl;
			i = tuioObjects.size();
		}
	}
}

void EventListener::refresh(TuioTime ftime)
{
	//cout << "refresh" << endl;
}
