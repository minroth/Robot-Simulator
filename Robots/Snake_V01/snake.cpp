/**
 *  
 *  @file   snake.cpp
 *  @Author Miguel Rodriguez (miguel.andres.rodriguez.s@gmail.com)
 *  @date   Julio 2013
 *  @brief  Un ejemplo de un robot modular 
 *  
 *  GPL LICENSE 
 *  
 *  Simulacion robot modular, utilizando un 
 *  motor de fisica de tiepo real llamado ODE (Open Dynamics Engine) y una libreria
 *  llamada Drawstuff para la representacion grafica de la simulacion.
 *  
 *  El usuario puede cambiar el tipo de movimiento con algunas teclas.
 *  El robot modular puede realizar 8 tipos distintos de movimiento:     
 *  -Moverse hacia adelante                                                    
 *  -Girar                                                           
 *  -Movimiento lateral                                                      
 *  -Movimiento lateral inclinado.                                           
 *  -Aleteo                                                          
 *  -En forma de rotacion                                                                                               
 *  -Rodando     
 *  
 *  Las configuraciones estan el en archivo parameters.h
 *  @see parameters.h
 *
 *  Este codigo es una extension al codigo snake.cpp.  Dr. Juan Gonzalez-Gomez. December 2008.
 *  Más informacion acerca del proyecto esta disponible en   
 *  http://www.iearobotics.com/wiki/index.php?title=Juan_Gonzalez:Tesis 
 */ 
#include <unistd.h>
#include <math.h>
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "XODEParser.h"
#include <tinyxml.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include "parameters.h" 

using namespace std;

/**
 * @struct  MySnake
 * @brief   Estructura para la serpiente. La estructure estrá contruida en funcion del numero de modulos
 *          que se quieran contruir.
 * @ingroup Snake
 *
 */
struct MySnake {
  dJointID joint[MOD];       /**< Uniones del Robot. */ 
  
  dBodyID body[MOD-1];       /**< Cuerpos centrales. */ 
                             
  dGeomID geom1[MOD-1];      /**< Cada cuerpo central está compuesto por 2 cajas. Una a la izquierda y otra a la derecha */   
  dGeomID geom2[MOD-1];       
  
  dBodyID body_left;         /**< Este es el cuerpo a la izquierda de la serpiente. Su geometria es de caja. */ 
  dGeomID geom_left;         
  
  dBodyID body_right;        /**< Este es el cuerpo a la derecha de la serpiente. Su geometria es de caja */ 
  dGeomID geom_right;        
  
  dReal servo_ref_pos[MOD];  /**< Pocisiones de referencia para los servos. */ 
};


/**
 *  @brief   Creacion del robot
 *
 *  Funcion que crea el las geometrias y constantes fisicas del robot a simular. 
 *  
 *  @param *snake Estructura del robot.
 *  @param world  ID del mundo a simular.
 *  @param space  ID del espacio que se esta simulando.
 *
 */
