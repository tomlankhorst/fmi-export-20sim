/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  %FILE_NAME%
 *  subm:  %SUBMODEL_NAME%
 *  model: %MODEL_NAME%
 *  expmt: %EXPERIMENT_NAME%
 *  date:  %GENERATION_DATE%
 *  time:  %GENERATION_TIME%
 *  user:  %USER_NAME%
 *  from:  %COMPANY_NAME%
 *  build: %GENERATION_BUILD%
 **********************************************************/

/* This file describes the integration methods
   that are supplied for computation.

   Currently only Euler, RungeKutta2 and RungeKutta4 are supplied,
   but it is easy for the user to add their own
   integration methods with these two as an example.
*/

/* the system include files */
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* our own include files */
#include "xxinteg.h"
#include "xxmodel.h"

%IF%%NUMBER_STATES%
#define %VARPREFIX%STATE_SIZE %NUMBER_STATES%
%ENDIF%

#ifdef Discrete_METHOD
/*********************************************************************
 * Discrete integration method
 *********************************************************************/

/* the initialization of the Discrete integration method */
void %FUNCTIONPREFIX%DiscreteInitialize (%VARPREFIX%ModelInstance* model_instance)
{
	/* nothing to be done */
	model_instance->major = XXTRUE;
}

/* the termination of the Discrete integration method */
void %FUNCTIONPREFIX%DiscreteTerminate (%VARPREFIX%ModelInstance* model_instance)
{
	/* nothing to be done */
}

/* the Discrete integration method itself */
void %FUNCTIONPREFIX%DiscreteStep (%VARPREFIX%ModelInstance* model_instance)
{
%IF%%NUMBER_STATES%
	XXInteger index;

	/* for each of the supplied states */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* just a move of the new state */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->%XX_RATE_ARRAY_NAME% [index];
	}
%ELSE%
	/* no states in the model */
%ENDIF%
	/* increment the simulation time */
	model_instance->time += model_instance->step_size;

	model_instance->major = XXTRUE;

	/* evaluate the dynamic part to calculate the new rates */
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
}
#endif /* Discrete_METHOD */


#ifdef Euler_METHOD
/*********************************************************************
 * Euler integration method
 *********************************************************************/

/* the initialization of the Euler integration method */
void %FUNCTIONPREFIX%EulerInitialize (%VARPREFIX%ModelInstance* model_instance)
{
	/* nothing to be done */
	model_instance->major = XXTRUE;
}

/* the termination of the Euler integration method */
void %FUNCTIONPREFIX%EulerTerminate (%VARPREFIX%ModelInstance* model_instance)
{
	/* nothing to be done */
}

/* the Euler integration method itself */
void %FUNCTIONPREFIX%EulerStep (%VARPREFIX%ModelInstance* model_instance)
{
%IF%%NUMBER_STATES%
	XXInteger index;

	/* for each of the supplied states */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* calculate the new state */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->%XX_STATE_ARRAY_NAME% [index] + model_instance->%XX_RATE_ARRAY_NAME% [index] * model_instance->step_size;
	}
%ELSE%
	/* no states in the model */
%ENDIF%
	/* increment the simulation time */
	model_instance->time += model_instance->step_size;

	model_instance->major = XXTRUE;

	/* evaluate the dynamic part to calculate the new rates */
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
}
#endif /* Euler_METHOD */


#ifdef RungeKutta2_METHOD
/*********************************************************************
 * RungeKutta2 integration method
 *********************************************************************/
/* the initialization of the RungeKutta2 integration method */
void %FUNCTIONPREFIX%RungeKutta2Initialize (%VARPREFIX%ModelInstance* model_instance)
{
%IF%%NUMBER_STATES%
	/* empty our static arrays */
	memset (model_instance->q0, 0, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));
%ENDIF%
}

/* the termination of the RungeKutta2 integration method */
void %FUNCTIONPREFIX%RungeKutta2Terminate (%VARPREFIX%ModelInstance* model_instance)
{
	/* nothing yet (our arrays are static) */
}


/* the Runge-Kutta-2 integration method itself */
void %FUNCTIONPREFIX%RungeKutta2Step (%VARPREFIX%ModelInstance* model_instance)
{
%IF%%NUMBER_STATES%
	XXInteger index;
	XXDouble rktime;

	/* This model has %NUMBER_STATES% states */
	/*********************************************************************************/
	/*          calculate intermediate result                                        */
	/*********************************************************************************/

	/* cache the simulation time */
	rktime = model_instance->time;

	/* the q0 will hold the value of the last evaluation. This is used multiple times
	   so remember the complete state array in this vector. */
	memcpy (model_instance->q0, model_instance->%XX_STATE_ARRAY_NAME%, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));

	/* calculate f (states, t) * 1/2 * dt  =  rates * 1/2 * dt  */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* set the new states to use */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->q0 [index] + model_instance->%XX_RATE_ARRAY_NAME% [index] * 0.5 * model_instance->step_size;
	}

	model_instance->time = rktime + 0.5 * model_instance->step_size;
	model_instance->major = XXFALSE;
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);

	/* for each state */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/*********************************************************************************/
		/*          calculate the next state from the intermediate results               */
		/*********************************************************************************/

		/* calculate the next state = classical Runge-Kutta integration step */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->q0 [index] +	model_instance->%XX_RATE_ARRAY_NAME% [index] * model_instance->step_size;
	}
	model_instance->time = rktime + model_instance->step_size;

