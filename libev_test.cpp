#include <iostream>
#include "ev.h"


using namespace std;

int main()
{
	cout << ev_supported_backends() << endl;
    return 0;
}

