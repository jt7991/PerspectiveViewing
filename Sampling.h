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
	void getJitteredPoints(double minX, double maxX, double minY, double maxY, int numSamples, Point2D * points);
	void getJitteredPoints(double minX, double maxX, double minY, double maxY, double numSamples, Point2D * points);

};

