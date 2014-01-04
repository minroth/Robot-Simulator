/**
 *  
 *  @file   parameters.h
 *  @Author Miguel Rodriguez (miguel.andres.rodriguez.s@gmail.com)
 *  @date   Julio 2013
 *  @brief  Un ejemplo de un robot modular utilizando Open Dynamics Engine (ODE) 
 *  
 *  GPL LICENSE 
 *
 *  Este codigo es una extension al codigo snake.cpp.  Dr. Juan Gonzalez-Gomez. December 2008.
 *  Más informacion acerca del proyecto está disponible en   
 *  http://www.iearobotics.com/wiki/index.php?title=Juan_Gonzalez:Tesis 
 */ 

/*----------*/
/* MACROS   */
/*----------*/
#define DEG2RAD(X) M_PI*X/180.0 	/**< Conversion de grados a radianes. */


/*-------------------------------------------------------------*/
/*--                       CONSTANTES	                     --*/
/*-------------------------------------------------------------*/

/*-------------------*/
/*-    	SNAKE        */
/*-------------------*/
#define MOD 8         				/**< Numero de modulos. */

/*-------------------*/
/*-    	MODULES      */
/*-------------------*/
#define L         0.072     		/**< Largo en metros. */

/*---------------------*/
/*-   	SERVOS         */
/*---------------------*/
#define WMAX 13.0*M_PI/9.0 			/**< Velocidad angular maxima el servo, en rad/seg. */


/*-------------------------------*/
/* 		CONTROLADOR DEL SERVO    */
/*-------------------------------*/
#define KP        8.3  				/**< Controlador del servo  */

#define ERROR     DEG2RAD(5)		/**< Error máximo en la posición (en grados). Si la distancia 
										 entre la posición del servo actual y la posición de referencia es menor que el error, 
										 el controlador asumirá que se ha alcanzado la posición de referencia. */          

#define N 32						/**< Número de muestras totales para las funciones sinusoidales 
										que determinan las posiciones de referencia de los servos */

/*----------------------------*/
/* 		VISUALIZACIÓN         */
/*----------------------------*/

#define VIEW_SCALE 10 				/**< Escala */

#define PAUSE       1   			/**< Pausa entre dos etapas de simulación (en microsegundos). Esta constante 
										se debe cambiar en función de la velocidad del ordenador donde se ejecuta la simulacion. 
										Si la simulación se ejecuta muy rápido, se debe aumentar PAUSA. */

/*-------------------------*/
/* 		CONSTANTES ODE     */
/*-------------------------*/

#define MU                   0.2  	/**< Constantes de friccion*/
#define MU2                  MU 	/**< Constantes de friccion */

//-- Otros parametros ODE
#define CFM                  1e-5
#define ERP                  0.2
#define MAX_CORRECTING_VEL   0.1
#define SURFACE_LAYER        0.0001
#define STEP                 0.01
#define BOUNCE               0.1
#define BOUNCE_VEL           0.1
#define SOFT_CFM             0.01
#define MAX_CONTACTS         4		

/*----------------------*/
/* 	TEXTURAS     		*/
/*----------------------*/
#define DRAWSTUFF_TEXTURE_PATH 	"../../Engine/drawstuff/textures"	/**< Ruta del directorio con las texturas. */

/*----------------------*/
/* 	XODE PARAMETERS		*/
/*----------------------*/
XODE_xode OXODE;						/**< Objeto que representa todos los objetos a simular, almacena todos los objetos y sus propìedades. */

/*--------------------------*/
/* 	VARIABLES GLOBALES	    */
/*--------------------------*/
static dWorldID world; 					/**< Identificador del mundo. */
static dSpaceID space; 					/**< Indentificador de espacion de colision. */
static dJointGroupID contactgroup; 		/**< Identificador para el grupo de uniones. */
static MySnake snake; 					/**< El robot modular a ser simulado. */
static int n=0;   						/**< Tiempo discreto usado en el muestreo de la funcion sinosoidales. */


//Parametros usados para controlar los movimientos.
dReal Av, Ah;           				/**< Amplitudes horizontales y verticales.. */
dReal PDv, PDh, PDvh;   				/**< Diferencia de fase. */
dReal F0;               				/**< Fase inicial. */
dReal Oh;               				/**< Desplazamiento horizontal. */
int direction=-1; 						/**< Direccion de la serpiente. */
