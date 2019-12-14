// BlobDetection.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "Blob.h"

using namespace std;
using namespace cv;

static vector<Blob*> blobs;
static unsigned int maxID = 0;
static TUIO::TuioServer* server = new TUIO::TuioServer();

static String nearestNeighbour(Point currentCenter, int videoWidth, int videoHeight);
static void calculateDistances(Point currentCenter);
static void reset();
static void cleanUp();
static void sendTUIOevents();

int main(int argc, char** argv)
{
	//Try to open the captured footage
	VideoCapture cap;
	cap.open("..\\mt_camera_raw.AVI");
	if (cap.isOpened() == true)
	{
		//Try to get the image that needs to be substracted from every frame
		//Mat subtrahend = imread("..\\subtrahend.jpg", 1);
		Mat subtrahend;
		cap >> subtrahend;

		int videoWidth = subtrahend.cols;
		int videoHeight = subtrahend.rows;

		if (subtrahend.data)
		{
			Mat frame;
			namedWindow("video", 1);
			for (;;)
			{
				//Get every frame of the video
				cap >> frame;
				if (!frame.data)
				{
					break;
				}

				//Get the difference between every frame minus the subtrahend
				Mat difference;
				absdiff(frame, subtrahend, difference);


				//Apply highpass filter
				Mat blurred;
				Mat highpassFiltered;
				blur(difference, blurred, Size(15, 15), Point(-1, -1));
				absdiff(difference, blurred, highpassFiltered);


				//Apply binarization
				Mat thresholdFiltered;
				threshold(highpassFiltered, thresholdFiltered, 9, 20, 0);


				//To grayscale
				Mat grayscaleThreshold;
				cvtColor(thresholdFiltered, grayscaleThreshold, COLOR_BGR2GRAY);


				//Find the contours in the frames
				vector<vector<Point>> contours;
				vector<Vec4i> hierarchy;


				findContours(grayscaleThreshold, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
				//iterate through all the top-level contours -> "hierarchy" may not be empty!
				if (hierarchy.size() > 0)
				{
					//Reset "active"-values of all blobs
					reset();

					for (int idx = 0; idx >= 0; idx = hierarchy[idx][0])
					{
						//check contour size (number of points) and area ("blob" size)
						if (contourArea(Mat(contours.at(idx))) > BLOB_MAX_SIZE && contours.at(idx).size() > 4)
						{
							RotatedRect rect = fitEllipse(Mat(contours.at(idx)));
							ellipse(frame, rect, Scalar(0, 0, 255), 1, 8);

							//draw contours
							drawContours(frame, contours, idx, Scalar(255, 0, 0), 1, 8, hierarchy);

							//Blob detection
							//draw center
							circle(frame, rect.center, 2, Scalar(0, 255, 0), 2);

							//draw IDs
							calculateDistances(rect.center);
							String blobID = nearestNeighbour(rect.center, videoWidth, videoHeight);
							putText(frame, blobID, rect.center, 0, TEXT_SIZE, Scalar(0, 255, 0));
						}
					}
					cleanUp();
				}

				//Show the edited frames in a window
				imshow("video", frame);
				if (waitKey(WAIT_KEY) >= 0)
				{
					break;
				}
			}
		}
		blobs.clear();
	}
}

//Compare the distance of two blobs and is used to sort the blobs-vector by distance
bool sortForLowestDistance(Blob* a, Blob* b)
{
	return (a->getDistance() < b->getDistance());
}

//Sorts the blobs-vector by distance. Blob with shortest distance first
static void calculateDistances(Point currentCenter)
{
	if (!blobs.empty())
	{
		for (int i = 0; i < blobs.size(); i++)
		{
			//Calculation of distance between two points in a 2D environment
			double blobDistance = std::abs(std::sqrt(std::pow(currentCenter.x - blobs.at(i)->getPosition().x, 2) + std::pow(currentCenter.y - blobs.at(i)->getPosition().y, 2)));
			blobs.at(i)->setDistance(blobDistance);
		}
	}
}

//Creates a new blob object and returns its ID as a string
static String newBlob(Point currentCenter, int videoWidth, int videoHeight)
{
	Blob* blob = new Blob();
	maxID++;
	float normalizedWidth = currentCenter.x / videoWidth;
	float normalizedHeight = currentCenter.y / videoHeight;
	TUIO::TuioCursor* cursor = new TUIO::TuioCursor(1, maxID, normalizedWidth, normalizedHeight);

	blob->setID(maxID);
	blob->setPosition(currentCenter);
	blob->setCursor(cursor);
	server->addExternalTuioCursor(cursor);
	blobs.push_back(blob);
	sendTUIOevents();

	return to_string(blob->getID());
}

//The nearest neighbour algorithm. Needs the position of the current blob. Returns the ID of the blob as a string
static String nearestNeighbour(Point currentCenter, int videoWidth, int videoHeight)
{
	if (!blobs.empty())
	{
		//Get the distance of the blob with the lowest distance to the current position
		sort(blobs.begin(), blobs.end(), sortForLowestDistance);
		double min_value = blobs.front()->getDistance();

		//Blob must be close enough and may not have been used already during this frame
		if ((min_value < BLOB_MAX_DISTANCE) && (blobs.front()->getActive() == false))
		{
			blobs.front()->setPosition(currentCenter);
			blobs.front()->setActive(true);
			blobs.front()->setLostFor(0);

			float normalizedWidth = currentCenter.x / videoWidth;
			float normalizedHeight = currentCenter.y / videoHeight;
			blobs.front()->getCursor()->getPosition().update(normalizedWidth, normalizedHeight);
			server->updateExternalTuioCursor(blobs.front()->getCursor());
			sendTUIOevents();

			return to_string(blobs.front()->getID());
		}
		else
		{
			return newBlob(currentCenter, videoWidth, videoHeight); //No fitting blob found, this must be a new one.
		}
	}
	else
	{
		return newBlob(currentCenter, videoWidth, videoHeight); //There are no preexisting blobs. Let's make a new one.
	}
}

static void sendTUIOevents()
{
	server->initFrame(TUIO::TuioTime::getSessionTime());
	server->sendFullMessages();
}

//Everything that needs to be done before analyzing a frame
static void reset()
{
	for (unsigned int i = 0; i < blobs.size(); i++)
	{
		blobs.at(i)->setActive(false);
		blobs.at(i)->setDistance(BLOB_DEFAULT_DISTANCE);
	}
}

//Everything that needs to be done after analyzing a frame
static void cleanUp()
{
	for (unsigned int i = 0; i < blobs.size(); i++)
	{
		//Is the blob lost?
		if (blobs.at(i)->getActive() == false)
		{
			//If the blob is lost for too long
			if (blobs.at(i)->getLostFor() > BLOB_LOST_TOLERANCE)
			{
				server->removeExternalTuioCursor(blobs.at(i)->getCursor());
				sendTUIOevents();
				blobs.erase(blobs.begin() + i);
				i--;
			}
			else //The blob is lost, but not long enough
			{
				blobs.at(i)->setLostFor(blobs.at(i)->getLostFor() + 1);
			}
		}
	}
}