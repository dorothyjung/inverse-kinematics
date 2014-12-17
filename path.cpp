#include <cmath>
#include <algorithm>
#include <math.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "path.h"

using namespace std;
using namespace Eigen;

//****************************************************
// Creates and stores information about target path
//****************************************************

Path::Path(int p) {
	// do stuff here; default is circle
	// have different paths based on int input with switch statement?
	path = p;
	
}