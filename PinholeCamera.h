#pragma once
#include "Vector3D.h"
#include "Point3D.h"
class PinholeCamera
{
public:
	Point3D eye;
	Point3D lookAt;
	Vector3D upVector;
	Vector3D u, v, w;
	double viewDistance;
	double nearClipDistance;
	double farClipDistance;
	PinholeCamera();
	PinholeCamera(Point3D position, Point3D viewDirection, Vector3D upVector, double viewDistance, double nearClipDistance, double farClipDistance);
	~PinholeCamera();
	void computeUVW();
};

