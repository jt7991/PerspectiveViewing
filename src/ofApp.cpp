#include "ofApp.h"
#include "of3dGraphics.h"
#include "ofApp.h"
//#include "../Sampling.h"
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
ofxToggle toggleAntialiasing;

double minX, maxX, minY, maxY;
ofImage img;
ofxPanel gui;
ofParameterGroup sphereCenter, cameraEye, cameraLookAt, renderImage;
ofParameter<int> sphereCenterX, sphereCenterY, sphereCenterZ, 
	cameraEyeX, cameraEyeY, cameraEyeZ, cameraLookAtX, cameraLookAtY, cameraLookAtZ, numSamples;
ofParameter<void> drawImageWithRandomBtn, drawImageWithJitteredBtn;
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



	img.allocate(viewPlane.hres, viewPlane.vres, OF_IMAGE_COLOR);
	img.setColor(ofColor(0));

	plane = Plane(Point3D(0, 0, 0), Normal(Vector3D(0, 1, 0)));

	renderImage.setName("Image Rendering Options");
	renderImage.add(numSamples.set("Number Of Samples", 1000, 0, 1000000));
	renderImage.add(drawImageWithRandomBtn.set("Draw Image With Random Sampling"));
	drawImageWithRandomBtn.addListener(this, &ofApp::drawImage);

	renderImage.add(drawImageWithJitteredBtn.set("Draw Image With Jittered Sampling"));
	drawImageWithJitteredBtn.addListener(this, &ofApp::drawImageWithJittered);
	
	
	gui.add(sphereCenter);
	gui.add(cameraEye);
	gui.add(cameraLookAt);
	gui.add(renderImage);

	//gui.draw();
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
	ofColor pixelColor = NULL;
	ofColor YELLOW = ofColor(255, 255, 0);
	ofColor GREEN = ofColor(0, 200, 0);
	ofColor ORANGE = ofColor(255, 155, 0);
	sphereYellow = Sphere(sphereCenterX, sphereCenterY, sphereCenterZ, 30);
	boxOrange = BBox(Point3D(-20, -30, 120), Point3D(40, 90, 150));
	triangleGreen = Triangle(Point3D(-90, 0, 60), Point3D(-60, 60, 150), Point3D(50, 30, 130));
	//                                   Eye                             Look At               Up Vector      dist   Near Clip    Far Clip
	PinholeCamera camera(Point3D(cameraEyeX, cameraEyeY, cameraEyeZ), Point3D(cameraLookAtX, cameraLookAtY, cameraLookAtZ), Vector3D(0, 1, 0), 100, 20, 1000);
	camera.computeUVW();
	Point2D* points = new Point2D[numSamples];
	Sampling sampling;
	sampling.getJitteredPoints(0.0, 500.0, 0.0, 500, (double)numSamples, points);
	for (int i = 0; i < numSamples; i++) {
		double minHitTime = std::numeric_limits<double>::max();
		ofColor pixelColor = NULL;

		ray.start = camera.eye;



		double viewX = viewPlane.pixelSize * (points[i].x - 0.5*(viewPlane.hres - 1));
		double viewY = viewPlane.pixelSize * ((viewPlane.vres - points[i].y) - 0.5*(viewPlane.vres - 1));
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
			img.setColor(points[i].x, points[i].y, pixelColor);
		}
	}
	delete [] points;
	img.update();


}
bool ofApp::isBlack(long x, long y, long z)
{
	if ((int)abs(x)/50 % 2 == (int)abs(z)/50 % 2)
		return true;
	return false;
}