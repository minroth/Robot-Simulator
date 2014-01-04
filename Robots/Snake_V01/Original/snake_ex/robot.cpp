/************************************************************************/
/* robot.cpp   (C) Dr. Juan Gonzalez-Gomez. January 2009                */
/*----------------------------------------------------------------------*/
/* GPL LICENSE                                                          */
/*----------------------------------------------------------------------*/
/* An example of simulation of a modular snake robot using the          */
/* Open Dynamics Engine (ODE)                                           */
/*----------------------------------------------------------------------*/
/* This file contains the functions needed for building the snake robot */
/* ODE model and drawing it                                             */
/************************************************************************/
 
#include <math.h>
#include <ode/ode.h>
#include "drawstuff.h"
#include "parameters.h"
#include "robot.h"


/*----------------------------------------------------------------------*/
/* Build the snake robot ODE model.                                     */
/*                                                                      */
/* INPUT:                                                               */
/*   -world: The ODE world in which the robot will be deployed          */
/*   -space: The collision space                                        */
/*                                                                      */
/* OUTPUT:                                                              */
/*   -snake: The robot built                                            */
/*----------------------------------------------------------------------*/
/*  The structure of the robot is composed of MOD joints and MOD+1      */
/*  rigid segments between the joints. The joints are modeled in ODE    */
/*  as hinge joints and the segments as bodies                          */
/*  Therefore, there are MOD hinge joints and MOD+1 bodies              */
/*                                                                      */
/*  The are three types of bodies:                                      */
/*    -The left and right bodies: composed of one Box each one          */
/*    -The central bodies: Composed of two boxes each                   */
/*                                                                      */
/*   As an example, a snake robot of 4 modules is composed of           */
/*   4 joints, 5 bodies and 8 boxes. It can be drawn as:                */
/*                                                                      */
/*        -*--*--*--*-                                                  */
/*                                                                      */
/*   Where - are the boxes and * the joints. The left and right bodies  */
/*   only have one box. The rest have two.                              */
/*                                                                      */
/*   The snake robot belong to the pitch-yaw connecting modular robot   */
/*   The even joints Pitch up and down and the odd joints Yaw left and  */
/*   right                                                              */
/*----------------------------------------------------------------------*/
void Robot_new(MySnake *snake, dWorldID world, dSpaceID space)
{
  dMass m;
  
  //-------------------------------------
  //-- Create the left body 
  //-------------------------------------
  snake->body_left = dBodyCreate(world);
  
  //-- Set its position in the virtual world
  dBodySetPosition(snake->body_left, 0.0, -L/4, H/2);
  
  //-- Set its mass
  dMassSetBoxTotal (&m, MASS/2, W,L/2,H);
  dBodySetMass (snake->body_left,&m);
  
  //-- Create its geometry (a box) and associate it to the body
  snake->geom_left = dCreateBox (space, W,L/2, H); 
  dGeomSetBody (snake->geom_left,snake->body_left);
  
  //-----------------------------------------------------------------
  //-- Create the central bodies. They are composed of two boxes
  //-----------------------------------------------------------------
  for (int i=0; i<MOD-1; i++) {
    
    //-- Create the ith body. Set its position and mass
    snake->body[i]=dBodyCreate(world);
    dBodySetPosition(snake->body[i], 0.0, -L*(i+1), H/2);
    dMassSetBoxTotal (&m, MASS, W,L/2,H);
    dBodySetMass (snake->body[i],&m);
    
    //-- Create the two boxes
    snake->geom1[i] = dCreateBox (space, W,L/2, H); 
    snake->geom2[i] = dCreateBox (space, W,L/2, H); 
    dGeomSetBody (snake->geom1[i],snake->body[i]);
    dGeomSetBody (snake->geom2[i],snake->body[i]);
    
    //-- Set the relative positions of the boxes within the body.
    //-- One is on the right and the other on the left
    dGeomSetOffsetPosition (snake->geom1[i], 0.0,  L/4, 0.0);
    dGeomSetOffsetPosition (snake->geom2[i], 0.0, -L/4, 0.0);
  }
  
  //---------------------------
  //-- Create the right body
  //---------------------------
  snake->body_right = dBodyCreate(world);
  
  //-- Set its position in the virtual world
  dBodySetPosition(snake->body_right, 0.0, -MOD*L+L/4, H/2);
  
  //-- Set its mass
  dMassSetBoxTotal (&m, MASS/2, W,L/2,H);
  dBodySetMass (snake->body_right,&m);

  //-- Create its geometry (a box) and associate it to the body
  snake->geom_right = dCreateBox (space, W,L/2, H); 
  dGeomSetBody (snake->geom_right,snake->body_right);
  
  //--------------------------
  //-- Create the joints
  //--------------------------
  
  //-- Build the central joints
  for (int i=1; i<=MOD-2; i++) {
    
    //-- Create the Hinge joint
    snake->joint[i]=dJointCreateHinge (world, 0);
    
    //-- The ith joint connects the i-1 and i bodies
    dJointAttach (snake->joint[i],snake->body[i-1],snake->body[i]);
    
    //-- Even joints pitch up and down
    if (i%2==0) 
      dJointSetHingeAxis (snake->joint[i], 1,0,0);  //-- Pitch
    else
      //-- Odd joints yaw right and left
      dJointSetHingeAxis (snake->joint[i], 0,0,1);  //-- Yaw
    
    //-- Set the joint position
    dJointSetHingeAnchor (snake->joint[i], 0, -i*L-L/2, H/2);
    
    //-- Set the joint parameters: Initial angular velocity and its maximum
    //-- torque
    dJointSetHingeParam(snake->joint[i], dParamFMax, TORQUE);
    dJointSetHingeParam(snake->joint[i], dParamVel, 0.0);
  }
  
  //-- Create the joint in the left
  snake->joint[0]=dJointCreateHinge (world, 0);
  
  //-- If the robot consist of only one module, the left joint is
  //-- connected to the left and right bodies.
  //-- If not, it is connected to the left body and the next
  if (MOD==1) 
    dJointAttach (snake->joint[0],snake->body_left,snake->body_right);
  else
    dJointAttach (snake->joint[0],snake->body_left,snake->body[0]);
  
  //-- Its a pitching joint.
  //-- Set its position and parameters
  dJointSetHingeAxis (snake->joint[0], 1,0,0); 
  dJointSetHingeAnchor (snake->joint[0], 0, -L/2, H/2);
  dJointSetHingeParam(snake->joint[0], dParamFMax, TORQUE);
  dJointSetHingeParam(snake->joint[0], dParamVel, 0.0);
  
  //-- Create the joint in the right. It only should be created if there
  //-- are more than one modules in the robot.
  if (MOD>1) {
    
    //-- Create the joint
    snake->joint[MOD-1]=dJointCreateHinge (world, 0);
    
    //-- Establish its connections
    dJointAttach (snake->joint[MOD-1],snake->body[MOD-2],snake->body_right);
    
    //-- Depending on the number of modules it is a pitching or yawing
    //-- joint
    if (MOD%2==0)
      dJointSetHingeAxis (snake->joint[MOD-1], 0,0,1);  //-- Yaw
    else
      dJointSetHingeAxis (snake->joint[MOD-1], 1,0,0);  //-- Pitch
    
    //-- Set its position and parameters
    dJointSetHingeAnchor (snake->joint[MOD-1], 0, -MOD*L+L/2, H/2);
    dJointSetHingeParam(snake->joint[MOD-1], dParamFMax, TORQUE);
    dJointSetHingeParam(snake->joint[MOD-1], dParamVel, 0.0);
  }  
  
}

