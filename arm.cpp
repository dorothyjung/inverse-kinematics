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

float dt = 0.1f;
Matrix3f rot_x;
Matrix3f rot_y;
Matrix3f rot_z;

rot_x << 	1.0,  		0.0,		0.0,
			0.0,		cos(dt),	sin(dt),
			0.0,		sin(dt), 	cos(dt);

rot_y <<	cos(dt),	0.0,		-sin(dt),
			0.0,		1.0,		0.0, 
			sin(dt),	0.0,		cos(dt);

rot_z <<	cos(dt),	sin(dt),	0.0,
			-sin(dt),	cos(dt),	0.0,
			0.0,		0.0,		1.0;

Arm::Arm(Eigen::VectorXf t){
	// Stores the current state of the arm
	// theta is a list of axis-aligned angles, 3 per segment
	// endpt stores the far ends of each of the 4 segments

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
	
	// theta-xyz-1
	Vector3f dpdx1 = (1.0/dt)*((rot_x*endpt[3])-endpt[3]);
	Vector3f dpdy1 = (1.0/dt)*((rot_y*endpt[3])-endpt[3]);
	Vector3f dpdz1 = (1.0/dt)*((rot_z*endpt[3])-endpt[3]);

	// theta-xyz-2
	Vector3f dpdx2 = (1.0/dt)*((rot_x*(endpt[3]-endpt[0])+endpt[0])-endpt[3]);
	Vector3f dpdy2 = (1.0/dt)*((rot_y*(endpt[3]-endpt[0])+endpt[0])-endpt[3]);
	Vector3f dpdz2 = (1.0/dt)*((rot_z*(endpt[3]-endpt[0])+endpt[0])-endpt[3]);

	// theta-xyz-3
	Vector3f dpdx3 = (1.0/dt)*((rot_x*(endpt[3]-endpt[1])+endpt[1])-endpt[3]);
	Vector3f dpdy3 = (1.0/dt)*((rot_y*(endpt[3]-endpt[1])+endpt[1])-endpt[3]);
	Vector3f dpdz3 = (1.0/dt)*((rot_z*(endpt[3]-endpt[1])+endpt[1])-endpt[3]);

	// theta-xyz-4
	Vector3f dpdx4 = (1.0/dt)*((rot_x*(endpt[3]-endpt[2])+endpt[2])-endpt[3]);
	Vector3f dpdy4 = (1.0/dt)*((rot_y*(endpt[3]-endpt[2])+endpt[2])-endpt[3]);
	Vector3f dpdz4 = (1.0/dt)*((rot_z*(endpt[3]-endpt[2])+endpt[2])-endpt[3]);

	// Construct Jacobian
	MatrixXf jacobian(3,12);
	jacobian << dpdx1[0], dpdy1[0], dpdz1[0], dpdx2[0], dpdy2[0], dpdz2[0], dpdx3[0], dpdy3[0], dpdz3[0], dpdx4[0], dpdy4[0], dpdz4[0],
				dpdx1[1], dpdy1[1], dpdz1[1], dpdx2[1], dpdy2[1], dpdz2[1], dpdx3[1], dpdy3[1], dpdz3[1], dpdx4[1], dpdy4[1], dpdz4[1],
				dpdx1[2], dpdy1[2], dpdz1[2], dpdx2[2], dpdy2[2], dpdz2[2], dpdx3[2], dpdy3[2], dpdz3[2], dpdx4[2], dpdy4[2], dpdz4[2];

	return jacobian;
	
}