void Robot_new(MySnake *snake, dWorldID world, dSpaceID space)
{
  dMass m;
  //-------------------------------------
  //-- XODE
  //-------------------------------------
  XODE_world oWorld = OXODE.getoWorld();
  XODE_space oSpace = oWorld.getoSpace().front();
  list<XODE_body> lstBody = oSpace.getOBody();
  
  //-------------------------------------
  //-- XODE_LEFT Body
  //-------------------------------------
  XODE_body oLeftBody = lstBody.front();
  XODE_simpleBody oSimpleLeftBody = oLeftBody.getoSimpleBody();
  XODE_position oPositionLeftBody= oSimpleLeftBody.getOTransform().getOPosition();
  XODE_mass_struct oLeftMass = oSimpleLeftBody.getOMass().getOMass_struct();
  XODE_centerGravity oLeftMassCG = oLeftMass.getOCenterGravity();
  XODE_box oLeftBox = oLeftBody.getOGeom().front().getOBox();
  
  
  //-------------------------------------
  //-- crea el body_left 
  //-------------------------------------
  snake->body_left = dBodyCreate(world);
  
  //-- Setea su posicion en el mundo de simulación.
  dBodySetPosition (
                    snake->body_left,
                    oPositionLeftBody.getX(),
                    oPositionLeftBody.getY(),
                    oPositionLeftBody.getZ()
                    );
  
  //-- Setea la masa
  dMassSetBoxTotal (
  					&m 
  					,oLeftMass.getTotal()
  					,oLeftMassCG.getX()
  					,oLeftMassCG.getY()
  					,oLeftMassCG.getZ()
  					);
  dBodySetMass (snake->body_left,&m);
  
  //-- Crea la geometria y la asocia al cuerpo.
  snake->geom_left = dCreateBox (space
  								,oLeftBox.getSizex()
  								,oLeftBox.getSizey()
  								,oLeftBox.getSizez()
  								); 
  dGeomSetBody (snake->geom_left,snake->body_left);
  
  
  
  //-------------------------------------
  //-- XODE CENTRAL Body
  //-------------------------------------
  lstBody.pop_front();
  XODE_body oCenterBody = lstBody.front();
  XODE_simpleBody oSimpleCenterBody = oCenterBody.getoSimpleBody();
  XODE_position oPositionCenterBody= oSimpleCenterBody.getOTransform().getOPosition();
  XODE_mass_struct oCenterMass = oSimpleCenterBody.getOMass().getOMass_struct();
  XODE_centerGravity oCenterMassCG = oCenterMass.getOCenterGravity();
  XODE_box oCenterBoxFront = lstBody.front().getOGeom().front().getOBox();
  list<XODE_geom> lstGEOM = oCenterBody.getOGeom();
  lstGEOM.pop_front();
  XODE_position oPositionCenterFrontBox = lstGEOM.front().getOTransform().getOPosition();
  XODE_box oCenterBoxBack = lstBody.front().getOGeom().back().getOBox();
  XODE_position oPositionCenterBackBox = oCenterBody.getOGeom().back().getOTransform().getOPosition();
  
  
  //-----------------------------------------------------------------
  //-- Crea el cuerpo central. El cuerpo central está compuesto por secciones de 2 cajas.
  //-----------------------------------------------------------------
  
  for (int i=0; i<MOD-1; i++) {
    //-- Crea el cuerpo y setea la posicion y la masa.
    snake->body[i]=dBodyCreate(world);

    dBodySetPosition(
    				snake->body[i], 
    				oPositionCenterBody.getX(), 
    				oPositionCenterBody.getY()*(i+1), 
    				oPositionCenterBody.getZ()
    				);
    				
    dMassSetBoxTotal (
    				&m, 
    				oCenterMass.getTotal(), 
    				oCenterMassCG.getX(),
    				oCenterMassCG.getY(),
    				oCenterMassCG.getZ()
    				);
    dBodySetMass (snake->body[i],&m);
	  
    //-- Crea la 2 cajas.
    snake->geom1[i] = dCreateBox (
    							space, 
								oCenterBoxFront.getSizex(),
								oCenterBoxFront.getSizey(), 
								oCenterBoxFront.getSizez()
								); 
				
    snake->geom2[i] = dCreateBox (
    							space, 	
								oCenterBoxBack.getSizex(),
								oCenterBoxBack.getSizey(), 
								oCenterBoxBack.getSizez()
								); 

    dGeomSetBody (snake->geom1[i],snake->body[i]);
    dGeomSetBody (snake->geom2[i],snake->body[i]);    
    
    //-- Setea su posicion relativa al cuerpo.
    //-- Una caja esta situada a la derecha y otra a la izquierda.

    dGeomSetOffsetPosition (
							snake->geom1[i], 
							oPositionCenterFrontBox.getX(),  
							oPositionCenterFrontBox.getY(), 
							oPositionCenterFrontBox.getZ()
							);

    dGeomSetOffsetPosition (
    						snake->geom2[i], 
    						oPositionCenterBackBox.getX(), 
    						oPositionCenterBackBox.getY(), 
    						oPositionCenterBackBox.getZ()
    						);    
  }
  
  
  
  //-------------------------------------
  //-- XODE RIGTH Body
  //-------------------------------------
  lstBody.pop_front();
  XODE_body oRigthBody = lstBody.front();
  XODE_simpleBody oSimpleRigthBody = oRigthBody.getoSimpleBody();
  XODE_position oPositionRigthBody= oSimpleRigthBody.getOTransform().getOPosition();
  XODE_mass_struct oRigthMass = oSimpleRigthBody.getOMass().getOMass_struct();
  XODE_centerGravity oRigthMassCG = oRigthMass.getOCenterGravity();
  XODE_box oRigthBox = oRigthBody.getOGeom().front().getOBox();
  
  
  //---------------------------
  //-- Crea el cuerpo a la derecha
  //---------------------------
  snake->body_right = dBodyCreate(world);
  
  //-- Setea su posicion en el munndo de simulacion.
  dBodySetPosition(
  					snake->body_right, 
  					oPositionRigthBody.getX(), 
  					-MOD*L+oPositionRigthBody.getY(), 
  					oPositionRigthBody.getZ()
  					);
  
  //-- Setea su masa.
  dMassSetBoxTotal (
  					&m, 
  					oRigthMass.getTotal(), 
  					oRigthMassCG.getX(),
  					oRigthMassCG.getY(),
  					oRigthMassCG.getZ()
  					);
  					
  dBodySetMass (snake->body_right,&m);

  //-- Crea la geometria y la asocia al cuerpo
  snake->geom_right = dCreateBox (
  							space, 
								oRigthBox.getSizex(),
								oRigthBox.getSizey(), 
								oRigthBox.getSizez()
								); 
  dGeomSetBody (snake->geom_right,snake->body_right);
  


  //-------------------------------------
  //-- XODE Central_Joints
  //-------------------------------------
  list<XODE_joint> lstJoint = oSpace.getOJoint();
  XODE_joint oCentralJoint = lstJoint.front();
  XODE_hinge oCentralHinge = oCentralJoint.getOHinge();
  XODE_axis oCentralAxis = oCentralHinge.getOAxis();
  XODE_anchor oCentralAnchor = oCentralHinge.getOAnchor();



  //--------------------------
  //-- Crea la uniones
  //--------------------------
  
  //-- Contruye las uniones centrales
  for (int i=1; i<=MOD-2; i++) {
    
    //-- Crea la union de tipo Hinge
    snake->joint[i]=dJointCreateHinge (world, 0);
    
    //-- La iesima union conecta los cuerpos i-1 e i
    dJointAttach (snake->joint[i],snake->body[i-1],snake->body[i]);
    
    //-- Para las uniones pares
    if (i%2==0)
      dJointSetHingeAxis (
                        snake->joint[i], 
                        oCentralAxis.getX(),
                        oCentralAxis.getY(),
                        oCentralAxis.getZ());  //-- Pitch
    else
      //-- Para las uniones impares
      dJointSetHingeAxis (
                        snake->joint[i], 
                        oCentralAxis.getZ(),
                        oCentralAxis.getY(),
                        oCentralAxis.getX());//-- Yaw
    
    //-- Setea la posicion de las uniones
    dJointSetHingeAnchor (
                        snake->joint[i], 
                        oCentralAnchor.getX(), 
                        -i*L-oCentralAnchor.getY(), 
                        oCentralAnchor.getZ());
    
    //-- Setea los parametros: Velocidad angular inicial y velocidad maxima de torque.s
    dJointSetHingeParam(
                      snake->joint[i], 
                      dParamFMax, 
                      oCentralAxis.getFMax());

    dJointSetHingeParam(snake->joint[i], 
                        dParamVel, 
                        oCentralAxis.getVel());
  }
  
  //-------------------------------------
  //-- XODE Left_Joints
  //-------------------------------------
  lstJoint.pop_front();
  XODE_joint oleftJoint = lstJoint.front();
  XODE_hinge oleftHinge = oleftJoint.getOHinge();
  XODE_axis oleftAxis = oleftHinge.getOAxis();
  XODE_anchor oleftAnchor = oleftHinge.getOAnchor();

  //-- Crea la union a la izquierda
  snake->joint[0]=dJointCreateHinge (world, 0);
  
  //-- Si el robot consiste solamente de un modulo, la union a la izquierda se
  //-- conecta a los cuerpos de la izquierda y derecha (cabeza y cola)7

  //-- De no ser asi, se conecta al cuerpo de la izquierda y al siguiente 
  if (MOD==1) 
    dJointAttach (snake->joint[0],snake->body_left,snake->body_right);
  else
    dJointAttach (snake->joint[0],snake->body_left,snake->body[0]);
  
  
  //-- Setea la posision y parametros.
  dJointSetHingeAxis (
                    snake->joint[0], 
                    oleftAxis.getX(),
                    oleftAxis.getY(),
                    oleftAxis.getZ()); 


  dJointSetHingeAnchor (
                    snake->joint[0], 
                    oleftAnchor.getX(), 
                    oleftAnchor.getY(), 
                    oleftAnchor.getZ());

  dJointSetHingeParam(
                    snake->joint[0], 
                    dParamFMax, 
                    oleftAxis.getFMax());

  dJointSetHingeParam(
                    snake->joint[0], 
                    dParamVel, 
                    oleftAxis.getVel());
  

  //-------------------------------------
  //-- XODE Right_Joints
  //-------------------------------------
  lstJoint.pop_front();
  XODE_joint oRightJoint = lstJoint.front();
  XODE_hinge oRightHinge = oRightJoint.getOHinge();
  XODE_axis oRightAxis = oRightHinge.getOAxis();
  XODE_anchor oRightAnchor = oRightHinge.getOAnchor();
  //-- Crea la union a la derecha. Solo debe crearla si exisete mas de un modulos
  if (MOD>1) {
    
    //-- Crea la union
    snake->joint[MOD-1]=dJointCreateHinge (world, 0);
    dJointAttach (snake->joint[MOD-1],snake->body[MOD-2],snake->body_right);
    
    
    //-- dependiendo del numero de modulos es el tipo de union.
    if (MOD%2==0)
      dJointSetHingeAxis (
                        snake->joint[MOD-1], 
                        oRightAxis.getX(),
                        oRightAxis.getY(),
                        oRightAxis.getZ());  
    else
      dJointSetHingeAxis (
                        snake->joint[MOD-1], 
                        oRightAxis.getZ(),
                        oRightAxis.getY(),
                        oRightAxis.getX());  
    
    //-- Setea la posicion y parametros.
    dJointSetHingeAnchor (
                          snake->joint[MOD-1], 
                          oRightAnchor.getX(), 
                          -MOD*L+oRightAnchor.getY(), 
                          oRightAnchor.getZ());

    dJointSetHingeParam(
                        snake->joint[MOD-1], 
                        dParamFMax, 
                        oRightAxis.getFMax());

    dJointSetHingeParam(
                        snake->joint[MOD-1], 
                        dParamVel, 
                        oRightAxis.getVel());
  }  
  
}

