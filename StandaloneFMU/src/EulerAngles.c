/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  %FILE_NAME%
 *  model: %MODEL_NAME%
 *  expmt: %EXPERIMENT_NAME%
 *  date:  %GENERATION_DATE%
 *  time:  %GENERATION_TIME%
 *  user:  %USER_NAME%
 *  from:  %COMPANY_NAME%
 *  build: %GENERATION_BUILD%
 **********************************************************/

/*****************************************************************************
--------------------------------------------------------------------------

 Copyright (c) 1995-2007 Controllab Products B.V. All Rights Reserved.

             www.controllab.nl
               www.20sim.com

 Authors: Frank Groen
--------------------------------------------------------------------------
 Description:

 Contains code for converting between different angle representations
 all 24 different kind of Euler angles
 Quaternions and H-Matrices and Rotation Matrices
--------------------------------------------------------------------------
******************************************************************************/

%IF%%NUMBEROF_DLL_EulerAngles%

/**** EulerAngles.c - Convert Euler angles to/from matrix or quat ****/
#include <math.h>
#include <float.h>
#include "xxtypes.h"
#include "EulerAngles.h"

typedef struct
{
	XXInteger i;
	XXInteger j;
	XXInteger k;
	XXInteger h;
	XXInteger n;
	XXInteger s;
	XXInteger f;
} EulOrderStruct;

EulOrderStruct g_EulerOrderStruct[24] = {
	{0, 1, 2, 0, 0, 0, 0},
	{0, 1, 2, 0, 0, 0, 1},
	{0, 1, 2, 2, 0, 1, 0},
	{0, 1, 2, 2, 0, 1, 1},
	{0, 2, 1, 0, 1, 0, 0},
	{0, 2, 1, 0, 1, 0, 1},
	{0, 2, 1, 1, 1, 1, 0},
	{0, 2, 1, 1, 1, 1, 1},
	{1, 2, 0, 1, 0, 0, 0},
	{1, 2, 0, 1, 0, 0, 1},
	{1, 2, 0, 0, 0, 1, 0},
	{1, 2, 0, 0, 0, 1, 1},
	{1, 0, 2, 1, 1, 0, 0},
	{1, 0, 2, 1, 1, 0, 1},
	{1, 0, 2, 2, 1, 1, 0},
	{1, 0, 2, 2, 1, 1, 1},
	{2, 0, 1, 2, 0, 0, 0},
	{2, 0, 1, 2, 0, 0, 1},
	{2, 0, 1, 1, 0, 1, 0},
	{2, 0, 1, 1, 0, 1, 1},
	{2, 1, 0, 2, 1, 0, 0},
	{2, 1, 0, 2, 1, 0, 1},
	{2, 1, 0, 0, 1, 1, 0},
	{2, 1, 0, 0, 1, 1, 1}
};

/* initialize and terminate function, which does nothing */

void EulerAngles_Initialize()
{
}
void EulerAngles_Terminate()
{
}

/* Construct quaternion from Euler angles (in radians). */
void Eul_ToQuat(EulerAngles *ea, Quat *qu)
{
	XXDouble a[3], ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	EulOrderStruct *eo;
	eo = &(g_EulerOrderStruct[ea->order]);

	if( eo->f == EulFrmR )
	{ 
		XXDouble t = ea->x;
		ea->x = ea->z;
		ea->z = t;
	}
	if( eo->n == EulParOdd )
		ea->y = -ea->y;

	ti = ea->x*0.5; tj = ea->y*0.5; th = ea->z*0.5;
	ci = cos(ti);  cj = cos(tj);  ch = cos(th);
	si = sin(ti);  sj = sin(tj);  sh = sin(th);
	cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;

	if( eo->s == EulRepYes )
	{
		a[eo->i] = cj*(cs + sc);	/* Could speed up with */
		a[eo->j] = sj*(cc + ss);	/* trig identities. */
		a[eo->k] = sj*(cs - sc);
		qu->w = cj*(cc - ss);
	}
	else
	{
		a[eo->i] = cj*sc - sj*cs;
		a[eo->j] = cj*ss + sj*cc;
		a[eo->k] = cj*cs - sj*sc;
		qu->w = cj*cc + sj*ss;
	}
	if( eo->n == EulParOdd )
		a[eo->j] = -a[eo->j];

	qu->x = a[Q_X]; qu->y = a[Q_Y]; qu->z = a[Q_Z];
}


