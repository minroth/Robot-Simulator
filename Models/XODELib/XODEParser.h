
#ifndef _XODEPHRASER_H 
#define _XODEPHRASER_H

#include <iostream>
#include <list>
#include <tinyxml.h>


using namespace std;
 

 class XODE_gravity {
 private:
 	float x;
 	float y;
 	float z;
 	bool empty;
 public:
 	XODE_gravity();
	bool isEmpty();
	float getX();
	float getY();
	float getZ();
	void loadGravity(TiXmlElement *gravityXML);	
 };
/**
@brief 	Clase XODE_ext
	
		<xs:element name="ext">
			<xs:complexType>
			<xs:choice>
			<xs:element name="location"/>
			<xs:element name="colour">
			<xs:complexType>...</xs:complexType>
			</xs:element>
			</xs:choice>
			<xs:attribute name="ename"/>
			</xs:complexType>
		</xs:element>

@note 	Se agregan etiquetas gravity y CFM, y se quitan las etiquetas
		location y colour ya que no se usaran en este momento.
*/
class XODE_ext {
private:
	XODE_gravity gravity;
	float CFM;
	std::string ename;
	bool empty;
public:
	XODE_ext();
	bool isEmpty();
	XODE_gravity getGravity();
	float getCFM();
	std::string getEname();
	void loadExt(TiXmlElement *extXML);
};
 
/**
@brief Clase XODE_euler
	
	<xs:element name="euler">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	<xs:attribute name="aformat" type="aformattype"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_euler {
private:
    float x;
    float y;
    float z;
    /** Puede contener radians o degrees*/
    std::string aformat;
    bool empty;
public:
	XODE_euler();
	bool  isEmpty();
	float getX();
	float getY();
	float getZ();
	std::string getAformat();
	void loadEuler(TiXmlElement *eulerXML);
	

};
/**
@brief Clase XODE_axisangle

<xs:element name="axisangle">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	<xs:attribute name="angle"/>
	<xs:attribute name="aformat" type="aformattype"/>
	</xs:complexType>
	</xs:element>	
*/
class XODE_axisangle {
private:
    float x;
    float y;
    float z;
    float angle;
    /** Puede contener radians o degrees*/
    std::string aformat;
    bool empty;
public:
	XODE_axisangle();
	bool  isEmpty();
	float getX();
	float getY();
	float getZ();
	float getAngle();
	std::string getAformat();
	void loadAxisangle(TiXmlElement *axisangleXML);
};


/**
@brief Clase XODE_quaternion 

<xs:element name="quaternion">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	<xs:attribute name="w"/>
	<xs:attribute name="aformat" type="aformattype"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_quaternion {
public:
    float x;
    float y;
    float z;
    /** Puede contener radians o degrees*/
    std::string aformat;
    bool empty;

 public:
 	XODE_quaternion ();
 	bool  isEmpty();
 	float getX();
	float getY();
	float getZ();
	std::string getAformat();
	void loadQuaternion(TiXmlElement *quaternionXML);

};
/**
@brief Clase XODE_rotation

<xs:element name="rotation">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="euler" minOccurs="0" maxOccurs="1"/>
	<xs:element ref="axisangle" minOccurs="0" maxOccurs="1"/>
	<xs:element ref="quaternion" minOccurs="0" maxOccurs="1"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>	

*/
class XODE_rotation {
private:
	XODE_euler oEuler;
	XODE_axisangle oAxisangle;
	XODE_quaternion oQuaternion;
	bool empty;
public:
	XODE_euler getOEuler();
	bool  isEmpty();
	XODE_axisangle getOAxisangle();
	XODE_quaternion getOQuaternion();
	void loadRotation(TiXmlElement *rotationXML);

};
/**
@brief Clase XODE_position
	Representacion de un objto en un plano cartesiano.
	<xs:element name="position">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>

*/
class XODE_position {
private:
    float x;
    float y;
    float z;
    bool empty;
public:
	XODE_position();
	float getX();
	float getY();
	float getZ();
	bool  isEmpty();
	void loadPosition(TiXmlElement *positionXML);
};
/**
@brief Clase XODE_matrix4f
<xs:element name="matrix4f">
<xs:complexType>
	<xs:attribute name="m00"/>
	<xs:attribute name="m01"/>
	<xs:attribute name="m02"/>
	<xs:attribute name="m03"/>
	<xs:attribute name="m10"/>
	<xs:attribute name="m11"/>
	<xs:attribute name="m12"/>
	<xs:attribute name="m13"/>
	<xs:attribute name="m20"/>
	<xs:attribute name="m21"/>
	<xs:attribute name="m22"/>
	<xs:attribute name="m23"/>
	<xs:attribute name="m30"/>
	<xs:attribute name="m31"/>
	<xs:attribute name="m32"/>
	<xs:attribute name="m33"/>
</xs:complexType>
</xs:element>
*/

