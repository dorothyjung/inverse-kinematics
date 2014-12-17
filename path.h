#include <cmath>
#include <algorithm>
#include <math.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/SVD>

using namespace std;
using namespace Eigen;

//****************************************************
// Represent target path for arm
//****************************************************
class Path {
	public:
		// Path()
		// 	: theta(VectorXf(12))
		// 	, endpt()
		// 	{ }			

	Path(int p);

	int path;

};