#pragma once
#include <math.h>
#include "of3dGraphics.h"
#include "ofImage.h"
#include "../Point2D.h"
class Sampling
{
public:
	Sampling();
	~Sampling();
	Point2D getRandomPoint(double minX, double maxX, double minY, double maxY);
	void getJitteredPoints(double numSamples, Point2D * points);

	void getDiscSamples(double numSamples, Point2D * lensPoints);
};