class XODE_matrix4f{
private:
	float m00;
	float m01;
	float m02;
	float m03;
	float m10;
	float m11;
	float m12;
	float m13;
	float m20;
	float m21;
	float m22;
	float m23;
	float m30;
	float m31;
	float m32;
	float m33;
	bool empty;
public:
	XODE_matrix4f();
	float getM00();
	float getM01();
	float getM02();
	float getM03();
	float getM10();
	float getM11();
	float getM12();
	float getM13();
	float getM20();
	float getM21();
	float getM22();
	float getM23();
	float getM30();
	float getM31();
	float getM32();
	float getM33();
	bool  isEmpty();
	void loadMatrix4f(TiXmlElement *matrix4fXML);

};
/**
@brief Clase XODE_transform
<xs:element name="transform">
	<xs:complexType>
	<xs:choice>
	<xs:sequence>
	<xs:element name="matrix4f">
	<xs:complexType>
	<xs:attribute name="m00"/>
	<xs:attribute name="m01"/>
	<xs:attribute name="m02"/>
	<xs:attribute name="m03"/>
	<xs:attribute name="m10"/>
	<xs:attribute name="m11"/>
	<xs:attribute name="m12"/>
	<xs:attribute name="m13"/>
	<xs:attribute name="m20"/>
	<xs:attribute name="m21"/>
	<xs:attribute name="m22"/>
	<xs:attribute name="m23"/>
	<xs:attribute name="m30"/>
	<xs:attribute name="m31"/>
	<xs:attribute name="m32"/>
	<xs:attribute name="m33"/>
	</xs:complexType>
	</xs:element>	
	</xs:sequence>
	<xs:sequence>
	<xs:element ref="position" minOccurs="0"/>
	<xs:element ref="rotation" minOccurs="0"/>
	
	</xs:sequence>
	</xs:choice>
	<xs:attribute name="absolute" type="xs:boolean" use="optional" default="true"/>
	<xs:attribute name="transformstyle" type="transformstyletype"/>
	<xs:attribute name="scale"/>
	</xs:complexType>
</xs:element>

*/
class XODE_transform {
private:
    XODE_matrix4f oMatrix4f;
    XODE_position oPosition;
    XODE_rotation oRotation;
    std::string absolute;
    std::string transformstyle;
    std::string scale;
    bool empty;
    
public:
	XODE_transform();
	XODE_matrix4f getOMatrix4f();
	XODE_position getOPosition();
	XODE_rotation getORotation();
	std::string getAbsolute();
	std::string getTransformstyle();
	std::string getScale();
	bool  isEmpty();
	void loadTransform(TiXmlElement *transformXML);
};
/**
@brief Clase XODE_cylinder
	Representacion de un cilindro
	<xs:element name="cylinder">
	<xs:complexType>
	<xs:attribute name="radius"/>
	<xs:attribute name="length"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_cylinder {
private:
    float radius;
    float length;
    bool empty;
public:
	XODE_cylinder();
    float getRadius();
    float getLength();
    bool  isEmpty();
    void  loadCylinder(TiXmlElement *cylinderXML); 
};
/**
@brief Clase XODE_cappedCylinder
	Representacion de un Cilindro con tapa
	<xs:element name="cappedCylinder">
	<xs:complexType>
	<xs:attribute name="radius"/>
	<xs:attribute name="length"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_cappedCylinder {
private:
    float radius;
    float length;
    bool empty;
public:
	XODE_cappedCylinder();
    float getRadius();
    float getLength();
    bool  isEmpty();
    void  loadCappedCylinder(TiXmlElement *cappedCylinderXML); 
};
/**
@brief Clase XODE_cone
	Representacion de un cono
	<xs:element name="cone">
	<xs:complexType>
	<xs:attribute name="radius"/>
	<xs:attribute name="length"/>
	</xs:complexType>
	</xs:element>

*/
class XODE_cone {
private:
    float radius;
    float length;
    bool empty;
public:
	XODE_cone();
	bool  isEmpty();
	float getRadius();
	float getLength();
	void loadCone(TiXmlElement *coneXML); 	
};
/**
@brief Clase XODE_plane
	Representacion de un Plano
	<xs:element name="plane">
	<xs:complexType>
	<xs:attribute name="a"/>
	<xs:attribute name="b"/>
	<xs:attribute name="c"/>
	<xs:attribute name="d"/>
	</xs:complexType>
	</xs:element>

*/
class XODE_plane {
private:
    float a;
    float b;
    float c;
    float d;
    bool empty;
public:
	XODE_plane();
	bool  isEmpty();
	float getA();
	float getB();
	float getC();
	float getD();
	void loadPlane(TiXmlElement *planeXML); 	
};
/**
@brief Clase XODE_ray
	Representacion de un Rayo
	<xs:element name="ray">
	<xs:complexType>
	<xs:attribute name="length"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_ray {
private:
    float length; 
    bool empty;   
public:
	XODE_ray();
	float getLength();
	bool  isEmpty();
	void loadRay(TiXmlElement *rayXML); 	
};
/**
@brief Clase v
<xs:element name="v" maxOccurs="unbounded">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	<xs:attribute name="e"/>
	</xs:complexType>
</xs:element>
*/
class XODE_v{
private:
    float x;
    float y;
    float z;
    float e;  
    bool empty;  
public:
	XODE_v();
	float getX();
	float getY();
	float getZ();
	float getE();
	bool  isEmpty();
	void loadV(TiXmlElement *vXML); 
};
/**
@brief Clase XODE_vertices
	Representacion de un conjunto de vertices
	<xs:element name="vertices">
	<xs:complexType>
	<xs:sequence>
	<xs:element name="v" maxOccurs="unbounded">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	<xs:attribute name="e"/>
	</xs:complexType>
	</xs:element>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
*/

