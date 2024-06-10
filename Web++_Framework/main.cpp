// Web++_Framework.cpp : Defines the entry point for the application.
//

#include "Web++.h"

using namespace std;
int port = 90;

int main()
{
	cout << "Hello CMake." << endl;
	asio::io_context context; 
	WebPlusPlus::InitServer(context, {});

	return 0;
}
