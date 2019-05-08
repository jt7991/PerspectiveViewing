#include "ofApp.h"
#include "of3dGraphics.h"
#include "ofApp.h"
#include "ofxGui/src/ofxGui.h"
#include "../ViewPlane.h"
#include "../Sphere.h"
#include "../BBox.h"
#include "../Triangle.h"
#include "../Plane.h"
#include "../ShadeRec.h"
#include "../PinholeCamera.h"
#include "../Sampling.h"
#include "../Point2D.h"
#include "../ThinLens.h"
#include "../Ambient.h"
ofxToggle toggleAntialiasing;

double minX, maxX, minY, maxY;
ofImage img;
ofxPanel gui;
ofParameterGroup sphereCenter, cameraEye, cameraLookAt, renderImage, renderImageThinLens, numberOfSamplesGroup, lighting;
ofParameter<int> sphereCenterX, sphereCenterY, sphereCenterZ,
	cameraEyeX, cameraEyeY, cameraEyeZ, cameraLookAtX, cameraLookAtY, cameraLookAtZ, numSamples, focalDistance;
ofParameter<float> lensRadius, ambientScaleRadiance;
ofParameter<void> drawImageWithRandomBtn, drawImageWithJitteredBtn, drawImageThinLens;
ViewPlane viewPlane = ViewPlane(500, 500);
Plane plane;
Sphere sphereYellow;
BBox boxOrange;
Triangle triangleGreen;
Sampling sampling;
//--------------------------------------------------------------
void ofApp::setup() {
	gui.setDefaultWidth(300);
	gui.setup();
	ofSetWindowShape(viewPlane.hres + gui.getWidth()+10, viewPlane.vres);
	ofSetWindowPosition(30, 30);
	ofBackground(ofColor(0));


	sphereCenter.setName("Sphere Center Coordinates");
	sphereCenter.add(sphereCenterX.set("X", -60, -250, 250));
	sphereCenter.add(sphereCenterY.set("Y", 60, -250, 250));
	sphereCenter.add(sphereCenterZ.set("Z", 100, 50, 1000));


	cameraEye.setName("Camera Eye");
	cameraEye.add(cameraEyeX.set("X", 0, -1000, 1000));
	cameraEye.add(cameraEyeY.set("Y", 20, -1000, 1000));
	cameraEye.add(cameraEyeZ.set("Z", 0, -1000, 1000));

	cameraLookAt.setName("Camera Look At Point");
	cameraLookAt.add(cameraLookAtX.set("X", 0, -1000, 1000));
	cameraLookAt.add(cameraLookAtY.set("Y", 0, -1000, 1000));
	cameraLookAt.add(cameraLookAtZ.set("Z", 1000, -1000, 1000));

	lighting.setName("Lighting");
	lighting.add(ambientScaleRadiance.set("Ambient Scale Radiance", 1, 0, 1));

	img.allocate(viewPlane.hres, viewPlane.vres, OF_IMAGE_COLOR);
	img.setColor(ofColor(0));
	
	numberOfSamplesGroup.setName("Number Of Samples");
	numberOfSamplesGroup.add(numSamples.set("Number Of Samples", 10, 1, 100));
	
	renderImage.setName("Pinhole Camera");
	renderImage.add(drawImageWithJitteredBtn.set("Draw Image With Jittered Sampling"));
	drawImageWithJitteredBtn.addListener(this, &ofApp::drawImageWithJittered);
	
	renderImageThinLens.setName("Thin Lens Camera");
	renderImageThinLens.add(lensRadius.set("Lens Radius", 1, 0, 5));
	renderImageThinLens.add(focalDistance.set("Focal Distance", 100, 1, 1000));
	renderImageThinLens.add(drawImageThinLens.set("Draw Image With Jittered Sampling"));
	drawImageThinLens.addListener(this, &ofApp::drawImageWithThinLens);


	gui.add(sphereCenter);
	gui.add(cameraEye);
	gui.add(cameraLookAt);
	gui.add(lighting);
	gui.add(numberOfSamplesGroup);
	gui.add(renderImage);
	gui.add(renderImageThinLens);

	plane = Plane(Point3D(0, 0, 0), Normal(Vector3D(0, 1, 0)));
	boxOrange = BBox(Point3D(-60, 0, 200), Point3D(80, 120, 220));
	triangleGreen = Triangle(Point3D(-290, 0, 360), Point3D(-260, 160, 350), Point3D(250, 130, 330));
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	img.draw(gui.getWidth() +10 , 0);
	gui.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
void ofApp::drawImage() {
	img.setColor(ofColor(0));
	Ray ray = Ray(); //the default direction is (0,0,1) which is what we want for all our rays here.
	ShadeRec hitReturnData = ShadeRec();
	double hitTime;
	ofColor pixelColor = NULL;
	ofColor YELLOW = ofColor(255,255,0);
	ofColor GREEN = ofColor(0, 200, 0);
	ofColor ORANGE = ofColor(255, 155, 0);
	sphereYellow = Sphere(sphereCenterX, sphereCenterY, sphereCenterZ, 30);
	boxOrange = BBox(Point3D(-20, -30, 120), Point3D(40, 90, 150));
	triangleGreen = Triangle(Point3D(-90, 0, 60), Point3D(-60, 60, 150), Point3D(50, 30, 130));
	//                                   Eye                             Look At               Up Vector      dist   Near Clip    Far Clip
	PinholeCamera camera(Point3D(cameraEyeX, cameraEyeY, cameraEyeZ), Point3D(cameraLookAtX, cameraLookAtY, cameraLookAtZ), Vector3D(0, 1, 0), 100 ,    20,        1000);
	camera.computeUVW();
	for (int i = 0; i<numSamples; i++){
			double minHitTime = std::numeric_limits<double>::max();
			ofColor pixelColor = NULL;

			ray.start = camera.eye;


			Sampling sampling;
			Point2D viewPoint = sampling.getRandomPoint(0.0,500.0,0.0,500.0);
			double viewX = viewPlane.pixelSize * (viewPoint.x - 0.5*(viewPlane.hres - 1));
			double viewY = viewPlane.pixelSize * ((viewPlane.vres - viewPoint.y) - 0.5*(viewPlane.vres - 1));
			Vector3D direction = viewX * camera.u + viewY * camera.v - camera.viewDistance * camera.w;
			ray.dir = direction / direction.magnitude();
			if (sphereYellow.hit(ray, hitTime, hitReturnData)) {
				if (hitTime < minHitTime) {
					minHitTime = hitTime;
					pixelColor = YELLOW;
				}
			}
			if (boxOrange.hit(ray, hitTime, hitReturnData)) {
				if (hitTime < minHitTime) {
					minHitTime = hitTime;
					pixelColor = ORANGE;
				}
			}
			if (triangleGreen.hit(ray, hitTime, hitReturnData)) {
				if (hitTime < minHitTime) {
					minHitTime = hitTime;
					pixelColor = GREEN;
				}
			}
			if (plane.hit(ray, hitTime, hitReturnData)) {
				if (hitTime < minHitTime) {
					minHitTime = hitTime;
					if (isBlack(hitReturnData.local_hit_point.x, hitReturnData.local_hit_point.y, hitReturnData.local_hit_point.z)) {
						pixelColor = ofColor(0);
					}
					else {
						pixelColor = ofColor(255);
					}
				}
			}

			if (pixelColor != NULL) {
				img.setColor(viewPoint.x, viewPoint.y, pixelColor);
			}
	}
	img.update();


}
void ofApp::drawImageWithJittered() {
	img.setColor(ofColor(0));
	Ray ray = Ray(); //the default direction is (0,0,1) which is what we want for all our rays here.
	ShadeRec hitReturnData = ShadeRec();
	double hitTime;
	ofColor YELLOW = ofColor(255, 255, 0);
	ofColor GREEN = ofColor(0, 200, 0);
	ofColor ORANGE = ofColor(255, 155, 0);
	sphereYellow = Sphere(sphereCenterX, sphereCenterY, sphereCenterZ, 30);

	//                                   Eye                             Look At                                        Up Vector         dist    lensRad    focalDistance   zoom
	PinholeCamera camera(Point3D(cameraEyeX, cameraEyeY, cameraEyeZ), Point3D(cameraLookAtX, cameraLookAtY, cameraLookAtZ), Vector3D(0, 1, 0), 100, 20, 1000);
	camera.computeUVW();
	Point2D* points = new Point2D[numSamples];
	Sampling sampling;

	sampling.getJitteredPoints(numSamples, points);
	
	for (int i = 0; i < viewPlane.hres; i++) {
		for (int j = 0; j < viewPlane.hres; j++) {
			int red = 0, green = 0, blue = 0;
			bool sphereHit = false;
			bool triangleHit = false;
			bool boxHit = false;
			for (int k = 0; k < numSamples; k++) {
				double minHitTime = std::numeric_limits<double>::max();

				ray.start = camera.eye;
				//std::cout << points[k].x << "\n";
				double viewX = viewPlane.pixelSize * -(j - .5* (viewPlane.hres - 1) + points[k].x);
				double viewY = viewPlane.pixelSize * -(i - .5 * (viewPlane.vres - 1) + points[k].y);
				Vector3D direction = viewX * camera.u + viewY * camera.v - camera.viewDistance * camera.w;
				ray.dir = direction / direction.magnitude();

				char closestObject = 'X';
				if (sphereYellow.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						minHitTime = hitTime;
						closestObject = 'S';

					}
				}
				if (boxOrange.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						minHitTime = hitTime;
						closestObject = 'B';

					}
				}
				if (triangleGreen.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						minHitTime = hitTime;
						closestObject = 'T';

					}
				}
				if (plane.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						closestObject = 'P';
						minHitTime = hitTime;

					}
				}
				switch (closestObject) {
				case 'S':
					red += 255;
					green += 255;
					blue += 0;
					break;
				case 'B':
					red += 255;
					green += 155;
					blue += 0;
					break;
				case 'T':
					red += 0;
					green += 200;
					blue += 0;
					break;
				case 'P':
					if (!isBlack(hitReturnData.local_hit_point.x, hitReturnData.local_hit_point.y, hitReturnData.local_hit_point.z)) {
						red += 255;
						green += 255;
						blue += 255;
					}
				default:
					break;
				}


			}
			red = red / numSamples;
			green = green / numSamples;
			blue = blue / numSamples;
			ofColor pixelColor = ofColor(red, green, blue);
			img.setColor(j, i, pixelColor);
		}
	}
	delete[] points;
	img.update();

}

