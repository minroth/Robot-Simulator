/************************************************************************/
/* robot.h   (C) Dr. Juan Gonzalez-Gomez. January 2009                  */
/*----------------------------------------------------------------------*/
/* GPL LICENSE                                                          */
/*----------------------------------------------------------------------*/
/* An example of simulation of a modular snake robot using the          */
/* Open Dynamics Engine (ODE)                                           */
/************************************************************************/

/*---------------------------*/
/*-- Snake data structure  --*/
/*---------------------------*/
struct MySnake {
  dJointID joint[MOD];       //-- Robot joints
  
  dBodyID body[MOD-1];       //-- Central bodies
                             //-- Every central body is composed of two boxes  
  dGeomID geom1[MOD-1];      //-- One in the left and another 
  dGeomID geom2[MOD-1];      //-- in the right
  
  dBodyID body_left;         //-- This is the body on the left of the snake
  dGeomID geom_left;         //-- and its geometry (a box)
  
  dBodyID body_right;        //-- This is the body on the right of the snake 
  dGeomID geom_right;        //-- and its geometry (a box)
  
  dReal servo_ref_pos[MOD];  //-- Reference positions for the servos
};


/*---------------------*/
/* Function prototypes */
/*---------------------*/

//*-- Build the ODE snake robot model 
void Robot_new(MySnake *snake, dWorldID world, dSpaceID space);

//-- Draw the snake robot on the screen
void Robot_render(MySnake *snake);
