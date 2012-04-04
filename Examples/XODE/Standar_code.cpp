/**
	Tomando en cuenta el proceso regular descrito en el documento oficial de ODE, esto es un prototipo de 
	aplicacion "standar"
	Como regla principal para este prototipo:
		Los archivos *.xode deben estar definidos de forma correcta segun  el estandar 1.0r23

*/


//Bibliotecas que se usaran en el Software.
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <tinyxml.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;


//Genericamente vamos a definir un mundo y un espacio.
static dWorldID world;
static dSpaceID space;
static dBodyID sphere;
static dBodyID box;
static dBodyID cylinder;
static dBodyID capsule;
static dBodyID plane;    
static dGeomID geomBox;
static dGeomID geomSphere;
static dGeomID geomCylinder;  
static dGeomID geomCapsule;  
static dJointGroupID contactgroup;
static dReal Boxsides[3];
static dReal lengthCylinder;
static dReal radiusCylinder;
static dReal lengthCapsule;
static dReal radiusCapsule;



/*********************************************************************************************/
/*								Funciones para el manejo del XML
/*
/*********************************************************************************************/


/**
  @brief Obtiene el nombre de un objeto dado un archivo de configuracion en formato xode

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.

  @returns 	retorna el nombre que sera tomado como referencia  al tipo de objeto a dibujar.


  Ejemplo:
  El archivo xode para una esfera se veria de la forma:
  \verbatim
      <body name="sphere">
      	...
      </body>	
   \endverbatim
   La funcion solo extraera el nombre del cuerpo.

  */
const char *GetObject(TiXmlElement *body)
{
    return body->Attribute("name"); 
} 


/*********************************************************************************************/
/**
  @brief Retorna la masa de un objeto descrito en XODE. La masa esta medida en Kg

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.

  @returns 	retorna la masa de un objeto.


  Ejemplo:
  El archivo xode para una esfera se veria de la forma:
  \verbatim
      <body name="sphere">
      	<mass>
      		<adjust total="1" />
      	</mass>
      </body>	
   \endverbatim
   La funcion solo extraera el atributo total de la etiqueta adjust.

  */

 dReal GetMass(TiXmlElement *body)
 {
 	
	TiXmlElement *mass; 					//Elementos Para el manejo de XML
	
	mass = body->FirstChildElement("mass");
	if(mass->FirstChildElement("adjust")){		
		return atof(mass->FirstChildElement("adjust")->Attribute("total"));
	}else return 1;									//en caso de no declararse una masa se asume masa de 1 

 }
/*********************************************************************************************/

/**
  @brief Retorna el radio de una esfera o un cilindro, medido en metros. basado en un archivo XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.

  @returns 	retorna el radio de una esfera o un cilindro.


  Ejemplo:
  El archivo xode para una esfera se veria de la forma:
  \verbatim
      <body name="sphere">
      	<mass>
      		<adjust total="1" />
      	</mass>
	      	<geom name="sphereGeom">
						<sphere
							radius="2"
						/>
					
			</geom>
      </body>	
   \endverbatim
   La funcion solo extraera el atributo radius.

  */
  dReal GetRadius(TiXmlElement *body){

	TiXmlElement *geom; 					//Elementos Para el manejo de XML
	
	geom = body->FirstChildElement("geom");
	if(geom->FirstChildElement("sphere"))
  {		
		return atof(geom->FirstChildElement("sphere")->Attribute("radius")); //me aseguro de que tipo de objeto es.
	}else{
		std::cout << "Error: could find radius: " << std::endl;
		return 0;									//en caso de no declararse un radio lo llevamos a 0 	
	} 

 }

/*********************************************************************************************/

/**
  @brief Retorna la pocision del objeto en el instante inicial basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.
  @x referencia a la posicion del objeto del eje x
  @y referencia a la posicion del objeto del eje y
  @z referencia a la posicion del objeto del eje z

  @returns 	retorna la direccion a un arreglo donde se define la pocision


  Ejemplo:
  El archivo xode para una esfera se veria de la forma:
  \verbatim
      <body name="sphere">
      	<transform>
            <position x="0" y="10" z="5"/>
        </transform> 
        ...
      </body>	
   \endverbatim
   La funcion solo extraera los atributos x,y,z

  */
 
void GetPosition(TiXmlElement *body, dReal &x, dReal &y, dReal &z){
	TiXmlElement *transform; 					//Elementos Para el manejo de XML
	
	transform = body ->FirstChildElement("transform");
	if(transform->FirstChildElement("position"))
  {		
		x= atof(transform->FirstChildElement("position")->Attribute("x"));
		y= atof(transform->FirstChildElement("position")->Attribute("y"));
		z= atof(transform->FirstChildElement("position")->Attribute("z"));
	}else{
		std::cout << "Error: could find Position: "<< std::endl;
		
	} 
 }	


 /*********************************************************************************************/

