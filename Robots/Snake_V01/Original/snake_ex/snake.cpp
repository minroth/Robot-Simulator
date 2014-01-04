/***********************************************************************/
/* snake.cpp.  Dr. Juan Gonzalez-Gomez. December 2008                  */
/*---------------------------------------------------------------------*/
/* GPL LICENSE                                                         */
/*---------------------------------------------------------------------*/
/* An example of simulation of a modular snake robot using the         */
/* Open Dynamics Engine (ODE)                                          */
/*---------------------------------------------------------------------*/
/* The user can change the gait by pressing some keys. The snake       */
/* can perform 8 different gaits:                                      */
/*  -Moving forward                                                    */
/*  -Turning                                                           */
/*  -Side winding                                                      */
/*  -Inclined side-winding                                             */
/*  -Flapping                                                          */
/*  -S-shaped rotating                                                 */
/*  -U-shaped rotating                                                 */
/*  -Rolling                                                           */
/*---------------------------------------------------------------------*/
/* More information about snake robots locomotion can be found in my   */
/* Ph. D. Dissertation, available at:                                  */
/* http://www.iearobotics.com/wiki/index.php?title=Juan_Gonzalez:Tesis */
/***********************************************************************/
 
#include <unistd.h>
#include <math.h>
#include <ode/ode.h>
#include "drawstuff.h"
#include "parameters.h"
#include "robot.h"

/**************************/
/*-- GLOBAL VARIABLES     */
/**************************/
//-- World identificator
static dWorldID world;

//-- Collision space identification
static dSpaceID space;

//-- JointGroup identification
static dJointGroupID contactgroup;

//-- The modular snake robot to be simulated
static MySnake snake;

//-- Sample number. It is the discrete time used for sampling the
//-- sin function
static int n=0;

//-- Generator's parameters
//-- These parameters are used for controlling the gaits
dReal Av, Ah;          //-- Horizontal and vertical amplitudes
dReal PDv, PDh, PDvh;  //-- Phase differences
dReal F0;              //-- Initial phase
dReal Oh;              //-- Horizontal Offset

//-- Snake direction
int direction=-1;

/***************************************************************************/
/*  Functions for realizing the gaits                                      */
/*  The generator's parameters are set according the gait to be realized   */
/*  There are one sinusoidal generator for each joint. The generators are  */
/*  divided into two groups: horizontal and vertical for controlling both  */
/*  the pitch and yaw joints                                               */
/*                                                                         */
/*   CONTROL SPACE                                                         */
/*   -Parameters for the vertical generators:                              */
/*      -Av: Amplitude. (0,90) degrees                                     */
/*      -PDv: Phase difference between two adjacent vertical generators    */
/*            (0,180) degrees                                              */
/*   -Parameters for the horizontal generators:                            */
/*      -Ah: Amplitude. (0,90) degrees                                     */
/*      -PDh: Phase difference between two adjacent horizontal generators  */
/*             (0,180) degrees                                             */
/*      -Oh: Offset (mean value)                                           */
/*   -Other parameters:                                                    */
/*      -F0: Initial phase                                                 */
/*      -PDvh: Phase difference between the vertical and horizontal        */
/*             generators                                                  */
/***************************************************************************/


/*---------------------------------------------------*/
/* The snake moves in a straight line.               */
/* Input parameters are from the SHAPE SPACE:        */
/*  -alfa: winding angle (0-120 degrees)             */
/*  -k: Number of vertical undulations               */
/*---------------------------------------------------*/
void straight(dReal alfa,dReal k)
{
  //-- No horizontal wave
  Ah=0; Oh=0; F0=0;
  
  //-- There are a vertical wave
  //-- Get the control parameters from the space parameters
  Av=2*alfa*sin(2*M_PI*k/MOD);
  PDv=direction*(4*M_PI*k/MOD)*180/M_PI;
}


/*------------------------------------------------*/
/* The snake moves in a circular arc              */
/* Input parameters from the SHAPE SPACE          */
/*   -alfah: arc's angle (0-360)                  */
/*   -kv: Number of vertical undulations          */
/*------------------------------------------------*/
void turning(dReal alfah, dReal kv)
{
 
  //-- There are no horizontal wave, but the yawing joints angle is set to
  //-- a fixed value
  Ah=0; F0=0;
  Oh=2*alfah/MOD;
  
  //-- There are a vertical wave
  //-- Get the control parameters from the space parameters
  Av=2*20*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
}

