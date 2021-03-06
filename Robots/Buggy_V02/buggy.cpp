/**
 *  @file   buggy.cpp
 *  @Author Miguel Rodriguez (miguel.andres.rodriguez.s@gmail.com)
 *  @date   Julio 2013
 *  @brief   Simulacion de un vehiculo de 3 ruedas usando ODE.
 *
 *  Simulacion de un vehiculo de 3 ruedas, utilizando un 
 *  motor de fisica de tiepo real llamado ODE (Open Dynamics Engine) y una libreria
 *  llamada Drawstuff para la representacion grafica de la simulacion.
 *  
 *  La simulacion contempla 2 tipos de motores para acelerar el vehiculo. El motor
 *  "ENGINE_TYPE_ONE", un motor con una fuerza de torque fija y un motor "ENGINE_TYPE_TWO"
 *  con fuerza de torque acomulativa y creciente en unidades representadas por "aceleration". 
 *  Las configuraciones estan el en archivo parameters.h
 *  @see parameters.h
 *
 *  Este codigo es una extension al codigo que es proporcionado con la libreria 
 *  ODE.
 */

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "XODEParser.h"
#include <tinyxml.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include "buggy.h"
#include "parameters.h" 

using namespace std;


/**
 *  @brief   Manejo de colisiones-
 *  @ingroup Colision
 *
 *  Funcion que evalua las colisiones en cada paso de la simulacion.
 *
 *  @param data Referencia al espacio donde ocurre la simulación
 *  @param o1   Geometria 1 a evaluar      
 *  @param o2   Geometria 2 a evaluar  
 */
static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i,n;

  int g1 = (o1 == ground || o1 == ground_box);
  int g2 = (o2 == ground || o2 == ground_box);
  if (!(g1 ^ g2)) return;

  const int N = MAX_CONTACTS;
  dContact contact[N];
  n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
  if (n > 0) {
    for (i=0; i<n; i++) {
      contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
  dContactSoftERP | dContactSoftCFM | dContactApprox1;
      contact[i].surface.mu = dInfinity;
      contact[i].surface.slip1 = SLIP1;
      contact[i].surface.slip2 = SLIP2;
      contact[i].surface.soft_erp = SOFT_ERP;
      contact[i].surface.soft_cfm = SOFT_CFM;
      dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
      dJointAttach (c,
        dGeomGetBody(contact[i].geom.g1),
        dGeomGetBody(contact[i].geom.g2));
    }
  }
}


/**
 *  @brief   Inicio de simulacion.
 *  @ingroup Drawstuff
 *
 *  Funcion que setea los parametros de la simulacion como el punto de vista desde el que comienza la simulacion.
 *  Esta funcion es utilizada por la libreria Drawstuff para representar la simulacion.
 *
 */

static void start()
{
  dAllocateODEDataForThread(dAllocateMaskAll);

  static float xyz[3] = {0.8317f,-0.9817f,0.8000f};
  static float hpr[3] = {121.0000f,-27.5000f,0.0000f};
  dsSetViewpoint (xyz,hpr);
  printf ("Presiona:\t'a' para incrementar la velocidad.\n"
    "\t'z' para dismunuir la velocidad.\n"
    "\t',' para virar a la izquierda.\n"
    "\t'.' para virar a la derecha.\n"
    "\t' ' para reiniciar la velocidad y el angulo de viraje.\n"
    "\t'1' Para guardar el estado en un archivo 'state.dif'.\n");
}

/**
 *  @brief   Comandos 
 *  @ingroup Drawstuff
 *
 *  Funcion utilizada por la libreria Draustuff para setear los comandos u ordenes 
 *  en cada paso de simulación.
 *
 *  @param cmd Entrada de teclado.
 */
static void command (int cmd)
{
  switch (cmd) {
  case 'a': case 'A':
    if(ENGINE_TYPE_ONE) speed = aceleration;
    else if(ENGINE_TYPE_TWO) speed += aceleration;
    break;
  case 'z': case 'Z':
    if(ENGINE_TYPE_ONE) speed =-aceleration;
    else if(ENGINE_TYPE_TWO) speed -= aceleration;
    break;
  case ',':
    steer -= steerWheel;
    break;
  case '.':
    steer += steerWheel;
    break;
  case ' ':
    speed = 0;
    steer = 0;
    break;
  case '1': {
      std::ostringstream buffer;
      float now = fileNumber;
      buffer << now;
      std::string str = buffer.str();
      std::string filename = "state" + str + ".dif";

      FILE *f = fopen (filename.c_str(),"wt");
      if (f) {
        dWorldExportDIF (world,f,"");
        fclose (f);
      }
      fileNumber++;
    }
  }
}


/**
 *  @brief   Render del vehiculo.
 *  @ingroup Drawstuff
 *
 *  Funcion que utiliza funciones de la libreria Drawstuff para dibujar el vehiculo en la simulación.
 *
 */