/**
  @brief Retorna la densidad del objeto XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.

  @returns  retorna la densidad del un objeto.

*/

dReal GetDensity(TiXmlElement *body){
  TiXmlElement *DENSITY;           //Elementos Para el manejo de XML

  if(body->FirstChildElement("mass_shape"))
  {    
    return atof(body->FirstChildElement("mass_shape")->Attribute("density"));
  }else return 1;                 //en caso de no declararse una densidad se asume 1 
}

/*********************************************************************************************/

/**
  @brief Retorna las proporciones de un cubo instante inicial basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.
  @x referencia a la proporcion del objeto del eje x
  @y referencia a la proporcion del objeto del eje y
  @z referencia a la proporcion del objeto del eje z

  @returns  retorna la direccion a un arreglo donde se define el cubo


  */

void GetBoxGeom(TiXmlElement *body,dReal &x,dReal &y,dReal &z){ 
  TiXmlElement *sidesBox;          //Elementos Para el manejo de XML

  sidesBox = body ->FirstChildElement("geom");
  if(sidesBox->FirstChildElement("box"))
  {
       
    x=atof(sidesBox->FirstChildElement("box")->Attribute("sizex"));
    y=atof(sidesBox->FirstChildElement("box")->Attribute("sizey"));
    z=atof(sidesBox->FirstChildElement("box")->Attribute("sizez"));

  }else{
    std::cout << "Error: could find BoxGeom: "<< std::endl;
 
  } 

 }


 /*********************************************************************************************/

/**
  @brief Retorna la orientacion de un cilindro instante basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param TiXMLElement nodo *.xode donde esta definido el objeto a dibujar.

  @returns  retorna la orintavion de un cilindro. 3 es el estado normal del objeto


  Es utilizado en cilindros y capsulas

  */
dReal GetDirection(TiXmlElement *body){
  TiXmlElement *transform;          //Elementos Para el manejo de XML

  
  transform = body ->FirstChildElement("geom");
  if(transform->FirstChildElement("direction"))
  {
    return atof(transform->FirstChildElement("direction")->Attribute("value"));
  }else return 3;
}

/*********************************************************************************************/

/**
  @brief Retorna el radio de un cilindro basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param filename El al archivo *.xode donde esta definido el objeto a dibujar.

  @returns  retorna el radio del cilindro


  */
dReal GetRadiusCylinder(TiXmlElement *body){
  TiXmlElement *cylinder;          //Elementos Para el manejo de XML

  cylinder = body -> FirstChildElement("geom")->FirstChildElement("cylinder");
  if(cylinder)
  {
    return atof(cylinder->Attribute("radius"));
  } else 
  {
    return 0;
  } 
}

/*********************************************************************************************/

/**
  @brief Retorna el larg de un cilindro basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param filename El al archivo *.xode donde esta definido el objeto a dibujar.

  @returns  retorna largo de un cilindro


  */
dReal GetLenghtCylinder(TiXmlElement *body){
  TiXmlElement *cylinder;          //Elementos Para el manejo de XML

  cylinder = body -> FirstChildElement("geom")->FirstChildElement("cylinder");
  if(cylinder)
  {
    return atof(cylinder->Attribute("length"));
  }else
  { 
    return 0;
  }
}

/*********************************************************************************************/

/**
  @brief Retorna el radio de un cilindro basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param filename El al archivo *.xode donde esta definido el objeto a dibujar.

  @returns  retorna el radio del cilindro


  */
dReal GetRadiusCapsule(TiXmlElement *body){
  TiXmlElement *capsule;          //Elementos Para el manejo de XML

  capsule = body -> FirstChildElement("geom")->FirstChildElement("capsule");
  if(capsule)
  {
    return atof(capsule->Attribute("radius"));
  } else 
  {
    return 0;
  } 
}

/*********************************************************************************************/

/**
  @brief Retorna el larg de un cilindro basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param filename El al archivo *.xode donde esta definido el objeto a dibujar.

  @returns  retorna largo de un cilindro


  */
dReal GetLenghtCapsule(TiXmlElement *body){
  TiXmlElement *capsule;          //Elementos Para el manejo de XML

  capsule = body -> FirstChildElement("geom")->FirstChildElement("capsule");
  if(capsule)
  {
    return atof(capsule->Attribute("length"));
  }else
  { 
    return 0;
  }
}

/*********************************************************************************************/



/*********************************************************************************************/
/*								Funciones genericas.
									.........
/*
/*********************************************************************************************/

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
    if (int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) 
    {
        dJointID c = dJointCreateContact (world,contactgroup,&contact);
        dJointAttach (c,b1,b2);
    }
}