class XODE_vertices {
private:
	list<XODE_v> oV;
public:
    list<XODE_v> getListOV();
    XODE_v	getElementByPosition(int position);
    int	getSize();
    bool 	empty();
    void 	loadVertices(TiXmlElement *verticesXML);
};
/**
@brief Clase XODE_t
<xs:element name="t" maxOccurs="unbounded">
	<xs:complexType>
	<xs:attribute name="ia"/>
	<xs:attribute name="ib"/>
	<xs:attribute name="ic"/>
	</xs:complexType>
</xs:element>
*/
class XODE_t{
private:
	float ia;
    float ib;
    float ic;
    bool empty;
public:
	XODE_t();
	bool  isEmpty();
	float getIa();
	float getIb();
	float getIc();
	void  loadT(TiXmlElement *tXML); 

};
/**
@brief Clase XODE_triangles
	Representacion de un vertice
	<xs:element name="triangles">
	<xs:complexType>
	<xs:sequence>
	<xs:element name="t" maxOccurs="unbounded">
	<xs:complexType>
	<xs:attribute name="ia"/>
	<xs:attribute name="ib"/>
	<xs:attribute name="ic"/>
	</xs:complexType>
	</xs:element>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
*/

class XODE_triangles {
private:
	list<XODE_t> oT;
public:
    list<XODE_t> getList();
    int	getSize();
    bool 	empty();
    XODE_t	getElementByPosition(int position);
    void 	loadTriangles(TiXmlElement *trianglesXML);
};

/**
@brief Clase XODE_trimesh
	Representacion de un maya
	<xs:element name="trimesh">
	<xs:complexType>
	<xs:sequence>
	<xs:element name="vertices">
	<xs:complexType>
	<xs:sequence>
	<xs:element name="v" maxOccurs="unbounded">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	<xs:attribute name="e"/>
	</xs:complexType>
	</xs:element>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="triangles">
	<xs:complexType>
	<xs:sequence>
	<xs:element name="t" maxOccurs="unbounded">
	<xs:complexType>
	<xs:attribute name="ia"/>
	<xs:attribute name="ib"/>
	<xs:attribute name="ic"/>
	</xs:complexType>
	</xs:element>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	</xs:sequence>
	</xs:complexType>
	</xs:element>

*/
class XODE_trimesh {
private:
	XODE_vertices  oVertices;
	XODE_triangles oTriangles;
	bool empty;
public:
	XODE_vertices  getOVertices();
	XODE_triangles getOTriangles();
	bool  isEmpty();
	void  loadTrimesh(TiXmlElement *trimeshXML); 
};
/**
@brief Clase XODE_anchor
	Representacion de un ancla
	<xs:element name="anchor">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>	

*/