/*****************************/
/* Draw a Box on the screen  */
/*****************************/
static void drawGeom (dGeomID g)
{
  const dReal *pos;
  const dReal *R;
  dReal pos2[4];	

  //-- Read its position and orientation
  pos = dGeomGetPosition (g);
  R = dGeomGetRotation (g);

  //-- Get the type of geometry. In this example it should always be a box
  int type = dGeomGetClass (g);

  //-- If it is a box (it should be)
  if (type == dBoxClass) {
    dVector3 sides;
    dGeomBoxGetLengths (g,sides);

    //-- Before drawing the box, the object is scaled
    pos2[0]=pos[0]*VIEW_SCALE;
    pos2[1]=pos[1]*VIEW_SCALE;
    pos2[2]=pos[2]*VIEW_SCALE;
    
    sides[0]*=VIEW_SCALE;
    sides[1]*=VIEW_SCALE;
    sides[2]*=VIEW_SCALE;
    
    //-- Draw the box
#ifdef dDOUBLE
    dsDrawBoxD (pos2, R, sides);
#else
    dsDrawBox (pos2, R, sides);
#endif
  }
}


/************************************************************************/
/* Draws the whole snake robot on the screen.                           */
/************************************************************************/
void Robot_render(MySnake *snake)
{
   //-- This is the "green" component color. The pitch and yaw modules are
   //-- drawn in different colors
   int G=0;
  
  //-- Set the snake texture
  dsSetTexture (DS_WOOD);
  
  //-- Draw the left body
  dsSetColor (1,1,0);
  drawGeom (snake->geom_left);
  
  //-- Draw all the central boxes
  for (int i=0; i<MOD-1; i++) {
    
    //-- Depending on the module, it is draw in one or another color
    if (i%2==0) G=1;
    else G=0;
    
    //-- Draw the first box
    dsSetColor (1,G,0);
    drawGeom(snake->geom1[i]);
    
    //-- Draw the second box
    dsSetColor (1,!G,0);
    drawGeom(snake->geom2[i]);
  }
  
  //-- Draw the right body
  dsSetColor (1,!G,0);
  drawGeom (snake->geom_right);
}