void buggy_render()
{
  dsSetColor (chasis_r,chasis_g,chasis_b);
  dsSetTexture (DS_WOOD);

  XODE_world oWorld = OXODE.getoWorld();
  XODE_space oSpace = oWorld.getoSpace().front();
  XODE_box oBox =  oSpace.getOBody().front().getOGeom().front().getOBox();
  list<XODE_body> lstRuedas =  oSpace.getOBody();
  list<XODE_body> lstRuedasTmp = lstRuedas;

  dReal sides[3] = {oBox.getSizex(),oBox.getSizey(),oBox.getSizez()};

  dsDrawBox (dBodyGetPosition(body[0]),dBodyGetRotation(body[0]),sides);
  dsSetColor (wheels_r,wheels_g,wheels_b);
  for (int i=1; i<=3; i++) 
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

/**
 *  @brief   Loop de simulacion.
 *
 *  Funcion que es llamada en cada paso de simulacion y calcula las colisiones, 
 *  la velocidad del motor y el angulo de giro de la rueda delantera.
 *
 *  @param pause
 */
static void simLoop (int pause)
{
  int i;
  if (!pause) {
    // Se agrega funa furza de torque igual a speed en el eje y.
    dBodyAddTorque           (body[1], 0, speed, 0);
    /**
    Implementacion para 2 motores traseros.
    */
    //dBodyAddForce            (body[0], 0, 0, speed);
    //dJointSetHinge2Param (joint[1],dParamVel2,-speed);
    //dJointSetHinge2Param (joint[1],dParamFMax2,0.1);
    //dJointSetHinge2Param (joint[2],dParamVel2,-speed);
    //dJointSetHinge2Param (joint[2],dParamFMax2,0.1);
    
    // "Volante"
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
    dWorldStep (world,STEP);

    // remove all contact joints
    dJointGroupEmpty (contactgroup);
  }
  buggy_render();
}




/**
 *  @brief   Creacion del vehiculo
 *
 *  Funcion que crea el las geometrias y constantes fisicas del vehiculo a simular. 
 *  
 *  @param world  ID del mundo que se esta simulando. 
 *  @param oSpace Objeto que representa el espacio de simulacion. Aqui estan contenidos todas 
 *		las constantes fisicas para los objetos.
 *  @param space  ID del espacio que se esta simulando
 *  @param m      Representacion de la masa.
 */
void buggy_new(dWorldID world,XODE_space oSpace, dSpaceID space,dMass m)
{
  int i;
   /************** CHASIS **********************/
  body[0] = dBodyCreate (world);
  XODE_simpleBody oChasis = oSpace.getOBody().front().getoSimpleBody();
  XODE_position oPositionChasis= oChasis.getOTransform().getOPosition();
  dBodySetPosition (
                    body[0],
                    oPositionChasis.getX(),
                    oPositionChasis.getY(),
                    oPositionChasis.getZ()
                    );

 

  XODE_box oBox =  oSpace.getOBody().front().getOGeom().front().getOBox();
  XODE_mass oMass = oSpace.getOBody().front().getoSimpleBody().getOMass();

  dMassSetBox (&m,
              1,
              oBox.getSizex(),
              oBox.getSizey(),
              oBox.getSizez());

  
  dMassAdjust (&m,oMass.getOAdjust().getTotal());

  dBodySetMass (body[0],&m);
  box   = dCreateBox (0,
                    oBox.getSizex(),
                    oBox.getSizey(),
                    oBox.getSizez());


  dGeomSetBody (box,body[0]);
  std::cout << std::endl;

   /************** RUEDAS **********************/
  
  list<XODE_body> lstRuedas =  oSpace.getOBody();
  list<XODE_body> lstRuedasTmp = lstRuedas;

  for (i=1; i<=3; i++) {
    std::cout << std::endl;
    lstRuedasTmp.pop_front();
    XODE_body oWheel = lstRuedasTmp.front();
    

    body[i] = dBodyCreate (world);

    dQuaternion q;
    XODE_axisangle oAnguloRueda = oWheel.getoSimpleBody().getOTransform().getORotation().getOAxisangle();
    dQFromAxisAndAngle (q,
                        oAnguloRueda.getX(),
                        oAnguloRueda.getY(),
                        oAnguloRueda.getZ(),
                        oAnguloRueda.getAngle()*M_PI);

    


    dBodySetQuaternion (body[i],q);

    XODE_sphere oGeometriaRueda = oWheel.getoSimpleBody().getOGeom().getOSphere();
    dMassSetSphere (&m,1,oGeometriaRueda.getRadius());
   

    XODE_mass oMasaRueda = oWheel.getoSimpleBody().getOMass();
    dMassAdjust (&m,oMasaRueda.getOAdjust().getTotal());
    
    dBodySetMass (body[i],&m);
    sphere[i-1] = dCreateSphere (0,oGeometriaRueda.getRadius());

    dGeomSetBody (sphere[i-1],body[i]);

    XODE_position oPosicionRueda = oWheel.getoSimpleBody().getOTransform().getOPosition();
    dBodySetPosition (body[i],
                      oPosicionRueda.getX(),
                      oPosicionRueda.getY(),
                      oPosicionRueda.getZ());

    std::cout << std::endl;

  }

   //Uniones y "bisagras"
  /******************************************/
  list<XODE_joint> lstJoint = oSpace.getOJoint();
  list<XODE_joint> lstJointTemp = lstJoint;
  
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

    XODE_axis oAxis2 = lstJointTemp.front().getOHinge2().getOAxis2();
    dJointSetHinge2Axis2 (joint[i],
                          oAxis2.getX(),
                          oAxis2.getY(),
                          oAxis2.getZ());

    lstJointTemp.pop_front();

    std::cout << std::endl;
  }

  //Suspenciones de la uniones
  lstJointTemp = lstJoint;
  for (i=0; i<3; i++) {

    XODE_axis oAxis1 = lstJointTemp.front().getOHinge2().getOAxis1();
    dJointSetHinge2Param (joint[i],
                          dParamSuspensionERP,
                          oAxis1.getSuspensionERP());


    XODE_axis oAxis2 = lstJointTemp.front().getOHinge2().getOAxis2();
    dJointSetHinge2Param (joint[i],
                        dParamSuspensionCFM,
                        oAxis1.getSuspensionCFM());

    lstJointTemp.pop_front();
    std::cout << std::endl;
  }

    //Ancla las ruedas traseras al "eje de giro"
  lstJointTemp = lstJoint;
  for (i=1; i<3; i++) {

    XODE_axis oAxis1 = lstJointTemp.front().getOHinge2().getOAxis1();
    dJointSetHinge2Param (joint[i],
                          dParamLoStop,
                          oAxis1.getLowStop());


    XODE_axis oAxis2 = lstJointTemp.front().getOHinge2().getOAxis2();
    dJointSetHinge2Param (joint[i],
                          dParamHiStop,
                          oAxis2.getHiStop());
    
  
    lstJointTemp.pop_front();
    std::cout << std::endl;
  }

  // Crea un espacio para el vehiculo.
  car_space = dSimpleSpaceCreate (space);
  dSpaceSetCleanup (car_space,0);
  dSpaceAdd (car_space,box);
  dSpaceAdd (car_space,sphere[0]);
  dSpaceAdd (car_space,sphere[1]);
  dSpaceAdd (car_space,sphere[2]);

}