void ofApp::drawImageWithThinLens() {
	img.setColor(ofColor(0));
	Ray ray = Ray(); 
	ShadeRec hitReturnData = ShadeRec();
	double hitTime;
	ofColor YELLOW = ofColor(255, 255, 0);
	ofColor GREEN = ofColor(0, 200, 0);
	ofColor ORANGE = ofColor(255, 155, 0);
	sphereYellow = Sphere(sphereCenterX, sphereCenterY, sphereCenterZ, 30);

	ThinLens camera(Point3D(cameraEyeX, cameraEyeY, cameraEyeZ), Point3D(cameraLookAtX, cameraLookAtY, cameraLookAtZ), Vector3D(0, 1, 0),   100,    lensRadius,           focalDistance,     1);
	double totalNumSamples = numSamples * viewPlane.hres * viewPlane.vres;
	Point2D* points = new Point2D[numSamples];
	Point2D* lensPoints = new Point2D[totalNumSamples];
	Sampling sampling;
	sampling.getJitteredPoints(numSamples, points);
	sampling.getDiscSamples(totalNumSamples, lensPoints);
	Ambient ambient;
	ambient.scale_radiance(ambientScaleRadiance);
	int pointIndex = 0;
	for (int i = 0; i < viewPlane.vres; i++) {
		for (int j = 0; j < viewPlane.hres; j++) {
			int red=0, green=0, blue=0;
			for (int k = 0; k < numSamples; k++) {
				double minHitTime = std::numeric_limits<double>::max();
				Point2D lensPoint (camera.lensRadius * lensPoints[pointIndex].x, camera.lensRadius * lensPoints[pointIndex].y);
				ray.start = Point3D(camera.eye.x + lensPoint.x, camera.eye.y + lensPoint.y, camera.eye.z);
				double viewX = viewPlane.pixelSize * -(j - .5* (viewPlane.hres-1)+ points[k].x);
				double viewY = viewPlane.pixelSize * -(i - .5 * (viewPlane.vres-1) + points[k].y);
				Vector3D direction = camera.rayDirection(Point2D(viewX, viewY), lensPoint);
				ray.dir = direction;
				char closestObject = 'X';
				if (sphereYellow.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						minHitTime = hitTime;
						closestObject = 'S';

					}
				}
				if (boxOrange.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						minHitTime = hitTime;
						closestObject = 'B';

					}
				}
				if (triangleGreen.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						minHitTime = hitTime;
						closestObject = 'T';

					}
				}
				if (plane.hit(ray, hitTime, hitReturnData)) {
					if (hitTime < minHitTime) {
						closestObject = 'P';
						minHitTime = hitTime;

					}
				}
				switch (closestObject) {
					case 'S':
						red += 255;
						green += 255;
						blue += 0;
						break;
					case 'B':
						red += 255;
						green += 155;
						blue += 0;
						break;
					case 'T':
						red += 0;
						green += 200;
						blue += 0;
						break;
					case 'P':
						if (!isBlack(hitReturnData.local_hit_point.x, hitReturnData.local_hit_point.y, hitReturnData.local_hit_point.z)) {
							red += 255;
							green += 255;
							blue += 255;
						}
					default:
						break;
				}
				pointIndex++;
			}
			
			red = red / numSamples;
			green = green / numSamples;
			blue = blue / numSamples;

			ofColor pixelColor = ofColor(red*ambient.ls, green*ambient.ls, blue*ambient.ls);
			img.setColor(j, i, pixelColor);
		}
	}
	delete[] points;
	img.update();


}
bool ofApp::isBlack(long x, long y, long z)
{
	if ((int)abs(x)/50 % 2 == (int)abs(z)/50 % 2)
		return true;
	return false;
}