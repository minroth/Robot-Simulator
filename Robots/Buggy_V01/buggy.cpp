/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

/*

buggy with suspension.
this also shows you how to use geom groups.

*/
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include "buggy.h"
#include "XODEParser.h"
#include <tinyxml.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

XODE_xode OXODE;

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i,n;

  // only collide things with the ground
  int g1 = (o1 == ground || o1 == ground_box);
  int g2 = (o2 == ground || o2 == ground_box);
  if (!(g1 ^ g2)) return;

  const int N = 10;
  dContact contact[N];
  n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
  dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.slip1 = 0.1;
      contact[i].surface.slip2 = 0.1;
      contact[i].surface.soft_erp = 0.5;
      contact[i].surface.soft_cfm = 0.01;
      dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
      dJointAttach (c,
        dGeomGetBody(contact[i].geom.g1),
        dGeomGetBody(contact[i].geom.g2));
    }
  }
}


// start simulation - set viewpoint

static void start()
{
  dAllocateODEDataForThread(dAllocateMaskAll);

  static float xyz[3] = {0.8317f,-0.9817f,0.8000f};
  static float hpr[3] = {121.0000f,-27.5000f,0.0000f};
  dsSetViewpoint (xyz,hpr);
  printf ("Press:\t'a' to increase speed.\n"
    "\t'z' to decrease speed.\n"
    "\t',' to steer left.\n"
    "\t'.' to steer right.\n"
    "\t' ' to reset speed and steering.\n"
    "\t'1' to save the current state to 'state.dif'.\n");
}


// called when a key pressed

static void command (int cmd)
{
  switch (cmd) {
  case 'a': case 'A':
    speed += 0.3;
    break;
  case 'z': case 'Z':
    speed -= 0.3;
    break;
  case ',':
    steer -= 0.5;
    break;
  case '.':
    steer += 0.5;
    break;
  case ' ':
    speed = 0;
    steer = 0;
    break;
  case '1': {
      FILE *f = fopen ("state.dif","wt");
      if (f) {
        dWorldExportDIF (world,f,"");
        fclose (f);
      }
    }
  }
}


// simulation loop
static void simLoop (int pause)
{
  int i;
  if (!pause) {
    // motor
    dJointSetHinge2Param (joint[0],dParamVel2,-speed);
    dJointSetHinge2Param (joint[0],dParamFMax2,0.1);

    
    //dJointAddHinge2Torques(joint[0], speed,-speed);

    // steering
    dReal v = steer - dJointGetHinge2Angle1 (joint[0]);
    if (v > 0.1) v = 0.1;
    if (v < -0.1) v = -0.1;
    v *= 10.0;
    dJointSetHinge2Param (joint[0],dParamVel,v);
    dJointSetHinge2Param (joint[0],dParamFMax,0.2);
    dJointSetHinge2Param (joint[0],dParamLoStop,-0.75);
    dJointSetHinge2Param (joint[0],dParamHiStop,0.75);
    dJointSetHinge2Param (joint[0],dParamFudgeFactor,0.1);

    dSpaceCollide (space,0,&nearCallback);
    dWorldStep (world,0.05);

    // remove all contact joints
    dJointGroupEmpty (contactgroup);
  }


  /*
  * Parametros para dibujar el objetos
  *
  */
  dsSetColor (0,1,1);
  dsSetTexture (DS_WOOD);

  XODE_world oWorld = OXODE.getoWorld();
  XODE_space oSpace = oWorld.getoSpace().front();
  XODE_box oBox =  oSpace.getOBody().front().getOGeom().front().getOBox();
  list<XODE_body> lstRuedas =  oSpace.getOBody();
  list<XODE_body> lstRuedasTmp = lstRuedas;

  dReal sides[3] = {oBox.getSizex(),oBox.getSizey(),oBox.getSizez()};

  dsDrawBox (dBodyGetPosition(body[0]),dBodyGetRotation(body[0]),sides);
  dsSetColor (1,1,1);
  for (i=1; i<=3; i++) 
  {
      lstRuedasTmp.pop_front();
      XODE_body oWheel = lstRuedasTmp.front();   
      XODE_sphere oGeometriaRueda = oWheel.getoSimpleBody().getOGeom().getOSphere();
      dsDrawCylinder (dBodyGetPosition(body[i]),dBodyGetRotation(body[i]),0.02f,oGeometriaRueda.getRadius());

  }

  dVector3 ss;
  dGeomBoxGetLengths (ground_box,ss);
  dsDrawBox (dGeomGetPosition(ground_box),dGeomGetRotation(ground_box),ss);  
}


