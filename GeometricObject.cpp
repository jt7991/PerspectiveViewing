// 	Copyright (C) Kevin Suffern 2000-2007.
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.
//	See the file COPYING.txt for the full license.


// this file contains the definition of the class GeometricObject 

#include "Constants.h"
#include "GeometricObject.h"


// ---------------------------------------------------------------------- default constructor

GeometricObject::GeometricObject(void)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject(const GeometricObject& object) {

}


// ---------------------------------------------------------------------- assignment operator

GeometricObject&														
GeometricObject::operator= (const GeometricObject& rhs) {
	if (this == &rhs)
		return (*this);
		
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject (void) {	
}


// ---------------------------------------------------------------------- add_object
// required for Compound objects 

void 												
GeometricObject::add_object(GeometricObject* object_ptr) {}


// ----------------------------------------------------------------------- get_normal

Normal
GeometricObject::get_normal(void) const{
	return (Normal());
} 


// ----------------------------------------------------------------------- set_material

void 
GeometricObject::set_material(Material* mPtr) {
	material_ptr = mPtr;
}


// ----------------------------------------------------------------------- get_material

Material* 
GeometricObject::get_material(void) const {
	return (material_ptr);
}


// ----------------------------------------------------------------------- compute_normal

Normal
GeometricObject::get_normal(const Point3D& p) {
	return (Normal());
}  

// ----------------------------------------------------------------------- sample
// returns a sample point on the object for area light shading

Point3D 
GeometricObject::sample(void) {
	return (Point3D(0.0));
}


// ----------------------------------------------------------------------- pdf
// returns the probability density function for area light shading
		
float
GeometricObject::pdf(ShadeRec& sr) {
	return (0.0);
}  	

									