/* Construct matrix from Euler angles (in radians). */
void Eul_ToHMatrix(EulerAngles *ea, MyHMatrix *M)
{
	XXDouble t, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	EulOrderStruct *eo;
	eo = &(g_EulerOrderStruct[ea->order]);

	if( eo->f == EulFrmR)
	{
		t = ea->x;
		ea->x = ea->z;
		ea->z = t;
	}
	if( eo->n == EulParOdd )
	{
		ea->x = -ea->x;
		ea->y = -ea->y;
		ea->z = -ea->z;
	}

	ci = cos(ea->x);
	cj = cos(ea->y);
	ch = cos(ea->z);

	si = sin(ea->x);
	sj = sin(ea->y);
	sh = sin(ea->z);
	
	cc = ci * ch;
	sc = si * ch;
	cs = ci * sh;
	ss = si * sh;

	if( eo->s == EulRepYes )
	{
		(*M)[INDEX4x4(eo->i,eo->i)] = cj;		(*M)[INDEX4x4(eo->i,eo->j)] =  sj*si;    (*M)[INDEX4x4(eo->i,eo->k)] =  sj*ci;
		(*M)[INDEX4x4(eo->j,eo->i)] = sj*sh;	(*M)[INDEX4x4(eo->j,eo->j)] = -cj*ss+cc; (*M)[INDEX4x4(eo->j,eo->k)] = -cj*cs-sc;
		(*M)[INDEX4x4(eo->k,eo->i)] = -sj*ch;	(*M)[INDEX4x4(eo->k,eo->j)] =  cj*sc+cs; (*M)[INDEX4x4(eo->k,eo->k)] =  cj*cc-ss;
	}
	else
	{
		(*M)[INDEX4x4(eo->i,eo->i)] = cj*ch;	(*M)[INDEX4x4(eo->i,eo->j)] = sj*sc-cs; (*M)[INDEX4x4(eo->i,eo->k)] = sj*cc+ss;
		(*M)[INDEX4x4(eo->j,eo->i)] = cj*sh;	(*M)[INDEX4x4(eo->j,eo->j)] = sj*ss+cc; (*M)[INDEX4x4(eo->j,eo->k)] = sj*cs-sc;
		(*M)[INDEX4x4(eo->k,eo->i)] = -sj;		(*M)[INDEX4x4(eo->k,eo->j)] = cj*si;    (*M)[INDEX4x4(eo->k,eo->k)] = cj*ci;
	}

	(*M)[INDEX4x4(Q_W,Q_X)]=0.0;	(*M)[INDEX4x4(Q_W,Q_Y)]=0.0;	(*M)[INDEX4x4(Q_W,Q_Z)]=0.0;
	(*M)[INDEX4x4(Q_X,Q_W)]=0.0;	(*M)[INDEX4x4(Q_Y,Q_W)]=0.0;	(*M)[INDEX4x4(Q_Z,Q_W)]=0.0;
	(*M)[INDEX4x4(Q_W,Q_W)]=1.0;
}

