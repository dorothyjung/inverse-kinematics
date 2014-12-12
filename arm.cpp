#include <cmath>
#include <algorithm>
#include <math.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include "arm.h"

using namespace std;
using namespace Eigen;

Arm::Arm(Eigen::VectorXf t){
	theta = t;
	
	Eigen::Vector3f endseg1(0,0,LEN1); 
	Eigen::Vector3f endseg2(0,0,LEN1+LEN2);
	Eigen::Vector3f endseg3(0,0,LEN1+LEN2+LEN3);
	Eigen::Vector3f endseg4(0,0,LEN1+LEN2+LEN3+LEN4);

	endpt.pushback(endseg1);
	endpt.pushback(endseg2);
	endpt.pushback(endseg3);
	endpt.pushback(endseg4);
}

MatrixXf Arm::computeJacobian(){

	// Numerically compute Jacobian matrix based on current arm position



}