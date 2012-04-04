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
#ifdef dDOUBLE
#define dsDrawBox      dsDrawBoxD
#define dsDrawSphere   dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule  dsDrawCapsuleD
#define dsDrawLine     dsDrawLineD
#endif


/***********************************************************************************/
/*
*               Parametros de los objetos
*/
/***********************************************************************************/




/***********************************************************************************/
/*
*               Obtengo un ID del mundo y creo los objetos
*/
/***********************************************************************************/
static dWorldID world;
static dSpaceID space;
static dBodyID box;    
static dGeomID geom;  
static dJointGroupID contactgroup;


/***********************************************************************************/
/*
*               ; los puntos de colision 
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
    contact.surface.bounce = 0.9;
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.98;
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

    //@a sphere
    dsSetColor(0,1,0);      // set color (red, green, blue) a value is between 0 and 1
    dSpaceCollide (space,0,&nearCallback); // find collisions and add contact joints
    dWorldStep(world,0.01);// Step a simulation world, time step is 0.05 [s]
    dJointGroupEmpty (contactgroup); // remove all contact joints
    dsSetSphereQuality(3);  // set sphere quality. 3 is enough
    pos1 = dBodyGetPosition(sphere); // get a body position
    R1   = dBodyGetRotation(sphere); // get a body rotation matrix
    dsDrawSphere (pos1,R1,dGeomSphereGetRadius (geom));
    printf ("Pocision: %f\n",pos1[2]);
    
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
    

    

    //@a sphere
    sphere = dBodyCreate (world);     // create a rigid body
    geom = dCreateSphere (space,0.3);   //create geometry.

    dReal radius = 0.5;                    // radius [m]
    dMassSetZero(&m); // Initialize mass parameters
    dMassSetSphere (&m,mass,radius);    // calculate a mass parameter for a sphere
    dBodySetMass (sphere,&m);         // set the mass parameter to the body
    dGeomSetBody(geom,sphere);
    dBodySetPosition (sphere,0,10, 5); // set the position of the body


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