/* Convert matrix to Euler angles (in radians). */
void Eul_FromHMatrix(MyHMatrix *M, XXInteger order, EulerAngles *ea)
{
	/* first set the order field. */
	EulOrderStruct *eo;
	eo = &(g_EulerOrderStruct[order]);
	ea->order = order;
	
	if( eo->s == EulRepYes )
	{
		XXDouble sy = sqrt((*M)[INDEX4x4(eo->i,eo->j)]*(*M)[INDEX4x4(eo->i,eo->j)] + (*M)[INDEX4x4(eo->i,eo->k)]*(*M)[INDEX4x4(eo->i,eo->k)]);
		if (sy > 16*FLT_EPSILON) 
		{
			ea->x = atan2((*M)[INDEX4x4(eo->i,eo->j)], (*M)[INDEX4x4(eo->i,eo->k)]);
			ea->y = atan2(sy, (*M)[INDEX4x4(eo->i,eo->i)]);
			ea->z = atan2((*M)[INDEX4x4(eo->j,eo->i)], -(*M)[INDEX4x4(eo->k,eo->i)]);
		}
		else
		{
			ea->x = atan2(-(*M)[INDEX4x4(eo->j,eo->k)], (*M)[INDEX4x4(eo->j,eo->j)]);
			ea->y = atan2(sy, (*M)[INDEX4x4(eo->i,eo->i)]);
			ea->z = 0;
		}
	}
	else
	{
		XXDouble cy = sqrt((*M)[INDEX4x4(eo->i,eo->i)]*(*M)[INDEX4x4(eo->i,eo->i)] + (*M)[INDEX4x4(eo->j,eo->i)]*(*M)[INDEX4x4(eo->j,eo->i)]);
		if( cy > 16*FLT_EPSILON )
		{
			ea->x = atan2((*M)[INDEX4x4(eo->k,eo->j)], (*M)[INDEX4x4(eo->k,eo->k)]);
			ea->y = atan2(-(*M)[INDEX4x4(eo->k,eo->i)], cy);
			ea->z = atan2((*M)[INDEX4x4(eo->j,eo->i)], (*M)[INDEX4x4(eo->i,eo->i)]);
		}
		else
		{
			ea->x = atan2(-(*M)[INDEX4x4(eo->j,eo->k)], (*M)[INDEX4x4(eo->j,eo->j)]);
			ea->y = atan2(-(*M)[INDEX4x4(eo->k,eo->i)], cy);
			ea->z = 0;
		}
	}
	if( eo->n == EulParOdd )
	{
		ea->x = -ea->x;
		ea->y = - ea->y;
		ea->z = -ea->z;
	}
	if( eo->f == EulFrmR )
	{
		XXDouble t = ea->x;
		ea->x = ea->z;
		ea->z = t;
	}
}