/*----------------------------------------------------------------------------*/
/* Side-winding gait. The snake moves parallel to its body axis, to the left  */
/* or to the right.                                                           */
/* Input parameters are from the SHAPE SPACE:                                 */
/*  -alfah: Horizontal winding angle (0-120 degrees)                          */
/*  -kh: Number of horizontal undulations                                     */
/*----------------------------------------------------------------------------*/
void side_winding(dReal alfah, dReal kh)
{
  dReal kv=kh;
  
  //-- There is a low amplitude vertical wave
  Av=2*5*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- There is a horizontal wave. Both the vertical and horizontal waves
  //-- have the same undulations
  Oh=0;
  Ah=2*alfah*sin(2*M_PI*kh/MOD);
  PDh=PDv;
  PDvh=0;
}

/*----------------------------------------------------------------------------*/
/* Inclined Side-winding gait.                                                */
/* Input parameters are from the SHAPE SPACE:                                 */
/*  -alfa: winding angle (0-120 degrees)                                      */
/*  -k: Number of undulations                                                 */
/*----------------------------------------------------------------------------*/
void inclined_side_winding(dReal alfa, dReal k)
{
  dReal alfav = alfa*sin(45);
  dReal alfah = alfav;
  dReal kv=k;
  dReal kh=k;
  
  
  //-- Vertical wave
  Av=2*alfav*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- Horizontal wave
  Oh=0;
  Ah=2*alfah*sin(2*M_PI*kh/MOD);
  PDh=PDv;
  PDvh=0; 
}

/*----------------------------------------------------------------------------*/
/* flapping. The robot performs a lateral shifting                            */
/*----------------------------------------------------------------------------*/
void flapping()
{
  dReal alfa=12;
  
  //-- All the vertical joints are in phase
  //-- All the horizontal joints are in phase
  //-- The phase difference between vertical and horizontal is 90 degrees
  //-- The Amplitudes are very small. If they are bigger, the rolling gait
  //-- is performed
  Av=2*alfa/MOD;
  Ah=Av;
  PDv=0;
  PDh=0;
  PDvh=direction*90;
  Oh=0;
}

/*-----------------------------------------------------------------------------*/
/* S-shaped Rotating gait. The snake performs a rotation parallel to the ground */
/* changing the orientation of its body.                                       */
/* It is quite similar to the side-winding gait but the robot rotates instead  */
/* of performing the lateral shifting.                                         */
/* Input parameters are from the SHAPE SPACE:                                  */
/*  -alfah: winding angle (0-120 degrees)                                      */
/*  -kh: Number of horizontal undulations                                      */
/*-----------------------------------------------------------------------------*/
void rotating_S(dReal alfah, dReal kh)
{
  dReal kv=2*kh;
  
  //-- The vertical wave have a very low amplitude and the number of undulation
  //-- is double than the horizontal wave
  Av=2*5*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- The horizontal wave has a bigger amplitude
  Oh=0;
  Ah=2*alfah*sin(2*M_PI*kh/MOD);
  PDh=direction*(4*M_PI*kh/MOD)*180/M_PI;
  PDvh=0;
}

/*-----------------------------------------------------------------------------*/
/* U-shaped Rotating gait. The snake performs a rotation parallel to the ground */
/* changing the orientation of its body. The snake body has a U-shaped         */
/* Input parameters are from the SHAPE SPACE:                                  */
/*  -alfa: horizontal arc angle (0-360 degrees)                                */
/*-----------------------------------------------------------------------------*/
void rotating_U(dReal alfa)
{
  dReal kv=2;
  
  //-- The vertical wave have a very low amplitude 
  Av=2*5*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- The horizontal joints are in phase
  Oh=0;
  Ah=2*alfa/MOD;
  PDh=0;
  PDvh=direction*90;
}

