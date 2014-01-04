/* ODE tutorial  by Kosei Demura  http://demura.net   */
/* sample4.cpp :Lesson 4 3D Graphics 2008-2011*/
// A program for drawing a sphere, a cylinder, a capsule, a box and a ray
// Only drawing, no collision detection and no dynamics

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "constantes.h"


struct MyObject
{
    dBodyID body;		// rigid body
};

static dWorldID world;  // a dynamic world
static MyObject sphere, box, capsule, cylinder; // objects

// start simulation
static void start()
{
    static float xyz[3] = {5,3,0.5};    // eye point[m]
    static float hpr[3] = {-180, 0, 0}; // gaze direction[Åã]
    dsSetViewpoint (xyz,hpr);           // set viewpoint
}


// simulation loop
static void simLoop (int pause)
{
    const dReal *pos1,*R1,*pos2,*R2,*pos3,*R3;

    //@a sphere
    dsSetColor(1,0,0);      // set color (red, green, blue a value is between 0 and 1
    dsSetSphereQuality(3);  // set sphere quality. 3 is enough
    pos1 = dBodyGetPosition(sphere.body); // get a body position
    R1   = dBodyGetRotation(sphere.body); // get a body rotation matrix
    dsDrawSphere(pos1,R1,radius);         // draw a sphere

    // a cylinder
    dsSetColorAlpha (0,1,0,1);
    pos2 = dBodyGetPosition(cylinder.body);
    R2   = dBodyGetRotation(cylinder.body);
    dsDrawCylinder(pos2,R2,length,radius);  // draw a cylinder

    // a capsule
    dsSetColorAlpha (1,1,1,1);
    pos2 = dBodyGetPosition(capsule.body);
    R2   = dBodyGetRotation(capsule.body);
    dsDrawCapsule(pos2,R2,length,radius);  // draw a capsule

    // a box
    dsSetColorAlpha (0,0,1,1);
    pos3 = dBodyGetPosition(box.body);
    R3   = dBodyGetRotation(box.body);
    dsDrawBox(pos3,R3,sides);             // draw a box

    // a ray
    dReal posA[3] = {0, 5, 0}, posB[3]= {0, 5, 1.9};
    dsDrawLine(posA,posB); // draw a ray
}

int main (int argc, char **argv)
{
    // set for drawing
    dsFunctions fn;
    fn.version = DS_VERSION;
    fn.start   = &start;
    fn.step    = &simLoop;
    fn.command = NULL;
    fn.stop    = NULL;
    fn.path_to_textures = "../textures";


    dInitODE();              // init ODE
    world = dWorldCreate();  // create a dynamic world
    dWorldSetGravity(world,0,0,-0.1);

    dMass m;                 // a parameter for mass
    dMassSetZero (&m);       // initialize the parameter

    //@a sphere
    sphere.body = dBodyCreate (world);     // create a rigid body
    dReal radius = 0.5;                    // radius [m]
    dMassSetSphere (&m,DENSITY,radius);    // calculate a mass parameter for a sphere
    dBodySetMass (sphere.body,&m);         // set the mass parameter to the body
    dBodySetPosition (sphere.body,0,1, 1); // set the position of the body


    //@a box
    box.body = dBodyCreate (world);
    dMassSetBox (&m,DENSITY,sides[0],sides[1],sides[2]);
    dBodySetMass (box.body,&m);
    dBodySetPosition (box.body,0,2,1);

    // a capsule
    capsule.body = dBodyCreate (world);
    dMassSetCapsule(&m,DENSITY,3,radius,length);
    dBodySetMass (capsule.body,&m);
    dBodySetPosition (capsule.body,0,4,1);

    // a cylinder
    cylinder.body = dBodyCreate (world);
    dMassSetCylinder(&m,DENSITY,3,radius,length);
    dBodySetMass (cylinder.body,&m);
    dBodySetPosition (cylinder.body,0,3,1);

    // do the simulation
    dsSimulationLoop (argc,argv,960,480,&fn);

    dWorldDestroy (world); // destroy the world
    dCloseODE();           // close ODE

    return 0;
}
