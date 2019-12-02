#include "Blob.h"

Blob::Blob()
{
	id = 0;
	position = Point(0, 0);
	distance = BLOB_DEFAULT_DISTANCE;
	active = true;
	lostFor = 0;
}

Blob::~Blob()
{

}

int Blob::getID(void)
{
	return id;
}

void Blob::setID(int id)
{
	this->id = id;
}

Point Blob::getPosition(void)
{
	return position;
}

void Blob::setPosition(Point position)
{
	this->position = position;
}

double Blob::getDistance(void)
{
	return distance;
}

void Blob::setDistance(double distance)
{
	this->distance = distance;
}

bool Blob::getActive(void)
{
	return active;
}

void Blob::setActive(bool active)
{
	this->active = active;
}

int Blob::getLostFor(void)
{
	return lostFor;
}

void Blob::setLostFor(int lostFor)
{
	this->lostFor = lostFor;
}