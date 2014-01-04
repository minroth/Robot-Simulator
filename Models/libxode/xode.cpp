
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include <tinyxml.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
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

/**
  @brief Retorna el la constante de aceleracion de gravedad basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param worldXML El al archivo *.xode donde esta definido el objeto y para este caso 
        el archivo donde se encuantrasn las constantes de aceleracion de gravedad.

  @returns  retorna la aceleracion de gravedad del mundo.


  */
dReal GetGravity(TiXmlElement *worldXML){
  TiXmlElement *gravity;          //Elementos Para el manejo de XML

  gravity = worldXML -> FirstChildElement("ext")-> FirstChildElement("gravity");
  if(gravity)
  {
    return atof(gravity->Attribute("value"));
  }else
  { 
    return 0;
  }
}

/*********************************************************************************************/

/**
  @brief Retorna el la constante CFM basado en un archico XODE

  Los archivos *.xode son una representacion de un objeto en ODE en formato XML.
  Esta funcion solo considera un archivos con solamente un objeto.
  La funcion utiliza la biblioteca TinyXML para manejar el XML.

  @param worldXML El al archivo *.xode donde esta definido el objeto y para este caso 
        el archivo donde se encuentran la constante CFM.

  @returns  retorna la constante CFM.


  */
dReal GetCFM(TiXmlElement *worldXML){
  TiXmlElement *CFM;          //Elementos Para el manejo de XML

  CFM = worldXML -> FirstChildElement("ext")-> FirstChildElement("CFM");
  if(CFM)
  {
    return atof(CFM->Attribute("value"));
  }else
  { 
    return 0;
  }
}