class XODE_anchor {
private:
    float x;
    float y;
    float z;
    bool empty;
public:
	XODE_anchor();
	float getX();
    float getY();
    float getZ();
    bool  isEmpty();
    void  loadAnchor(TiXmlElement *anchorXML); 
};
/**
@brief Clase XODE_axis
	Representacion de un eje
	<xs:element name="axis">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	
	<xs:attribute name="LowStop"/>
	<xs:attribute name="HiStop"/>
	<xs:attribute name="Vel"/>
	<xs:attribute name="FMax"/>
	<xs:attribute name="FudgeFactor"/>
	<xs:attribute name="Bounce"/>
	<xs:attribute name="CFM"/>
	<xs:attribute name="StopERP"/>
	<xs:attribute name="StopCFM"/>
	<xs:attribute name="SuspensionERP"/>
	<xs:attribute name="SuspensionCFM"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_axis {
private:
    float x;
    float y;
    float z;
    float lowStop;
	float hiStop;
	float vel;
	float fMax;
	float fudgeFactor;
	float bounce;
	float CFM;
	float stopERP;
	float stopCFM;
	float suspensionERP;
	float suspensionCFM;
	bool empty;
public:
	XODE_axis();
	float getX();
	float getY();
	float getZ();
	float getLowStop();
	float getHiStop();
	float getVel();
	float getFMax();
	float getFudgeFactor();
	float getBounce();
	float getCFM();
	float getStopERP();
	float getStopCFM();
	float getSuspensionERP();
	float getSuspensionCFM();
	bool  isEmpty();
	void  loadAxis(TiXmlElement *axisXML); 
};
/**
@brief Clase XODE_centerGravity
	Representacion del centro de gravedad
	<xs:element name="centerGravity">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>

*/
class XODE_centerGravity {
private:
    float x;
    float y;
    float z;
    bool empty;
public:
	XODE_centerGravity();
	float getX();
    float getY();
    float getZ();
    bool  isEmpty();
    void  loadCenterGravity(TiXmlElement *centerGravityXML); 
};
/**
@brief Clase XODE_imatrix
	Representacion de una matriz
	<xs:element name="imatrix">
	<xs:complexType>
	<xs:attribute name="m00"/>
	<xs:attribute name="m01"/>
	<xs:attribute name="m02"/>
	<xs:attribute name="m10"/>
	<xs:attribute name="m11"/>
	<xs:attribute name="m12"/>
	<xs:attribute name="m20"/>
	<xs:attribute name="m21"/>
	<xs:attribute name="m22"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_imatrix {
private:
    float m00;
    float m01;
    float m02;
    float m10;
    float m11;
    float m12;
    float m20;
    float m21;
    float m22;
    bool empty;
public:
	XODE_imatrix();
	float getM00();
	float getM01();
	float getM02();
	float getM10();
	float getM11();
	float getM12();
	float getM20();
	float getM21();
	float getM22();
	bool  isEmpty();
	void loadImatrix(TiXmlElement *imatrixXML);
};

/**
@brief Clase XODE_mass_struct
	Representacion una estructura con masa.
	<xs:element name="mass_struct">
	<xs:complexType>
	<xs:sequence>
	<xs:element name="centerGravity">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>
	<xs:element name="imatrix">
	<xs:complexType>
	<xs:attribute name="m00"/>
	<xs:attribute name="m01"/>
	<xs:attribute name="m02"/>
	<xs:attribute name="m10"/>
	<xs:attribute name="m11"/>
	<xs:attribute name="m12"/>
	<xs:attribute name="m20"/>
	<xs:attribute name="m21"/>
	<xs:attribute name="m22"/>
	</xs:complexType>
	</xs:element>
	</xs:sequence>
	<xs:attribute name="density"/>
	<xs:attribute name="total"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_mass_struct {
private:
    XODE_centerGravity oCenterGravity;
    XODE_imatrix oImatrix;
    float density;
    float total;
    bool empty;
public:
	XODE_mass_struct();
	XODE_centerGravity 	getOCenterGravity();
	XODE_imatrix 	getOImatrix();
	float 	getDensity();
	float 	getTotal();
	bool  	isEmpty();
	void 	loadMass_struct(TiXmlElement *mass_structXML);
};
/**
@brief Clase box
	Representacion de una caja
	<xs:element name="box">
	<xs:complexType>
	<xs:attribute name="sizex"/>
	<xs:attribute name="sizey"/>
	<xs:attribute name="sizez"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_box {
private:
    float sizex;
    float sizey;
    float sizez;
    bool empty;
public:
	XODE_box();
	float getSizex();
    float getSizey();
    float getSizez();
    bool  isEmpty();
    void  loadBox(TiXmlElement *boxXML); 
};
/**
@brief Clase sphere
	Representacion de una esfera
	<xs:element name="sphere">
	<xs:complexType>
	<xs:attribute name="radius"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_sphere {
private:
    float radius;
    bool empty;
public:
	XODE_sphere();
	float getRadius();
	bool  isEmpty();
	void loadSphere(TiXmlElement *sphereXML); 
};

/**
@brief Clase XODE_mass_shape
	Representacion una estructura de masa
	<xs:element name="mass_shape">
	<xs:complexType>
	<xs:sequence>
	<xs:choice>
	<xs:element ref="cylinder"/>
	<xs:element ref="sphere"/>
	<xs:element ref="box"/>
	<xs:element ref="cappedCylinder"/>
	</xs:choice>
	</xs:sequence>
	<xs:attribute name="density"/>
	<xs:attribute name="total"/>
	</xs:complexType>
	</xs:element>
*/