%ELSE%
	/* no states in the model */
	/* increment the simulation time */
	model_instance->time += model_instance->step_size;

%ENDIF%
	model_instance->major = XXTRUE;

	/* evaluate the derivative model to calculate the new rates */
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
}
#endif /* RungeKutta2_METHOD */


#ifdef RungeKutta4_METHOD
/*********************************************************************
 * RungeKutta4 integration method
 *********************************************************************/

/*********************************************************************
 * the initialization of the RungeKutta4 integration method
 */
static const XXDouble OneOverSix = 1.0 / 6.0;

void %FUNCTIONPREFIX%RungeKutta4Initialize (%VARPREFIX%ModelInstance* model_instance)
{
%IF%%NUMBER_STATES%
	/* empty our static arrays */
	memset (model_instance->q0, 0, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));
	memset (model_instance->q1, 0, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));
	memset (model_instance->q2, 0, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));
	memset (model_instance->q3, 0, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));
	memset (model_instance->q4, 0, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));
%ENDIF%
}

/* the termination of the RungeKutta4 integration method */
void %FUNCTIONPREFIX%RungeKutta4Terminate (%VARPREFIX%ModelInstance* model_instance)
{
	/* nothing yet (our arrays are static) */
}


/* the Runge-Kutta-4 integration method itself */
void %FUNCTIONPREFIX%RungeKutta4Step (%VARPREFIX%ModelInstance* model_instance)
{
%IF%%NUMBER_STATES%
	XXInteger index;
	XXDouble rktime;

	/* This model has %NUMBER_STATES% states */
	/*********************************************************************************/
	/*          calculate intermediate state results q1, q2, q3 and q4               */
	/*********************************************************************************/

	/* cache the simulation time */
	rktime = model_instance->time;

	/* the q0 will hold the value of the last evaluation. This is used multiple times
	   so remember the complete state array in this vector. */
	memcpy (model_instance->q0, model_instance->%XX_STATE_ARRAY_NAME%, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));

	/* calculate q1 = f (states, t) * dt  =  rates * dt  */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* set the intermediate q1 */
		model_instance->q1 [index] = model_instance->%XX_RATE_ARRAY_NAME% [index] * model_instance->step_size;

		/* set the new states to use  for q2 */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->q0 [index] + model_instance->q1 [index] / 2;
	}

	/* calculate q2 = f (states + q1 / 2, t + dt / 2) * dt  */
	model_instance->time = rktime + model_instance->step_size / 2;

	model_instance->major = XXFALSE;

	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
	memcpy (model_instance->q2, model_instance->%XX_RATE_ARRAY_NAME%, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));

	/* for each state */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* set the ultimate q2 */
		model_instance->q2 [index] = model_instance->q2 [index] * model_instance->step_size;

		/* set the new states to use */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->q0 [index] + model_instance->q2 [index] / 2;
	}

	/* calculate q3 = f (states + q2 / 2, t + dt / 2) * dt  */
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
	memcpy (model_instance->q3, model_instance->%XX_RATE_ARRAY_NAME%, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));

	/* for each state */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* set the ultimate q3 */
		model_instance->q3 [index] = model_instance->q3 [index] * model_instance->step_size;

		/* set the new states */
		model_instance->%XX_STATE_ARRAY_NAME% [index] = model_instance->q0 [index] + model_instance->q3 [index];
	}

	/* calculate q4 = f (states + q3, t + dt) * dt */
	model_instance->time = rktime + model_instance->step_size;
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
	memcpy (model_instance->q4, model_instance->%XX_RATE_ARRAY_NAME%, %VARPREFIX%STATE_SIZE * sizeof (XXDouble));

	/* for each state */
	for (index = 0; index < %VARPREFIX%STATE_SIZE; index++)
	{
		/* set the ultimate q4 */
		model_instance->q4 [index] = model_instance->q4 [index] * model_instance->step_size;

		/*********************************************************************************/
		/*          calculate the next state from the intermediate results               */
		/*********************************************************************************/

		/* calculate the next state = classical Runge-Kutta integration step */
		model_instance->%XX_STATE_ARRAY_NAME%[index] = model_instance->q0[index] + (model_instance->q1[index] + model_instance->q2[index] + model_instance->q2[index] + model_instance->q3[index] + model_instance->q3[index] + model_instance->q4[index]) * OneOverSix;
	}
%ELSE%
	/* no states in the model */
	/* increment the simulation time */
	model_instance->time += model_instance->step_size;
%ENDIF%

	model_instance->major = XXTRUE;

	/* evaluate the derivative model to calculate the new rates */
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
}
#endif /* RungeKutta4_METHOD */