void Quat_ToHMatrix(Quat *q, MyHMatrix *M)
{
	XXDouble Nq = q->x*q->x+q->y*q->y+q->z*q->z+q->w*q->w;
	XXDouble s = (Nq > 0.0) ? (1.0 / Nq) : 0.0;

	XXDouble xx = s * q->x * q->x;
	XXDouble xy = s * q->x * q->y;
	XXDouble xz = s * q->x * q->z;
	XXDouble xw = s * q->x * q->w;
	XXDouble yy = s * q->y * q->y;
	XXDouble yz = s * q->y * q->z;
	XXDouble yw = s * q->y * q->w;
	XXDouble zz = s * q->z * q->z;
	XXDouble zw = s * q->z * q->w;
	XXDouble ww = s * q->w * q->w;

	(*M)[INDEX4x4(Q_X,Q_X)] = ww + xx - yy - zz;
	(*M)[INDEX4x4(Q_X,Q_Y)] = 2.0 * ( xy + zw );
	(*M)[INDEX4x4(Q_X,Q_Z)] = 2.0 * ( xz - yw );

	(*M)[INDEX4x4(Q_Y,Q_X)] = 2.0 * ( xy - zw );
	(*M)[INDEX4x4(Q_Y,Q_Y)] = ww - xx + yy - zz;
	(*M)[INDEX4x4(Q_Y,Q_Z)] = 2.0 * ( yz + xw );

	(*M)[INDEX4x4(Q_Z,Q_X)] = 2.0 * ( xz + yw );
	(*M)[INDEX4x4(Q_Z,Q_Y)] = 2.0 * ( yz - xw );
	(*M)[INDEX4x4(Q_Z,Q_Z)] = ww - xx - yy + zz;

	(*M)[INDEX4x4(Q_W,Q_X)]=0.0;
	(*M)[INDEX4x4(Q_W,Q_Y)]=0.0;
	(*M)[INDEX4x4(Q_W,Q_Z)]=0.0;
	(*M)[INDEX4x4(Q_X,Q_W)]=0.0;
	(*M)[INDEX4x4(Q_Y,Q_W)]=0.0;
	(*M)[INDEX4x4(Q_Z,Q_W)]=0.0;
	
	(*M)[INDEX4x4(Q_W,Q_W)]=1.0;
}
void Quat_FromHMatrix(MyHMatrix *M, Quat *q)
{
	XXDouble T = 1.0 + (*M)[INDEX4x4(Q_X,Q_X)] + (*M)[INDEX4x4(Q_Y,Q_Y)] + (*M)[INDEX4x4(Q_Z,Q_Z)];

	XXDouble S;
	XXDouble sqrtT;

	/* if the "w" is non-zero */
	if( T != 0.0 )
	{
		sqrtT = 2.0 * sqrt(T);
		S = 1.0 / sqrtT;
		q->x = S * ((*M)[INDEX4x4(Q_Y,Q_Z)] - (*M)[INDEX4x4(Q_Z,Q_Y)]);
		q->y = S * ((*M)[INDEX4x4(Q_Z,Q_X)] - (*M)[INDEX4x4(Q_X,Q_Z)]);
		q->z = S * ((*M)[INDEX4x4(Q_X,Q_Y)] - (*M)[INDEX4x4(Q_Y,Q_X)]);

		q->w = 0.25 * sqrtT;
		return;
	}

	if ( (*M)[INDEX4x4(Q_X,Q_X)] > (*M)[INDEX4x4(Q_Y,Q_Y)] && (*M)[INDEX4x4(Q_X,Q_X)] > (*M)[INDEX4x4(Q_Z,Q_Z)] ) 
	{	
		/* Column 0:  */
		sqrtT = 2.0 * sqrt( 1.0 + (*M)[INDEX4x4(Q_X,Q_X)] - (*M)[INDEX4x4(Q_Y,Q_Y)] - (*M)[INDEX4x4(Q_Z,Q_Z)] );
		S = 1.0 / sqrtT;
		q->x = 0.25 * sqrtT;
		q->y = S * ((*M)[INDEX4x4(Q_Y,Q_X)] + (*M)[INDEX4x4(Q_X,Q_Y)] );
		q->z = S * ((*M)[INDEX4x4(Q_X,Q_Z)] + (*M)[INDEX4x4(Q_Z,Q_X)] );
		q->w = S * ((*M)[INDEX4x4(Q_Z,Q_Y)] - (*M)[INDEX4x4(Q_Y,Q_Z)] );
		return;
	}
	
	if ( (*M)[INDEX4x4(Q_Y,Q_Y)] > (*M)[INDEX4x4(Q_Z,Q_Z)] )
	{
		/* Column 1:  */
		sqrtT  = sqrt( 1.0 + (*M)[INDEX4x4(Q_Y,Q_Y)] - (*M)[INDEX4x4(Q_X,Q_X)] - (*M)[INDEX4x4(Q_Z,Q_Z)] ) * 2.0;
		S = 1.0 / sqrtT;
		q->x = S * ((*M)[INDEX4x4(Q_Y,Q_X)] + (*M)[INDEX4x4(Q_X,Q_Y)] );
		q->y = 0.25 * sqrtT;
		q->z = S * ((*M)[INDEX4x4(Q_Z,Q_Y)] + (*M)[INDEX4x4(Q_Y,Q_Z)] );
		q->w = S * ((*M)[INDEX4x4(Q_X,Q_Z)] - (*M)[INDEX4x4(Q_Z,Q_X)] );
		return;
	} 
			
	/* Column 2: */
	sqrtT  = sqrt( 1.0 + (*M)[INDEX4x4(Q_Z,Q_Z)] - (*M)[INDEX4x4(Q_X,Q_X)] - (*M)[INDEX4x4(Q_Y,Q_Y)] ) * 2.0;
	S = 1.0 / sqrtT;
	q->x = S * ((*M)[INDEX4x4(Q_X,Q_Z)] + (*M)[INDEX4x4(Q_Z,Q_X)] );
	q->y = S * ((*M)[INDEX4x4(Q_Z,Q_Y)] + (*M)[INDEX4x4(Q_Y,Q_Z)] );
	q->z = 0.25 * sqrtT;
	q->w = S * ((*M)[INDEX4x4(Q_Y,Q_X)] - (*M)[INDEX4x4(Q_X,Q_Y)] );
}
/* Convert quaternion to Euler angles (in radians). */
void Eul_FromQuat(Quat *q, XXInteger order, EulerAngles *ea)
{
	MyHMatrix M;

	Quat_ToHMatrix(q, &M);
	Eul_FromHMatrix(&M, order, ea);
}




void FillHmatrixFromHMatrixArray(MyHMatrix *hM, XXDouble *inarr)
{
	XXInteger row, column;
	XXInteger inarrIndex = 0;

	for( row = 0; row < 4; row++)
	{
		for( column = 0; column < 4; column++ )
		{
			(*hM)[INDEX4x4(row, column)] = inarr[inarrIndex];
			inarrIndex++;
		}
	}
}

