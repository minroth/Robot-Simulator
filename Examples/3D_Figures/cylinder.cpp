/* ODE tutorial  by Kosei Demura  http://demura.net   */
/* sample4.cpp :Lesson 4 3D Graphics 2008-2011*/
// A program for drawing a sphere, a cylinder, a capsule, a box and a ray
// Only drawing, no collision detection and no dynamics

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

/***********************************************************************************/
/*
*               Esctructura de Objetos
*/
/***********************************************************************************/



/***********************************************************************************/
/*
*               Parametros de los objetos
*/
/***********************************************************************************/

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

#define DENSITY (0.0000010)   // density

dReal radius = 0.25; // radius
dReal length = 1.0;  // length
dReal sides[3] = {0.5,0.5,0.5}; // sides


/***********************************************************************************/
/*
*               Obtengo un ID del mundo y creo los objetos
*/
/***********************************************************************************/
static dWorldID world;
static dSpaceID space;
static dBodyID cylinder;    
static dGeomID geom;  
static dJointGroupID contactgroup;
static dBodyID plane;    

/***********************************************************************************/
/*
*               los puntos de colision 
*/
/***********************************************************************************/

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    dContact contact;  

    contact.surface.mode = dContactBounce | dContactSoftCFM;
    // friction parameter
    contact.surface.mu = dInfinity;
    // bounce is the amount of "bouncyness".
    contact.surface.bounce = 0.09;
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.098;
    // constraint force mixing parameter
    contact.surface.soft_cfm = 0.001;  
    if (int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) {
        dJointID c = dJointCreateContact (world,contactgroup,&contact);
        dJointAttach (c,b1,b2);
    }
}

/***********************************************************************************/
/*
*               Comienzo de la simulacion
*/
/***********************************************************************************/
void start()
{
  // Set a camera
  static float xyz[3] = {0.0,-2.0,1.0};  // View position (x, y, z [m])
  static float hpr[3] = {90.0,0.0,0.0};  // View direction head, pitch, roll[°]
  dsSetViewpoint (xyz,hpr);// Set a view point
}


/***********************************************************************************/
/*
*               Loop de la simulacion
*/
/***********************************************************************************/

static void simLoop (int pause)
{
    const dReal *pos1,*R1;


    dsSetColorAlpha (0,1,0,1);
    dSpaceCollide (space,0,&nearCallback);
    dWorldStep(world,0.01);

    pos1 = dBodyGetPosition(cylinder);
    R1   = dBodyGetRotation(cylinder);
    dsDrawCylinder(pos1,R1,length,radius);  // draw a cylinder

    
}


/***********************************************************************************/
/*
*               Funcion Principal
*/
/***********************************************************************************/
int main (int argc, char **argv)
{
     
    static dMass m;
    
    dReal mass = 1.0;
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
    space = dSimpleSpaceCreate  (0);
    

    

    //@a cylinder    
    cylinder= dBodyCreate (world);
    geom =  dCreateCylinder (space,length,radius);   //create geometry.
    dMassSetCylinder(&m,DENSITY,2,radius,length);
    dBodySetMass (cylinder,&m);
    dGeomSetBody(geom,cylinder);
    dBodySetPosition (cylinder,0,3,1);

    //Gravedad y cosas de simulacion
    dWorldSetGravity(world,0,0,-9.81);
    dWorldSetCFM (world,1e-5);
    dCreatePlane (space,0,0,1,0);
    contactgroup = dJointGroupCreate (0);

    //  the simulation
    dsSimulationLoop (argc,argv,960,480,&fn);

  
  //-- Destroy the world!!! 
    
    dJointGroupDestroy (contactgroup);
    dSpaceDestroy (space);
    dWorldDestroy (world);
    dCloseODE();

    return 0;
}