int main (int argc, char **argv)
{

  int i;
  dMass m;
//Pruebas de carga exitosa de archivos.
/******************************************/

  TiXmlDocument XMLdoc=OXODE.loadXODE(argv[1]);
  std::cout << std::endl << "XODE."<< std::endl;
  std::cout << "Name: " << OXODE.getName() << std::endl;
  std::cout << "Version: " << OXODE.getVersion() << std::endl;
  std::cout << std::endl;
  
/******************************************/
  


//Variables necesarias para dibujar el mundo de simulación
/******************************************/
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = &command;
  fn.stop = 0;
  fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;


//Creacion del mundo de simulación
/******************************************/
  dInitODE2(0);
  world = dWorldCreate();
  XODE_world oWorld = OXODE.getoWorld();

  space = dHashSpaceCreate (0);
  XODE_space oSpace = oWorld.getoSpace().front();
  
  contactgroup = dJointGroupCreate (0);
  
  XODE_gravity oGravity= OXODE.getoWorld().getoExt().getGravity();

  dWorldSetGravity (world,
                  oGravity.getX(),
                  oGravity.getY(),
                  oGravity.getZ());

  /* Control de salida de variables */
  std::cout << "/****************************************************************/"<< std::endl ;
  std::cout << "......................Control de salida..........................."<< std::endl ;
  std::cout << "/****************************************************************/"<< std::endl ;
  std::cout << std::endl;

  std::cout << "Constantes globles:"<< std::endl;
  std::cout << "dWorldSetGravity(" 
            << oGravity.getX() << "," 
            << oGravity.getY() << "," 
            << oGravity.getZ() << ");" 
            << std::endl;

  XODE_plane oGround = oSpace.getOGeom().front().getOPlane();
  ground = dCreatePlane (space,oGround.getA(),oGround.getB(),oGround.getC(),oGround.getD());

  /* Control de salida de variables */
  std::cout << "dCreatePlane(" 
            << oGround.getA() << "," 
            << oGround.getB() << "," 
            << oGround.getC() << ","
            << oGround.getD() << ");" 
            << std::endl;
  std::cout << std::endl;  


//Creacion del los cuerpos 
/******************************************/

/************** CHASIS **********************/
    body[0] = dBodyCreate (world);
  XODE_simpleBody oChasis = oSpace.getOBody().front().getoSimpleBody();
  XODE_position oPositionChasis= oChasis.getOTransform().getOPosition();
  std::cout << "Cuerpo " << oChasis.getName()<< ": " <<std::endl;
  dBodySetPosition (
                    body[0],
                    oPositionChasis.getX(),
                    oPositionChasis.getY(),
                    oPositionChasis.getZ()
                    );

  /* Control de salida de variables */
  std::cout << "dBodySetPosition(" 
            << oPositionChasis.getX() << "," 
            << oPositionChasis.getY() << "," 
            << oPositionChasis.getZ() << ");"
            << std::endl;

  XODE_box oBox =  oSpace.getOBody().front().getOGeom().front().getOBox();
  XODE_mass oMass = oSpace.getOBody().front().getoSimpleBody().getOMass();

  dMassSetBox (&m,
              1,
              oBox.getSizex(),
              oBox.getSizey(),
              oBox.getSizez());

  /* Control de salida de variables */
  std::cout << "dMassSetBox(" 
            << "1," 
            << oBox.getSizex() << "," 
            << oBox.getSizey() << "," 
            << oBox.getSizez() << ");"
            << std::endl;

  dMassAdjust (&m,oMass.getOAdjust().getTotal());

  /* Control de salida de variables */
  std::cout << "dMassAdjust: " 
            << oMass.getOAdjust().getTotal()
            << std::endl;

  dBodySetMass (body[0],&m);
  box[0] = dCreateBox (0,
                    oBox.getSizex(),
                    oBox.getSizey(),
                    oBox.getSizez());

  /* Control de salida de variables */
  std::cout << "dCreateBox(" 
            << "0," 
            << oBox.getSizex() << "," 
            << oBox.getSizey() << "," 
            << oBox.getSizez() << ")"
            << std::endl;

  dGeomSetBody (box[0],body[0]);
  std::cout << std::endl;

/************** RUEDAS **********************/
  std::cout << "++++++++++++++++++++++++Ruedas++++++++++++++++++++++"<< std::endl;
  list<XODE_body> lstRuedas =  oSpace.getOBody();
  list<XODE_body> lstRuedasTmp = lstRuedas;

  for (i=1; i<=3; i++) {
    std::cout << std::endl;
    lstRuedasTmp.pop_front();
    XODE_body oWheel = lstRuedasTmp.front();
    std::cout << "Cuerpo " << oWheel.getoSimpleBody().getName()<< ": " <<std::endl;

    body[i] = dBodyCreate (world);

    dQuaternion q;
    XODE_axisangle oAnguloRueda = oWheel.getoSimpleBody().getOTransform().getORotation().getOAxisangle();
    dQFromAxisAndAngle (q,
                        oAnguloRueda.getX(),
                        oAnguloRueda.getY(),
                        oAnguloRueda.getZ(),
                        oAnguloRueda.getAngle()*M_PI);

    /* Control de salida de variables */
    std::cout << "dQFromAxisAndAngle(" 
            << "q," 
            << oAnguloRueda.getX() << "," 
            << oAnguloRueda.getY() << "," 
            << oAnguloRueda.getZ() << ","
            << oAnguloRueda.getAngle()<< "*M_PI);"
            << std::endl;


    dBodySetQuaternion (body[i],q);

    XODE_sphere oGeometriaRueda = oWheel.getoSimpleBody().getOGeom().getOSphere();
    dMassSetSphere (&m,1,oGeometriaRueda.getRadius());
    /* Control de salida de variables */
    std::cout << "dMassSetSphere(" 
            << "&m," 
            << "1,"
            << oGeometriaRueda.getRadius()<< ");"
            << std::endl;


    XODE_mass oMasaRueda = oWheel.getoSimpleBody().getOMass();
    dMassAdjust (&m,oMasaRueda.getOAdjust().getTotal());
    
    /* Control de salida de variables */
    std::cout << "dMassAdjust(" 
            << "&m," 
            << oMasaRueda.getOAdjust().getTotal()<< ");"
            << std::endl;


    dBodySetMass (body[i],&m);
    sphere[i-1] = dCreateSphere (0,oGeometriaRueda.getRadius());

    /* Control de salida de variables */
    std::cout << "dCreateSphere(" 
            << "0," 
            << oGeometriaRueda.getRadius()<< ");"
            << std::endl;


    dGeomSetBody (sphere[i-1],body[i]);

    XODE_position oPosicionRueda = oWheel.getoSimpleBody().getOTransform().getOPosition();
    dBodySetPosition (body[i],
                      oPosicionRueda.getX(),
                      oPosicionRueda.getY(),
                      oPosicionRueda.getZ());

    /* Control de salida de variables */
    std::cout << "dBodySetPosition(" 
            << "body["<<i<<"]," 
            << oPosicionRueda.getX()<<","
            << oPosicionRueda.getY()<<","
            << oPosicionRueda.getZ()<<");"
            << std::endl;

    std::cout << std::endl;

  }

//Uniones y "bisagras"
/******************************************/
  list<XODE_joint> lstJoint = oSpace.getOJoint();
  list<XODE_joint> lstJointTemp = lstJoint;
  std::cout << "++++++++++++++++++++++++Joints++++++++++++++++++++++"<< std::endl;
  for (i=0; i<3; i++) {
    joint[i] = dJointCreateHinge2 (world,0);

    dJointAttach (joint[i],body[0],body[i+1]);
    const dReal *a = dBodyGetPosition (body[i+1]);

    XODE_anchor oAnchor = lstJointTemp.front().getOHinge2().getOAnchor();
    dJointSetHinge2Anchor (joint[i],a[0],a[1],a[2]);

    XODE_axis oAxis1 = lstJointTemp.front().getOHinge2().getOAxis1();

    dJointSetHinge2Axis1 (joint[i],
                          oAxis1.getX(),
                          oAxis1.getY(),
                          oAxis1.getZ());

    /* Control de salida de variables */
    std::cout << "dJointSetHinge2Axis1(" 
            << "joint["<<i<<"]," 
            << oAxis1.getX()<<","
            << oAxis1.getY()<<","
            << oAxis1.getZ()<<");"
            << std::endl;


    XODE_axis oAxis2 = lstJointTemp.front().getOHinge2().getOAxis2();
    dJointSetHinge2Axis2 (joint[i],
                          oAxis2.getX(),
                          oAxis2.getY(),
                          oAxis2.getZ());

     /* Control de salida de variables */
    std::cout << "dJointSetHinge2Axis2(" 
            << "joint["<<i<<"]," 
            << oAxis2.getX()<<","
            << oAxis2.getY()<<","
            << oAxis2.getZ()<<");"
            << std::endl;

    lstJointTemp.pop_front();

    std::cout << std::endl;
  }

  //Suspenciones de la uniones
  lstJointTemp = lstJoint;
  std::cout << "++++++++++++++++++++++++Suspencion++++++++++++++++++++++"<< std::endl;
  for (i=0; i<3; i++) {

    XODE_axis oAxis1 = lstJointTemp.front().getOHinge2().getOAxis1();
    dJointSetHinge2Param (joint[i],
                          dParamSuspensionERP,
                          oAxis1.getSuspensionERP());

    /* Control de salida de variables */
     std::cout << "dJointSetHinge2Param(" 
            << "joint["<<i<<"]," 
            << "dParamSuspensionERP"<<","
            << oAxis1.getSuspensionERP()<<");"
            << std::endl;

    XODE_axis oAxis2 = lstJointTemp.front().getOHinge2().getOAxis2();
    dJointSetHinge2Param (joint[i],
                        dParamSuspensionCFM,
                        oAxis1.getSuspensionCFM());

     /* Control de salida de variables */
     std::cout << "dJointSetHinge2Param(" 
            << "joint["<<i<<"]," 
            << "dParamSuspensionCFM"<<","
            << oAxis1.getSuspensionCFM()<<");"
            << std::endl;

    lstJointTemp.pop_front();
    std::cout << std::endl;
  }

  std::cout << "++++++++++++++++++++++++Anclaje Ruedas++++++++++++++++++++++"<< std::endl;
  //Ancla las ruedas traseras al "eje de giro"
  lstJointTemp = lstJoint;
  for (i=1; i<3; i++) {

    XODE_axis oAxis1 = lstJointTemp.front().getOHinge2().getOAxis1();
    dJointSetHinge2Param (joint[i],
                          dParamLoStop,
                          oAxis1.getLowStop());


    /* Control de salida de variables */
     std::cout << "dJointSetHinge2Param(" 
            << "joint["<<i<<"]," 
            << "dParamLoStop"<<","
            << oAxis1.getLowStop()<<");"
            << std::endl;

    XODE_axis oAxis2 = lstJointTemp.front().getOHinge2().getOAxis2();
    dJointSetHinge2Param (joint[i],
                          dParamHiStop,
                          oAxis2.getHiStop());
    
    
    /* Control de salida de variables */
     std::cout << "dJointSetHinge2Param(" 
            << "joint["<<i<<"]," 
            << "dParamHiStop"<<","
            << oAxis2.getHiStop()<<");"
            << std::endl;

    lstJointTemp.pop_front();
    std::cout << std::endl;
  }

  // create car space and add it to the top level space
  car_space = dSimpleSpaceCreate (space);
  dSpaceSetCleanup (car_space,0);
  dSpaceAdd (car_space,box[0]);
  dSpaceAdd (car_space,sphere[0]);
  dSpaceAdd (car_space,sphere[1]);
  dSpaceAdd (car_space,sphere[2]);

  // environment
  ground_box = dCreateBox (space,2,1.5,1);
  dMatrix3 R;
  dRFromAxisAndAngle (R,0,1,0,-0.15);
  dGeomSetPosition (ground_box,2,0,-0.34);
  dGeomSetRotation (ground_box,R);

  // run simulation
  dsSimulationLoop (argc,argv,800,600,&fn);

  dGeomDestroy (box[0]);
  dGeomDestroy (sphere[0]);
  dGeomDestroy (sphere[1]);
  dGeomDestroy (sphere[2]);
  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
  return 0;
}