class XODE_mass_shape {
private:
    XODE_cylinder oCylinder;
    XODE_sphere oSphere;
    XODE_box oBox;
    XODE_cappedCylinder oCappedCylinder;
    float density;
    float total;
    bool empty;

public:
	XODE_mass_shape ();
	XODE_cylinder 	getOCylinder();
	XODE_sphere 	getOSphere();
	XODE_box 	getBox();
	XODE_cappedCylinder 	getOCappedCylinder();
	float	getDensity();
	float	getTotal();
	bool  	isEmpty();
	void	loadMass_shape(TiXmlElement *mass_shapeXML);
};
/**
@brief Clase XODE_adjust
	
	<xs:element name="adjust">
	<xs:complexType>
	<xs:attribute name="density"/>
	<xs:attribute name="total"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_adjust {
private:
    float density;
    float total;
    bool empty;
public:
	XODE_adjust();
	float	getDensity();
	float	getTotal();
	bool  	isEmpty();
	void	loadAdjust(TiXmlElement *adjustXML);
};
/**
@brief Clase XODE_mass
	Represetacion de una forma con masa
	<xs:element name="mass">
	<xs:complexType>
	<xs:sequence>
	<xs:choice>
	<xs:element ref="mass_shape" minOccurs="0"/>
	<xs:element ref="mass_struct" minOccurs="0"/>
	</xs:choice>
	<xs:element ref="transform" minOccurs="0"/>
	<xs:element ref="adjust" minOccurs="0"/>
	<xs:element ref="mass" minOccurs="0"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
*/
class XODE_mass {
private:
	XODE_mass_shape oMass_shape;
	XODE_mass_struct oMass_struct;
	XODE_transform oTransform;
	XODE_adjust oAdjust;
	bool empty;
public:
	XODE_mass();
	XODE_mass_shape 	getOMass_shape();
	XODE_mass_struct 	getOMass_struct();
	XODE_transform 	getoTransform();
	XODE_adjust 	getOAdjust();
	bool  	isEmpty();
	void	loadMass(TiXmlElement *massXML);
};
/**
@brief Clase link1
<xs:element name="link1" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="body"/>
	</xs:complexType>
</xs:element>
*/
class XODE_link{
private:
	std::string body;
	bool empty;
public:
	XODE_link();
	std::string getBody();
	bool  isEmpty();
	void loadLink(TiXmlElement *linkXML);
};

/**
@brief	Clase XODE_amotor
<xs:element name="amotor">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="0" maxOccurs="3"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>
*/
class XODE_amotor
{
private:
	XODE_anchor oAnchor;
	XODE_axis oAxis;
	XODE_axis oAxis2;
	XODE_axis oAxis3;
	bool empty;
public:
	XODE_amotor();
	XODE_anchor 	getOAnchor();
	XODE_axis 	getOAxis();
	XODE_axis 	getOAxis2();
	XODE_axis 	getOAxis3();
	bool  	isEmpty();
	void 	loadAmotor(TiXmlElement *amotorXML);
};

/**
@brief Clase XODE_ball
<xs:element name="ball">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="0" maxOccurs="0"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>
*/
class XODE_ball
{
private:
	XODE_anchor oAnchor;
	XODE_axis oAxis;
	bool empty;
public:
	XODE_ball();
	XODE_anchor  getOAnchor();
	XODE_axis 	getOAxis();
	bool  	isEmpty();
	void	loadBall(TiXmlElement *ballXML);
};