void start()
{
  // Configuracion de la "camara"
  static float xyz[3] = {0.0,-2.0,1.0};  // Puntos de vista medidos en metros
  static float hpr[3] = {90.0,0.0,0.0};  // Vista observador
  dsSetViewpoint (xyz,hpr);// Setea la camara y al angulo al que apunta
}


static void simLoop (int pause)
{
    const dReal *posSphere,*RSphere,*posBox,*RBox,*posCylinder,*RCylinder,*posCapsule,*RCapsule;
    if (sphere)
    {
      //@a sphere
        dsSetColor(0,1,0);      // Color
        dSpaceCollide (space,0,&nearCallback); // Encuentra puntos de colision y añade Joints
        dWorldStep(world,0.01);// Tamaña de "paso" de la simulacion
        dJointGroupEmpty (contactgroup); // Remueve todos los puntos de contacto
        dsSetSphereQuality(3);  // Calidad de la esfera
        posSphere = dBodyGetPosition(sphere); // Obtengo la pocicion del cuerpo
        RSphere   = dBodyGetRotation(sphere); // Obtengo la matrs de rotacion        
        dsDrawSphere (posSphere,RSphere,dGeomSphereGetRadius (geomSphere));
        printf ("Pocision: %f\n",posSphere[2]);
  
    }
    if (box)
    {
        dsSetColorAlpha (0,0,1,1);
        dSpaceCollide (space,0,&nearCallback);
        dWorldStep(world,0.01);
        dJointGroupEmpty (contactgroup);
        posBox = dBodyGetPosition(box);
        RBox   = dBodyGetRotation(box);

        dsDrawBox(posBox,RBox,Boxsides);             // draw a box
        cout<<"(x,y,z)=("<<posBox[0]<<","<<posBox[1]<<","<<posBox[2]<<")"<<endl;;

    }
    
    if (cylinder)
    {
      dsSetColorAlpha (0,1,0,1);
      dSpaceCollide (space,0,&nearCallback);
      dWorldStep(world,0.01);
      dJointGroupEmpty (contactgroup);
      dsSetSphereQuality(3); 
      posCylinder = dBodyGetPosition(cylinder);
      RCylinder   = dBodyGetRotation(cylinder);
      
      dsDrawCylinder(posCylinder,RCylinder,lengthCylinder,radiusCylinder);  // draw a cylinder    


    }

    if (capsule)
    {
      dsSetColorAlpha (1,1,1,1);
      dSpaceCollide (space,0,&nearCallback);
      dWorldStep(world,0.01);
      dJointGroupEmpty (contactgroup);
      
      posCapsule = dBodyGetPosition(capsule);
      RCapsule   = dBodyGetRotation(capsule);
      
      dsDrawCapsule(posCapsule,RCapsule,lengthCapsule,radiusCapsule);  // draw a cylinder    


    }
    

      
      
      
}






/*********************************************************************************************/
/*								Funcion Principal
/*
/*********************************************************************************************/