void FillHmatrixFromRotationMatrixArray(MyHMatrix *hM, XXDouble *inarr)
{
	XXInteger row, column;
	XXInteger inarrIndex = 0;

	/* first the 3x3 part */
	for( row = 0; row < 3; row++)
	{
		for( column = 0; column < 3; column++ )
		{
			(*hM)[INDEX4x4(row, column)] = inarr[inarrIndex];
			inarrIndex++;
		}
	}

	/* last column */
	for( row = 0; row < 3; row++ )
	{
		(*hM)[INDEX4x4(row, 3)] = 0.0;
	}
	/* last row */
	for( column = 0; column < 3; column++ )
	{
		(*hM)[INDEX4x4(3, column)] = 0.0;
	}

	/* and last element */
	(*hM)[INDEX4x4(3, 3)] = 1.0;
}

void FillHmatrixArrayFromHMatrix(XXDouble *outarr, MyHMatrix *hM)
{
	XXInteger row, column;
	XXInteger inarrIndex = 0;

	for( row = 0; row < 4; row++)
	{
		for( column = 0; column < 4; column++ )
		{
			outarr[inarrIndex] = (*hM)[INDEX4x4(row, column)];
			inarrIndex++;
		}
	}
}
void FillRotationMatrixArrayFromHMatrix(XXDouble *outarr, MyHMatrix *hM)
{
	XXInteger row, column;
	XXInteger outarrIndex = 0;

	/* assume only the rotation part of the matrix
	   only the 3x3 part */
	for( row = 0; row < 3; row++)
	{
		for( column = 0; column < 3; column++ )
		{
			outarr[outarrIndex] = (*hM)[INDEX4x4(row, column)];
			outarrIndex++;
		}
	}
}


XXInteger GeneralEulFromHMatrix(XXInteger order, XXDouble *inarr, XXDouble *outarr)
{
	/* inarr is the MyHMatrix
	   outarr is the angles */

	MyHMatrix hM;
	EulerAngles eul;

	/* fill the matrix */
	FillHmatrixFromHMatrixArray(&hM, inarr);

	Eul_FromHMatrix(&hM, order, &eul);

	outarr[0] = eul.x;	outarr[1] = eul.y;	outarr[2] = eul.z;

	/* 0 is success */
	return 0;
}

XXInteger GeneralEulFromRotationMatrix(XXInteger order, XXDouble *inarr, XXDouble *outarr)
{
	/* inarr is the MyHMatrix
	   outarr is the angles */

	MyHMatrix hM;
	EulerAngles eul;

	/* fill the matrix */
	FillHmatrixFromRotationMatrixArray(&hM, inarr);

	Eul_FromHMatrix(&hM, order, &eul);

	outarr[0] = eul.x;	outarr[1] = eul.y;	outarr[2] = eul.z;

	/* 0 is success */
	return 0;

}

XXInteger GeneralHMatrixFromEul(XXInteger order, XXDouble *inarr, XXDouble *outarr)
{
	/* inarr is the angles
	   outarr is the MyHMatrix */

	MyHMatrix hM;
	EulerAngles eul;
	
	eul.x = inarr[0];
	eul.y = inarr[1];
	eul.z = inarr[2];
	eul.order = order;

	/* call the function */
	Eul_ToHMatrix(&eul, &hM);

	/* and fill the result in the matrix */
	FillHmatrixArrayFromHMatrix(outarr, &hM);

	/* 0 is success */
	return 0;
}

XXInteger GeneralRotationMatrixFromEul(XXInteger order, XXDouble *inarr, XXDouble *outarr)
{
	/* inarr is the angles
	   outarr is the MyHMatrix */

	MyHMatrix hM;
	EulerAngles eul;
	
	eul.x = inarr[0];
	eul.y = inarr[1];
	eul.z = inarr[2];
	eul.order = order;

	Eul_ToHMatrix(&eul, &hM);

	/* and fill the result in the matrix */
	FillRotationMatrixArrayFromHMatrix(outarr, &hM);

	/* 0 is success */
	return 0;
}


/************************************************************************/
/* Here come the external functions                                     */
/************************************************************************/