/**
 *  @brief   Dijuno de una geometria
 *  Funcion que dibuja el una de las secciones del robot modular (una de las cajas).
 *  @param g  ID de la geometria del robot.
 *
 */
static void drawGeom (dGeomID g)
{
  const dReal *pos;
  const dReal *R;
  dReal pos2[4];  

  //-- Lee la posicion y orientacion
  pos = dGeomGetPosition (g);
  R = dGeomGetRotation (g);

  //-- Obtiene el tipo de geometria.
  int type = dGeomGetClass (g);

  //-- Si es una caja (siempre debe ser una caja)
  if (type == dBoxClass) {
    dVector3 sides;
    dGeomBoxGetLengths (g,sides);

    //-- Antes de dibujar, el objeto es escalado.
    pos2[0]=pos[0]*VIEW_SCALE;
    pos2[1]=pos[1]*VIEW_SCALE;
    pos2[2]=pos[2]*VIEW_SCALE;
    
    sides[0]*=VIEW_SCALE;
    sides[1]*=VIEW_SCALE;
    sides[2]*=VIEW_SCALE;
    
    //-- Dibuja la caja
  #ifdef dDOUBLE
      dsDrawBoxD (pos2, R, sides);
  #else
      dsDrawBox (pos2, R, sides);
  #endif
  }
}


