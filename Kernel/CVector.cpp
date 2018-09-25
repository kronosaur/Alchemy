//	CVector.cpp
//
//	Implements CVector object

#include "Kernel.h"
#include "KernelObjID.h"

#include "Euclid.h"

bool CVector::Clip (Metric rLength)

//	Clip
//
//	Clips the vector so that it is no longer than the given length.
//	Returns TRUE if we had to clip it

	{
	Metric rCurLen2 = (x * x + y * y);
	if (rCurLen2 <= (rLength * rLength))
		return false;

	//	Clip

	Metric rFactor = rLength / sqrt(rCurLen2);
	x = rFactor * x;
	y = rFactor * y;

	return true;
	}

void CVector::GenerateOrthogonals (const CVector &vNormal, Metric *retvPara, Metric *retvPerp) const

//	GenerateOrthogonals
//
//	Generates two orthogonal vectors that would generate this vector
//	where one orthogonal is parallel to the given normal.
//
//	vNormal must be a unit vector.

	{
	*retvPara = Dot(vNormal);
	*retvPerp = Dot(vNormal.Perpendicular());
	}

Metric CVector::Polar (Metric *retrRadius) const

//	Polar
//
//	Returns the angle of the vector (in radians) and optionally the radius.

	{
	if (retrRadius)
		*retrRadius = Length();

	return mathAngleMod(atan2(y, x));
	}

CVector CVector::Rotate (Metric rRadians) const

//	Rotate
//
//	Returns the vector rotated by the given angle

	{
	return CVector(x * cos(rRadians) - y * sin(rRadians),
			x * sin(rRadians) + y * cos(rRadians));
	}

CVector CVector::Rotate (int iAngle) const

//	Rotate
//
//	Returns the vector rotated by the given angle

	{
	return CVector(x * g_Cosine[AngleMod(iAngle)] - y * g_Sine[AngleMod(iAngle)],
			x * g_Sine[AngleMod(iAngle)] + y * g_Cosine[AngleMod(iAngle)]);
	}

void IntPolarToVector (int iAngle, Metric rRadius, int *iox, int *ioy)

//	IntPolarToVector
//
//	PolarToVector using integers

	{
	*iox = (int)(rRadius * g_Cosine[AngleMod(iAngle)]);
	*ioy = (int)(rRadius * g_Sine[AngleMod(iAngle)]);
	}

int IntProportionalTransition (int iFrom, int iTo, int iPercent)

//  IntProportionalTransition
//
//  Moves from iFrom to iTo by a fraction of the distance between them. We 
//  guarantee that we always make progress (unless we're at to).

    {
    int iDiff = iTo - iFrom;
    if (iDiff == 0)
        return iTo;

    int iMove = iDiff * iPercent / 100;
    if (iMove == 0)
        return iFrom + (iDiff > 0 ? 1 : -1);

    return iFrom + iMove;
    }

int IntVectorToPolar (int x, int y, int *retiRadius)

//	IntVectorToPolar
//
//	Returns the angle from 0-359 and radius

	{
	int iSqrRadius = (x * x) + (y * y);

	//	If we are at the origin then the angle is undefined

	if (iSqrRadius == 0)
		{
		if (retiRadius)
			*retiRadius = 0;
		return 0;
		}

	//	Compute angle

	Metric rAngle = atan2(y, x);
	int iAngle = AngleToDegrees(rAngle);

	//	Compute radius

	if (retiRadius)
		*retiRadius = mathSqrt(iSqrRadius);

	//	Done

	return iAngle;
	}

CVector PolarToVector (int iAngle, Metric rRadius)

//	PolarToVector
//
//	Creates a vector from polar coordinates. iAngle is an angle from
//	0-359 with 0 pointing to the right and going counter-clockwise.
//	rRadius is a magnitude.

	{
	return CVector(rRadius * g_Cosine[AngleMod(iAngle)], rRadius * g_Sine[AngleMod(iAngle)]);
	}

CVector PolarToVectorRadians (Metric rRadians, Metric rRadius)

//	PolarToVector
//
//	Creates a vector from polar coordinates.

	{
	return CVector(rRadius * cos(rRadians), rRadius * sin(rRadians));
	}

int VectorToPolar (const CVector &vP, Metric *retrRadius)

//	VectorToPolar
//
//	Converts from a vector to polar coordinates (see PolarToVector)

	{
	Metric rAngle = atan2(vP.GetY(), vP.GetX());

	if (retrRadius)
		*retrRadius = vP.Length();

	return AngleToDegrees(rAngle);
	}

Metric VectorToPolarRadians (const CVector &vP, Metric *retrRadius)

//	VectorToPolarRadians
//
//	Converts from a vector to polar coordinates (see PolarToVector)

	{
	//	LATER: Swtich to atan2 when we verify that all the callers can deal with
	//	the result (i.e., can deal with negative values, etc.)

#ifdef USE_ATAN2
	Metric rAngle = atan2(vP.GetY(), vP.GetX());

	if (retrRadius)
		*retrRadius = vP.Length();

	return rAngle;
#else
	Metric rAngle;
	Metric rRadius;
	Metric rSqrRadius = vP.Dot(vP);

	//	If we are at the origin then the angle is undefined

	if (rSqrRadius == 0.0)
		{
		rAngle = 0.0;
		rRadius = 0.0;
		}
	else
		{
		rRadius = sqrt(rSqrRadius);
		if (vP.GetX() >= 0.0)
			rAngle = (vP.GetY() >= 0.0 ? asin(vP.GetY() / rRadius) : (2 * PI) + asin(vP.GetY() / rRadius));
		else
			rAngle = PI - asin(vP.GetY() / rRadius);
		}

	//	Done

	if (retrRadius)
		*retrRadius = rRadius;

	return rAngle;
#endif
	}

