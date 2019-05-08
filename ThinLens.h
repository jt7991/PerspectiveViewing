#pragma once
#include "Point3D.h"
#include "Point2D.h"
#include "Vector3D.h"
class ThinLens
{
public:
	ThinLens();
	ThinLens(Point3D e, Point3D l, Vector3D u, double viewDist, float lensRad, float focDist, float z);
	~ThinLens();
	void computeUVW();
	Vector3D rayDirection(Point2D pixelPoint, Point2D lensPoint);
	Point3D eye;
	Point3D lookAt;
	Vector3D upVector;
	Vector3D u, v, w;
	double viewDistance;
	float lensRadius;
	float focalDistance;
	float zoom;
};