/**
@brief Clase XODE_hinge
<xs:element name="hinge">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>
*/	
class XODE_hinge
{
private:
	XODE_anchor oAnchor;
	XODE_axis oAxis;	
	bool empty;
public:
	XODE_hinge();
	XODE_anchor 	getOAnchor();
	XODE_axis 	getOAxis();
	bool  	isEmpty();
	void	loadHinge(TiXmlElement *hingeXML);
};

/**
@brief Clase XODE_hinge2
<xs:element name="hinge2">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="2" maxOccurs="2"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>
*/

class XODE_hinge2
{
private:
	XODE_anchor oAnchor;
	XODE_axis oAxis1;
	XODE_axis oAxis2;
	bool empty;	
public:
	XODE_hinge2();
	XODE_anchor 	getOAnchor();
	XODE_axis 	getOAxis1();
	XODE_axis 	getOAxis2();
	bool  	isEmpty();
	void	loadHinge2(TiXmlElement *hinge2XML);
	void 	loadUniversal(TiXmlElement *hinge2XML);
};
/**
@brief Clase fixed
<xs:element name="fixed">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="axis" minOccurs="0" maxOccurs="0"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>

<xs:element name="XODE_slider">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="axis"/>
	</xs:sequence>
	</xs:complexType>
</xs:element>

@note 	Utilizo slider para fixed y para slider, ya que tienen los mismos 
	atributos, pero solo cambia las condiciones.
*/
class XODE_slider
{
private:
	XODE_axis oAxis;
	bool empty;
public:
	XODE_slider();
	XODE_axis getOAxis();
	bool  isEmpty();
	void loadSlider(TiXmlElement *sliderXML);
	void loadFixed(TiXmlElement *fixedXML);
};
/**
@brief Clase XODE_joint
	Represetacion de una forma con masa
	<xs:element name="joint">
	<xs:complexType>
	<xs:sequence>
	<xs:sequence>
	<xs:element name="link1" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="body"/>
	</xs:complexType>
	</xs:element>
	<xs:element name="link2">
	<xs:complexType>
	<xs:attribute name="body"/>
	</xs:complexType>
	</xs:element>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	<xs:choice>
	<xs:element name="amotor">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="0" maxOccurs="3"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="ball">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="0" maxOccurs="0"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="fixed">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="axis" minOccurs="0" maxOccurs="0"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="hinge">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="hinge2">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="2" maxOccurs="2"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="slider">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="axis"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	<xs:element name="universal">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="anchor" minOccurs="0"/>
	<xs:element ref="axis" minOccurs="2" maxOccurs="2"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
	</xs:choice>
	</xs:sequence>
	<xs:attribute name="name"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_joint {
private:
	XODE_link 	link1;
	XODE_link 	link2;
	XODE_amotor 	oAmotor;
	XODE_ball 	oBall;
	XODE_hinge 	oHinge;
	XODE_hinge2  	oHinge2;
	XODE_slider 	oFixed;
	XODE_slider  	oSlider;
	XODE_hinge2 	oUniversal;
	std::string name;
	bool empty;
public:
	XODE_joint();
	XODE_link 	getLink1();
	XODE_link 	getLink2 ();
	XODE_amotor 	getOAmotor();
	XODE_ball 	getOBall();
	XODE_hinge 	getOHinge();
	XODE_hinge2  	getOHinge2();
	XODE_slider 	getOFixed();
	XODE_slider  	getOSlider();
	XODE_hinge2 	getOUniversal();
	std::string getName();
	bool  	isEmpty();
	void loadJoint(TiXmlElement *jointXML);
};
/**
@brief Clase XODE_geom
@note por simplicidad no se agregan los elementos de joint, jointfroup y body a la geometria.
	<xs:element name="geom">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="transform" minOccurs="0"/>
	<xs:choice>
	<xs:element ref="box"/>
	<xs:element ref="cappedCylinder"/>
	<xs:element ref="cone"/>
	<xs:element ref="cylinder"/>
	<xs:element ref="plane"/>
	<xs:element ref="ray"/>
	<xs:element ref="sphere"/>
	<xs:element ref="trimesh"/>
	</xs:choice>
	<xs:element ref="geom" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="body" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="group" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="geom" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="joint" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="jointgroup" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	
	<xs:attribute name="name"/>
	</xs:complexType>
	</xs:element>	
	
*/
class XODE_geom {	
private:
	XODE_transform oTransform;
	XODE_box 	oBox;
	XODE_cappedCylinder 	oCappedCylinder;
	XODE_cone 	oCone;
	XODE_cylinder 	oCylinder;
	XODE_plane 	oPlane;
	XODE_ray 	oRay;
	XODE_sphere 	oSphere;
	XODE_trimesh 	oTrimesh;
	std::string 	name;
	bool empty;
public:
	XODE_geom();
	XODE_transform getOTransform();
	XODE_box 	getOBox();
	XODE_cappedCylinder 	getOCappedCylinder();
	XODE_cone 	getOCone();
	XODE_cylinder 	getOCylinder();
	XODE_plane 	getOPlane();
	XODE_ray 	getORay();
	XODE_sphere 	getOSphere();
	XODE_trimesh 	getOTrimesh();
	std::string 	getName();
	bool  	isEmpty();
	void 	loadGeom(TiXmlElement *geomXML);
};

