#include <iostream>
#include <list>
#include <tinyxml.h>
#include "XODEParser.h"



using namespace std;

/**
*	@brief 	Constructor clase XODE_ext
*/
XODE_gravity::XODE_gravity()
{
	x=y=z=0;
	empty = false;
}
/**
*	@brief 	Metodo que determina si el objeto esta vacio.
*	@return true, cuando el objeto esta vacio.
*/
bool XODE_gravity::isEmpty()
{
	return this->empty;	
}
/**
*	@brief 	Metodo que obtiene el atributo x
*	@return Atributo x
*/
float XODE_gravity::getX()
{
	return this->x;
}
/**
*	@brief 	Metodo que obtiene el atributo y
*	@return Atributo y
*/
float XODE_gravity::getY()
{
	return this->y;
}
/**
*	@brief 	Metodo que obtiene el atributo z
*	@return Atributo z
*/
float XODE_gravity::getZ()
{
	return this->z;
}

/**
*	@brief 	Metodo loadGravity
*	@param	gravityXML : Representa la etiqueta XML con los atributos que busco.
*	@note	Se agregan condiciones de nulidad a la carga del archivo, para evitar 
*	violaciones de segmento.
*/
void XODE_gravity::loadGravity(TiXmlElement *gravityXML)
{
	if(gravityXML)
	{
		if(gravityXML->Attribute("x")) 
		{
			x=atof(gravityXML->Attribute("x"));
		}
		if(gravityXML->Attribute("y")) 
		{
			y=atof(gravityXML->Attribute("y"));
		}
		if(gravityXML->Attribute("z")) 
		{
			z=atof(gravityXML->Attribute("z"));
		}
		empty = false;
	}
}	


/**
*	@brief 	Constructor clase XODE_ext
*/
XODE_ext::XODE_ext()
{
	CFM = 0;
	ename = ' ';
	empty = true;
}


/**
*	@brief 	Metodo que determina si el objeto esta vacio.
*	@return true, cuando el objeto esta vacio.
*/
bool XODE_ext::isEmpty()
{
	return this->empty;	
}


/**
*	@brief 	Metodo que obtiene el atributo gravity
*	@return Atributo gravity
*/
XODE_gravity XODE_ext::getGravity()
{
	return this->gravity;
}


/**
*	@brief 	Metodo que obtiene el atributo CFM
*	@return Atributo CFM
*/
float XODE_ext::getCFM()
{
	return this->CFM;
}


/**
*	@brief 	Metodo que obtiene el atributo ename
*	@return Atributo ename
*/
std::string XODE_ext::getEname()
{
	return this->ename;
}

/**
*	@brief 	Metodo loadExt
*	@param	extXML : Representa la etiqueta XML con los atributos que busco.
*	@note	Se agregan condiciones de nulidad a la carga del archivo, para evitar 
*	violaciones de segmento.
*/
void XODE_ext::loadExt(TiXmlElement *extXML)
{
	if(extXML)
	{

	if(extXML->FirstChildElement( "gravity" )) 
	{
		gravity.loadGravity(extXML->FirstChildElement( "gravity" ));
	}
	if(extXML->FirstChildElement( "CFM" )->Attribute("value"))
	{ 
		CFM=atof(extXML->FirstChildElement( "CFM" )->Attribute("value"));
	}
	if(extXML->Attribute("ename")) 
	{	
		ename=extXML->Attribute("ename");
	}
	empty = false;
	}
}

/**
*	@brief 	Constructor clase XODE_euler
*/
XODE_euler::XODE_euler()	
{ 
	x=y=z=0;
	aformat=' '
	;empty=true;	
}


/**
*	@brief 	Metodo que determina si el objeto esta vacio.
*	@return true, cuando el objeto esta vacio.
*/
bool  XODE_euler::isEmpty()	
{ 
	return this->empty;	
}


/**
*	@brief 	Metodo que obtiene el atributo X
*	@return Atributo x
*/
float XODE_euler::getX()	
{ 
	return this->x; 	
}


/**
*	@brief 	Metodo que obtiene el atributo Y
*	@return Atributo Y
*/
float XODE_euler::getY()	
{ 
	return this->y; 	
}


/**
*	@brief 	Metodo que obtiene el atributo Z
*	@return Atributo Z
*/
float XODE_euler::getZ()	
{ 
	return this->z; 	
}


/**
*	@brief 	Metodo que obtiene el atributo aformat
*	@return Atributo aformat
*/
std::string XODE_euler::getAformat() 	
{ 
	return this->aformat; 	
}
	


/**
*	@brief 	Metodo loadEuler
*	@param	eulerXML : Representa la etiqueta XML con los atributos que busco.
*	@note	Se agregan condiciones de nulidad a la carga del archivo, para evitar 
*	violaciones de segmento.
*/
void XODE_euler::loadEuler(TiXmlElement *eulerXML)
{
	if(eulerXML)
	{
	if(eulerXML->Attribute("x")) x=atof(eulerXML->Attribute("x"));
	if(eulerXML->Attribute("y")) y=atof(eulerXML->Attribute("y"));
	if(eulerXML->Attribute("z")) z=atof(eulerXML->Attribute("z"));
	if(eulerXML->Attribute("aformat")) aformat = eulerXML->Attribute("aformat");
	empty = false;
	}

}


/**
*	@brief 	Constructor clase XODE_axisangle
*/
XODE_axisangle::XODE_axisangle()	
{ 
	x=y=z=angle=0;
	aformat=' ';	
}


/**
*	@brief 	Metodo que determina si el objeto esta vacio.
*	@return true, cuando el objeto esta vacio.
*/
bool  XODE_axisangle::isEmpty()	
{ 
	return this->empty;	
}


/**
*	@brief 	Metodo que obtiene el atributo X
*	@return Atributo X
*/
float XODE_axisangle::getX()	
{ 
	return this->x; 	
}


/**
*	@brief 	Metodo que obtiene el atributo Y
*	@return Atributo Y
*/
float XODE_axisangle::getY()	
{ 
	return this->y; 	
}


/**
*	@brief 	Metodo que obtiene el atributo Z
*	@return Atributo Z
*/
float XODE_axisangle::getZ()	
{ 
	return this->z; 	
}


/**
*	@brief 	Metodo que obtiene el atributo angle
*	@return Atributo angle
*/
float XODE_axisangle::getAngle()	
{ 
	return this->angle;	
}


/**
*	@brief 	Metodo que obtiene el atributo aformat
*	@return Atributo aformat
*/
std::string XODE_axisangle::getAformat() 	
{ 
	return this->aformat;	
}


/**
*	@brief 	Metodo loadAxisangle
*	@param	axisangleXML : Representa la etiqueta XML con los atributos.
*/
void XODE_axisangle::loadAxisangle(TiXmlElement *axisangleXML)
{
	if(axisangleXML)
	{

	if(axisangleXML->Attribute("x")) x=atof(axisangleXML->Attribute("x"));
	if(axisangleXML->Attribute("y")) y=atof(axisangleXML->Attribute("y"));
	if(axisangleXML->Attribute("z")) z=atof(axisangleXML->Attribute("z"));
	if(axisangleXML->Attribute("angle")) angle=atof(axisangleXML->Attribute("angle"));
	if(axisangleXML->Attribute("aformat")) aformat = axisangleXML->Attribute("aformat");
	empty = false;
	}
}


/**
*	@brief 	Constructor clase XODE_quaternion
*/
XODE_quaternion::XODE_quaternion()	
{ 
	x=y=z=0;
	aformat=' ';
	empty=true;	
}


/**
*	@brief 	Metodo que determina si el objeto esta vacio.
*	@return true, cuando el objeto esta vacio.
*/
bool  XODE_quaternion::isEmpty()	
{ 
	return this->empty;	
}


/**
*	@brief 	Metodo que obtiene el atributo X
*	@return Atributo X
*/
float XODE_quaternion::getX()	
{ 
	return this->x; 	
}


/**
*	@brief 	Metodo que obtiene el atributo Y
*	@return Atributo Y
*/
float XODE_quaternion::getY()	
{ 
	return this->y; 	
}

/**
*	@brief 	Metodo que obtiene el atributo Z
*	@return Atributo Z
*/
float XODE_quaternion::getZ()	
{ 
	return this->z; 	
}

/**
*	@brief 	Metodo que obtiene el atributo aformat
*	@return Atributo aformat
*/
std::string XODE_quaternion::getAformat() 	
{ 
	return this->aformat; 	
}


/**
*	@brief 	Metodo loadQuaternion
*	@param	quaternionXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_quaternion::loadQuaternion(TiXmlElement *quaternionXML)
{
	if(quaternionXML)
	{
	if(quaternionXML->Attribute("x")) x=atof(quaternionXML->Attribute("x"));
	if(quaternionXML->Attribute("y")) y=atof(quaternionXML->Attribute("y"));
	if(quaternionXML->Attribute("z")) z=atof(quaternionXML->Attribute("z"));
	if(quaternionXML->Attribute("aformat")) aformat = quaternionXML->Attribute("aformat");
	empty = false;
	}
}

/**
*	@brief 	Metodo getOEuler
*	@return	oEuler : Un objeto de tipo Euler contenido dentro del objeto XODE_rotation
*/
XODE_euler XODE_rotation::getOEuler()	
{ 
	return this->oEuler; 	
}


/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_rotation::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo getOAxisangle
*	@return	oAxisangle : Un objeto de tipo Axisangle contenido dentro del objeto XODE_rotation
*/
XODE_axisangle XODE_rotation::getOAxisangle()	
{ 
	return this->oAxisangle; 	
}


/**
*	@brief 	Metodo getOQuaternion
*	@return	oQuaternion : Un objeto de tipo Quaternion contenido dentro del objeto XODE_rotation
*/
XODE_quaternion XODE_rotation::getOQuaternion()	
{ 
	return this->oQuaternion; 	
}



/**
*	@brief 	Metodo loadRotation
*	@param	rotationXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_rotation::loadRotation(TiXmlElement *rotationXML)
{
	if(rotationXML)
	{
	TiXmlElement 	*euler = rotationXML->FirstChildElement( "euler" );
	TiXmlElement 	*axisangle = rotationXML->FirstChildElement( "axisangle" );
	TiXmlElement 	*quaternion = rotationXML->FirstChildElement( "quaternion" );
	if(euler) 	oEuler.loadEuler(euler);
	if(axisangle) 	oAxisangle.loadAxisangle(axisangle);
	if(quaternion)	oQuaternion.loadQuaternion(quaternion);
	
	if(euler || axisangle || quaternion )
	empty 	= 	false;
	}

}

/**
*	@brief 	Constructor clase XODE_position
*/
XODE_position::XODE_position()	
{ 
	x=y=z=0; 
	empty=true;	
}


