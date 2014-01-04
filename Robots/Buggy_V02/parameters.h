/**
 * @file   parameters.h
 * @Author Miguel Rodriguez (miguel.andres.rodriguez.s@gmail.com)
 * @date   Julio 2013
 * @brief  Archivo con constantes para la simulacion.
 *
 * Este archivo tiene las constantes utilizadas para describir la simulación, por ejemplo, pararametros
 * como STEP definen que tan presisa será la simulación, mientras más pequeño sea el parametro, más 
 * precosa es la simulación.
 */


/** @defgroup Colision 
 *	Constantes para la presicion y calculo de las colisiones.
 *  @{
 */

	/** @{	*/
#define SLIP1 0.1 				/**< 
									El SLIP1 y SLIP12 representan el FDS aplicado en la direccion1 y la direccion 2. 
									EL FDS es el efecto entre las superficies de contacto cuando una de deslisa sobre la otra con una velocidad proporcional a la fuerza que se aplica tangencialmente a la superficie.
									En pabras simples es el coeficiente de friccion, mientras mas grande, mas dificil será deslizar una superficie sobre la otra. */
#define SLIP2 0.1 				
	/** @} */
#define SOFT_ERP 0.5			/**< 
									El SOFT_ERP define el "Error Reduction Parameter" utilizado para manejar uniones en ODE. 
									El ERP define que proporcion del error en el contacto con la union será arreglado durante el el proximi paso de simulacion.
									Si el ERP es igual a 0, entonces no se aplicara ninguna fuerza de correccion y los cuerpos eventualmente se separaran mientras la simulación avance.
									Si el ERP es igual a 1, entonces la simulacion intentara arreglar todos los errores durante el proximo paso de simulación. A pesar de esto, 
									no se recomienda colocar el ERP en  1. 
									Los valores recomendados ocilan entre 0.1 y 0.8.
								*/
#define SOFT_CFM 0.01			/**<
									El SOFT_CFM define el "soft Constraint Force Mixing"  utilizado para manejar uniones en ODE.
									Junto con el ERP, el CFM controla que tan "esponjoso" o cuanto "juego" puede tener una union.
									Si el CFM es puesto en 0, la restriccion sera "dura" o fuerte. Si el CFM es puesto en un valor positivo, significa que es posible 
									"violar" la restriccion (punto de colision entre superficies). Esta condicion crecerá mientras mas se incremente este valor.
									*/
#define MAX_CONTACTS 10			/**<
									Puntos de contacto maximos que se evaluaran cada vez que suceda una colision.
									Mientras mas crece este valor, mas preciso son los calculos, pero tambien son mas costosos en lo que se refiere a calculo.
									*/

/** @} */ 


/** @defgroup Controles 
*	Controles para la las velocidades y angulos de giro viraje de las ruedas.
*  @{
*/

#define aceleration 0.01		/**< Define en cuanto aumentará la velocidad de del buggy cada vez que se incremente la velocidad.*/
#define steerWheel 0.5			/**< Define cual sera el angulo de giro por cada vez que se intente girar. El angulo esta medido en radianes.*/
/** @{	*/
/** 
	El motor en la simulacion se puede comportar como un motor de multiples velocidades o como un motor con una sola velocidad.
	El ENGINE_TYPE_ONE define un motor de solo una velocidad con una fuerza de torque definida por aceleration. @see aceleration.
	El ENGINE_TYPE_TWO define un motor con multiples velocidades, todas mmultiplos de aceleration.
	Solo uno de los parametros debe estar en TRUE.
*/
#define ENGINE_TYPE_ONE false	
#define ENGINE_TYPE_TWO true	
/** @} */ 

/** @} */ 

/** @{	*/
/** 
	Definicion de los colores. 
	Para la representacion visual de los objetos tambien es posible definir sus colores, lor colores estan en RGB, sin la utilizacion de letras (F = 16) 
*/
#define chasis_r 1  				
#define chasis_g 0
#define chasis_b 0
#define wheels_r 0
#define wheels_g 1
#define wheels_b 0
/** @}	*/



#define STEP 0.02			/**< 
							El STEP determinará que tan precisa es la simulación. Mientras mas pequeño se a el parametro, más calculos se realizaran por segundo. 
							Este valor debe estar entre 0 y 1, no pudiendo tomar ninguno de estos valores. Mientras mas cerca este de 1, menos precisa y mas rapida sera la simulacion. 
							Mientras mas cerca de 0, la simulacion sera mas recisa, pero tambien mas costosa en procesamiento.
							*/