/**
 *  @brief   Dibuja todo el robot en pantalla
 *
 *  Funcion que dibuja el robot en pantalla.
 *  
 *  @param *snake Estructura del robot.
 *  
 */
void Robot_render(MySnake *snake)
{
  int G=0;
  
  //-- Setea la textura para el robot.
  dsSetTexture (DS_WOOD);
  
  //-- Dibuja el cuerpo a la izquierda.
  dsSetColor (1,1,0);
  drawGeom (snake->geom_left);
  
  //-- Dibuja todos los cuerpos centrales.
  for (int i=0; i<MOD-1; i++) {
    
    //-- Dependiendo del modulo, se dibuja en un color distinto.
    if (i%2==0) G=1;
    else G=0;
    
    //-- Dibuja la primera caja.
    dsSetColor (1,G,0);
    drawGeom(snake->geom1[i]);
    
    //-- Dibuja la segunda caja.
    dsSetColor (1,!G,0);
    drawGeom(snake->geom2[i]);
  }
  
  //-- Dibuja el cuerpo a la derecha.
  dsSetColor (1,!G,0);
  drawGeom (snake->geom_right);
}


/**
 *  Funciones que describen los tipos de movimiento del robot.             
 *  Los parametros para las funciones son colocadas de acuerdo al tipos    
 *  de movimiento a ser realizado. Existe un generador sinosoidal para     
 *  cada union. Los "generadores de movimiento" son subdivididos en 2      
 *  categorias: horizontal y vertical.                                     
 *                                                                         
 *   CONTROLES                                                             
 *   -Parametros para los generadores verticales:                          
 *      -Av: Amplitud. (0,90) grados                                       
 *      -PDv: Diferencia de fase entre 2 generadores verticañes adjacentes 
 *            (0,180) grados                                               
 *   -Parametros para los generadores horizontales:                        
 *      -Av: Amplitud. (0,90) grados                                       
 *      -PDv: Diferencia de fase entre 2 generadores verticañes adjacentes 
 *            (0,180) grados                                               
 *      -Oh: Offset                                                        
 *   -Otros parametros:                                                    
 *      -F0: Fase Inicial                                                  
 *      -PDvh:  Diferencia de fase entre los generadores                   
 *              horizontales y verticales                                  
 *                                                                         
 */