/*---------------------------------------------------------------------------*/
/* Rolling gait. The snake rolls around its body axis. The shape of its body */
/* has a U-shape.                                                            */
/* Input parameters are from the SHAPE SPACE:                                */
/*  -alfa: arc angle (0-360 degrees)                                         */
/*---------------------------------------------------------------------------*/
void rolling(dReal alfa)
{
  //-- All the vertical joints are in phase
  //-- All the horizontal joints are in phase
  //-- The phase difference between vertical and horizontal is 90 degrees
  //-- The amplitudes should not be so small. If they are, the snake will
  //-- perform the flapping gait.
  
  Av=2*alfa/MOD;
  Ah=Av;
  PDv=0;
  PDh=0;
  PDvh=direction*90;
  Oh=0;
  F0=90;
}


/***************************************************************************/
/* CALLBACK FUNCTIONS                                                      */
/***************************************************************************/

/*---------------------------------------------------------------------------*/
/* Callback function invoked by the dSpaceCollide function when two objects  */
/* are about to collide                                                      */
/* The contact points should be found and a special contact joints should be */
/* added. After that simulation step, the contact joints should be removed   */
/*---------------------------------------------------------------------------*/
static void nearCallback (void *data, dGeomID o1, dGeomID o2)   
{
  int i;
   
  //-- Get the body's ID
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  
  //-- If they are connected by a joint, no colision detection is done. Finish
  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) {
    return;
  }
 
  //-- Configure the properties for the contact points
  dContact contact[MAX_CONTACTS]; 
  for (i=0; i<MAX_CONTACTS; i++) {
    contact[i].surface.mode = dContactBounce | dContactSoftCFM;
    contact[i].surface.mu = MU;
    contact[i].surface.mu2 = MU2;
    contact[i].surface.bounce = BOUNCE;
    contact[i].surface.bounce_vel = BOUNCE_VEL;
    contact[i].surface.soft_cfm = SOFT_CFM;
    
  }
  
  //-- Get the contact points
  int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom, sizeof(dContact));

  
  //-- If there are at least one contact point...
  if (numc!=0) {
    
    //-- For every contact point a joint should be created
    for (i=0; i<numc; i++) {

      //-- Create the joint and add it to the contact group
      dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
      
      //-- Set the articulation between the two bodies
      dJointAttach (c,b1,b2);
    }
  }   
}

//----------------------------------------------------------------------
//-- Simulation start callback function. This function is called when
//-- the simulation is started.
//-- The camera viewpoint is set
//----------------------------------------------------------------------
static void start()
{
  //-- Camera position. 
  static float xyz[3] = {-8.540,-5.74,3.34};

  //-- Camera orientation. The three values are Pan, Tilt and Roll.
  //-- A (0,0,0) value means the camera is point to the positive direction of
  //-- of the x axis and it is parallel to the ground.
  static float hpr[3] = {0.0f,-15.0f,0.0f};

  //-- Set camera position and orientation
  dsSetViewpoint (xyz,hpr);

  //-- Print the Menu for controlling the snake robot
  printf ("Keys for selecting the gaits: \n");
  printf ("1: Straight\n");
  printf ("2: Turning\n");
  printf ("3: Side-winding\n");
  printf ("4: Inclined side-winding\n");
  printf ("5: Flapping\n");
  printf ("6: S-shaped rotating\n");
  printf ("7: U-sahped rotating\n");
  printf ("8: Rolling\n");
  printf ("q: Quit simulation\n");

}

/*---------------------------------------------------------*/
/* Callback function that is called when a key is pressed  */
/* The snake gait is set depending on the key pressed      */
/*---------------------------------------------------------*/
static void command (int cmd)
{

  //-- Every time a gait is selected the direction is changed.
  //-- For example, if the '1' key is pressed once, the snake moves
  //-- in a straight line. But if it is pressed again, the snake will
  //-- move backward. If pressed again, it will move forward.
  
  if (cmd=='1') {
    direction=direction*-1;
    straight(20,2);
  }
  else if (cmd=='2') {
    direction=direction*-1;
    turning(180,2);
  }
  else if (cmd=='3') {
    direction=direction*-1;
    side_winding(40,1);
  }
  else if (cmd=='4') {
    direction=direction*-1;
    inclined_side_winding(40,1);
  }
  else if (cmd=='5') {
    direction=direction*-1;
    flapping();
  }
  else if (cmd=='6') {
    direction=direction*-1;
    rotating_S(40,1); 
  }  
  else if (cmd=='7') {
    direction=direction*-1;
    rotating_U(180);
  }
  else if (cmd=='8') {
    direction=direction*-1;
    rolling(180);
  }
  else if (cmd=='q') {
    //-- Finish the simulation and exit
    dsStop();
  }
}


