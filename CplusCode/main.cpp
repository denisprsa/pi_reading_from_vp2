#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

#include "VremenskaPostaja.h"

using namespace std;

int main(int argc, char *argv[])
{

    VremenskaPostaja *p = new VremenskaPostaja(1999, 7, 1);

    cout << "NAPOVED "<< p->getYear() << endl;
    return 0;
}