int main(int argc, char *argv[]){

	
	   const char *body;
	   // Esto es para seteat as variables de Drawstuff
   
    dsFunctions fn;
    fn.version = DS_VERSION;
    fn.start   = &start;
    fn.step    = &simLoop;
    fn.command = NULL;
    fn.stop    = NULL;
    fn.path_to_textures = "../textures";
    //Reviso si es que se cargo el parametro, de no ser asi termino el programa
    if (argc != 2)
    {
      cout<<"No se incluyo un archivo del tipo Archivo.xode con la geometria del objeto" << endl;
      return 0;
    }

    //Esto se encarga de cargar los parametros del archivo XODE

    TiXmlDocument XMLdoc(argv[1]);
    TiXmlElement *worldXML, *bodyXML;           //Elementos Para el manejo de XML
    
    
    
    if(!XMLdoc.LoadFile())    //Manejo de error en la carga del archivo
    {             
      std::cout << "Error GetMass: could not load file: " << argv[1] << std::endl;
      return 0;
    }
    worldXML = XMLdoc.FirstChildElement( "world" );  //se coloca en el primer tag (world)
    bodyXML=worldXML->FirstChildElement("space")->FirstChildElement("body"); //Con esto situamos el cuerpo con el que se trabajara.
    //Valida si se cargaron los archivos por parametros.
    

      
  

    //Primero inicializamos ODE dInitODE();
    dInitODE();
    //En la variable world se guarda el id del mundo que usaremos
    //Para esto se crea un mundo con fWorldCreate()              
    world = dWorldCreate();
    space = dSimpleSpaceCreate (0);
    body= GetObject(bodyXML);

/*********************************************************************************************/
/*                Esfera
/*
/*********************************************************************************************/
    if(!strcmp(body,"sphere"))
    {
	    
	    //Una esfera
	    //Definicion de constantes para el objeto
	    
	    dMass mSphere;
	    dReal massSphere = GetMass(bodyXML);
	    dReal radiusSphere = GetRadius(bodyXML);
      dReal x,y,z;
	    GetPosition(bodyXML,x,y,z);
	

	    //Creacion y locacion de la esfera
	    sphere = dBodyCreate (world);     // creacion del objeto
	    geomSphere = dCreateSphere (space,radiusSphere);   //creacion de la geometria
	    dMassSetZero(&mSphere); // Inicializacion de los parametros
	    dMassSetSphere (&mSphere,massSphere,radiusSphere);    // calcular los parametros de masa poara el objeto
	    dBodySetMass (sphere,&mSphere);         // setear los parametros de masa para el objeto
	    dGeomSetBody(geomSphere,sphere);
	    dBodySetPosition (sphere,x,y,z); // setear la posocion del objeto
	   }

/*********************************************************************************************/
/*                Caja
/*
/*********************************************************************************************/
    if(!strcmp(body,"box"))
    {  
      
      //una "caja"
      //Definicion de constantes para el objeto
      dMass mBox;
      dReal massBox;
      dReal x,y,z;

      GetBoxGeom(bodyXML,Boxsides[0],Boxsides[1],Boxsides[2]);
      GetPosition(bodyXML,x,y,z);
      massBox= GetMass(bodyXML);

      //Creacion y locacion de la esfera
      box = dBodyCreate (world);       // create a rigid body
      geomBox =  dCreateBox (space,Boxsides[0],Boxsides[1],Boxsides[2]);   //create geometry.
      dMassSetBox (&mBox,massBox,Boxsides[0],Boxsides[1],Boxsides[2]);
      dBodySetMass (box,&mBox);
      dGeomSetBody(geomBox,box);
      dBodySetPosition (box,x,y,z);
      
    }

/*********************************************************************************************/
/*            Cilindro
/*
/*********************************************************************************************/
    if(!strcmp(body,"cylinder"))
    {
      //un Cilindro
      //Definicion de constantes para el objeto
      dReal directionCylinder = GetDirection(bodyXML);
      dMass mCylinder;
      dReal massCylinder = GetMass(bodyXML);  
      radiusCylinder = GetRadiusCylinder(bodyXML);
      lengthCylinder =GetLenghtCylinder(bodyXML); 
      dReal x,y,z;
      GetPosition(bodyXML,x,y,z);
      
       //Creacion y locacion del objeto
      cylinder = dBodyCreate (world);
      geomCylinder = dCreateCylinder (space,radiusCylinder,lengthCylinder);   
      dMassSetCylinderTotal(&mCylinder,massCylinder,directionCylinder,radiusCylinder,lengthCylinder);
      dBodySetMass (cylinder,&mCylinder);
      dGeomSetBody(geomCylinder,cylinder);
      dBodySetPosition (cylinder,x,y,z);
    }


/*********************************************************************************************/
/*                Capsula
/*
/*********************************************************************************************/
    if (!strcmp(body,"capsule")){
      // Una capsula
      //Definicion de constantes para el objeto
      dReal directionCapsule = GetDirection(bodyXML);
      dMass mCapsule;
      dReal massCapsule = GetMass(bodyXML);  
      radiusCapsule = GetRadiusCapsule(bodyXML);
      lengthCapsule =GetLenghtCapsule(bodyXML); 
      dReal x,y,z;
      GetPosition(bodyXML,x,y,z);
      dMassSetZero(&mCapsule);
      
       //Creacion y locacion del objeto
      capsule = dBodyCreate (world);
      geomCapsule = dCreateCapsule (space,radiusCapsule,lengthCapsule);  
      dMassSetCapsule(&mCapsule,1,directionCapsule,radiusCapsule,lengthCapsule);
      dBodySetMass (capsule,&mCapsule);
      dGeomSetBody(geomCapsule,capsule);
      dBodySetPosition (capsule,x,y,z);
    }

/*********************************************************************************************/
/*                Definicion de constantes del mundo
/*
/*********************************************************************************************/
    
    
	//Gravedad y cosas de simulacion
    dWorldSetGravity(world,0,0,-9.81);
    dWorldSetCFM (world,1e-5);
    dCreatePlane (space,0,0,1,0);
    contactgroup = dJointGroupCreate (0);

    //  the simulation
    dsSimulationLoop (argc,argv,960,480,&fn);

  
  	//-- Destruir el mundo
    
    dJointGroupDestroy (contactgroup);
    dSpaceDestroy (space);
    dWorldDestroy (world);
    dCloseODE();
     
	return 0;
}