/* relative from MyHMatrix */
XXInteger EulerAngles_EulXYXrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXYXr, inarr, outarr);
}
XXInteger EulerAngles_EulXYZrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXYZr, inarr, outarr);
}
XXInteger EulerAngles_EulXZXrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXZXr, inarr, outarr);
}
XXInteger EulerAngles_EulXZYrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXZYr, inarr, outarr);
}
XXInteger EulerAngles_EulYXYrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYXYr, inarr, outarr);
}
XXInteger EulerAngles_EulYXZrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYXZr, inarr, outarr);
}
XXInteger EulerAngles_EulYZXrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYZXr, inarr, outarr);
}
XXInteger EulerAngles_EulYZYrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYZYr, inarr, outarr);
}
XXInteger EulerAngles_EulZXYrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZXYr, inarr, outarr);
}
XXInteger EulerAngles_EulZXZrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZXZr, inarr, outarr);
}
XXInteger EulerAngles_EulZYXrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZYXr, inarr, outarr);
}
XXInteger EulerAngles_EulZYZrFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZYZr, inarr, outarr);
}

/* relative from RotationMatrix */
XXInteger EulerAngles_EulXYXrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXYXr, inarr, outarr);
}
XXInteger EulerAngles_EulXYZrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXYZr, inarr, outarr);
}
XXInteger EulerAngles_EulXZXrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXZXr, inarr, outarr);
}
XXInteger EulerAngles_EulXZYrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXZYr, inarr, outarr);
}
XXInteger EulerAngles_EulYXYrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYXYr, inarr, outarr);
}
XXInteger EulerAngles_EulYXZrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYXZr, inarr, outarr);
}
XXInteger EulerAngles_EulYZXrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYZXr, inarr, outarr);
}
XXInteger EulerAngles_EulYZYrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYZYr, inarr, outarr);
}
XXInteger EulerAngles_EulZXYrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZXYr, inarr, outarr);
}
XXInteger EulerAngles_EulZXZrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZXZr, inarr, outarr);
}
XXInteger EulerAngles_EulZYXrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZYXr, inarr, outarr);
}
XXInteger EulerAngles_EulZYZrFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZYZr, inarr, outarr);
}


/* static from MyHMatrix */
XXInteger EulerAngles_EulXYXsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXYXs, inarr, outarr);
}
XXInteger EulerAngles_EulXYZsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXYZs, inarr, outarr);
}
XXInteger EulerAngles_EulXZXsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXZXs, inarr, outarr);
}
XXInteger EulerAngles_EulXZYsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdXZYs, inarr, outarr);
}
XXInteger EulerAngles_EulYXYsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYXYs, inarr, outarr);
}
XXInteger EulerAngles_EulYXZsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYXZs, inarr, outarr);
}
XXInteger EulerAngles_EulYZXsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYZXs, inarr, outarr);
}
XXInteger EulerAngles_EulYZYsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdYZYs, inarr, outarr);
}
XXInteger EulerAngles_EulZXYsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZXYs, inarr, outarr);
}
XXInteger EulerAngles_EulZXZsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZXZs, inarr, outarr);
}
XXInteger EulerAngles_EulZYXsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZYXs, inarr, outarr);
}
XXInteger EulerAngles_EulZYZsFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromHMatrix(EulOrdZYZs, inarr, outarr);
}

/* static from RotationMatrix */
XXInteger EulerAngles_EulXYXsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXYXs, inarr, outarr);
}
XXInteger EulerAngles_EulXYZsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXYZs, inarr, outarr);
}
XXInteger EulerAngles_EulXZXsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXZXs, inarr, outarr);
}
XXInteger EulerAngles_EulXZYsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdXZYs, inarr, outarr);
}
XXInteger EulerAngles_EulYXYsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYXYs, inarr, outarr);
}
XXInteger EulerAngles_EulYXZsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYXZs, inarr, outarr);
}
XXInteger EulerAngles_EulYZXsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYZXs, inarr, outarr);
}
XXInteger EulerAngles_EulYZYsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdYZYs, inarr, outarr);
}
XXInteger EulerAngles_EulZXYsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZXYs, inarr, outarr);
}
XXInteger EulerAngles_EulZXZsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZXZs, inarr, outarr);
}
XXInteger EulerAngles_EulZYXsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZYXs, inarr, outarr);
}
XXInteger EulerAngles_EulZYZsFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralEulFromRotationMatrix(EulOrdZYZs, inarr, outarr);
}



/************************************************************************/
/* the other way round                                                  */
/************************************************************************/

