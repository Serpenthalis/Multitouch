#include "EventListener.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	EventListener myClient;
	TuioClient client(3333);
	client.addTuioListener(&myClient);
	client.connect(true);
	cout << "Jup";
}
