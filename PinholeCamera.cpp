#include "PinholeCamera.h"

PinholeCamera::PinholeCamera(Point3D position, Point3D viewDirection, 
	Vector3D upVector, double viewDistance, double nearClipDistance, double farClipDistance) :
	upVector(upVector),
	lookAt(viewDirection),
	eye(position),
	viewDistance(viewDistance),
	nearClipDistance(nearClipDistance),
	farClipDistance(farClipDistance)
{}


PinholeCamera::~PinholeCamera()
{
}

void PinholeCamera::computeUVW() {
	w = eye - lookAt;
	w.normalize();
	u = upVector ^ w;
	u.normalize();
	v = w ^ u;
}