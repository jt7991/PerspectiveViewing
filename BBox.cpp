// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.

#include "BBox.h"
#include "Constants.h"
#include "ShadeRec.h"


// --------------------------------------------------------------------- default constructor

BBox::BBox (void)
	: x0(-1), x1(1), y0(-1), y1(1), z0(-1), z1(1)
{}	


// --------------------------------------------------------------------- constructor

BBox::BBox (	const double _x0, const double _x1,			
				const double _y0, const double _y1, 
				const double _z0, const double _z1)
	: x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1)
{}


// --------------------------------------------------------------------- constructor

BBox::BBox (const Point3D p0, const Point3D p1)
	: x0(p0.x), x1(p1.x), y0(p0.y), y1(p1.y), z0(p0.z), z1(p1.z)
{}
										


// --------------------------------------------------------------------- copy constructor

BBox::BBox (const BBox& bbox)
	: x0(bbox.x0), x1(bbox.x1), y0(bbox.y0), y1(bbox.y1), z0(bbox.z0), z1(bbox.z1)
{}						
				

// --------------------------------------------------------------------- assignment operator

BBox&
BBox::operator= (const BBox& rhs) {
	if (this == &rhs)
		return (*this);

	x0	= rhs.x0;
	x1	= rhs.x1;
	y0	= rhs.y0;
	y1	= rhs.y1;
	z0	= rhs.z0;
	z1	= rhs.z1;	
	
	return (*this);
}			


// --------------------------------------------------------------------- destructor

BBox::~BBox (void) {}	


// --------------------------------------------------------------------- hit
Normal BBox::get_normal(const int face_hit) const {
	switch (face_hit) {
		case 0: return (Normal(-1, 0, 0));
		case 1: return (Normal(0, -1, 0));
		case 2: return (Normal(0, 0, -1));
		case 3: return (Normal(1, 0, 0));
		case 4: return (Normal(0, 1, 0));
		case 5: return (Normal(0, 0, 1));
	}
}
bool 									
BBox::hit(const Ray& ray, double& tmin, ShadeRec& sr ) const {	
	double ox = ray.start.x; double oy = ray.start.y; double oz = ray.start.z;
	double dx = ray.dir.x; double dy = ray.dir.y; double dz = ray.dir.z;
	
	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max; 
	int face_in, face_out;
	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}
	
	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}
	
	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}
	
	double t0, t1;
	
	// find largest entering t value
	
	if (tx_min > ty_min) {
		t0 = tx_min;
		face_in = (a >= 0.0) ? 0 : 3;
	}
	else {
		t0 = ty_min;
		face_in = (b >= 0.0) ? 1 : 4;
	}
	if (tz_min > t0) {
		t0 = tz_min;
		face_in = (c >= 0.0) ? 2 : 5;

	}
		
	// find smallest exiting t value
		
	if (tx_max < ty_max) {
		t1 = tx_max;
		face_out = (a >= 0.0) ? 3 : 0;
	}
	else {
		t1 = ty_max;
		face_out = (b >= 0.0) ? 4 : 1;
	}
	if (tz_max < t1) {
		t1 = tz_max;
		face_out = (c >= 0.0) ? 5 : 2;
	}
	if (t0< t1 && t1 > kEpsilon) {
		if (t0 > kEpsilon) {
			tmin = t0;
			sr.normal = get_normal(face_in);
		}
		else {
			tmin = t1;
			sr.normal = get_normal(face_out);
		}
		sr.local_hit_point = ray.start + tmin * ray.dir;
		return true;
	}
	else return false;
}


// --------------------------------------------------------------------- inside
// used to test if a ray starts inside a grid

bool
BBox::inside(const Point3D& p) const {
	return ((p.x > x0 && p.x < x1) && (p.y > y0 && p.y < y1) && (p.z > z0 && p.z < z1));
};