/**
 *  @brief   Movimiento "Recto".
 *  @ingroup Snake
 *
 *  Funcion que realiza el movimiento en linea recta.
 *  
 *  @param *alfa  angulo (0-120 grados).
 *  @param *k     Numero de ondulaciones verticales.
 *  
 */
void straight(dReal alfa,dReal k)
{
  //-- Parametros seteados inicialmente en 0
  Ah=0; Oh=0; F0=0;
  
  //-- Se crean las ondas verticales.
  Av=2*alfa*sin(2*M_PI*k/MOD);
  PDv=direction*(4*M_PI*k/MOD)*180/M_PI;
}


/**
 *  @brief  Movimiento de "Giro"  
 *  @ingroup Snake
 *
 *  El robot se mueve en un arco circular.
 *  
 *  @param alfah  angulo del arco (0-120 grados).
 *  @param kv     Numero de ondulaciones verticales.
 *  
 */
void turning(dReal alfah, dReal kv)
{
 
  //-- Sin ondas horizontales, pero las uniones impares son seteadas en un angulo fijo.
  Ah=0; F0=0;
  Oh=2*alfah/MOD;
  
  //-- Se crea la onda vertical.
  Av=2*20*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
}

/**
 *  @brief  Moviemito "Sidewinding" 
 *  @ingroup Snake 
 *
 *  Es un movimiento tipico de la serpientes. La serpiente se mueve paralela 
 *  al eje de su cuerpo, a la derecha o a la iquierda.
 *
 *  @param alfah  angulo (0-120 grados).
 *  @param kv     Numero de ondulaciones verticales.
 *  
 */
void side_winding(dReal alfah, dReal kh)
{
  dReal kv=kh;
  
  //-- Baja amplitud para las ondas verticales.
  Av=2*5*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- Se crea las ondas verticales. Ambas ondas, verticales y horizontales tienen la misma amplitud.
  Oh=0;
  Ah=2*alfah*sin(2*M_PI*kh/MOD);
  PDh=PDv;
  PDvh=0;
}

/**
 *  @brief  Moviemito "Sidewinding" inclinado 
 *  @ingroup Snake
 *  
 *  @param alfa  angulo (0-120 grados).
 *  @param k     Numero de ondulaciones.
 *  
 */
void inclined_side_winding(dReal alfa, dReal k)
{
  dReal alfav = alfa*sin(45);
  dReal alfah = alfav;
  dReal kv=k;
  dReal kh=k;
  
  
  //-- Onda vertical
  Av=2*alfav*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- Onda horizontal
  Oh=0;
  Ah=2*alfah*sin(2*M_PI*kh/MOD);
  PDh=PDv;
  PDvh=0; 
}

/**
 *  @brief  Moviemito de "Aleteo" 
 *  @ingroup Snake
 *
 */
void flapping()
{
  dReal alfa=12;
  
  //-- Todas las uniones verticales son puestas en fase.
  //-- Todas las ondas horizontales son puestas en fase.
  //-- La diferencia de fase entre las ondas verticales y horizontales es de 90 grados.
  //-- La amplitud es muy pequeña.
  Av=2*alfa/MOD;
  Ah=Av;
  PDv=0;
  PDh=0;
  PDvh=direction*90;
  Oh=0;
}


/**
 *  @brief  Moviemito "Rotar_S" 
 *  @ingroup Snake
 *
 *  La serpiente o el robot realiza una rotacion paralelaal piso 
 *  persiguiendo la orientacion de su cuerpo. Basicamente, 
 *  una rotacion en forma de S
 *
 *  @param alfah  angulo (0-120 grados).
 *  @param kv     Numero de ondulaciones verticales.
 *  
 */
