// Sample3 by Kosei Demura
#include <ode/ode.h>
#include <drawstuff/drawstuff.h> 
#ifdef  dDOUBLE
#define dsDrawSphere dsDrawSphereD
#endif

static dWorldID world; // a physics world
dBodyID ball;  // an ball
const dReal radius = 0.2, mass = 10.0;// radius(m) weight(kg) of an ball

// Simulation loop
static void simLoop (int pause)
{
  const dReal *pos,*R;//position, ratation matrix 
  dWorldStep(world,0.05);// Step a simulation world, time step is 0.05 [s]
  dsSetColor(1.0,0.0,0.0);  // Set color  (red, green, blue) value is from 0 to 1.0
  pos = dBodyGetPosition(ball);// Get a position
  R = dBodyGetRotation(ball);// Get a ratation
  dsDrawSphere(pos,R,radius);// Draw a sphere
}

// Start function 

void start()
{
  // Set a camera
  static float xyz[3] = {0.0,-2.0,1.0};  // View position (x, y, z [m])
  static float hpr[3] = {90.0,0.0,0.0};  // View direction head, pitch, roll[°]
  dsSetViewpoint (xyz,hpr);// Set a view point
}

// main function
int main (int argc, char **argv)
{
  dReal x0 = 0.0, y0 = 0.0, z0 = 1.0; // initial position of an ball[m]
  dMass m1; // mass parameter

  // for drawstuff
  dsFunctions fn;//drawstuff structure
  fn.version = DS_VERSION;  // the version of the drawstuff
  fn.start = &start;// start function
  fn.step = &simLoop;        // step function
  fn.command = NULL;   // no command function for keyboard
  fn.stop  = NULL;     // no stop function
  fn.path_to_textures = "../textures";//path to the texture

  dInitODE();      // Initialize ODE
  world = dWorldCreate();  // Create a dynamic world
  dWorldSetGravity(world,0,0,-0.001);// Set gravity (x, y, z)

  //Create a ball
  ball = dBodyCreate(world); //  Crate a rigid body
  dMassSetZero(&m1); // Initialize mass parameters
  dMassSetSphereTotal(&m1,mass,radius);// Calcurate a mass parameter
  dBodySetMass(ball,&m1); // Set a mass parameter
  dBodySetPosition(ball, x0, y0, z0);//Set a position(x, y, z)

  // Simulation loop
  // argc, argv are argument of main function. Window size is  352 x 288 [pixel]
  // fn is a structure of drawstuff
  dsSimulationLoop (argc,argv,800,600,&fn);

  dWorldDestroy (world);// Destroy a world 
  dCloseODE();                 // Close ODE
  return 0;
}