/* relative from MyHMatrix */
XXInteger EulerAngles_HMatrixFromEulXYXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXYXr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulXYZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXYZr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulXZXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXZXr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulXZYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXZYr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYXYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYXYr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYXZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYXZr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYZXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYZXr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYZYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYZYr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZXYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZXYr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZXZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZXZr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZYXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZYXr, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZYZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZYZr, inarr, outarr);
}

/* static from MyHMatrix */
XXInteger EulerAngles_HMatrixFromEulXYXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXYXs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulXYZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXYZs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulXZXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXZXs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulXZYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdXZYs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYXYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYXYs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYXZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYXZs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYZXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYZXs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulYZYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdYZYs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZXYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZXYs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZXZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZXZs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZYXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZYXs, inarr, outarr);
}
XXInteger EulerAngles_HMatrixFromEulZYZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralHMatrixFromEul(EulOrdZYZs, inarr, outarr);
}

/* relative from RotationMatrix */
XXInteger EulerAngles_RotationMatrixFromEulXYXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXYXr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulXYZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXYZr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulXZXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXZXr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulXZYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXZYr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYXYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYXYr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYXZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYXZr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYZXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYZXr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYZYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYZYr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZXYr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZXYr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZXZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZXZr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZYXr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZYXr, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZYZr (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZYZr, inarr, outarr);
}
/* static from RotationMatrix */
XXInteger EulerAngles_RotationMatrixFromEulXYXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXYXs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulXYZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXYZs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulXZXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXZXs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulXZYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdXZYs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYXYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYXYs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYXZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYXZs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYZXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYZXs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulYZYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdYZYs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZXYs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZXYs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZXZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZXZs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZYXs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZYXs, inarr, outarr);
}
XXInteger EulerAngles_RotationMatrixFromEulZYZs (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	return GeneralRotationMatrixFromEul(EulOrdZYZs, inarr, outarr);
}

/* Quaternion to MyHMatrix */
XXInteger EulerAngles_HMatrixFromQuaternion (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	/* inarr is quaternion
	   outarr is MyHMatrix */

	Quat q;
	MyHMatrix hM;

	q.x = inarr[0];	q.y = inarr[1];	q.z = inarr[2];	q.w = inarr[3];

	Quat_ToHMatrix(&q, &hM);

	/* and fill the result in the matrix */
	FillHmatrixArrayFromHMatrix(outarr, &hM);

	/* 0 is success */
	return 0;
}

/* MyHMatrix to Quaternion */
XXInteger EulerAngles_QuaternionFromHMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	/* inarr is MyHMatrix
	   outarr is quaternion */

	Quat q;
	MyHMatrix hM;

	FillHmatrixFromHMatrixArray(&hM, inarr);

	Quat_FromHMatrix(&hM, &q);

	/* and the result to the outarr */
	outarr[0] = q.x;	outarr[1] = q.y;	outarr[2] = q.z;	outarr[3] = q.w;

	/* 0 is success */
	return 0;
}

/* Quaternion to RotationMatrix */
XXInteger EulerAngles_RotationMatrixFromQuaternion (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	/* inarr is quaternion
	   outarr is MyHMatrix */

	Quat q;
	MyHMatrix hM;

	q.x = inarr[0];	q.y = inarr[1];	q.z = inarr[2];	q.w = inarr[3];

	Quat_ToHMatrix(&q, &hM);

	/* and fill the result in the matrix */
	FillRotationMatrixArrayFromHMatrix(outarr, &hM);

	/* 0 is success */
	return 0;
}

/* RotationMatrix to Quaternion */
XXInteger EulerAngles_QuaternionFromRotationMatrix (XXDouble *inarr, XXInteger inputs, XXDouble *outarr, XXInteger outputs, XXInteger major)
{
	/* inarr is MyHMatrix
	   outarr is quaternion */

	Quat q;
	MyHMatrix hM;

	FillHmatrixFromRotationMatrixArray(&hM, inarr);

	Quat_FromHMatrix(&hM, &q);

	/* and the result to the outarr */
	outarr[0] = q.x;	outarr[1] = q.y;	outarr[2] = q.z;	outarr[3] = q.w;

	/* 0 is success */
	return 0;
}
%ENDIF%

