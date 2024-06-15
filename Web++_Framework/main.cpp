// Web++_Framework.cpp : Defines the entry point for the application.
//

#include "Web++ServerSide/Web++.h"

using namespace std;

int main()
{
	cout << "Hello Server!" << endl;
	asio::io_context context; 
	WebPlusPlus::InitServer(context, {});

	return 0;
}
