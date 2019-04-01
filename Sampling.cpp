#include "Sampling.h"
#include "of3dGraphics.h"
#include "ofImage.h"
#include "../Point2D.h"
#include <cstdlib>
double gridSpacing = 10;
double lineThickness = .5;
Sampling::Sampling()
{
}


Sampling::~Sampling()
{
}

double Sampling::drawLines(int x, int y, double lineWidth) {
	int xlinePositions[5] = { 90,180,270,360,450 };
	int ylinePositions[5] = { 95,185,275,365,455 };
	if (std::find(std::begin(xlinePositions), std::end(xlinePositions), x) != std::end(xlinePositions) ||
		std::find(std::begin(xlinePositions), std::end(xlinePositions), x+1) != std::end(xlinePositions) ||
		std::find(std::begin(ylinePositions), std::end(ylinePositions), y) != std::end(ylinePositions) ||
		std::find(std::begin(ylinePositions), std::end(ylinePositions), y+1) != std::end(ylinePositions))
	{
		
		double r = ((double)rand() / (RAND_MAX)) ;
		
		if (r <= lineWidth / 2) {
			return 1;
		}
		else return 0;
	}
	else {
		return 0;
	}
}
double Sampling::drawLinesWithSampling(double x, double y, double lineWidth) {
	int xlinePositions[5] = { 90,180,270,360,450 };
	int ylinePositions[5] = { 95,185,275,365,455 };
	
	for (int i=0; i < 5; i++) {
		if ((x >= xlinePositions[i] + (1.0-lineWidth / 2) && x <= xlinePositions[i] + 1 + lineWidth / 2)||
			(y >= ylinePositions[i] + (1.0-lineWidth / 2) && y <= ylinePositions[i] + 1 + lineWidth / 2)) {
			return 1;
		}
	}
	return 0;
}
int Sampling::screenX(double x, int gridWidth) {
	return round(x + gridWidth / 2 + .5);

}
int Sampling::screenY(double y, int gridHeight) {
	return gridHeight - round(y + gridHeight / 2 + .5);

}
bool Sampling::hitGrid(double x, double y, int spacing, double lineWidth) {
	int ix = round(x);
	if (ix % spacing <= floor(lineWidth)) {
		if (abs(x - ix) < lineWidth / 2) return true;
	}
	int iy = round(y);
	if (iy % spacing <= floor(lineWidth)) {
		if (abs(y - iy) < lineWidth / 2) return true;
	}
	return false;
}

ofImage Sampling::startRandomSampling(double minX, double maxX, double minY, double maxY,
	ofImage img, double lineWidth, int numSamples) {
	double pixelWidth = img.getWidth();
	double pixelHeight = img.getHeight();
	double dx = (maxX - minX) / pixelWidth;
	double dy = (maxY - minY) / pixelHeight;
	ofPixels& pixels = img.getPixels();
	for (int i = 0; i < numSamples; i++) {
		double x = ((double)rand() / (RAND_MAX + 1)) * (maxX - minX) + minX;
		double y = ((double)rand() / (RAND_MAX + 1)) * (maxY - minY) + minY;
		if (hitGrid(x, y, gridSpacing, lineWidth)) {
			ofColor color(0);
			pixels.setColor(x, y, color);
		}
	}
	img.update();
	return img;
}
double Sampling::startJitteredSampling(double gridWidth, double x, double y, double lineWidth) {
	double darkness = 0;
	double dx = 1 / gridWidth;
	for (double i = 0; i <= gridWidth; i += dx) {
		for (double j = 0; j <= gridWidth; j += dx) {
			double r = ((double)rand() / (RAND_MAX + 1))*dx;
			double sampleX = x + r + j;
			r = ((double)rand() / (RAND_MAX + 1))*dx;
			double sampleY = y + r + i;
			darkness += drawLinesWithSampling(sampleX, sampleY, lineWidth);
		}
	}

	return (darkness / (gridWidth*gridWidth));
}
ofImage Sampling::startJitteredSampling(double minX, double maxX, double minY, double maxY,
	ofImage img, double lineWidth, int numSamples, int sampleGridWidth) {
	double pixelWidth = img.getWidth();
	double pixelHeight = img.getHeight();
	double dx = (maxX - minX) / pixelWidth;
	double dy = (maxY - minY) / pixelHeight;
	ofPixels& pixels = img.getPixels();
	for (double i = sampleGridWidth; i <= maxX; i += sampleGridWidth) {
		for (double j = sampleGridWidth; j <= maxY; j += sampleGridWidth) {
			for (int k = 0; k < numSamples; k++) {
				double x = ((double)rand() / (RAND_MAX + 1)) * (i - (i-sampleGridWidth)) + (i - sampleGridWidth);
				double y = ((double)rand() / (RAND_MAX + 1)) * (j - (j - sampleGridWidth)) + (j - sampleGridWidth);
				if (hitGrid(x, y, gridSpacing, lineWidth)) {
					ofColor color(0);
					pixels.setColor(x, y, color);
				}
			}
		}
	}
	img.update();
	return img;
}
Point2D Sampling::getRandomPoint(double minX, double maxX, double minY, double maxY) {
	double x = ((double)rand() / (RAND_MAX + 1)) * (maxX - minX) + minX;
	double y = ((double)rand() / (RAND_MAX + 1)) * (maxY - minY) + minY;
	return Point2D(x, y);
}