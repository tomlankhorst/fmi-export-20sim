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

/* Standard include files */
#include <stdlib.h>

/* 20-sim include files */
#include "xxmodel.h"
#include "xxinteg.h"
#include "xxfuncs.h"
#include "xxsubmod.h"
#include "motionprofiles.h"
#include "EulerAngles.h"


/* The initialization function for submodel */
void %FUNCTIONPREFIX%InitializeSubmodel (%VARPREFIX%ModelInstance* model_instance)
{
	/* Initialization phase (allocating memory) */
	model_instance->%XX_INITIALIZE% = XXTRUE;
	model_instance->steps = 0;
	%FUNCTIONPREFIX%%INTEGRATION_METHOD_NAME%Initialize (model_instance);

	/* Calculate the model for the first time */
	%FUNCTIONPREFIX%CalculateInitial (model_instance);
	%FUNCTIONPREFIX%CalculateStatic (model_instance);
	%FUNCTIONPREFIX%CalculateInput (model_instance);
	%FUNCTIONPREFIX%CalculateDynamic (model_instance);
	%FUNCTIONPREFIX%CalculateOutput (model_instance);

	/* End of initialization phase */
	model_instance->%XX_INITIALIZE% = XXFALSE;
}

/* The function that calculates the submodel */
void %FUNCTIONPREFIX%CalculateSubmodel (%VARPREFIX%ModelInstance* model_instance, XXDouble t, XXDouble outputTime)
{
	/* Copy the time */
	model_instance->time = t;

	/* Calculate the model */
	%FUNCTIONPREFIX%CalculateInput (model_instance);
	%FUNCTIONPREFIX%%INTEGRATION_METHOD_NAME%Step (model_instance, outputTime);
	%FUNCTIONPREFIX%CalculateOutput (model_instance);
}

/* The termination function for submodel */
void %FUNCTIONPREFIX%TerminateSubmodel (%VARPREFIX%ModelInstance* model_instance, XXDouble t)
{
	/* Copy the time */
	model_instance->time = t;

	/* Calculate the final model equations */
	%FUNCTIONPREFIX%CalculateFinal (model_instance);

	/* and terminate the model itself (releasing memory) */
	%FUNCTIONPREFIX%ModelTerminate (model_instance);
	%FUNCTIONPREFIX%%INTEGRATION_METHOD_NAME%Terminate (model_instance);
}