/***************************************************************************/
/* FUNCTIONS FOR PERFORMING THE SIMULATION                                 */
/***************************************************************************/


/*---------------------------------------------------------------------------*/
/* Simulation of the sinusoidal oscillators. This function is called         */
/* every time all the servos have reached their reference positions. The     */
/* next servo positions are calculated by means of sampling sinusoidal       */
/* functions                                                                 */
/*  The n variable is the discrete time                                     */
/* The function for calculating the servo position depends on the type of    */
/*  module: horizontal (yawing) or vertical (pitching)                       */
/* The equations are:                                                        */
/*   -Vertical servos:                                                       */
/*         pos(i) = Av*sin(2*PI*n/N + PDv*(i/2) + F0);                       */
/*   -Horizontal servos:                                                     */
/*         pos(i) = Ah*sin(2*PI*n/N + PDh(i-1)/2 + PDvh + F0) + Oh           */
/*  Where N is the total number of samples. The default is 32                */
/*----------------------------------------------------------------------------*/
void sequence_generation()
{
  dReal phase;
  
  //-- The new positions are calculated for all the servos
  //-- There are MOD modules
  for (int i=0; i<MOD; i++) {
    
    if (i%2==0) {
      //-- when i is even: Pitching servo
      //-- Calculate its position according the equation
      phase = PDv*(i/2) + F0;
      snake.servo_ref_pos[i]=Av*sin(2*M_PI*n/N + DEG2RAD(phase) );
    }
    else {
      //-- When i is odd: Yawing servo
      //-- Calculate its position according the equation
      phase = PDh*(i-1)/2 + PDvh + F0;
      snake.servo_ref_pos[i]=Ah*sin(2*M_PI*n/N + DEG2RAD(phase))+Oh;
    }    
  }
  
  //-- Increment the discrete time. Its value is between 0 and N-1
  n = (n + 1) % N;  
}

/*-------------------------------------------------------------------------*/
/* Servos simulation                                                       */
/* This function performs a simulation step in the servos.                 */
/* A proportional controller is used, with a KP gain.                      */
/* The current servo angle is read and compared to the reference position  */
/* This difference is the error. Then the servo angular velocity is set.   */
/* It is proportional to the error, but it has a maximum value (WMAX).     */
/*                                                                         */
/* If all the servos have reached their reference positions, the           */
/* sequence_generation() function is called for calculating the next       */
/* reference angles                                                        */
/*                                                                         */
/* For obtaining a smoother simulation the servos are not allowed to reach */
/* their reference position. When the servo angle is near the reference    */
/* position (and therefore the servo velocity is not 0) the new position is */
/* calculated. This way, the servos never stop and the movement is         */
/* smoother.                                                               */
/*-------------------------------------------------------------------------*/
void servos_sim()
{
  int stable=0;
  
  //-- For every servo in the snake robot...
  for (int i=0; i<MOD; i++) {
     
    //-- Get the current servo angle
    dReal pos = dJointGetHingeAngle(snake.joint[i]);
  
    
    //-- Calculate the error as the difference between the current and
    //-- the reference positions. It is the distance the servos should
    //-- move to reach the reference position
    dReal error = pos - DEG2RAD(snake.servo_ref_pos[i]);
    
    //-- Proportional controller. The servo angular velocity is calculated
    //-- by multiplying the error and the KP constant. If the distance is
    //-- big, the servo moves fast. If the distance is small, the servo
    //-- moves slow. When it reach the reference position, the velocity is 0
    dReal velocity = -error*KP;

    //-- Velocity limitation. It cannot move faster than the maximum servo
    //-- angular velocity (given by the WMAX constant)
    if (velocity > WMAX) velocity = WMAX;
    if (velocity < -WMAX) velocity = -WMAX;
    
    //-- Set the servo angular velocity
    dJointSetHingeParam(snake.joint[i], dParamVel, velocity);
    
    //-- Check if the ith servo is near the reference position. The servo
    //-- is "near" the reference position when its distance to it is smaller
    //-- than the ERROR constant
    if (fabs(error)<ERROR) stable++;
  }  
 
  //-- when all the servos are "near" their reference positions, the next
  //-- positions should be calculated
  if (stable==MOD) {
    sequence_generation();
  }
}