/**
 *  @brief   Creacion del Ambiente
 *
 *  Funcion que crea un nuevo ambiente de simulacion. Para el caso de esta simulacion, solo existe una 
 *  "rampla" 
 *  
 *  @param space  ID del espacio que se esta simulando
 *  @param oSpace Objeto que representa el espacio de simulacion. Aqui estan contenidos todas las constantes fisicas para los objetos.
 */
void enviremont_new(dSpaceID space,XODE_space oSpace)
{
  XODE_simpleBody oGround = oSpace.getOBody().front().getoSimpleBody();
  XODE_position oPositionGround= oGround.getOTransform().getOPosition();
  XODE_box oBox =  oSpace.getOBody().front().getOGeom().front().getOBox();
  XODE_axisangle oAngulo = oGround.getOTransform().getORotation().getOAxisangle();
  
  ground_box = dCreateBox (space,oBox.getSizex(),oBox.getSizey(),oBox.getSizez());
  dMatrix3 R;
  dRFromAxisAndAngle (R,oAngulo.getX(),oAngulo.getY(),oAngulo.getZ(),oAngulo.getAngle()*M_PI);
  dGeomSetPosition (ground_box,oPositionGround.getX(),oPositionGround.getY(),oPositionGround.getZ());
  dGeomSetRotation (ground_box,R);
}



int main (int argc, char **argv)
{

  int i;
  dMass m;
  XODE_xode OXODE_enviremont;

  //carga de archivo con la definicion del objeto a simular.
  /******************************************/

  TiXmlDocument XMLdoc=OXODE.loadXODE(argv[1]);
  std::cout << std::endl << "XODE."<< std::endl;
  std::cout << "Name: " << OXODE.getName() << std::endl;
  std::cout << "Version: " << OXODE.getVersion() << std::endl;
  std::cout << std::endl;
  
  /******************************************/

  //carga de archivo con la definicion del Ambiente
  /******************************************/
  if(argv[2])
  {
    TiXmlDocument XMLenviremont=OXODE_enviremont.loadXODE(argv[2]);
    std::cout << std::endl << "XODE."<< std::endl;
    std::cout << "Name: " << OXODE_enviremont.getName() << std::endl;
    std::cout << "Version: " << OXODE_enviremont.getVersion() << std::endl;
    std::cout << std::endl;
  }
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

  

  XODE_plane oGround = oSpace.getOGeom().front().getOPlane();
  ground = dCreatePlane (space,oGround.getA(),oGround.getB(),oGround.getC(),oGround.getD());

  /******************************************/


  //Creando la geometria y el spacio.
  buggy_new(world,oSpace,space,m);
 
  //Creando el "ambiente"
  if(argv[2]) enviremont_new(space,OXODE_enviremont.getoWorld().getoSpace().front());
  else return 0;
  
  
  //Corriendo la simulacion.
  dsSimulationLoop (argc,argv,800,600,&fn);

  dGeomDestroy (box);
  dGeomDestroy (sphere[0]);
  dGeomDestroy (sphere[1]);
  dGeomDestroy (sphere[2]);
  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
  return 0;
}