void rotating_S(dReal alfah, dReal kh)
{
  dReal kv=2*kh;
  
  //-- La onda vertical es de amplitud baja y el numero de ondulaciones es el 
  // --doble que para las ondas horizontales
  Av=2*5*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- La onda horontal tiene una amplitud mayor.
  Oh=0;
  Ah=2*alfah*sin(2*M_PI*kh/MOD);
  PDh=direction*(4*M_PI*kh/MOD)*180/M_PI;
  PDvh=0;
}

/**
 *  @brief  Moviemito "Rotar_U" 
 *  @ingroup Snake
 *
 *  La serpiente o robot realiza una rotacion paralela al suelo, 
 *  cambiando la orientacion de su cuerpo. El cuerpo de la 
 *  serpiente tiene forma de U
 *
 *  @param alfa  Angulo del arco horizontal (0-360 grados)
 *  
 */
void rotating_U(dReal alfa)
{
  dReal kv=2;
  
  //-- La onda vertical tien una amplitud de onda baja 
  Av=2*5*sin(2*M_PI*kv/MOD);
  PDv=direction*(4*M_PI*kv/MOD)*180/M_PI;
  
  //-- Las uniones horizontales estan en fase.
  Oh=0;
  Ah=2*alfa/MOD;
  PDh=0;
  PDvh=direction*90;
}

/**
 *  @brief  Moviemito "Rodar" 
 *  @ingroup Snake
 *
 *  La serpiente o robot rueda alrrededor del elje de su cuerpo. 
 *  La forma de su cuerpo es de U
 *
 *  @param alfa  Angulo del arco (0-360 grados)
 *  
 */
void rolling(dReal alfa)
{
  //-- Todas las uniones verticales son puestas en fase.
  //-- Todas las ondas horizontales son puestas en fase.
  //-- La diferencia de fase entre las ondas verticales y horizontales es de 90 grados.
  //-- La amplitud es grande, de otra forma seria un aleteo.
  
  Av=2*alfa/MOD;
  Ah=Av;
  PDv=0;
  PDh=0;
  PDvh=direction*90;
  Oh=0;
  F0=90;
}


/**
 *  @brief  Funciones "CALLBACK" 
 *  
 *  Las funciones Callback son invocadas por dSpaceCollide 
 *  cuando dos objetos estan a punto de colisionar.
 *  Esta es la funcion donde se determinan las colisiones en la simulación.
 *
 *  @param data Referencia al espacio donde ocurre la simulación
 *  @param o1   Geometria 1 a evaluar      
 *  @param o2   Geometria 2 a evaluar
 *  
 */

static void nearCallback (void *data, dGeomID o1, dGeomID o2)   
{
  int i;
   
  //-- Obtiene los Id para los cnuerpos
  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  
  //-- Si estan conectados por una union no se realiza una deteccion de colision
  if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) {
    return;
  }
 
  //-- Configura las propiedades para los puntkos de contacto.
  dContact contact[MAX_CONTACTS]; 
  for (i=0; i<MAX_CONTACTS; i++) {
    contact[i].surface.mode = dContactBounce | dContactSoftCFM;
    contact[i].surface.mu = MU;
    contact[i].surface.mu2 = MU2;
    contact[i].surface.bounce = BOUNCE;
    contact[i].surface.bounce_vel = BOUNCE_VEL;
    contact[i].surface.soft_cfm = SOFT_CFM;
    
  }
  
  //-- Obtiene los puntkos de contacto.
  int numc = dCollide (o1,o2,MAX_CONTACTS,&contact[0].geom, sizeof(dContact));

  
  //-- Si exiate por lo menos un punto de contacto.
  if (numc!=0) {
    
    //-- Por cada punto de contacto una union debe ser creada.
    for (i=0; i<numc; i++) {

      //-- Crea la union la agreg al jointGroup.
      dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
      
      //-- Setea la articulacion 
      dJointAttach (c,b1,b2);
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
  //-- Posicion de la camara. 
  static float xyz[3] = {-8.540,-5.74,3.34};

  //-- Orientacion de la camara. (Pan, Tilt and Roll).
  static float hpr[3] = {0.0f,-15.0f,0.0f};

  //-- Setea la posicion y orientación de la camara.
  dsSetViewpoint (xyz,hpr);

  //-- Menu que se muentra en la consola.
  printf ("Presiona una tecla para las siguientes opiones: \n");
  printf ("1: Straight\n");
  printf ("2: Turning\n");
  printf ("3: Side-winding\n");
  printf ("4: Inclined side-winding\n");
  printf ("5: Flapping\n");
  printf ("6: S-shaped rotating\n");
  printf ("7: U-sahped rotating\n");
  printf ("8: Rolling\n");
  printf ("q: Salir\n");

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

  //-- Cada vez que un tipo de movimiento es seleccionado, la direccion cambia.
  
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
    //-- Terminar la simulacion y salir.
    dsStop();
  }
}