/*--------------------------------------------------------------------------*/
/*- Simulation loop. This function is called at every simulation step.      */
/*  IT IS THE MAIN SIMULATION FUNCTION.                                     */
/*  For every step, the following task should be done:                      */
/*    -Check the collisions between the objects                             */
/*    -Perform the simulation step: all the body positions, velocities and  */
/*     acceleration are calculated for this instant.                        */
/*    -Servo simulation: All the servo should be simulated                  */
/*    -Remove the contact points                                            */
/*    -Draw the Snake on the screen                                         */
/*--------------------------------------------------------------------------*/
static void simLoop (int pause)
{
  if (!pause) {
    //-- Collision detection. If two or more objects are about to collide, the
    //-- "nearcallback" function is called.
    dSpaceCollide (space,0,&nearCallback);

    //-- Perform a simulation step. All the objects are updated
    dWorldStep(world,STEP);

    //-- Perform a simulation step on the servos. All of them are updated
    servos_sim();

    //-- Remove the contacting points
    dJointGroupEmpty (contactgroup);

    //-- Wait. It is a small pause for reducing the CPU usage.
    //-- If the snake moves very fast, the PAUSE constant should be bigger.
    //-- If the snake moves very slow, the PAUSE constant should be smaller.
    usleep(PAUSE);
  }

  //-- Draw the snake on the screen
  Robot_render(&snake);
  
}

/*******************/
/*     MAIN        */
/*******************/
int main (int argc, char **argv)
{
  /*-------------------------------------------------------------*/
  /* Set the drawstuff parameters and callback functions.        */
  /*-------------------------------------------------------------*/
  
  dsFunctions fn;
  
  //-- Set the Starting simulation Callback function
  fn.start = &start;
  
  //-- Set the Simulation step callback function
  fn.step = &simLoop;

  //-- Set the key pressed callback function
  fn.command = &command;

  //-- Others
  fn.version = DS_VERSION;
  fn.stop = 0;
  fn.path_to_textures = (char *)"./textures";

  /*------------------------------------------------------------------*/
  /* Create the simulation world. It is a container for all the       */
  /* virtual objects that want to be simulated                        */
  /* This virtual world knows nothing about how to draw the objects   */
  /*------------------------------------------------------------------*/
 
  //-- Create the virtual world
  world = dWorldCreate();
  
  //-- Set the gravity. (Earth gravity: -9.81)
  dWorldSetGravity (world,0,0,GRAVITY_CTE);

  //-- Set the CFM parameter.
  dWorldSetCFM (world,CFM);

  //-- Set the auto disabled flag. All the idle objects are disabled, so that
  //-- no resources are needed for its simulation.
  dWorldSetAutoDisableFlag (world,1);

  //-- Set the other ODE parameters (see the ODE documentation for details)
  dWorldSetContactMaxCorrectingVel (world,MAX_CORRECTING_VEL);
  dWorldSetContactSurfaceLayer (world,SURFACE_LAYER);

  //-- Create the collision space. It contains all the geometries that should
  //-- be check for collisions.
  space = dHashSpaceCreate (0);

  //-- Create the contact group for storing the contact point of every collision
  contactgroup = dJointGroupCreate (0);
  
  //-- Create a plane that acts as the ground. It is given by the equation:
  //-- a*x + b*y + c*z = d, where (a,b,c) is the unitary vector perpendicular
  //-- to the plane. In this simulation, the ground is located at (0,0,1). 
  //-- That is, the z=0 plane.
  dCreatePlane (space,0,0,1,0);

  //-- Build the snake robot
  Robot_new(&snake,world,space);
  
  //-- Set the straight movement by default
  straight(20,2);

  /********************************/
  /** START THE SIMULATION        */
  /********************************/
  //-- This is the main loop. All the callback functions will be called.
  //-- The screen dimensions are passed as parameters
  dsSimulationLoop (argc,argv,400,300,&fn);

  /**************************/
  /* END OF THE SIMULATION  */
  /**************************/

  //-- Destroy the contact group
  dJointGroupDestroy (contactgroup);
  
  //-- Destroy de collision space
  dSpaceDestroy (space);
  
  //-- Destroy the world!!! 
  dWorldDestroy (world);

  return 0;
}
