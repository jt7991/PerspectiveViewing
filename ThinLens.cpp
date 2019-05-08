#include "ThinLens.h"


ThinLens::ThinLens(Point3D e, Point3D l, Vector3D u, double viewDist, float lensRad, float focDist, float z) :
	upVector(u),
	lookAt(l),
	eye(e),
	viewDistance(viewDist),
	lensRadius(lensRad),
	focalDistance(focDist),
	zoom(z)
{
	computeUVW();
}


ThinLens::~ThinLens()
{
}
void ThinLens::computeUVW() {
	w = eye - lookAt;
	w.normalize();
	u = upVector ^ w;
	u.normalize();
	v = w ^ u;
}
Vector3D ThinLens::rayDirection(Point2D pixelPoint, Point2D lensPoint) {
	Point2D pointOnFocalPlane(pixelPoint.x * focalDistance / viewDistance, pixelPoint.y * focalDistance / viewDistance);
	Vector3D dir = (pointOnFocalPlane.x - lensPoint.x) * u + (pointOnFocalPlane.y - lensPoint.y) * v - focalDistance * w;
	return dir;
}