/**
@brief Clase XODE_torque
<xs:element name="torque" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
</xs:element>
*/
class XODE_torque
{
private:
	float x;
	float y;
	float z;
	bool empty;
public:
	XODE_torque();
	float getX();
	float getY();
	float getZ();
	bool  isEmpty();
	void  loadToque (TiXmlElement *torqueXML);
};
/**
@brief Clase XODE_force
<xs:element name="force" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
</xs:element>
*/
class XODE_force
{
private:	
	float x;
	float y;
	float z;
	bool empty;
public:
	XODE_force();
	float getX();
	float getY();
	float getZ();
	bool  isEmpty();
	void  loadForce (TiXmlElement *forceXML);
};
/**
@brief Clase XODE_finiteRotation
<xs:element name="finiteRotation" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="mode"/>
	<xs:attribute name="xaxis"/>
	<xs:attribute name="yxaxis"/>
	<xs:attribute name="zaxis"/>
	</xs:complexType>
</xs:element>
*/	
class XODE_finiteRotation
{
private:
	std::string mode;
	float 	xaxis;
	float 	yaxis;
	float 	zaxis;
	bool empty;
public:
	XODE_finiteRotation();
	std::string getMode();
	float 	getXaxis();
	float 	getYaxis();
	float 	getZaxis();
	bool  	isEmpty();
	void 	loadFiniteRotation(TiXmlElement *finiteRotationXML);
};

/**
@brief Clase XODE_linearVel
<xs:element name="linearVel" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
</xs:element>

*/
class XODE_linearVel
{
private:
	float x;
	float y;
	float z;
	bool empty;
public:
	XODE_linearVel();
	float getX();
	float getY();
	float getZ();
	bool  isEmpty();
	void  loadLinearVel (TiXmlElement *linearVelXML);
};

/**
@brief Clase XODE_angularVel
<xs:element name="angularVel" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
</xs:element>

*/
class XODE_angularVel
{
private:
	float x;
	float y;
	float z;
	bool empty;
public:
	XODE_angularVel();
	float getX();
	float getY();
	float getZ();
	bool  isEmpty();
	void  loadAngularVel (TiXmlElement *AngularVelXML);
};

/**
@brief Clase XODE_simpleBody
	Represetacion de un cuerpo simple.
@note El cuerpo simple solo contendra la estructura de cuerpo sin las respectivas uniones o grupos de uniones.

<xs:element name="body">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="transform" minOccurs="0"/>
	<xs:element name="torque" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>
	<xs:element name="force" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>
	<xs:element name="finiteRotation" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="mode"/>
	<xs:attribute name="xaxis"/>
	<xs:attribute name="yxaxis"/>
	<xs:attribute name="zaxis"/>
	</xs:complexType>
	</xs:element>
	<xs:element name="linearVel" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>
	<xs:element name="angularVel" minOccurs="0">
	<xs:complexType>
	<xs:attribute name="x"/>
	<xs:attribute name="y"/>
	<xs:attribute name="z"/>
	</xs:complexType>
	</xs:element>
	<xs:element ref="geom" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="body" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="group" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="joint" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="jointgroup" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="mass" minOccurs="0"/>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	<xs:attribute name="enabled" type="xs:boolean"/>
	<xs:attribute name="gravitymode"/>
	<xs:attribute name="name"/>
	</xs:complexType>
</xs:element>
	
*/
class XODE_simpleBody {
private:
	XODE_transform 	oTransform;
	XODE_torque 	oTorque;
	XODE_force 	oForce;
	XODE_finiteRotation 	oFiniteTotation;
	XODE_linearVel 	oLinealVel;
	XODE_angularVel 	oAngularVel;
	XODE_mass 	oMass;
	XODE_geom oGeom;
	bool 	enabled;
	std::string 	gravitymode;
	std::string	name;
	bool empty;

public:
	XODE_simpleBody();
	XODE_transform 	getOTransform();
	XODE_torque 	getOTorque();
	XODE_force 	getOForce();
	XODE_finiteRotation 	getOFiniteTotation();
	XODE_linearVel 	getOLinealVel();
	XODE_angularVel 	getOAngularVel();
	XODE_mass 	getOMass();
	XODE_geom   getOGeom();
	bool 	getEnabled();
	std::string 	getFravitymode();
	std::string	getName();
	bool  	isEmpty();
	void	loadSimpleBody(TiXmlElement *simpleBody);
};


