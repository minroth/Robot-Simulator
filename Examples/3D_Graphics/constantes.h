#ifdef _MSC_VER
#pragma warning(disable:4244 4305)
#endif

#ifdef dDOUBLE
#define dsDrawBox      dsDrawBoxD
#define dsDrawSphere   dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule  dsDrawCapsuleD
#define dsDrawLine     dsDrawLineD
#endif

#define DENSITY (5.0)	// density

dReal radius = 0.25; // radius
dReal length = 1.0;  // length
dReal sides[3] = {0.5,0.5,0.5}; // sides