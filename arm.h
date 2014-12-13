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
// Constant values for arm length
//****************************************************
#define LEN1 5.0
#define LEN2 4.0
#define LEN3 3.0
#define LEN4 2.0

//****************************************************
// Initialize rotation matrices
//****************************************************
void initRotationMatrices();

//****************************************************
// Represent 4-segment arm
//****************************************************
class Arm {
	public:
		Arm()
			: theta(VectorXf(12))
			, endpt()
			{ }			

	Arm(Eigen::VectorXf);
	
	Eigen::MatrixXf computeJacobian();
	void updateArm(MatrixXf, Vector3f);

	Eigen::VectorXf theta;
	vector<Eigen::Vector3f> endpt;

};