class XODE_body{
private:
	XODE_simpleBody oSimpleBody;
	list<XODE_joint> oJoint;
	list<XODE_geom> oGeom;
	bool empty;
public:
	XODE_body();
	XODE_simpleBody  	 	getoSimpleBody();
	list<XODE_joint> 	getOJoint();
	list<XODE_geom> 	getOGeom();
	bool  	isEmpty();
	void 	loadBody(TiXmlElement *bodyXML); 	
};
/**
@brief Clase XODE_jointGroup
	Representacion de un conjunto de uniones
@note Por simplicidad se elimina el contenedor Group

<xs:element name="jointgroup">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="joint" maxOccurs="unbounded"/>
	<xs:element ref="geom" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="body" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="group" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="geom" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="jointgroup" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
*/
class XODE_jointGroup{
private:
	list<XODE_body> oBody;
	list<XODE_joint> oJoint;
	list<XODE_geom> oGeom;
	bool empty;

public:
	XODE_jointGroup();
	list<XODE_body> 	getOBody();
	list<XODE_joint> 	getOJoint();
	list<XODE_geom> 	getOGeom();
	bool  	isEmpty();
	void 	loadJoinGroup(TiXmlElement *jointgroupXML);
};

/**
@brief Clase XODE_space
	Representacion del espacio de una simulaciòn.
@note Por simplicidad se elimina el contenedor Group

<xs:element name="space">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="transform" minOccurs="0"/>
	<xs:element ref="geom" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="group" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="body" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="jointgroup" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="joint" minOccurs="0" maxOccurs="unbounded"/>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
*/
class XODE_space{
private:
	XODE_transform oTransform;
	list<XODE_geom> oGeom;
	list<XODE_body> oBody;
	list<XODE_jointGroup> oJointGroup;
	list<XODE_joint> oJoint;
	bool empty;
public:
	XODE_space();
	XODE_transform 	getOTransform();
	list<XODE_geom> 	getOGeom();
	list<XODE_body> 	getOBody();
	list<XODE_jointGroup> 	getOJointGroup();
	list<XODE_joint> 	getOJoint();
	bool  	isEmpty();
	void 	loadSpace(TiXmlElement *spaceXML);
};
/**
@brief Clase XODE_world
	Representacion del mundo.
	<xs:element name="world">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="transform" minOccurs="0"/>
	<xs:element ref="space" minOccurs="1" maxOccurs="unbounded"/>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	</xs:complexType>
	</xs:element>
*/
class XODE_world{
private:
	XODE_transform oTransform;
	list<XODE_space> oSpace;
	XODE_ext oExt;
	bool empty;
public:
	XODE_world();
	XODE_world loadWorld(TiXmlElement *worldXML);
	XODE_transform 	getoTransform();
	bool  	isEmpty();
	list<XODE_space> getoSpace();
	XODE_ext getoExt();
};
/**
@brief Clase XODE_xode
	Representacion objeto XODE
	<xs:element name="xode">
	<xs:complexType>
	<xs:sequence>
	<xs:element ref="world" minOccurs="1" maxOccurs="unbounded"/>
	<xs:element ref="ext" minOccurs="0" maxOccurs="unbounded"/>
	</xs:sequence>
	<xs:attribute name="name" type="xs:string"/>
	<xs:attribute name="version" use="required" fixed="1.0r22"/>
	</xs:complexType>
	</xs:element>
*/
class XODE_xode{
private:
	std::string name;
	std::string version;
	XODE_world oWorld;
	bool empty;
public:
	XODE_xode();
	TiXmlDocument loadXODE(char file[]);
	std::string getName();
	std::string getVersion();
	bool  	isEmpty();
	XODE_world getoWorld();
};


#endif