/**
*	@brief 	Metodo que obtiene el atributo X
*	@return Atributo X
*/
float XODE_position::getX()	
{ 
	return this->x; 	
}

/**
*	@brief 	Metodo que obtiene el atributo Y
*	@return Atributo Y
*/
float XODE_position::getY()	
{ 
	return this->y; 	
}

/**
*	@brief 	Metodo que obtiene el atributo Z
*	@return Atributo Z
*/
float XODE_position::getZ()	
{ 
	return this->z; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.s
*	@return	empty TRUE or FALSE
*/
bool  XODE_position::isEmpty()	
{ 
	return this->empty;	
}



/**
*	@brief 	Metodo loadPosition
*	@param	positionXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_position::loadPosition(TiXmlElement *positionXML)
{
	if(positionXML)
	{
	if(positionXML->Attribute("x")) x=atof(positionXML->Attribute("x"));
	if(positionXML->Attribute("y"))	y=atof(positionXML->Attribute("y"));
	if(positionXML->Attribute("z")) z=atof(positionXML->Attribute("z"));
	empty = false;
	}

}



/**
*	@brief 	Metodo que obtiene el atributo m00. Corresponde a la 
*			coordenada 00 de una matriz
*	@return Atributo m00
*/
float XODE_matrix4f::getM00()	
{ 
	return this->m00; 	
}


/**
*	@brief 	Metodo que obtiene el atributo m01. Corresponde a la 
*			coordenada 01 de una matriz
*	@return Atributo m01
*/
float XODE_matrix4f::getM01()	
{ 
	return this->m01; 	
}


/**
*	@brief 	Metodo que obtiene el atributo m02. Corresponde a la 
*			coordenada 02 de una matriz
*	@return Atributo m02
*/
float XODE_matrix4f::getM02()	
{ 
	return this->m02; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m03. Corresponde a la 
*			coordenada 03 de una matriz
*	@return Atributo m03
*/
float XODE_matrix4f::getM03()	
{ 
	return this->m03; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m10. Corresponde a la 
*			coordenada 10 de una matriz
*	@return Atributo m10
*/
float XODE_matrix4f::getM10()	
{ 
	return this->m10; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m11. Corresponde a la 
*			coordenada 11 de una matriz
*	@return Atributo m11
*/
float XODE_matrix4f::getM11()	
{ 
	return this->m11; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m12. Corresponde a la 
*			coordenada 12 de una matriz
*	@return Atributo m12
*/
float XODE_matrix4f::getM12()	
{ 
	return this->m12; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m13. Corresponde a la 
*			coordenada 13 de una matriz
*	@return Atributo m13
*/
float XODE_matrix4f::getM13()	
{ 
	return this->m13; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m20. Corresponde a la 
*			coordenada 20 de una matriz
*	@return Atributo m20
*/
float XODE_matrix4f::getM20()	
{ 
	return this->m20; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m21. Corresponde a la 
*			coordenada 21 de una matriz
*	@return Atributo m21
*/
float XODE_matrix4f::getM21()	
{ 
	return this->m21; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m22. Corresponde a la 
*			coordenada 22 de una matriz
*	@return Atributo m22
*/
float XODE_matrix4f::getM22()	
{ 
	return this->m22; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m23. Corresponde a la 
*			coordenada 23 de una matriz
*	@return Atributo m23
*/
float XODE_matrix4f::getM23()	
{ 
	return this->m23; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m30. Corresponde a la 
*			coordenada 30 de una matriz
*	@return Atributo m30
*/
float XODE_matrix4f::getM30()	
{ 
	return this->m30; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m31. Corresponde a la 
*			coordenada 31 de una matriz
*	@return Atributo m31
*/
float XODE_matrix4f::getM31()	
{ 
	return this->m31; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m32. Corresponde a la 
*			coordenada 32 de una matriz
*	@return Atributo m32
*/
float XODE_matrix4f::getM32()	
{ 
	return this->m32; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m33. Corresponde a la 
*			coordenada 33 de una matriz
*	@return Atributo m33
*/
float XODE_matrix4f::getM33()	
{ 
	return this->m33; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.s
*	@return	empty TRUE or FALSE
*/
bool  XODE_matrix4f::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Constructor XODE_matrix4f
*/
XODE_matrix4f::XODE_matrix4f()
{
	m00=m02=m03=m10=m11=m12=m13=m20=m21=m22=m33=0;
	empty = true;
}

/**
*	@brief 	Metodo loadMatrix4f
*	@param	matrix4fXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_matrix4f::loadMatrix4f(TiXmlElement *matrix4fXML)
{
	if(matrix4fXML)
	{
	if(matrix4fXML->Attribute("m00")) m00 =  atof (matrix4fXML->Attribute("m00"));
	if(matrix4fXML->Attribute("m01")) m01 =  atof (matrix4fXML->Attribute("m01"));
	if(matrix4fXML->Attribute("m02")) m02 =  atof (matrix4fXML->Attribute("m02"));
	if(matrix4fXML->Attribute("m03")) m03 =  atof (matrix4fXML->Attribute("m03"));
	if(matrix4fXML->Attribute("m10")) m10 =  atof (matrix4fXML->Attribute("m10"));
	if(matrix4fXML->Attribute("m11")) m11 =  atof (matrix4fXML->Attribute("m11"));
	if(matrix4fXML->Attribute("m12")) m12 =  atof (matrix4fXML->Attribute("m12"));
	if(matrix4fXML->Attribute("m13")) m13 =  atof (matrix4fXML->Attribute("m13"));
	if(matrix4fXML->Attribute("m20")) m20 =  atof (matrix4fXML->Attribute("m20"));
	if(matrix4fXML->Attribute("m21")) m21 =  atof (matrix4fXML->Attribute("m21"));
	if(matrix4fXML->Attribute("m22")) m22 =  atof (matrix4fXML->Attribute("m22"));
	if(matrix4fXML->Attribute("m23")) m23 =  atof (matrix4fXML->Attribute("m23"));
	if(matrix4fXML->Attribute("m30")) m30 =  atof (matrix4fXML->Attribute("m30"));
	if(matrix4fXML->Attribute("m31")) m31 =  atof (matrix4fXML->Attribute("m31"));
	if(matrix4fXML->Attribute("m32")) m32 =  atof (matrix4fXML->Attribute("m32"));
	if(matrix4fXML->Attribute("m33")) m33 =  atof (matrix4fXML->Attribute("m33"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_transform
*/
XODE_transform::XODE_transform()	
{ 
	absolute=transformstyle=scale=" ";
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oMatrix4f. 
*	@return Atributo oMatrix4f
*/
XODE_matrix4f XODE_transform::getOMatrix4f()	
{ 
	return this->oMatrix4f; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oPosition. 
*	@return Atributo oPosition
*/
XODE_position XODE_transform::getOPosition()	
{ 
	return this->oPosition; 	
}


/**
*	@brief 	Metodo que obtiene el atributo oRotation. 
*	@return Atributo oRotation
*/
XODE_rotation XODE_transform::getORotation()	
{ 
	return this->oRotation; 	
}

/**
*	@brief 	Metodo que obtiene el atributo absolute. 
*	@return Atributo absolute
*/
std::string XODE_transform::getAbsolute()	
{ 
	return this->absolute; 	
}


/**
*	@brief 	Metodo que obtiene el atributo transformstyle. 
*	@return Atributo transformstyle
*/	
std::string XODE_transform::getTransformstyle()	
{ 
	return this->transformstyle;	
}


/**
*	@brief 	Metodo que obtiene el atributo scale. 
*	@return Atributo scale
*/	
std::string XODE_transform::getScale()	
{ 
	return this->scale; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_transform::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadTransform
*	@param	transformXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_transform::loadTransform(TiXmlElement *transformXML)
{
	if(transformXML)
	{
	TiXmlElement *matrix4f = transformXML->FirstChildElement( "matrix4f" );
	TiXmlElement *position = transformXML->FirstChildElement( "position" );
	TiXmlElement *rotation = transformXML->FirstChildElement( "rotation" );
	if(matrix4f) oMatrix4f.loadMatrix4f(matrix4f);
	if(position) oPosition.loadPosition(position);
	if(rotation) oRotation.loadRotation(rotation);
	if(transformXML->Attribute("transformstyle")) transformstyle = transformXML->Attribute("transformstyle");
	if(transformXML->Attribute("scale")) scale = transformXML->Attribute("scale");
	absolute 	= "true";
	

	//------------------------------------------------------------------------
	//--	absolute no es cargado a menos que venga como parametro
	//------------------------------------------------------------------------
	if(transformXML->Attribute("absolute"))
	{
	absolute=transformXML->Attribute("absolute");
	}
	empty = false;
	}
}


/**
*	@brief 	Constructor XODE_cylinder
*/
XODE_cylinder::XODE_cylinder()	
{ 
	radius=length=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo radius. 
*	@return Atributo radius
*/
float XODE_cylinder::getRadius()	
{ 
	return this->radius; 	
}

/**
*	@brief 	Metodo que obtiene el atributo length. 
*	@return Atributo length
*/
float XODE_cylinder::getLength()	
{ 
	return this->length; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_cylinder::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadCylinder
*	@param	cylinderXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_cylinder::loadCylinder(TiXmlElement *cylinderXML)
{
	if(cylinderXML)
	{
	if(cylinderXML->Attribute("radius")) radius = atof (cylinderXML->Attribute("radius"));
	if(cylinderXML->Attribute("length")) length = atof (cylinderXML->Attribute("length"));
	empty  = false;
	}
}

/**
*	@brief 	Constructor XODE_cappedCylinder
*/	
XODE_cappedCylinder::XODE_cappedCylinder()	
{ 
	radius=length=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo radius. 
*	@return Atributo radius
*/
float XODE_cappedCylinder::getRadius()	
{ 
	return this->radius; 	
}

/**
*	@brief 	Metodo que obtiene el atributo length. 
*	@return Atributo length
*/
float XODE_cappedCylinder::getLength()	
{ 
	return this->length; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_cappedCylinder::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadCappedCylinder
*	@param	cappedCylinderXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_cappedCylinder::loadCappedCylinder(TiXmlElement *cappedCylinderXML)
{
	if(cappedCylinderXML)
	{
	if(cappedCylinderXML->Attribute("radius")) radius = atof (cappedCylinderXML->Attribute("radius"));
	if(cappedCylinderXML->Attribute("length")) length = atof (cappedCylinderXML->Attribute("length"));
	empty  = false;
	}
}

/**
*	@brief 	Constructor XODE_box
*/	
XODE_box::XODE_box()	
{ 
	sizex=sizey=sizez=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo sizex. 
*	@return Atributo sizex
*/
float XODE_box::getSizex()	
{ 
	return this->sizex; 	
}

/**
*	@brief 	Metodo que obtiene el atributo sizey. 
*	@return Atributo sizey
*/
float XODE_box::getSizey()	
{ 
	return this->sizey; 	
}

/**
*	@brief 	Metodo que obtiene el atributo sizez. 
*	@return Atributo sizez
*/
float XODE_box::getSizez()	
{ 
	return this->sizez; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_box::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadBox
*	@param	boxXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_box::loadBox(TiXmlElement *boxXML)
{
	if(boxXML)
	{
	if(boxXML->Attribute("sizex")) sizex = atof (boxXML->Attribute("sizex"));
	if(boxXML->Attribute("sizey")) sizey = atof (boxXML->Attribute("sizey"));
	if(boxXML->Attribute("sizez")) sizez = atof (boxXML->Attribute("sizez"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_sphere
*/
XODE_sphere::XODE_sphere()	
{
	radius=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo radius. 
*	@return Atributo radius
*/
float XODE_sphere::getRadius()	
{
	return this->radius; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_sphere::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadSphere
*	@param	sphereXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_sphere::loadSphere(TiXmlElement *sphereXML)
{
	if(sphereXML)
	{
	if(sphereXML->Attribute("radius")) radius = atof (sphereXML->Attribute("radius"));
	empty  = false;
	}
}

/**
*	@brief 	Constructor XODE_cone
*/	
XODE_cone::XODE_cone()	
{
	radius=length=0;
	empty=true;
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_cone::isEmpty()	
{
	return this->empty;	
}

/**
*	@brief 	Metodo que obtiene el atributo radius. 
*	@return Atributo radius
*/
float XODE_cone::getRadius()	
{
	return this->radius; 	
}

/**
*	@brief 	Metodo que obtiene el atributo length. 
*	@return Atributo length
*/
float XODE_cone::getLength()	
{
	return this->length; 	
}

/**
*	@brief 	Metodo loadCone
*	@param	coneXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_cone::loadCone(TiXmlElement *coneXML)
{
	if(coneXML)
	{
	if(coneXML->Attribute("radius")) radius = atof (coneXML->Attribute("radius"));
	if(coneXML->Attribute("length")) length = atof (coneXML->Attribute("length"));
	empty = false;
	}
}


/**
*	@brief 	Constructor XODE_plane
*/
XODE_plane::XODE_plane()	
{
	a=b=c=d=0;
	empty=true;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_plane::isEmpty()	
{
	return this->empty;	
}

/**
*	@brief 	Metodo que obtiene el atributo a. 
*	@return Atributo a
*/
float XODE_plane::getA()	
{
	return this->a; 	
}

/**
*	@brief 	Metodo que obtiene el atributo b. 
*	@return Atributo b
*/
float XODE_plane::getB()	
{
	return this->b; 	
}

/**
*	@brief 	Metodo que obtiene el atributo c. 
*	@return Atributo c
*/
float XODE_plane::getC()	
{
	return this->c; 	
}

/**
*	@brief 	Metodo que obtiene el atributo d. 
*	@return Atributo d
*/
float XODE_plane::getD()	
{
	return this->d; 	
}

/**
*	@brief 	Metodo loadPlane
*	@param	planeXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_plane::loadPlane(TiXmlElement *planeXML)
{
	if(planeXML)
	{
	if(planeXML->Attribute("a")) a = atof (planeXML->Attribute("a"));
	if(planeXML->Attribute("b")) b = atof (planeXML->Attribute("b"));
	if(planeXML->Attribute("c")) c = atof (planeXML->Attribute("c"));
	if(planeXML->Attribute("d")) d = atof (planeXML->Attribute("d"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_ray
*/
XODE_ray::XODE_ray()	
{
	length=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo length. 
*	@return Atributo length
*/
float XODE_ray::getLength()
{
	return this->length;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_ray::isEmpty()	
{
	return this->empty;	

}

/**
*	@brief 	Metodo loadRay
*	@param	rayXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_ray::loadRay(TiXmlElement *rayXML)
{
	if(rayXML)
	{
	if(rayXML->Attribute("length")) length = atof (rayXML->Attribute("length"));
	empty = false;
	}
}


/**
*	@brief 	Constructor XODE_v
*/
XODE_v::XODE_v()	
{
	x=y=z=e=0;	
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_v::getX()	
{ 
	return this->x;	
}

/**
*	@brief 	Metodo que obtiene el atributo y.
*	@return Atributo y
*/
float XODE_v::getY()	
{ 
	return this->y;	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_v::getZ()	
{ 
	return this->z;	
}

/**
*	@brief 	Metodo que obtiene el atributo e. 
*	@return Atributo e
*/
float XODE_v::getE()	
{ 
	return this->e;
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_v::isEmpty()	
{ 
	return this->empty;
}
	
/**
*	@brief 	Metodo loadV(TiXmlElement *vXML)
*	@param 	VXML, el XML que se cargará en el objeto	
*/
void XODE_v::loadV(TiXmlElement *vXML)
{
	if(vXML)
	{
	if(vXML->Attribute("x")) x = atof (vXML->Attribute("x")); 
	if(vXML->Attribute("y")) y = atof (vXML->Attribute("y"));
	if(vXML->Attribute("z")) z = atof (vXML->Attribute("z"));
	if(vXML->Attribute("e")) e = atof (vXML->Attribute("e"));
	}
}

/**
*	@brief 	Metodo que obtiene el atributo oV. 
*	@return Atributo oV
*/
list<XODE_v> XODE_vertices::getListOV()	
{ 
	return this->oV;	
}
    

/**
*	@brief 	Metodo empty()
*	@return TRUE or FALSE dependiendo si existen elementos en la lista de vertices.
*/
bool XODE_vertices::empty()
{
	if (oV.empty()) 	return true;
	else	return false;
}

/**
*	@brief 	Metodo getElementByPosition(int position)
*	@param 	position posicion del elemento que se regresara.
*	@return retorna el elemento en la posocion position
*/
XODE_v XODE_vertices::getElementByPosition(int position)
{
	int i = 0;
  	list<XODE_v>::iterator iterator;
  	XODE_v element;
  	iterator = oV.begin();
  	while( iterator != oV.end())
  	{
  	if(i == position)
  	{
  	element = *iterator;
  	}
    	iterator++;
    	i++;
  	}
  	return element;
}


/**
*	@brief 	Metodo loadVertices. Carga en una lista los n vertices v
*	@param 	el XML con los elementos a cargar
*	
*/
void XODE_vertices::loadVertices(TiXmlElement *verticesXML)
{
	XODE_v oVertice;
	TiXmlElement *vXML = verticesXML->FirstChildElement ("v");
	while(vXML)
	{
	oVertice.loadV(vXML);
	oV.push_back(oVertice);
	vXML = vXML-> NextSiblingElement("v");
	}
}


/**
*	@brief 	Metodo getSize(). Entrega el tamaño de vertices contenidos
*	en el objeto	
*/
int XODE_vertices::getSize()
{
	return oV.size();
}


/**
*	@brief 	Constructor XODE_t
*/
XODE_t::XODE_t()	
{ 
	ia=ib=ic=0;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_t::isEmpty()	
{ 
	return this->empty;
}

/**
*	@brief 	Metodo que obtiene el atributo ia. 
*	@return Atributo ia
*/
float XODE_t::getIa()	
{ 
	return this->ia;	
}

/**
*	@brief 	Metodo que obtiene el atributo ib. 
*	@return Atributo ib
*/
float XODE_t::getIb()	
{ 
	return this->ib;	
}

/**
*	@brief 	Metodo que obtiene el atributo ic. 
*	@return Atributo ic
*/
float XODE_t::getIc()	
{ 
	return this->ic;	
}

/**
*	@brief 	Metodo loadT(TiXmlElement *tXML)
*	@param 	tXML, el XML que se cargará en el objeto	
*/
void XODE_t::loadT(TiXmlElement *tXML)
{
	if(tXML)
	{	
	if(tXML->Attribute("ia")) ia = atof (tXML->Attribute("ia")); 
	if(tXML->Attribute("ib")) ib = atof (tXML->Attribute("ib"));
	if(tXML->Attribute("ic")) ic = atof (tXML->Attribute("ic"));
	empty = false;
	}
}

/**
*	@brief 	Metodo que obtiene el atributo oT. 
*	@return Atributo oT
*/
list<XODE_t> XODE_triangles::getList()	
{ 
	return this->oT;	
}

/**
*	@brief 	Metodo que obtiene el atributo size. 
*	@return Atributo size
*/
int	XODE_triangles::getSize()	
{ 
	return this->oT.size();	
}    

/**
*	@brief 	Metodo empty()
*	@return TRUE or FALSE dependiendo si existen elementos en la lista.
*/
bool XODE_triangles::empty()
{
	if (oT.empty()) 	return true;
	else	return false;
}

/**
*	@brief 	Metodo getElementByPosition(int position)
*	@param 	position posicion del elemento que se regresara.
*	@return retorna el elemento en la posicion position
*/
XODE_t XODE_triangles::getElementByPosition(int position)
{
	int i = 0;
  	list<XODE_t>::iterator iterator;
  	XODE_t element;
  	iterator = oT.begin();
  	while( iterator != oT.end())
  	{
  	if(i == position)
  	{
  	element = *iterator;
  	}
    	iterator++;
    	i++;
  	}
  	return element;
}


/**
*	@brief 	Metodo loadVertices. Carga en una lista los n vertices
*	@param 	el XML con los elementos a cargar
*	
*/
void XODE_triangles::loadTriangles(TiXmlElement *trianglesXML)
{
	XODE_t oTriangle;
	TiXmlElement *tXML = trianglesXML->FirstChildElement ("t");
	while(tXML)
	{
	oTriangle.loadT(tXML);
	oT.push_back(oTriangle);
	tXML = tXML-> NextSiblingElement("t");
	}
}



/**
*	@brief 	Metodo que obtiene el atributo oVertices. 
*	@return Atributo oVertices
*/
XODE_vertices  XODE_trimesh::getOVertices()	
{ 
	return this->oVertices;	
}

/**
*	@brief 	Metodo que obtiene el atributo oTriangles. 
*	@return Atributo oTriangles
*/
XODE_triangles XODE_trimesh::getOTriangles()	
{ 
	return this->oTriangles;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_trimesh::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadTrimesh. Carga en una lista los n vertices
*	@param 	trimeshXML: el XML con los elementos a cargar
*	
*/
void XODE_trimesh::loadTrimesh(TiXmlElement *trimeshXML)
{
	TiXmlElement *vertices = trimeshXML->FirstChildElement("vertices");
	TiXmlElement *triangles = trimeshXML->FirstChildElement("triangles");
	if(vertices) oVertices.loadVertices(vertices);
	if(triangles) oTriangles.loadTriangles(triangles);
	if(vertices && triangles) empty = false;
}


/**
*	@brief 	Constructor XODE_anchor
*/
XODE_anchor::XODE_anchor()	
{
	x=y=z=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_anchor::getX()	
{ 
	return this->x; 	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_anchor::getY()	
{ 
	return this->y; 	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_anchor::getZ()	
{ 
	return this->z; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_anchor::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadAnchor
*	@param	anchorXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_anchor::loadAnchor(TiXmlElement *anchorXML)
{
	if(anchorXML)
	{
	if(anchorXML->Attribute("x")) x = atof (anchorXML->Attribute("x"));
	if(anchorXML->Attribute("y")) y = atof (anchorXML->Attribute("y"));
	if(anchorXML->Attribute("z")) z = atof (anchorXML->Attribute("z"));
	empty = false;
	}
}	

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_axis::getX()	
{ 
	return this->x; 	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_axis::getY()	
{ 
	return this->y; 	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_axis::getZ()	
{ 
	return this->z; 	
}

/**
*	@brief 	Metodo que obtiene el atributo lowStop. 
*	@return Atributo lowStop
*/
float XODE_axis::getLowStop()	
{ 
	return this->lowStop;	
}

/**
*	@brief 	Metodo que obtiene el atributo hiStop. 
*	@return Atributo hiStop
*/
float XODE_axis::getHiStop()	
{ 
	return this->hiStop; 	
}

/**
*	@brief 	Metodo que obtiene el atributo vel. 
*	@return Atributo vel
*/
float XODE_axis::getVel()	
{ 
	return this->vel;	
}

/**
*	@brief 	Metodo que obtiene el atributo fMax. 
*	@return Atributo fMax
*/
float XODE_axis::getFMax()	
{ 
	return this->fMax; 	
}	

/**
*	@brief 	Metodo que obtiene el atributo fudgeFactor. 
*	@return Atributo fudgeFactor
*/
float XODE_axis::getFudgeFactor()	
{ 
	return this->fudgeFactor;	
}

/**
*	@brief 	Metodo que obtiene el atributo bounce. 
*	@return Atributo bounce
*/
float XODE_axis::getBounce()	
{ 
	return this->bounce; 	
}

/**
*	@brief 	Metodo que obtiene el atributo CFM. 
*	@return Atributo CFM
*/
float XODE_axis::getCFM()	
{ 
	return this->CFM; 	
}

/**
*	@brief 	Metodo que obtiene el atributo stopERP. 
*	@return Atributo stopERP
*/
float XODE_axis::getStopERP()	
{ 
	return this->stopERP; 	
}

/**
*	@brief 	Metodo que obtiene el atributo stopCFM. 
*	@return Atributo stopCFM
*/
float XODE_axis::getStopCFM()	
{ 
	return this->stopCFM; 	
}

/**
*	@brief 	Metodo que obtiene el atributo suspensionERP. 
*	@return Atributo suspensionERP
*/
float XODE_axis::getSuspensionERP()	
{ 
	return this->suspensionERP;	
}

/**
*	@brief 	Metodo que obtiene el atributo suspensionCFM. 
*	@return Atributo suspensionCFM
*/
float XODE_axis::getSuspensionCFM()	
{ 
	return this->suspensionCFM;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_axis::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	constructor axis
*/
XODE_axis::XODE_axis()
{
	x=0;
    y=0;
    z=0;
    lowStop=0;
	hiStop=0;
	vel=0;
	fMax=0;
	fudgeFactor=0;
	bounce=0;
	CFM=0;
	stopERP=0;
	stopCFM=0;
	suspensionERP=0;
	suspensionCFM=0;
	empty = true;
}
/**
*	@brief 	Metodo loadAxis
*	@param	axisXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_axis::loadAxis(TiXmlElement *axisXML)
{
	if(axisXML)
	{
	if(axisXML->Attribute("x")) x = atof (axisXML->Attribute("x"));
	if(axisXML->Attribute("y")) y = atof (axisXML->Attribute("y"));
	if(axisXML->Attribute("z")) z = atof (axisXML->Attribute("z"));
	if(axisXML->Attribute("LowStop")) lowStop = atof (axisXML->Attribute("LowStop"));
	if(axisXML->Attribute("HiStop")) hiStop = atof (axisXML->Attribute("HiStop"));
	if(axisXML->Attribute("Vel")) vel = atof (axisXML->Attribute("Vel"));
	if(axisXML->Attribute("FMax")) fMax = atof (axisXML->Attribute("FMax"));
	if(axisXML->Attribute("FudgeFactor")) fudgeFactor = atof (axisXML->Attribute("FudgeFactor"));
	if(axisXML->Attribute("Bounce")) bounce = atof (axisXML->Attribute("Bounce"));
	if(axisXML->Attribute("CFM")) CFM = atof (axisXML->Attribute("CFM"));
	if(axisXML->Attribute("StopERP")) stopERP = atof (axisXML->Attribute("StopERP"));
	if(axisXML->Attribute("StopCFM")) stopCFM = atof (axisXML->Attribute("StopCFM"));
	if(axisXML->Attribute("SuspensionERP")) suspensionERP 	= atof (axisXML->Attribute("SuspensionERP"));
	if(axisXML->Attribute("SuspensionCFM")) suspensionCFM 	= atof (axisXML->Attribute("SuspensionCFM"));
	empty 	= false;
	}
}	

/**
*	@brief 	Constructor XODE_centerGravity
*/
XODE_centerGravity::XODE_centerGravity()	
{
	x=y=z=0;empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_centerGravity::getX()	
{ 
	return this->x; 	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_centerGravity::getY()	
{ 
	return this->y; 	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_centerGravity::getZ()	
{ 
	return this->z; 
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_centerGravity::isEmpty()	
{ 
	return this->empty;	
}
    
/**
*	@brief 	Metodo loadCenterGravity
*	@param	centerGravityXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_centerGravity::loadCenterGravity(TiXmlElement *centerGravityXML)
{
	if(centerGravityXML)
	{
	if(centerGravityXML->Attribute("x")) x = atof (centerGravityXML->Attribute("x"));
	if(centerGravityXML->Attribute("y")) y = atof (centerGravityXML->Attribute("y"));
	if(centerGravityXML->Attribute("z")) z = atof (centerGravityXML->Attribute("z"));
	empty = false;
	}
}	


/**
*	@brief 	Constructor XODE_imatrix
*/
XODE_imatrix::XODE_imatrix()	
{ 
	m00=m01=m21=m10=m11=m12=m20=m21=m22=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo m00. 
*	@return Atributo m00
*/
float XODE_imatrix::getM00()	
{ 
	return this->m00; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m01. 
*	@return Atributo m01
*/
float XODE_imatrix::getM01()	
{ 
	return this->m01; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m02. 
*	@return Atributo m02
*/
float XODE_imatrix::getM02()	
{ 
	return this->m02; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m10. 
*	@return Atributo m10
*/
float XODE_imatrix::getM10()	
{ 
	return this->m10; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m11. 
*	@return Atributo m11
*/
float XODE_imatrix::getM11()	
{ 
	return this->m11; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m12. 
*	@return Atributo m12
*/
float XODE_imatrix::getM12()	
{ 
	return this->m12; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m20. 
*	@return Atributo m20
*/
float XODE_imatrix::getM20()	
{ 
	return this->m20; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m21. 
*	@return Atributo m21
*/
float XODE_imatrix::getM21()	
{ 
	return this->m21; 	
}

/**
*	@brief 	Metodo que obtiene el atributo m22. 
*	@return Atributo m22
*/
float XODE_imatrix::getM22()	
{ 
	return this->m22; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_imatrix::isEmpty()	
{ 
	return this->empty;	
}


/**
*	@brief 	Metodo loadImatrix
*	@param	imatrixXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_imatrix::loadImatrix(TiXmlElement *imatrixXML)
{
	if(imatrixXML)
	{
	if(imatrixXML->Attribute("m00")) m00 =  atof (imatrixXML->Attribute("m00"));
	if(imatrixXML->Attribute("m01")) m01 =  atof (imatrixXML->Attribute("m01"));
	if(imatrixXML->Attribute("m02")) m02 =  atof (imatrixXML->Attribute("m02"));
	if(imatrixXML->Attribute("m10")) m10 =  atof (imatrixXML->Attribute("m10"));
	if(imatrixXML->Attribute("m11")) m11 =  atof (imatrixXML->Attribute("m11"));
	if(imatrixXML->Attribute("m12")) m12 =  atof (imatrixXML->Attribute("m12"));
	if(imatrixXML->Attribute("m20")) m20 =  atof (imatrixXML->Attribute("m20"));
	if(imatrixXML->Attribute("m21")) m21 =  atof (imatrixXML->Attribute("m21"));
	if(imatrixXML->Attribute("m22")) m22 =  atof (imatrixXML->Attribute("m22"));
	empty = false;
	}

	
}


/**
*	@brief 	Constructor XODE_mass_struct
*/	
XODE_mass_struct::XODE_mass_struct()	
{
	density=total=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oCenterGravity. 
*	@return Atributo oCenterGravity
*/
XODE_centerGravity 	XODE_mass_struct::getOCenterGravity()	
{ 
	return this->oCenterGravity; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oImatrix. 
*	@return Atributo oImatrix
*/
XODE_imatrix 	XODE_mass_struct::getOImatrix()
{ 
	return this->oImatrix; 	
}

/**
*	@brief 	Metodo que obtiene el atributo density. 
*	@return Atributo density
*/
float 	XODE_mass_struct::getDensity()	
{ 
	return this->density; 	
}

/**
*	@brief 	Metodo que obtiene el atributo total. 
*	@return Atributo total
*/
float 	XODE_mass_struct::getTotal()	
{ 
	return this->total; 	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_mass_struct::isEmpty()	
{ 
	return this->empty;	
}
		
/**
*	@brief 	Metodo loadMass_struct
*	@param	mass_struct : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_mass_struct::loadMass_struct(TiXmlElement *mass_structXML)
{
	TiXmlElement *centerGravity=mass_structXML->FirstChildElement( "centerGravity" );
	TiXmlElement *imatrix=mass_structXML->FirstChildElement( "imatrix" );

	if(mass_structXML)
	{
	if(centerGravity) oCenterGravity.loadCenterGravity(centerGravity);
	if(imatrix) oImatrix.loadImatrix(imatrix);
	if(mass_structXML->Attribute("density")) density =  atof (mass_structXML->Attribute("density"));
	if(mass_structXML->Attribute("total")) total 	=  atof (mass_structXML->Attribute("total"));
	empty = false;
	}
}


/**
*	@brief 	Constructor XODE_mass_shape
*/
XODE_mass_shape::XODE_mass_shape ()	
{
	density=total=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oCylinder. 
*	@return Atributo oCylinder
*/
XODE_cylinder 	XODE_mass_shape::getOCylinder()	
{ 
	return this->oCylinder; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oSphere. 
*	@return Atributo oSphere
*/
XODE_sphere 	XODE_mass_shape::getOSphere()	
{ 
	return this->oSphere; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oBox. 
*	@return Atributo oBox
*/
XODE_box 	XODE_mass_shape::getBox()	
{ 
	return this->oBox;	
}

/**
*	@brief 	Metodo que obtiene el atributo oCappedCylinder. 
*	@return Atributo oCappedCylinder
*/
XODE_cappedCylinder 	XODE_mass_shape::getOCappedCylinder()
{ 
	return this->oCappedCylinder;	
}

/**
*	@brief 	Metodo que obtiene el atributo density. 
*	@return Atributo density
*/
float	XODE_mass_shape::getDensity()	
{ 
	return this->density;	
}

/**
*	@brief 	Metodo que obtiene el atributo total. 
*	@return Atributo total
*/
float	XODE_mass_shape::getTotal()	
{ 
	return this->total;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_mass_shape::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadMass_shape
*	@param	mass_shapeXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_mass_shape::loadMass_shape(TiXmlElement *mass_shapeXML)
{
	TiXmlElement *cylinder = mass_shapeXML->FirstChildElement("cylinder");
	TiXmlElement *sphere = mass_shapeXML->FirstChildElement("sphere");
	TiXmlElement *box = mass_shapeXML->FirstChildElement("box");
	TiXmlElement *cappedCylinder = mass_shapeXML->FirstChildElement("cappedCylinder");

	if(mass_shapeXML)
	{
	if(cylinder) oCylinder.loadCylinder(cylinder);
	if(sphere) oSphere.loadSphere(sphere);
	if(box) oBox.loadBox(box);
	if(cappedCylinder) oCappedCylinder.loadCappedCylinder(cappedCylinder);
	if(mass_shapeXML->Attribute("density")) density =  atof (mass_shapeXML->Attribute("density"));
	if(mass_shapeXML->Attribute("total")) total 	=  atof (mass_shapeXML->Attribute("total"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_mass_shape
*/
XODE_adjust::XODE_mass_shape()	
{
	density=total=0;
	empty=true;	
}
float	XODE_adjust::getDensity()	{ return this->density;	}
float	XODE_adjust::getTotal()	{ return this->total;	}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_adjust::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadAdjust
*	@param	adjustXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_adjust::loadAdjust(TiXmlElement *adjustXML)
{
	if(adjustXML)
	{
	if(adjustXML->Attribute("density")) density =  atof (adjustXML->Attribute("density"));
	if(adjustXML->Attribute("total")) total =  atof (adjustXML->Attribute("total"));
	empty   = false;
	}
}

/**
*	@brief 	Constructor XODE_mass
*/
XODE_mass::XODE_mass()	
{ 
	empty=true;
}

/**
*	@brief 	Metodo que obtiene el atributo oMass_shape. 
*	@return Atributo oMass_shape
*/
XODE_mass_shape 	XODE_mass::getOMass_shape()	
{ 
	return this->oMass_shape;	
}

/**
*	@brief 	Metodo que obtiene el atributo oMass_struct. 
*	@return Atributo oMass_struct
*/
XODE_mass_struct 	XODE_mass::getOMass_struct()	
{ 
	return this->oMass_struct; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oTransform. 
*	@return Atributo oTransform
*/
XODE_transform 	XODE_mass::getoTransform()	
{ 
	return this->oTransform;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAdjust. 
*	@return Atributo oAdjust
*/
XODE_adjust 	XODE_mass::getOAdjust()	
{ 
	return this->oAdjust;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_mass::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadAdjust
*	@param	adjustXML : Representa la etiqueta XML con los atributos que busco.
*/

void XODE_mass::loadMass(TiXmlElement *massXML)
{
	TiXmlElement *mass_shape = massXML->FirstChildElement("mass_shape");
	TiXmlElement *mass_struct = massXML->FirstChildElement("mass_struct");
	TiXmlElement *transform = massXML->FirstChildElement("transform");
	TiXmlElement *adjust = massXML->FirstChildElement("adjust");

	if(massXML)
	{
	if(mass_shape) oMass_shape.loadMass_shape(mass_shape);
	if(mass_struct) oMass_struct.loadMass_struct(mass_struct);
	if(transform) oTransform.loadTransform(transform);
	if(adjust) oAdjust.loadAdjust(adjust); 	
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_link
*/
XODE_link::XODE_link()	
{
	body=' ';
	empty=true;
}

/**
*	@brief 	Metodo que obtiene el atributo body. 
*	@return Atributo body
*/
std::string XODE_link::getBody()	
{ 
	return this->body;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_link::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadLink
*	@param	linkXML : Representa la etiqueta XML con los atributos que busco.
*	@note	para ahorrar codigo, se escribe solo un link que puede ser utilizado 
*	para ambos link en el joint.
*/
void XODE_link::loadLink(TiXmlElement *linkXML)
{
	if(linkXML->Attribute("body"))
	{
	body = linkXML->Attribute("body"); 
	empty = false;
	}
}


/**
*	@brief 	Constructor XODE_amotor
*/
XODE_amotor::XODE_amotor()	
{ 
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAnchor. 
*	@return Atributo oAnchor
*/
XODE_anchor 	XODE_amotor::getOAnchor()	
{ 
	return this->oAnchor;	
} 

/**
*	@brief 	Metodo que obtiene el atributo oAxis. 
*	@return Atributo oAxis
*/
XODE_axis 	XODE_amotor::getOAxis()	
{ 
	return this->oAxis;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAxis2. 
*	@return Atributo oAxis2
*/
XODE_axis 	XODE_amotor::getOAxis2()	
{ 
	return this->oAxis2;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAxis3. 
*	@return Atributo oAxis3
*/
XODE_axis 	XODE_amotor::getOAxis3()	
{ 
	return this->oAxis3;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_amotor::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadAmotor
*	@param	amotorXML : Representa la etiqueta XML con los atributos que busco.
*	@note	Se crean 3 axis ya que es posible que aparezcan desde 0 hasta 3 axis.
*	Se desestima el uso de arrays o listas.
*/
void	XODE_amotor::loadAmotor(TiXmlElement *amotorXML)
{
	XODE_axis oAxis;
	int	i=0;
	TiXmlElement *axisXML = amotorXML->FirstChildElement ("axis");
	TiXmlElement *anchor  = amotorXML->FirstChildElement ("anchor");
	while(axisXML)
	{
	oAxis.loadAxis(axisXML);
	switch(i)
	{
	case 0 : 
	oAxis.loadAxis(axisXML);
	break;
	case 1 :
	oAxis2.loadAxis(axisXML);
	break;
	case 2 :
	oAxis3.loadAxis(axisXML);
	}
	axisXML = axisXML-> NextSiblingElement("axis");
	i++;
	}

	if(anchor) oAnchor.loadAnchor(anchor);

	empty = false;
}

/**
*	@brief 	Constructor XODE_ball
*/
XODE_ball::XODE_ball()	
{ 
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAnchor. 
*	@return Atributo oAnchor
*/
XODE_anchor  XODE_ball::getOAnchor() 	
{ 
	return this->oAnchor; 
}	

/**
*	@brief 	Metodo que obtiene el atributo oAxis. 
*	@return Atributo oAxis
*/
XODE_axis 	XODE_ball::getOAxis()	
{ 
	return this->oAxis;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_ball::isEmpty()	
{ 
	return this->empty;
}
	
/**
*	@brief 	Metodo loadBall
*	@param	ballXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_ball::loadBall(TiXmlElement *ballXML)
{
	TiXmlElement *anchor = ballXML->FirstChildElement("anchor");
	TiXmlElement *axis   = ballXML->FirstChildElement("axis");

	if(ballXML)
	{
	if(anchor) oAnchor.loadAnchor(anchor);
	if (axis) oAxis.loadAxis(axis);
	empty = false;
	}
}

	
/**
*	@brief 	Constructor XODE_hinge
*/
XODE_hinge::XODE_hinge()	
{
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAnchor. 
*	@return Atributo oAnchor
*/
XODE_anchor 	XODE_hinge::getOAnchor()	
{ 
	return this->oAnchor; 
}

/**
*	@brief 	Metodo que obtiene el atributo oAxis. 
*	@return Atributo oAxis
*/
XODE_axis 	XODE_hinge::getOAxis()	
{ 
	return this->oAxis;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_hinge::isEmpty()	
{ 
	return this->empty;	
}
	

/**
*	@brief 	Metodo loadHinge
*	@param	hingeXML : Representa la etiqueta XML con los atributos que busco.
*/

void XODE_hinge::loadHinge(TiXmlElement *hingeXML)
{
	TiXmlElement *anchor = hingeXML->FirstChildElement("anchor");
	TiXmlElement *axis   = hingeXML->FirstChildElement("axis");


	if(hingeXML)
	{
	if(anchor) oAnchor.loadAnchor(anchor);
	if(axis) oAxis.loadAxis(axis);
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_hinge2
*/
XODE_hinge2::XODE_hinge2()	
{ 
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAnchor. 
*	@return Atributo oAnchor
*/
XODE_anchor 	XODE_hinge2::getOAnchor()	
{ 
	return this->oAnchor; 
}

/**
*	@brief 	Metodo que obtiene el atributo oAxis1. 
*	@return Atributo oAxis1
*/
XODE_axis 	XODE_hinge2::getOAxis1()	
{ 
	return this->oAxis1;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAxis2. 
*	@return Atributo oAxis2
*/
XODE_axis 	XODE_hinge2::getOAxis2()	
{ 
	return this->oAxis2;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_hinge2::isEmpty()	
{ 
	return this->empty;	
}
	

/**
*	@brief 	Metodo loadHinge2
*	@param	hinge2XML : Representa la etiqueta XML con los atributos que busco.
*/

void XODE_hinge2::loadHinge2(TiXmlElement *hinge2XML)
{

	TiXmlElement *axisXML = hinge2XML->FirstChildElement ("axis");
	TiXmlElement *anchor= hinge2XML->FirstChildElement("anchor");

	if(hinge2XML)
	{ 
	if(anchor) oAnchor.loadAnchor(anchor);
	if(axisXML) oAxis1.loadAxis(axisXML);
	if(axisXML-> NextSiblingElement("axis"))
	{
	axisXML = axisXML-> NextSiblingElement("axis");
	oAxis2.loadAxis(axisXML);
	}
	empty = false;
	}
}


/**
*	@brief 	Metodo loadUniversal
*	@param	universalXML : Representa la etiqueta XML con los atributos que busco.
*/

void XODE_hinge2::loadUniversal(TiXmlElement *universalXML)
{	
	TiXmlElement *axisXML = universalXML->FirstChildElement ("axis");
	TiXmlElement *anchor= universalXML->FirstChildElement("anchor"); 

	if(universalXML)
	{
	if(anchor) oAnchor.loadAnchor(anchor);
	if(axisXML) oAxis1.loadAxis(axisXML);
	if(axisXML-> NextSiblingElement("axis"))
	{
	axisXML = axisXML-> NextSiblingElement("axis");
	oAxis2.loadAxis(axisXML);
	}
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_slider
*/
XODE_slider::XODE_slider()	
{ 
	empty=true;	
}
XODE_axis XODE_slider::getOAxis()	{	return this->oAxis;	}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_slider::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadSlider
*	@param	sliderXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_slider::loadSlider(TiXmlElement *sliderXML)
{
	if(sliderXML->FirstChildElement("axis"))
	{
	oAxis.loadAxis(sliderXML->FirstChildElement("axis"));
	empty = false;
	}
}

/**
*	@brief 	Metodo loadFixed
*	@param	fixedXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_slider::loadFixed(TiXmlElement *fixedXML)
{
	if(fixedXML->FirstChildElement("axis"))
	{
	oAxis.loadAxis(fixedXML->FirstChildElement("axis"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_joint
*/
XODE_joint::XODE_joint()	
{ 
	name=" ";
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo link1. 
*	@return Atributo link1
*/
XODE_link 	XODE_joint::getLink1() 	
{ 
	return this->link1; 	
}

/**
*	@brief 	Metodo que obtiene el atributo link2. 
*	@return Atributo link2
*/
XODE_link 	XODE_joint::getLink2 ()	
{ 
	return this->link2; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oAmotor. 
*	@return Atributo oAmotor
*/
XODE_amotor 	XODE_joint::getOAmotor() 	
{ 
	return this->oAmotor; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oBall. 
*	@return Atributo oBall
*/
XODE_ball 	XODE_joint::getOBall() 	
{ 
	return this->oBall; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oHinge. 
*	@return Atributo oHinge
*/
XODE_hinge 	XODE_joint::getOHinge() 	
{ 
	return this->oHinge; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oHinge2. 
*	@return Atributo oHinge2
*/
XODE_hinge2  XODE_joint::getOHinge2() 	
{ 
	return this->oHinge2; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oFixed. 
*	@return Atributo oFixed
*/
XODE_slider 	XODE_joint::getOFixed() 	
{ 
	return this->oFixed; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oSlider. 
*	@return Atributo oSlider
*/
XODE_slider  XODE_joint::getOSlider() 	
{ 
	return this->oSlider; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oUniversal. 
*	@return Atributo oUniversal
*/
XODE_hinge2 	XODE_joint::getOUniversal()	
{ 
	return this->oUniversal; 
}

/**
*	@brief 	Metodo que obtiene el atributo name. 
*	@return Atributo name
*/
std::string XODE_joint::getName() 	
{ 
	return this->name; 
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_joint::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadJoint
*	@param	jointXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_joint::loadJoint(TiXmlElement *jointXML)
{
	TiXmlElement *link1XML = jointXML->FirstChildElement("link1");
	TiXmlElement *link2XML = jointXML->FirstChildElement("link2");
	TiXmlElement *amotor = jointXML->FirstChildElement("amotor");
	TiXmlElement *ball = jointXML->FirstChildElement("ball");
	TiXmlElement *hinge = jointXML->FirstChildElement("hinge");
	TiXmlElement *hinge2 = jointXML->FirstChildElement("hinge2");
	TiXmlElement *fixed = jointXML->FirstChildElement("fixed");
	TiXmlElement *slider = jointXML->FirstChildElement("slider");
	TiXmlElement *universal = jointXML->FirstChildElement("universal");

	if(jointXML)
	{
	if(link1XML) link1.loadLink(link1XML);
	if(link2XML) link2.loadLink(link2XML);
	if(amotor) oAmotor.loadAmotor(amotor);
	if(ball) oBall.loadBall(ball);
	if(hinge) oHinge.loadHinge(hinge);
	if(hinge2) oHinge2.loadHinge2(hinge2);
	if(fixed) oFixed.loadFixed(fixed);
	if(slider) oSlider.loadSlider(slider);
	if(universal) oUniversal.loadUniversal(universal);
	if(jointXML->Attribute("name")) name = jointXML->Attribute("name");
	empty = false;
	}
}
/**
*	@brief 	Constructor XODE_geom
*/
XODE_geom::XODE_geom()	
{ 
	name=" ";empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oTransform. 
*	@return Atributo oTransform
*/
XODE_transform 		XODE_geom::getOTransform() 
{ 
	return this-> oTransform; 
}

/**
*	@brief 	Metodo que obtiene el atributo oBox. 
*	@return Atributo oBox
*/
XODE_box 			XODE_geom::getOBox()	
{ 
	return this-> oBox;	
}

/**
*	@brief 	Metodo que obtiene el atributo oCappedCylinder. 
*	@return Atributo oCappedCylinder
*/
XODE_cappedCylinder 	XODE_geom::getOCappedCylinder()	
{ 
	return this-> oCappedCylinder;
}

/**
*	@brief 	Metodo que obtiene el atributo oCone. 
*	@return Atributo oCone
*/
XODE_cone 			XODE_geom::getOCone()	
{ 
	return this-> oCone;	
}

/**
*	@brief 	Metodo que obtiene el atributo oCylinder. 
*	@return Atributo oCylinder
*/
XODE_cylinder 		XODE_geom::getOCylinder()	
{ 
	return this-> oCylinder;	
}

/**
*	@brief 	Metodo que obtiene el atributo oPlane. 
*	@return Atributo oPlane
*/
XODE_plane 			XODE_geom::getOPlane()	
{ 
	return this-> oPlane;	
}

/**
*	@brief 	Metodo que obtiene el atributo oRay. 
*	@return Atributo oRay
*/
XODE_ray 			XODE_geom::getORay()	
{ 
	return this-> oRay;	
}

/**
*	@brief 	Metodo que obtiene el atributo oSphere. 
*	@return Atributo oSphere
*/
XODE_sphere 			XODE_geom::getOSphere()	
{ 
	return this-> oSphere;	
}

/**
*	@brief 	Metodo que obtiene el atributo oTrimesh. 
*	@return Atributo oTrimesh
*/
XODE_trimesh 		XODE_geom::getOTrimesh()	
{ 
	return this-> oTrimesh;	
}

/**
*	@brief 	Metodo que obtiene el atributo name. 
*	@return Atributo name
*/
std::string 	XODE_geom::getName()	
{ 
	return this-> name;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  			XODE_geom::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadGeom
*	@param	geomXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_geom::loadGeom(TiXmlElement *geomXML)
{
	TiXmlElement *transform = geomXML->FirstChildElement("transform");	
	TiXmlElement *box = geomXML->FirstChildElement("box");
	TiXmlElement *cappedCylinder = geomXML->FirstChildElement("cappedCylinder");
	TiXmlElement *cone = geomXML->FirstChildElement("cone");
	TiXmlElement *cylinder = geomXML->FirstChildElement("cylinder");
	TiXmlElement *plane = geomXML->FirstChildElement("plane");
	TiXmlElement *ray = geomXML->FirstChildElement("ray");
	TiXmlElement *sphere = geomXML->FirstChildElement("sphere");
	TiXmlElement *trimesh = geomXML->FirstChildElement("trimesh");

	if(geomXML)
	{
	if(transform) oTransform.loadTransform(transform);
	if(box) oBox.loadBox(box);
	if(cappedCylinder) oCappedCylinder.loadCappedCylinder(cappedCylinder);
	if(cone) oCone.loadCone(cone);
	if(cylinder) oCylinder.loadCylinder(cylinder);
	if(plane) oPlane.loadPlane(plane);
	if(ray) oRay.loadRay(ray);
	if(sphere) oSphere.loadSphere(sphere);
	if(trimesh) oTrimesh.loadTrimesh(trimesh);
	if(geomXML->Attribute("name")) name = geomXML->Attribute("name");
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_torque
*/
XODE_torque::XODE_torque()	 
{ 
	x=y=z=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_torque::getX() 
{ 
	return this->x;	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_torque::getY() 
{ 
	return this->y;	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_torque::getZ() 
{ 
	return this->z;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_torque::isEmpty()
{ 
	return this->empty;
}
	
/**
*	@brief 	Metodo loadToque
*	@param	torqueXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_torque::loadToque(TiXmlElement *torqueXML)
{
	if(torqueXML)
	{	
	if(torqueXML->Attribute("x")) x = atof (torqueXML->Attribute("x"));
	if(torqueXML->Attribute("y")) y = atof (torqueXML->Attribute("y"));
	if(torqueXML->Attribute("z")) z = atof (torqueXML->Attribute("z"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_force
*/
XODE_force::XODE_force()	 	
{ 
	x=y=z=0;
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_force::getX() 	
{ 
	return this->x;	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_force::getY() 	
{ 
	return this->y;	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_force::getZ() 	
{ 
	return this->z;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_force::isEmpty() 
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadForce
*	@param	forceXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_force::loadForce(TiXmlElement *forceXML)
{
	if(forceXML)
	{
	if(forceXML->Attribute("x")) x = atof (forceXML->Attribute("x"));
	if(forceXML->Attribute("y")) y = atof (forceXML->Attribute("y"));
	if(forceXML->Attribute("z")) z = atof (forceXML->Attribute("z"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_finiteRotation
*/	
XODE_finiteRotation::XODE_finiteRotation()	
{
	mode=" ";
	xaxis=yaxis=zaxis=0;
	empty=true;
}

/**
*	@brief 	Metodo que obtiene el atributo mode. 
*	@return Atributo mode
*/
std::string XODE_finiteRotation::getMode()	
{
	return this->mode;	
}

/**
*	@brief 	Metodo que obtiene el atributo xaxis. 
*	@return Atributo xaxis
*/
float 	XODE_finiteRotation::getXaxis()	
{
	return this->xaxis;
}

/**
*	@brief 	Metodo que obtiene el atributo yaxis. 
*	@return Atributo yaxis
*/
float 	XODE_finiteRotation::getYaxis()	
{
	return this->yaxis;
}

/**
*	@brief 	Metodo que obtiene el atributo zaxis. 
*	@return Atributo zaxis
*/
float 	XODE_finiteRotation::getZaxis()	
{
	return this->zaxis;
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_finiteRotation::isEmpty()	
{ 
	return this->empty;
}

/**
*	@brief 	Metodo loadFiniteRotation
*	@param	finiteRotationXML : Representa la etiqueta XML con los atributos que busco.
*/
void	XODE_finiteRotation::loadFiniteRotation(TiXmlElement *finiteRotationXML)
{
	if(finiteRotationXML)
	{
	if(finiteRotationXML->Attribute("xaxis")) xaxis = atof (finiteRotationXML->Attribute("xaxis"));
	if(finiteRotationXML->Attribute("yaxis")) yaxis = atof (finiteRotationXML->Attribute("yaxis"));
	if(finiteRotationXML->Attribute("zaxis")) zaxis = atof (finiteRotationXML->Attribute("zaxis"));
	if(finiteRotationXML->Attribute("mode")) mode  = finiteRotationXML->Attribute("mode");
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_linearVel
*/
XODE_linearVel::XODE_linearVel()	 
{ 
	x=y=z=0;
	empty=true;
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_linearVel::getX() 
{ 
	return this->x;	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_linearVel::getY() 
{ 
	return this->y;	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_linearVel::getZ() 
{ 
	return this->z;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_linearVel::isEmpty()
{ 
	return this->empty;
}
	
/**
*	@brief 	Metodo loadLinearVel
*	@param	linearVelXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_linearVel::loadLinearVel(TiXmlElement *linearVelXML)
{
	if(linearVelXML)
	{
	if(linearVelXML->Attribute("x")) x = atof (linearVelXML->Attribute("x"));
	if(linearVelXML->Attribute("y")) y = atof (linearVelXML->Attribute("y"));
	if(linearVelXML->Attribute("z")) z = atof (linearVelXML->Attribute("z"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_angularVel
*/
XODE_angularVel::XODE_angularVel() 
{ 
	x=y=z=0; 
	empty=true;
}

/**
*	@brief 	Metodo que obtiene el atributo x. 
*	@return Atributo x
*/
float XODE_angularVel::getX() 
{ 
	return this->x;	
}

/**
*	@brief 	Metodo que obtiene el atributo y. 
*	@return Atributo y
*/
float XODE_angularVel::getY() 
{ 
	return this->y;	
}

/**
*	@brief 	Metodo que obtiene el atributo z. 
*	@return Atributo z
*/
float XODE_angularVel::getZ() 
{ 
	return this->z;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  XODE_angularVel::isEmpty()
{ 
	return this->empty;
}
	
/**
*	@brief 	Metodo loadAngularVel
*	@param	linearVelXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_angularVel::loadAngularVel(TiXmlElement *AngularVelXML)
{
	if(AngularVelXML)
	{
	if(AngularVelXML->Attribute("x")) x = atof (AngularVelXML->Attribute("x"));
	if(AngularVelXML->Attribute("y")) y = atof (AngularVelXML->Attribute("y"));
	if(AngularVelXML->Attribute("z")) z = atof (AngularVelXML->Attribute("z"));
	empty = false;
	}
}

/**
*	@brief 	Constructor XODE_simpleBody
*/
XODE_simpleBody::XODE_simpleBody()	
{ 
	enabled=false;
	gravitymode=name=" ";
	empty=true;
}

/**
*	@brief 	Metodo que obtiene el atributo oTransform. 
*	@return Atributo oTransform
*/
XODE_transform 	XODE_simpleBody::getOTransform()	
{ 
	return this->oTransform;	
}

/**
*	@brief 	Metodo que obtiene el atributo oTorque. 
*	@return Atributo oTorque
*/
XODE_torque 	XODE_simpleBody::getOTorque()	
{ 
	return this->oTorque;	
}

/**
*	@brief 	Metodo que obtiene el atributo oForce. 
*	@return Atributo oForce
*/
XODE_force 	XODE_simpleBody::getOForce()	
{ 
	return this->oForce;	
}

/**
*	@brief 	Metodo que obtiene el atributo oFiniteTotation. 
*	@return Atributo oFiniteTotation
*/
XODE_finiteRotation 	XODE_simpleBody::getOFiniteTotation()	
{ 
	return this->oFiniteTotation;	
}

/**
*	@brief 	Metodo que obtiene el atributo oLinealVel. 
*	@return Atributo oLinealVel
*/
XODE_linearVel 	XODE_simpleBody::getOLinealVel()	
{ 
	return this->oLinealVel;	
}

/**
*	@brief 	Metodo que obtiene el atributo oAngularVel. 
*	@return Atributo oAngularVel
*/
XODE_angularVel 	XODE_simpleBody::getOAngularVel()	
{ 
	return this->oAngularVel;	
}

/**
*	@brief 	Metodo que obtiene el atributo oMass. 
*	@return Atributo oMass
*/
XODE_mass 	XODE_simpleBody::getOMass()	
{ 
	return this->oMass;	
}

/**
*	@brief 	Metodo que obtiene el atributo oGeom. 
*	@return Atributo oGeom
*/
XODE_geom   XODE_simpleBody::getOGeom()
{ 
	return this->oGeom;	
}

/**
*	@brief 	Metodo que obtiene el atributo enabled. 
*	@return Atributo enabled
*/
bool 	XODE_simpleBody::getEnabled()	
{ 
	return this->enabled;	
}

/**
*	@brief 	Metodo que obtiene el atributo gravitymode. 
*	@return Atributo gravitymode
*/
std::string 	XODE_simpleBody::getFravitymode()	
{ 
	return this->gravitymode;	
}

/**
*	@brief 	Metodo que obtiene el atributo name. 
*	@return Atributo name
*/
std::string	XODE_simpleBody::getName()	
{ 
	return this->name;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_simpleBody::isEmpty()	
{ 
	return this->empty;	
}
	

/**
*	@brief 	Metodo loadSimpleBody
*	@param	simpleBody : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_simpleBody::loadSimpleBody(TiXmlElement *simpleBody)
{
	//std::cout << "<simpleBody>"<< std::endl;
	if(simpleBody)
	{
	if(simpleBody->FirstChildElement("transform"))
	{
	oTransform.loadTransform(simpleBody->FirstChildElement("transform"));
	}
	if(simpleBody->FirstChildElement("torque"))
		oTorque.loadToque(simpleBody->FirstChildElement("torque"));
	if(simpleBody->FirstChildElement("force"))
		oForce.loadForce(simpleBody->FirstChildElement("force"));
	if(simpleBody->FirstChildElement("finiteRotation"))
		oFiniteTotation.loadFiniteRotation(simpleBody->FirstChildElement("finiteRotation"));
	if(simpleBody->FirstChildElement("linearVel"))
		oLinealVel.loadLinearVel(simpleBody->FirstChildElement("linearVel"));
	if(simpleBody->FirstChildElement("angularVel"))
		oAngularVel.loadAngularVel(simpleBody->FirstChildElement("angularVel"));
	if(simpleBody->FirstChildElement("mass"))
		oMass.loadMass(simpleBody->FirstChildElement("mass"));
	if(simpleBody->FirstChildElement("geom"))
		oGeom.loadGeom(simpleBody->FirstChildElement("geom"));

	if(simpleBody->Attribute("enabled") == "TRUE") enabled = true;
	else enabled = false;

	if(simpleBody->Attribute("gravitymode"))
	gravitymode = simpleBody->Attribute("gravitymode");
	if(simpleBody->Attribute("name"))
	name = simpleBody->Attribute("name");	
	empty = false;
	}
	//std::cout << "</simpleBody>"<< std::endl;
}

/**
*	@brief 	Constructor XODE_body
*/
XODE_body::XODE_body()	
{ 
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oSimpleBody. 
*	@return Atributo oSimpleBody
*/
XODE_simpleBody  	 	XODE_body::getoSimpleBody()	
{ 
	return this->oSimpleBody;	
}

/**
*	@brief 	Metodo que obtiene el atributo oJoint. 
*	@return Atributo oJoint
*/
list<XODE_joint> 	XODE_body::getOJoint()	
{ 
	return this->oJoint;	
}

/**
*	@brief 	Metodo que obtiene el atributo oGeom. 
*	@return Atributo oGeom
*/
list<XODE_geom> 	XODE_body::getOGeom()	
{ 
	return this->oGeom;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_body::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*	@brief 	Metodo loadBody
*	@param	bodyXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_body::loadBody(TiXmlElement *bodyXML)
{
	//std::cout << "<body>"<< std::endl;
	if(bodyXML)
	{
	oSimpleBody.loadSimpleBody(bodyXML);
	
	

	XODE_joint oOJoint;
	TiXmlElement *oJointXML = bodyXML->FirstChildElement ("joint");
	while(oJointXML)
	{
	oOJoint.loadJoint(oJointXML);
	oJoint.push_back(oOJoint);
	oJointXML = oJointXML-> NextSiblingElement("joint");
	}

	XODE_geom oOGeom;
	TiXmlElement *oGeomXML = bodyXML->FirstChildElement ("geom");
	while(oGeomXML)
	{
	oOGeom.loadGeom(oGeomXML);
	oGeom.push_back(oOGeom);
	oGeomXML = oGeomXML-> NextSiblingElement("geom");
	}
	empty = false;
	}
	//std::cout << "</body>"<< std::endl;
}

/**
*	@brief 	Constructor XODE_jointGroup
*/
XODE_jointGroup::XODE_jointGroup()	
{ 
	empty = true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oBody. 
*	@return Atributo oBody
*/
list<XODE_body> 	XODE_jointGroup::getOBody()	
{ 
	return this->oBody;	
}

/**
*	@brief 	Metodo que obtiene el atributo oJoint. 
*	@return Atributo oJoint
*/
list<XODE_joint> 	XODE_jointGroup::getOJoint()	
{ 
	return this->oJoint;	
}

/**
*	@brief 	Metodo que obtiene el atributo oGeom. 
*	@return Atributo oGeom
*/
list<XODE_geom> 	XODE_jointGroup::getOGeom()	
{ 
	return this->oGeom;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_jointGroup::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo loadJoinGroup
*	@param	jointgroupXML : Representa la etiqueta XML con los atributos que busco.
*/
void XODE_jointGroup::loadJoinGroup(TiXmlElement *jointgroupXML)
{
	//std::cout << "<jointGroup>"<< std::endl;
	XODE_body oOBody;
	TiXmlElement *oBodyXML = jointgroupXML->FirstChildElement ("body");
	while(oBodyXML)
	{
	oOBody.loadBody(oBodyXML);
	oBody.push_back(oOBody);
	oBodyXML = oBodyXML-> NextSiblingElement("body");
	}

	XODE_joint oOJoint;
	TiXmlElement *oJointXML = jointgroupXML->FirstChildElement ("joint");
	while(oJointXML)
	{
	oOJoint.loadJoint(oJointXML);
	oJoint.push_back(oOJoint);
	oJointXML = oJointXML-> NextSiblingElement("joint");
	}

	XODE_geom oOGeom;
	TiXmlElement *oGeomXML = jointgroupXML->FirstChildElement ("geom");
	while(oGeomXML)
	{
	oOGeom.loadGeom(oGeomXML);
	oGeom.push_back(oOGeom);
	oGeomXML = oGeomXML-> NextSiblingElement("geom");
	}
	empty = false;
	//std::cout << "</jointgroup>"<< std::endl;
}


/**
*	@brief 	Constructor XODE_space
*/
XODE_space::XODE_space()	
{
	empty = true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oTransform. 
*	@return Atributo oTransform
*/
XODE_transform 	XODE_space::getOTransform()	
{ 
	return this->oTransform; 	
}

/**
*	@brief 	Metodo que obtiene el atributo oGeom. 
*	@return Atributo oGeom
*/
list<XODE_geom> 	XODE_space::getOGeom()	
{ 
	return this->oGeom;	
}

/**
*	@brief 	Metodo que obtiene el atributo oBody. 
*	@return Atributo oBody
*/
list<XODE_body> 	XODE_space::getOBody()	
{ 
	return this->oBody;	
}

/**
*	@brief 	Metodo que obtiene el atributo oJointGroup. 
*	@return Atributo oJointGroup
*/
list<XODE_jointGroup> 	XODE_space::getOJointGroup()
{ 
	return this->oJointGroup; 
}

/**
*	@brief 	Metodo que obtiene el atributo oJoint. 
*	@return Atributo oJoint
*/
list<XODE_joint> 	XODE_space::getOJoint()	
{ 
	return this->oJoint;	
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_space::isEmpty()	
{ 
	return this->empty;	
}
	
/**
*
*	@brief 	Funcion loadSpace
*	@param 	TiXmlElement *spaceXML Representa el mundo que se sedea cargar. 
*	Es un XML con la etiqueta "<space>" completa.
*	@return	retorna el obeto oSpace con los datos argados desde el 
*	XML
*
*/
void XODE_space::loadSpace(TiXmlElement *spaceXML)
{
	//std::cout << "<space>"<< std::endl;
	if(spaceXML)
	{
	TiXmlHandle docHandle( spaceXML );
	TiXmlElement* transform = docHandle.FirstChild( "transform" ).ToElement();
	
	if(transform)
	{
	oTransform.loadTransform(spaceXML->FirstChildElement("transform"));
	
	}	
	
	XODE_body oOBody;
	TiXmlElement *oBodyXML = spaceXML->FirstChildElement ("body");
	while(oBodyXML)
	{
	oOBody.loadBody(oBodyXML);
	oBody.push_back(oOBody);
	//std::cout << std::endl << "Element: "<< oBodyXML->Attribute("name") << std::endl;
	oBodyXML = oBodyXML-> NextSiblingElement("body");
	}

	XODE_joint oOJoint;
	TiXmlElement *oJointXML = spaceXML->FirstChildElement ("joint");
	while(oJointXML)
	{
	oOJoint.loadJoint(oJointXML);
	oJoint.push_back(oOJoint);
	oJointXML = oJointXML-> NextSiblingElement("joint");
	}

	XODE_geom oOGeom;
	TiXmlElement *oGeomXML = spaceXML->FirstChildElement ("geom");
	while(oGeomXML)
	{
	oOGeom.loadGeom(oGeomXML);
	oGeom.push_back(oOGeom);
	oGeomXML = oGeomXML-> NextSiblingElement("geom");
	}

	XODE_jointGroup oOJointGroup;
	TiXmlElement *oJointGroupXML = spaceXML->FirstChildElement ("jointgroup");
	while(oJointGroupXML)
	{
	oOJointGroup.loadJoinGroup(oJointGroupXML);
	oJointGroup.push_back(oOJointGroup);
	oJointGroupXML = oJointGroupXML-> NextSiblingElement("jointgroup");
	}
	empty = false;
	}
	//std::cout << "</space>"<< std::endl;

}

/**
*	@brief 	Constructor XODE_world
*/
XODE_world::XODE_world()	
{ 
	empty=true;	
}

/**
*	@brief 	Metodo que obtiene el atributo oTransform. 
*	@return Atributo oTransform
*/
XODE_transform 	XODE_world::getoTransform()	
{ 
	return this->oTransform;
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_world::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo que obtiene el atributo oSpace. 
*	@return Atributo oSpace
*/
list<XODE_space> XODE_world::getoSpace()	
{ 
	return this->oSpace; 	
}


/**
*	@brief 	Metodo que obtiene el atributo oExt. 
*	@return Atributo oExt
*/
XODE_ext XODE_world::getoExt()
{	
	return this->oExt;
}


/**
*
*	@brief 	Funcion loadWorld
*	@param 	TiXmlElement *worldXML Representa el mundo que se sedea cargar. 
*	Es un XML con la etiqueta "<world>"" completa.
*	@return	retorna el obeto oWorld con los datos argados desde el 
*	XML
*
*/
XODE_world XODE_world::loadWorld(TiXmlElement *worldXML)
{
	//std::cout << "<world>"<< std::endl;
	TiXmlHandle docHandle( worldXML );
	TiXmlElement* transform = docHandle.FirstChild( "transform" ).ToElement();
	
	if(transform)
	{	
		
		std::cout << "... "<< std::endl;	
		oTransform.loadTransform(worldXML->FirstChildElement("transform"));
		
	}

	XODE_space oOSpace;
	TiXmlElement *oSpaceXML = worldXML->FirstChildElement ("space");
	
	while(oSpaceXML)
	{
		
		oOSpace.loadSpace(oSpaceXML);
		oSpace.push_back(oOSpace);
		oSpaceXML = oSpaceXML-> NextSiblingElement("space");
		
	}

	
	TiXmlElement *oExtXML = docHandle.FirstChild( "ext" ).ToElement();
	
	if(oExtXML)
	{
		oExt.loadExt(oExtXML);
	}
	
	
	empty = false;
	//std::cout << "</world>"<< std::endl;
	XODE_world oWorld = *this;
	return oWorld;
}

/**
*	@brief 	Constructor XODE_xode
*/
XODE_xode::XODE_xode()	
{ 
	name=version=" "; 
	empty=true;	
}


/**
*	@brief 	Metodo que obtiene el atributo name. 
*	@return Atributo name
*/
std::string XODE_xode::getName()	
{ 
	return this->name; 	
}

/**
*	@brief 	Metodo que obtiene el atributo version. 
*	@return Atributo version
*/
std::string XODE_xode::getVersion()	
{ 
	return this->version; 
}

/**
*	@brief 	Metodo isEmpty. Retorna si el elemento es vacio.
*	@return	empty TRUE or FALSE
*/
bool  	XODE_xode::isEmpty()	
{ 
	return this->empty;	
}

/**
*	@brief 	Metodo que obtiene el atributo oWorld. 
*	@return Atributo oWorld
*/
XODE_world XODE_xode::getoWorld()	{ return this->oWorld; 	}


/**
*
*	@brief 	Funcion loadXODE
*	@param 	char *file, el nombre del archivo XODE cargado desde linea de comandos.	
*	@return	Retorna el XML que fue cargado.
*
*/
TiXmlDocument XODE_xode::loadXODE(char *file)
{
	//std::cout << "<xode>"<< std::endl;
	TiXmlDocument XMLdoc(file);
	XODE_world oWorld;
	TiXmlElement *worldXML;     

	if(!XMLdoc.LoadFile())    //Manejo de error en la carga del archivo
	{             
	  std::cout << "Error: No es posible cargar el archivo: " << file << std::endl;
	  return NULL;
	}
	//Carga de los elementos de cabecera.
    this->name=XMLdoc.FirstChildElement( "xode" )->Attribute("name"); 
    this->version=XMLdoc.FirstChildElement( "xode" )->Attribute("version"); 
    
    //Creacion y carga de world.
    worldXML = XMLdoc.FirstChildElement( "world" );
   	if(worldXML) this->oWorld = oWorld.loadWorld(worldXML);

   	empty = false;
   	//std::cout << "</xode>"<< std::endl;
    return XMLdoc;

}
