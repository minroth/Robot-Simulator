/**
 * @file   buggy.h
 * @Author Miguel Rodriguez (miguel.andres.rodriguez.s@gmail.com)
 * @date   Julio 2013
 * @brief  Archivo con variables para la simulacion.
 *
 * En este archivo se guardan las variables para la simulacion de un "buggy". Este "buggy" es un vehiculo 
 * de 3 ruedas con comandos de movimiento y cambios de velocidad.	
 */


#ifndef DRAWSTUFF_TEXTURE_PATH
#define DRAWSTUFF_TEXTURE_PATH "../../Engine/drawstuff/textures"	/**< Ruta del directorio con las texturas. */
#endif


static dWorldID world;					/**< Almacena el ID del "mundo". Esta variable representa el mundo en la simulación. */
static dSpaceID space;					/**< Almacena el ID del "espacio". Esta variable representa el espacio donde ocurre la simulaciòn. */
static dBodyID body[4];					/**< Array con 4 ID de cuerpos. Los cuepos son comunmente formas dentro de la simulaciòn, en este caso representa el chasis y las 3 ruedas. */
static dJointID joint[3]; 				/**< Array con 3 ID de uniones. Este array representa las uniones entre las ruedas y el chasis. */
static dJointGroupID contactgroup;		/**< Almacena el ID del "grupo de contacto". El grupo de contacto un grupo de cuerpos que pueden colisionar dentro de una simulacion. */
static dGeomID ground;					/**< Almacena el ID de la geometria del piso. Esta geometria representa el piso, este es solo el ID de esta geometria, es necesario definir sus propiedades para utilizarla. */
static dSpaceID car_space;				/**< Almacena el ID del "espacio" correspondiente al buggy. En un mundo pueden existir muchos espacios, cada uno con propiedades de simulacion diferente, este espacio representa al buggy. */
static dGeomID box;						/**< Almacena el ID de la geometria del "chasis" del buggy. */
static dGeomID sphere[3];				/**< Almacena el ID de la geometria de 3 "esferas". Estas esferas representan las 3 ruedas del buggy. */
static dGeomID ground_box;				/**< Almacena el ID de la geometria de una caja. Este es la "rampla" que esta estatica e inmovil dentro de la simulación.*/
static dReal speed=0;					/**< Variables de tipo "Real". Esta variable es utilizada para incrementar la velocidad del buggy. */
static dReal steer=0; 					/**< Variables de tipo "Real". Esta variable es utilizada para manejar el angulo de rotacion del la rueda delantera del buggy.*/
static float fileNumber=0;				/**< Numero utilizado como numero archivo a guardar */
		
XODE_xode OXODE;						/**< Objeto que representa todos los objetos a simular, almacena todos los objetos y sus propìedades. */