/**
 *  @brief   Simulación de los osciladores sinusoidales. 
 *  @ingroup Snake
 *
 *  Esta función se llama cada vez que todos los servos han llegado a 
 *  sus posiciones de referencia. Las siguientes posiciones de los servos 
 *  se calculan por medio de un muestreo a las funciones sinusoidales.
 *  La variable n es el tiempo discreto.
 *  La función para el cálculo de la posición del servo depende del 
 *  tipo de módulo vertical u horizontal.
 *  
 *  Las ecuaciones son:
 *   -Servos verticales:
 *         pos(i) = Av*sin(2*PI*n/N + PDv*(i/2) + F0);                       
 *   -Servos horizontales:                                                     
 *         pos(i) = Ah*sin(2*PI*n/N + PDh(i-1)/2 + PDvh + F0) + Oh           
 *  
 *  Donde N esel total de muestras. Por defecto es 32.
 *
 */
void sequence_generation()
{
  dReal phase;
  
  //-- La nueva posicion es calculada para todos los servos.
  //-- El numero de modulos es MOD (Definido en parameters.h)
  for (int i=0; i<MOD; i++) {
    
    if (i%2==0) {
      phase = PDv*(i/2) + F0;
      snake.servo_ref_pos[i]=Av*sin(2*M_PI*n/N + DEG2RAD(phase) );
    }
    else {
      phase = PDh*(i-1)/2 + PDvh + F0;
      snake.servo_ref_pos[i]=Ah*sin(2*M_PI*n/N + DEG2RAD(phase))+Oh;
    }    
  }

  n = (n + 1) % N;  
}

/**
 *  @brief   Simulación de los servos
 *  @ingroup Snake
 *
 *  Esta funcion realiza un paso de simulacion en los servos.
 *  Se utiliza un controlador proporcional, con una ganancia KP.
 *  El ángulo del servo actual se lee y se compara con la posición de referencia.
 *  Esta diferencia es el error. A continuación, se ajusta la velocidad angular del servo. 
 *  Esta velocidad es proporcional al error, pero tiene un valor máximo (WMAX). 
 *
 *  Si todos los servos han llegado a sus posiciones de referencia, 
 *  se llama a sequence_generation() para el cálculo de los siguientes ángulos de referencia.
 * 
 *  Para la obtención de una simulación más "suave" a los servos no se les permite alcanzar 
 *  su posición de referencia. Cuando el ángulo de servo está cerca de la posición de referencia 
 *  (y por lo tanto la velocidad no es 0), se calcula la nueva posición. De esta manera, 
 *  los servos no se detienen y el movimiento es más suave.
 *  
 */
void servos_sim()
{
  int stable=0;
  
  //-- Para cada servo en el robot...
  for (int i=0; i<MOD; i++) {
     
    //-- Obtiene el angulo actual del servo.
    dReal pos = dJointGetHingeAngle(snake.joint[i]);
  
    
    //-- Calcula el error como la diferencia entre la posicion actual 
    //-- y la de referencia. Es la distancia que los servos deberian 
    //-- moverse para alcanzar la posicion de referencia
    dReal error = pos - DEG2RAD(snake.servo_ref_pos[i]);
    
    //-- Controlador proporcional. La velocidad angular del servo es calculada
    //-- multiplicando el error y la contante KP. Si la distancia es grande,
    //-- el servo se mueve rapido. Si la velocidad es pequeña, el servo se mueve 
    //-- lentamente. Cuando llega a la posicion de referencia, la velocidad es 0.
    dReal velocity = -error*KP;

    //-- Limitacion de velocidad. No se puede mover mas rapido que la velocidad 
    //-- angular maxima del servo (Constante WMAX).
    if (velocity > WMAX) velocity = WMAX;
    if (velocity < -WMAX) velocity = -WMAX;
    
    //-- Setea la velocidad angular del servo.
    dJointSetHingeParam(snake.joint[i], dParamVel, velocity);
    
    //-- Se revisa si el servo esta cerca de la posicion de referencnia. 
    //-- El servo esta "cerca" de la posicion de referencia cuando su 
    //-- distancia es mas pequeña que la constante de error.
    if (fabs(error)<ERROR) stable++;
  }  
 
  //-- Cuando todos los servos esten "cerca", la proxima posicion es calculada
  if (stable==MOD) {
    sequence_generation();
  }
}

