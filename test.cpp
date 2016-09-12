// test.cpp : Defines the entry point for the console application.
//

#include "ev.h"
#include <iostream>

using namespace std;

int main()
{
	int backend = ev_supported_backends();
	cout << backend << endl;
    return 0;
}