/**
 *  @brief   Loop de simulacion.
 *
 *  Para cada paso de simulacion se realiza lo siguiente:
 *   -Se revisan las colisiones entre los objetos.
 *   -Lleve a cabo el paso de simulación: todas las posiciones del cuerpo, velocidades y aceleraciones se calculan para este instante.
 *   -simulacion del servo: Todo el servo debe ser simulado.
 *   -Se eliminan los puntos de contacto.
 *   -Se dibuja el robot en pantalla.
 *
 *  @param pause
 */
static void simLoop (int pause)
{
  if (!pause) {
    dSpaceCollide (space,0,&nearCallback);

    dWorldStep(world,STEP);

    //-- Paso de simulacion en los servos. Todos son actualizados.
    servos_sim();

    //-- Quita los puntos de contacto.
    dJointGroupEmpty (contactgroup);

    //-- Realiza una pausa. El parametro PAUSE está dedinido en parameters.h
    usleep(PAUSE);
  }

  //-- Dibuja el robot en pantalla.
  Robot_render(&snake);
  
}

/*******************/
/*     MAIN        */
/*******************/
int main (int argc, char **argv)
{

  
  /*-------------------------------------------------------------*/
  /* Carga del archivo XML con la definicion robot				 */
  /*-------------------------------------------------------------*/
  

  TiXmlDocument XMLdoc=OXODE.loadXODE(argv[1]);
  std::cout << std::endl << "XODE."<< std::endl;
  std::cout << "Name: " << OXODE.getName() << std::endl;
  std::cout << "Version: " << OXODE.getVersion() << std::endl;
  std::cout << std::endl;
  
  /*-------------------------------------------------------------*/
  /* Setea los parametros para drawstuff y las 
  /* funciones de deteccion de colisiones
  /*-------------------------------------------------------------*/
  
  dsFunctions fn;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = &command;
  fn.version = DS_VERSION;
  fn.stop = 0;
  fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;

  /*------------------------------------------------------------------*/
  /* Crea el mundo de simulacion. Este contiene todos los objetos a ser
  /* simulados. Se utiliza un archivo .XODE para las definiciones 
  /*------------------------------------------------------------------*/
  XODE_world oWorld = OXODE.getoWorld();
  XODE_space oSpace = oWorld.getoSpace().front();
  XODE_gravity oGravity= oWorld.getoExt().getGravity();
  float XODE_CFM = oWorld.getoExt().getCFM();
  XODE_plane oGround = oSpace.getOGeom().front().getOPlane();
 
  
  //-- Crea el mundo virtual.
  dInitODE2(0);
  world = dWorldCreate();
  dWorldSetGravity (world,
                  oGravity.getX(),
                  oGravity.getY(),
                  oGravity.getZ());
                  
  //-- Setea el parametro los parametros para la simulacion.
  dWorldSetCFM (world,XODE_CFM);
  dWorldSetAutoDisableFlag (world,1);
  dWorldSetContactMaxCorrectingVel (world,MAX_CORRECTING_VEL);
  dWorldSetContactSurfaceLayer (world,SURFACE_LAYER);

  //-- Crea el espacio de colision. Este espacio contjiene todas las 
  //-- geometrias que se analizaraán para determinar si existen colisiones.
  space = dHashSpaceCreate (0);

  //-- Crea el grupo de contacto, para almacenar los puntos de contacto de todas las colisiones.
  contactgroup = dJointGroupCreate (0);
  
  //Crea el suelo.
  dCreatePlane (space,oGround.getA(),oGround.getB(),oGround.getC(),oGround.getD());
  
  //-- Contruye el robot
  Robot_new(&snake,world,space);
  
  //-- Coloca el movimiento recto por defecto.
  straight(20,2);

  /********************************/
  /** COMIENZO DE LA SIMULACION   */
  /********************************/
  //-- Este es el loop que contiene la simulación.
  dsSimulationLoop (argc,argv,800,600,&fn);

  /**************************/
  /* FINALIZA LA SIMULACION */
  /**************************/

  //-- Destruye los grupos de contactos.
  dJointGroupDestroy (contactgroup);
  
  //-- Destruye el espacio de colision.
  dSpaceDestroy (space);
  
  //-- Destruye el mundo.
  dWorldDestroy (world);

  return 0;
}
