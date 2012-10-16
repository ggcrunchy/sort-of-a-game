/********************************************************************
*																	*
*							Mathematics.c							*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains implementation of mathematics tool			*
*																	*
********************************************************************/

#include "Mathematics.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Globals									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

static pPoint2 G_endPoint2;
// Used to quicken Point2 loops
static pPoint3 G_endPoint3;
// Used to quicken Point3 loops
static pVector2 G_endVector2;
// Used to quicken Vector2 loops
static pVector3 G_endVector3;
// Used to quicken Vector3 loops

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Mathematics access functions			*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Geometry								*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/********************************************************************
*																	*
*							Curves									*
*																	*
********************************************************************/

/********************************************************************
*	Bezier2 - Bézier curve											*
*	Input:	Start and end points, array of control points, and a	*
*			 value to evaluate the curve at							*
*	Output:	Point2 that curve evaluates to at given parameter		*
********************************************************************/

Point2 Bezier2 (Curve2 curve, float t)
{
	Point2 point;						// Point to be returned
	pPoint2 control = curve.control;	// Storage for control points
	double A, B, C, D, E, F;			// Coefficients of curve terms
	double diff = (double) (1.0 - t);	// Optimize "1 - t" instances
	double diffsquare = diff * diff;	// Optimize "(1 - t)^2" instances
	double square = (double) (t * t);	// Optimize "t * t" instances

	assert (control);
	// Verify that control points to valid memory

	switch (curve.mode)	// Get Bezier mode
	{
	case kQuadratic:	// Quadratic curve
		A = diffsquare;		// Compute A coefficient
		B = 2.0 * t * diff;	// Compute B coefficient
		C = square;			// Compute C coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * curve.end.y);	// Calculate y coordinate

		break;	// Break out of switch statement

	case kCubic:		// Cubic curve
		A = diffsquare * diff;		// Compute A coefficient
		B = 3.0 * diffsquare * t;	// Compute B coefficient
		C = 3.0 * diff * square;	// Compute C coefficient
		D = square * t;				// Compute D coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * (control + 1)->x + D * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * (control + 1)->y + D * curve.end.y);	// Calculate y coordinate

		break;	// Break out of switch statement

	case kQuartic:		// Quartic curve
		A = diffsquare * diffsquare;		// Compute A coefficient
		B = 4.0 * t * diffsquare * diff;	// Compute B coefficient
		C = 6.0 * square * diffsquare;		// Compute C coefficient
		D = 4.0 * square * t * diff;		// Compute D coefficient
		E = square * square;				// Compute E coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * (control + 1)->x + D * (control + 2)->x + E * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * (control + 1)->y + D * (control + 2)->y + E * curve.end.y);	// Calculate y coordinate

		break;	// Break out of switch statement

	case kQuintic:		// Quintic curve
		A = diffsquare * diffsquare * diff;		// Compute A coefficient
		B = 5.0 * t * diffsquare * diffsquare;	// Compute B coefficient
		C = 10.0 * square * diffsquare * diff;	// Compute C coefficient
		D = 10.0 * square * t * diffsquare;		// Compute D coefficient
		E = 5.0 * square * square * diff;		// Compute E coefficient
		F = square * square * t;				// Compute F coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * (control + 1)->x + D * (control + 2)->x + E * (control + 3)->x + F * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * (control + 1)->y + D * (control + 2)->y + E * (control + 3)->y + F * curve.end.y);	// Calculate y coordinate

		break;	// Break out of switch statement

	default:
		NOBREAK_MESSAGE("Unsupported mode: Bezier2 failed","1");
		ZeroMemory (&point, sizeof (Point2));	// Invalid case

		break;	// Break out of switch statement
	}

	return point;
	// Return point evaluated at parameter t
}

/********************************************************************
*	Bezier3 - Bézier curve											*
*	Input:	Start and end points, array of control points, and a	*
*			value to evaluate the curve at							*
*	Output:	Point3 that curve evaluates to at given parameter		*
********************************************************************/

Point3 Bezier3 (Curve3 curve, float t)
{
	Point3 point;						// Point to be returned
	pPoint3 control = curve.control;	// Storage for control points
	double A, B, C, D, E, F;			// Coefficients of curve terms
	double diff = (double) (1.0 - t);	// Optimize "1 - t" instances
	double diffsquare = diff * diff;	// Optimize "(1 - t)^2" instances
	double square = (double) (t * t);	// Optimize "t * t" instances

	assert (control);
	// Verify that control points to valid memory

	switch (curve.mode)	// Get Bezier mode
	{
	case kQuadratic:	// Quadratic curve
		A = diffsquare;		// Compute A coefficient
		B = 2.0 * t * diff;	// Compute B coefficient
		C = square;			// Compute C coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * curve.end.y);	// Calculate y coordinate
		point.z = (float) (A * curve.start.z + B * control->z + C * curve.end.z);	// Calculate z coordinate

		break;	// Break out of switch statement

	case kCubic:		// Cubic curve
		A = diffsquare * diff;		// Compute A coefficient
		B = 3.0 * diffsquare * t;	// Compute B coefficient
		C = 3.0 * diff * square;	// Compute C coefficient
		D = square * t;				// Compute D coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * (control + 1)->x + D * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * (control + 1)->y + D * curve.end.y);	// Calculate y coordinate
		point.z = (float) (A * curve.start.z + B * control->z + C * (control + 1)->z + D * curve.end.z);	// Calculate z coordinate

		break;	// Break out of switch statement

	case kQuartic:		// Quartic curve
		A = diffsquare * diffsquare;		// Compute A coefficient
		B = 4.0 * t * diffsquare * diff;	// Compute B coefficient
		C = 6.0 * square * diffsquare;		// Compute C coefficient
		D = 4.0 * square * t * diff;		// Compute D coefficient
		E = square * square;				// Compute E coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * (control + 1)->x + D * (control + 2)->x + E * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * (control + 1)->y + D * (control + 2)->y + E * curve.end.y);	// Calculate y coordinate
		point.z = (float) (A * curve.start.z + B * control->z + C * (control + 1)->z + D * (control + 2)->z + E * curve.end.z);	// Calculate z coordinate

		break;	// Break out of switch statement

	case kQuintic:		// Quintic curve
		A = diffsquare * diffsquare * diff;		// Compute A coefficient
		B = 5.0 * t * diffsquare * diffsquare;	// Compute B coefficient
		C = 10.0 * square * diffsquare * diff;	// Compute C coefficient
		D = 10.0 * square * t * diffsquare;		// Compute D coefficient
		E = 5.0 * square * square * diff;		// Compute E coefficient
		F = square * square * t;				// Compute F coefficient

		point.x = (float) (A * curve.start.x + B * control->x + C * (control + 1)->x + D * (control + 2)->x + E * (control + 3)->x + F * curve.end.x);	// Calculate x coordinate
		point.y = (float) (A * curve.start.y + B * control->y + C * (control + 1)->y + D * (control + 2)->y + E * (control + 3)->y + F * curve.end.y);	// Calculate y coordinate
		point.z = (float) (A * curve.start.z + B * control->z + C * (control + 1)->z + D * (control + 2)->z + E * (control + 3)->z + F * curve.end.z);	// Calculate z coordinate

		break;	// Break out of switch statement

	default:
		NOBREAK_MESSAGE("Unsupported mode: Bezier3 failed","1");
		ZeroMemory (&point, sizeof (Point3));	// Invalid case

		break;	// Break out of switch statement
	}

	return point;
	// Return point evaluated at parameter t
}

/********************************************************************
*																	*
*							Formulae and operations					*
*																	*
********************************************************************/

/********************************************************************
*	Normalize2 - Normalize the given vector							*
*	Input:	Address of a two-dimensional vector						*
*	Output: Vector is normalized									*
********************************************************************/

void Normalize2 (Vector2 * vector)
{
	float magnitude;

	assert (vector);
	// Verify that vector points to valid memory

	magnitude = (float) Mag2 (*vector);

	vector->x /= magnitude;
	vector->y /= magnitude;
}

/********************************************************************
*	Normalize3 - Normalize the given vector							*
*	Input:	Address of a three-dimensional vector					*
*	Output:	Vector is normalized									*
********************************************************************/

void Normalize3 (Vector3 * vector)
{
	float magnitude;

	assert (vector);
	// Verify that vector points to valid memory

	magnitude = (float) Mag3 (*vector);

	vector->x /= magnitude;
	vector->y /= magnitude;
	vector->z /= magnitude;
}

/********************************************************************
*	Dot2 - Calculate the dot product of two vectors					*
*	Input:	Two 2-dimensional vectors								*
*	Output:	The dot product of the vectors							*
********************************************************************/

double Dot2 (Vector2 vector1, Vector2 vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
	// Return dot product of two vectors
}

/********************************************************************
*	Dot3 - Calculate the dot product of two vectors					*
*	Input:	Two 3-dimensional vectors								*
*	Output:	The dot product of the vectors							*
********************************************************************/

double Dot3 (Vector3 vector1, Vector3 vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
	// Return dot product of two vectors
}

/********************************************************************
*	Cross - Compute the cross product of two vectors				*
*	Input:	Two 3-dimensional vectors								*
*	Output:	The vector perpendicular to both the input vectors		*
********************************************************************/

Vector3 Cross (Vector3 vector1, Vector3 vector2)
{
	Vector3 cross;	// Vector to return as cross product of two vectors

	cross.x = vector1.y * vector2.z - vector1.z * vector2.y;	// Calculate x component of vector
	cross.y = vector1.z * vector2.x - vector1.x * vector2.z;	// Calculate y component of vector
	cross.z = vector1.x * vector2.y - vector1.y * vector2.x;	// Calculate z component of vector

	return cross;
	// Return cross product of two vectors
}

/********************************************************************
*	Angle2 - Compute the angle between two vectors					*
*	Input:	Two 2-dimensional vectors, and angle mode				*
*	Output:	The angle subtended by the vectors						*
********************************************************************/

double Angle2 (Vector2 vector1, Vector2 vector2, AngleMode mode)
{
	double angle = acos (Dot2 (vector1, vector2) / (Mag2 (vector1) * Mag2 (vector2)));	// Angle in given mode between two vectors

	switch (mode)	// Get the angle mode
	{
	case kDegrees:	// Degrees case
		angle = RADTODEG(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kGradians:	// Gradians case
		angle = RADTOGRAD(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kRadians:	// Radians case
		break;	// Break out of switch statement

	default:
		NOBREAK_MESSAGE("Unsupported mode: Angle2 failed","1");
		angle = 0.0;// Invalid case

		break;	// Break out of switch statement
	}

	return angle;
	// Return the angle between vectors in radians
}

/********************************************************************
*	Angle3 - Compute the angle between two vectors					*
*	Input:	Two 3-dimensional vectors, and angle mode				*
*	Output:	The angle subtended by the vectors						*
********************************************************************/

double Angle3 (Vector3 vector1, Vector3 vector2, AngleMode mode)
{
	double angle = acos (Dot3 (vector1, vector2) / (Mag3 (vector1) * Mag3 (vector2)));	// Angle in given mode between two vectors

	switch (mode)	// Get the angle mode
	{
	case kDegrees:	// Degrees case
		angle = RADTODEG(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kGradians:	// Gradians case
		angle = RADTOGRAD(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kRadians:	// Radians case
		break;	// Break out of switch statement

	default:
		NOBREAK_MESSAGE("Unsupported mode: Angle3 failed","1");
		angle = 0.0;// Invalid case

		break;	// Break out of switch statement
	}

	return angle;
	// Return the angle between vectors in radians
}

/********************************************************************
*	Mag2 - Calculate the magnitude of a vector						*
*	Input:	A 2-dimensional vector									*
*	Output:	The magnitude of the vector								*
********************************************************************/

double Mag2 (Vector2 vector)
{
	return sqrt (vector.x * vector.x + vector.y * vector.y);
	// Return the magnitude of the vector
}

/********************************************************************
*	Mag3 - Calculate the magnitude of a vector						*
*	Input:	A 3-dimensional vector									*
*	Output:	The magnitude of the vector								*
********************************************************************/

double Mag3 (Vector3 vector)
{
	return sqrt (vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	// Return the magnitude of the vector
}

/********************************************************************
*	Map2 - Maps a point and / or vector into a different space		*
*	Input:	A basis, 2-dimensional point/vector to map				*
*	Output:	Point and / or vector is mapped							*
********************************************************************/

void Map2 (Vector2 const * basis, Point2 * point, Vector2 * vector)
{
	float pointX, pointY;	// Storage for point coordinates
	float vectorX, vectorY;	// Storage for vector components

	assert (basis);
	// Verify that basis points to valid memory

	if (point)	// Ensure that point points to something
	{
		pointX = point->x;	// Store x coordinate of point
		pointY = point->y;	// Store y coordinate of point

		point->x = basis->x * pointX + (basis + 1)->x * pointY;
		point->y = basis->y * pointX + (basis + 1)->y * pointY;
		// Map point into space defined by basis
	}

	if (vector)	// Ensure that vector points to something
	{
		vectorX = vector->x;// Store x component of vector
		vectorY = vector->y;// Store y component of vector

		vector->x = basis->x * vectorX + (basis + 1)->x * vectorY;
		vector->y = basis->y * vectorX + (basis + 1)->y * vectorY;
		// Map vector into space defined by basis
	}
}

/********************************************************************
*	Map3 - Maps a point and / or vector into a different space		*
*	Input:	A basis, 3-dimensional point/vector to map				*
*	Output:	Point and / or vector is mapped							*
********************************************************************/

void Map3 (Vector3 const * basis, Point3 * point, Vector3 * vector)
{
	float pointX, pointY, pointZ;	// Storage for point coordinates
	float vectorX, vectorY, vectorZ;// Storage for vector components

	assert (basis);
	// Verify that basis points to valid memory

	if (point)	// Verify that point points to valid memory
	{
		pointX = point->x;	// Store x coordinate of point
		pointY = point->y;	// Store y coordinate of point
		pointZ = point->z;	// Store z coordinate of point

		point->x = basis->x * pointX + (basis + 1)->x * pointY + (basis + 2)->x * pointZ;
		point->y = basis->y * pointX + (basis + 1)->y * pointY + (basis + 2)->y * pointZ;
		point->z = basis->z * pointX + (basis + 1)->z * pointY + (basis + 2)->z * pointZ;
		// Map point into space defined by basis
	}

	if (vector)	// Verify that vector points to valid memory
	{
		vectorX = vector->x;// Store x component of vector
		vectorY = vector->y;// Store y component of vector
		vectorZ = vector->z;// Store z component of vector

		vector->x = basis->x * vectorX + (basis + 1)->x * vectorY + (basis + 2)->x * vectorZ;
		vector->y = basis->y * vectorX + (basis + 1)->y * vectorY + (basis + 2)->y * vectorZ;
		vector->z = basis->z * vectorX + (basis + 1)->z * vectorY + (basis + 2)->z * vectorZ;
		// Map vector into space defined by basis
	}
}

/********************************************************************
*	Projection2 - Compute the projection of a point					*
*	Input:	A point, and a vector to project onto					*
*	Output:	Point of projection										*
********************************************************************/

Point2 Projection2 (Point2 point, Vector2 base)
{
	double scalar;							// Storage for vector scalar
	Point2 projection;						// Point of projection to be returned
	Vector2 vector = CastToVector2 (point);	// Point cast as vector

	scalar = Dot2 (vector, base) / (Mag2 (base) * Mag2 (base));
	// Compute scalar

	projection.x = (float) (scalar * base.x);	// Calculate x coordinate
	projection.y = (float) (scalar * base.y);	// Calculate y coordinate

	return projection;
	// Return point of projection
}

/********************************************************************
*	Projection3 - Compute the projection of a point					*
*	Input:	A point, a direction vector, and a normal to a plane	*
*	Output: Point of projection										*
********************************************************************/

Point3 Projection3 (Point3 point, Vector3 direction, Vector3 normal)
{
	double scalar = Dot3 (direction, normal);	// Compute scalar value used in matrix
	double vectorScalar;						// Vector scalar
	Point3 projection;							// Point of projection to be returned
	Vector3 basis [3], vectorCast;				// A matrix, and a vector to cast point to

	vectorCast = CastToVector3 (normal.tail);	// Cast point to vector

	vectorScalar = Dot3 (vectorCast, normal) / scalar;
	// Compute vector scalar

	basis->x = (float) (1.0 - (direction.x * normal.x) / scalar);		// Row 1, Column 1 of matrix
	(basis + 1)->x = (float) ((direction.x * normal.y) / scalar);		// Row 1, Column 2 of matrix
	(basis + 2)->x = (float) ((direction.x * normal.z) / scalar);		// Row 1, Column 3 of matrix
	basis->y = (float) ((direction.y * normal.x) / scalar);				// Row 2, Column 1 of matrix
	(basis + 1)->y = (float) (1.0 - (direction.y * normal.y) / scalar);	// Row 2, Column 2 of matrix
	(basis + 2)->y = (float) ((direction.y * normal.z) / scalar);		// Row 2, Column 3 of matrix
	basis->z = (float) ((direction.z * normal.x) / scalar);				// Row 3, Column 1 of matrix
	(basis + 1)->z = (float) ((direction.z * normal.y) / scalar);		// Row 3, Column 2 of matrix
	(basis + 2)->z = (float) (1.0 - (direction.z * normal.z) / scalar);	// Row 3, Column 3 of matrix

	Map3 (basis, &point, NULL);
	// Map pointCopy into a new space

	projection.x = (float) (point.x + vectorScalar * direction.x);	// Compute the x coordinate of projection
	projection.y = (float) (point.y + vectorScalar * direction.y);	// Compute the y coordinate of projection
	projection.z = (float) (point.z + vectorScalar * direction.z);	// Compute the z coordinate of projection

	return projection;
	// Return point of projection
}

/********************************************************************
*	PerspectiveProjection - Compute point's perspective projection	*
*	Input:	A point, and a normal to a plane						*
*	Output:	Point of perspective projection							*
********************************************************************/

Point3 PerspectiveProjection (Point3 point, Vector3 normal)
{
	double scalar;		// Scalar value
	Vector3 vectorCast;	// A vector to cast point to
	Point3 projection;	// Point of perspective projection to be returned

	vectorCast = CastToVector3 (normal.tail);	// Cast point to vector

	scalar = Dot3 (vectorCast, normal);
	// Compute first portion of scalar value

	vectorCast = CastToVector3 (point);			// Cast point to vector

	scalar /= Dot3 (vectorCast, normal);
	// Compute second portion of scalar value

	projection.x = (float) (scalar * point.x);	// Compute x coordinate of projection
	projection.y = (float) (scalar * point.y);	// Compute y coordinate of projection
	projection.z = (float) (scalar * point.z);	// Compute z coordinate of projection

	return projection;
	// Return point of perspective projection
}

/********************************************************************
*	Rotate2 - Rotate a point and / or vector through alpha units	*
*	Input:	A point and / or vector to rotate, a point to rotate	*
*				about, units, angle mode							*
*	Output: Point and / or vector is rotated						*
********************************************************************/

void Rotate2 (Point2 * point, Vector2 * vector, Point2 center, float alpha, AngleMode mode)
{
	double pointX, pointY;	// Storage for point coordinates
	double vectorX, vectorY;// Storage for vector components
	double cosine, sine;	// Storage for cosine and sine of alpha
	double angle = (double) alpha;	// Angle in given mode

	switch (mode)	// Get angle mode
	{
	case kDegrees:	// Degrees case
		angle = DEGTORAD(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kGradians:	// Gradians case
		angle = GRADTORAD(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kRadians:	// Radians case
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: Rotate2 failed","1");
		// Return failure
	}

	cosine = cos (angle);	// Get cosine of angle
	sine = sin (angle);		// Get sine of angle

	if (point)	// Ensure that point points to something
	{
		pointX = point->x - center.x;	// Store x coordinate of point
		pointY = point->y - center.y;	// Store y coordinate of point

		point->x = (float) (cosine * pointX - sine * pointY) + center.x;
		point->y = (float) (sine * pointX + cosine * pointY) + center.y;
		// Rotate point through angle radians
	}

	if (vector)	// Ensure that vector points to something
	{
		vectorX = vector->x - center.x;// Store x component of vector
		vectorY = vector->y - center.y;// Store y component of vector

		Rotate2 (&vector->tail, NULL, center, (float) angle, kRadians);
		// Rotate tail of vector through angle radians, recursively

		vector->x = (float) (cosine * vectorX - sine * vectorY) + center.x;
		vector->y = (float) (sine * vectorX + cosine * vectorY) + center.y;
		// Rotate vector through angle radians
	}
}

/********************************************************************
*	Rotate3 - Rotate a point and / or vector through alpha units	*
*				about a given axis									*
*	Input:	A point and / or vector to rotate, units, rotation axis	*
*				and angle mode										*
*	Output:	Point and / or vector is rotated						*
********************************************************************/

void Rotate3 (Point3 * point, Vector3 * vector, float alpha, Vector3 axis, AngleMode mode)
{
	double pointX, pointY, pointZ;		// Storage for point coordinates
	double vectorX, vectorY, vectorZ;	// Storage for vector components
	double cosine, sine;				// Storage for cosine and sine of alpha
	double angle = (double) alpha;		// Angle in given mode
	double A1, A2, A3;					// Components of matrix's first column
	double B1, B2, B3;					// Components of matrix's second column
	double C1, C2, C3;					// Components of matrix's third column

	switch (mode)	// Get angle mode
	{
	case kDegrees:	// Degrees case
		angle = DEGTORAD(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kGradians:	// Gradians case
		angle = GRADTORAD(angle);
		// Convert angle to radians

		break;	// Break out of switch statement

	case kRadians:	// Radians case
		break;	// Break out of switch statement

	default:
		NORET_MESSAGE("Unsupported mode: Rotate3 failed","1");
		// Return failure
	}

	// Initialization block
	{
		double a1a1 = axis.x * axis.x;		// Optimize "a1a1" instances
		double a2a2 = axis.y * axis.y;		// Optimize "a2a2" instances
		double a3a3 = axis.z * axis.z;		// Optimize "a3a3" instances
		double diff;						// Optimize "1.0 - cosine" instances

		cosine = cos (angle);	// Get cosine of angle
		sine = sin (angle);		// Get sine of angle

		diff = 1.0 - cosine;	// Store away value in diff

		// Nested initialization block
		{
			double a1a2diff = (axis.x * axis.y) * diff;	// Optimize "a1a2 * diff" instances
			double a1a3diff = (axis.x * axis.z) * diff;	// Optimize "a1a3 * diff" instances
			double a2a3diff = (axis.y * axis.z) * diff;	// Optimize "a2a3 * diff" instances
			double xSine = axis.x * sine;				// Optimize "a1 * sine" instances
			double ySine = axis.y * sine;				// Optimize "a2 * sine" instances
			double zSine = axis.z * sine;				// Optimize "a3 * sine" instances

			A1 = a1a1 + cosine * (1.0 - a1a1);	// Row 1, Column 1 of matrix
			B1 = a1a2diff - zSine;				// Row 1, Column 2 of matrix
			C1 = a1a3diff + ySine;				// Row 1, Column 3 of matrix
			A2 = a1a2diff + zSine;				// Row 2, Column 1 of matrix
			B2 = a2a2 + cosine * (1.0 - a2a2);	// Row 2, Column 2 of matrix
			C2 = a2a3diff - xSine;				// Row 2, Column 3 of matrix
			A3 = a1a3diff - ySine;				// Row 3, Column 1 of matrix
			B3 = a2a3diff + xSine;				// Row 3, Column 2 of matrix
			C3 = a3a3 + cosine * (1.0 - a3a3);	// Row 3, Column 3 of matrix
		}
	}

	if (point)	// Ensure that point points to something
	{
		pointX = point->x;	// Store x coordinate of point
		pointY = point->y;	// Store y coordinate of point
		pointZ = point->z;	// Store z coordinate of point

		point->x = (float) (A1 * pointX + B1 * pointY + C1 * pointZ);
		point->y = (float) (A2 * pointX + B2 * pointY + C2 * pointZ);
		point->z = (float) (A3 * pointX + B3 * pointY + C3 * pointZ);
		// Rotate point through angle radians
	}

	if (vector)	// Ensure that vector points to something
	{
		vectorX = vector->x;// Store x component of vector
		vectorY = vector->y;// Store y component of vector
		vectorZ = vector->z;// Store z component of vector

		Rotate3 (&vector->tail, NULL, (float) angle, axis, kRadians);
		// Rotate tail of vector through angle radians, recursively

		vector->x = (float) (A1 * vectorX + B1 * vectorY + C1 * vectorZ);
		vector->y = (float) (A2 * vectorX + B2 * vectorY + C2 * vectorZ);
		vector->z = (float) (A3 * vectorX + B3 * vectorY + C3 * vectorZ);
		// Rotate vector through angle radians
	}
}

/********************************************************************
*	xRotate3 - Rotate a series of points and / or vectors about the	*
*				x axis												*
*	Input:	Array of points and / or vectors to rotate, cosine and	*
*				sine values, and a count of entries to rotate		*
*	Output:	Points and / or vectors are rotated	about the x axis	*
********************************************************************/

void xRotate3 (Point3 * point, Vector3 * vector, float cosine, float sine, int numEntries)
{
	double pointY, pointZ;		// Storage for point coordinates
	double vectorY, vectorZ;	// Storage for vector components

	if (point)	// Ensure that point points to something
	{
		for (G_endPoint3 = point + numEntries; point < G_endPoint3; point++)
		{
			pointY = point->y;	// Store y coordinate of point
			pointZ = point->z;	// Store z coordinate of point

			point->y = (float) (cosine * pointY - sine * pointZ);
			point->z = (float) (sine * pointY + cosine * pointZ);
			// Rotate point through angle radians
		}
	}

	if (vector)	// Ensure that vector points to something
	{
		for (G_endVector3 = vector + numEntries; vector < G_endVector3; vector++)
		{
			vectorY = vector->y;// Store y component of vector
			vectorZ = vector->z;// Store z component of vector

			xRotate3 (&vector->tail, NULL, cosine, sine, 1);
			// Rotate tail of vector through angle radians, recursively

			vector->y = (float) (cosine * vectorY - sine * vectorZ);
			vector->z = (float) (sine * vectorY + cosine * vectorZ);
			// Rotate vector through angle radians
		}
	}
}

/********************************************************************
*	yRotate3 - Rotate a series of points and / or vectors about the	*
*				y axis												*
*	Input:	Array of points and / or vectors to rotate, cosine and	*
*				sine values, and a count of entries to rotate		*
*	Output:	Points and / or vectors are rotated	about the y axis	*
********************************************************************/

void yRotate3 (Point3 * point, Vector3 * vector, float cosine, float sine, int numEntries)
{
	double pointX, pointZ;		// Storage for point coordinates
	double vectorX, vectorZ;	// Storage for vector components

	if (point)	// Ensure that point points to something
	{
		for (G_endPoint3 = point + numEntries; point < G_endPoint3; point++)
		{
			pointX = point->x;	// Store y coordinate of point
			pointZ = point->z;	// Store z coordinate of point

			point->x = (float) (cosine * pointX - sine * pointZ);
			point->z = (float) (sine * pointX + cosine * pointZ);
			// Rotate point through angle radians
		}
	}

	if (vector)	// Ensure that vector points to something
	{
		for (G_endVector3 = vector + numEntries; vector < G_endVector3; vector++)
		{
			vectorX = vector->x;// Store y component of vector
			vectorZ = vector->z;// Store z component of vector

			yRotate3 (&vector->tail, NULL, cosine, sine, 1);
			// Rotate tail of vector through angle radians, recursively

			vector->x = (float) (cosine * vectorX - sine * vectorZ);
			vector->z = (float) (sine * vectorX + cosine * vectorZ);
			// Rotate vector through angle radians
		}
	}
}

/********************************************************************
*	zRotate3 - Rotate a series of points and / or vectors about the	*
*				z axis												*
*	Input:	Array of points and / or vectors to rotate, cosine and	*
*				sine values, and a count of entries to rotate		*
*	Output:	Points and / or vectors are rotated	about the z axis	*
********************************************************************/

void zRotate3 (Point3 * point, Vector3 * vector, float cosine, float sine, int numEntries)
{
	double pointX, pointY;		// Storage for point coordinates
	double vectorX, vectorY;	// Storage for vector components

	if (point)	// Ensure that point points to something
	{
		for (G_endPoint3 = point + numEntries; point < G_endPoint3; point++)
		{
			pointX = point->x;	// Store y coordinate of point
			pointY = point->y;	// Store z coordinate of point

			point->x = (float) (cosine * pointX - sine * pointY);
			point->y = (float) (sine * pointX + cosine * pointY);
			// Rotate point through angle radians
		}
	}

	if (vector)	// Ensure that vector points to something
	{
		for (G_endVector3 = vector + numEntries; vector < G_endVector3; vector++)
		{
			vectorX = vector->x;// Store y component of vector
			vectorY = vector->y;// Store z component of vector

			zRotate3 (&vector->tail, NULL, cosine, sine, 1);
			// Rotate tail of vector through angle radians, recursively

			vector->x = (float) (cosine * vectorX - sine * vectorY);
			vector->y = (float) (sine * vectorX + cosine * vectorY);
			// Rotate vector through angle radians
		}
	}
}

/********************************************************************
*	Barycenter2 - Compute the barycenter of an array of points		*
*	Input:	An array of points, and a count of points				*
*	Output:	Two-dimensionsal barycenter of point set is returned	*
********************************************************************/

Point2 Barycenter2 (Point2 * point, int numPoints)
{
	Point2 barycenter = {0.0, 0.0};

	assert (point);
	// Verify that point points to valid memory

	for (G_endPoint2 = point + numPoints; point < G_endPoint2; point++)
	{
		barycenter.x += point->x;
		barycenter.y += point->y;
	}

	barycenter.x /= numPoints;
	barycenter.y /= numPoints;

	return barycenter;
}

/********************************************************************
*	Barycenter3 - Compute the barycenter of an array of points		*
*	Input:	An array of points, and a count of points				*
*	Output:	Three-dimensionsal barycenter of point set is returned	*
********************************************************************/

Point3 Barycenter3 (Point3 * point, int numPoints)
{
	Point3 barycenter = {0.0, 0.0, 0.0};

	assert (point);
	// Verify that point points to valid memory

	for (G_endPoint3 = point + numPoints; point < G_endPoint3; point++)
	{
		barycenter.x += point->x;
		barycenter.y += point->y;
		barycenter.z += point->z;
	}

	barycenter.x /= numPoints;
	barycenter.y /= numPoints;
	barycenter.z /= numPoints;

	return barycenter;
}

/********************************************************************
*	DiffPoints2 - Compute the vector spanning two points			*
*	Input:	Starting point and end point							*
*	Output:	Vector as a difference of two points					*
********************************************************************/

Vector2 DiffPoints2 (Point2 start, Point2 end)
{
	Vector2 vector;	// Vector to be returned as difference of points

	vector.tail = start;	// Memberwise copy from start to vector's tail field

	vector.x = end.x - start.x;	// Calculate x component of vector
	vector.y = end.y - start.y;	// Calculate y component of vector

	return vector;
	// Return vector as difference of points
}

/********************************************************************
*	DiffPoints3 - Compute the vector spanning two points			*
*	Input:	Starting point and end point							*
*	Output:	Vector as a difference of two points					*
********************************************************************/

Vector3 DiffPoints3 (Point3 start, Point3 end)
{
	Vector3 vector;	// Vector to be returned as difference of points

	vector.tail = start;	// Memberwise copy from start to vector's tail field

	vector.x = end.x - start.x;	// Calculate x component of vector
	vector.y = end.y - start.y;	// Calculate y component of vector
	vector.z = end.z - start.z;	// Calculate z component of vector

	return vector;
	// Return vector as difference of points
}

/********************************************************************
*	CastToPoint2 - Treat a vector as the origin plus its direction	*
*	Input:	A two-dimensional vector								*
*	Output:	An equivalent two-dimensional point is returned			*
********************************************************************/

Point2 CastToPoint2 (Vector2 vector)
{
	Point2 point;	// Point to be returned

	point.x = vector.x;	// Convert x component of vector to x coordinate of point
	point.y = vector.y; // Convert y component of vector to y coordinate of point

	return point;
	// Return vector cast to point
}

/********************************************************************
*	CastToPoint3 - Treat a vector as the origin plus its direction	*
*	Input:	A three-dimensional vector								*
*	Output:	An equivalent two-dimensional point is returned			*
********************************************************************/

Point3 CastToPoint3 (Vector3 vector)
{
	Point3 point;	// Point to be returned

	point.x = vector.x;	// Convert x component of vector to x coordinate of point
	point.y = vector.y;	// Convert y component of vector to y coordinate of point
	point.z = vector.z;	// Convert z component of vector to z coordinate of point

	return point;
	// Return vector cast as point
}

/********************************************************************
*	CastToVector2 - Treat a point as a displacement from the origin	*
*	Input:	A two-dimensional point									*
*	Output:	An equivalent two-dimensional vector is returned		*
********************************************************************/

Vector2 CastToVector2 (Point2 point)
{
	Vector2 vector;	// Vector to be returned

	vector.tail.x = vector.tail.y = 0;	// Set vector.tail to origin

	vector.x = point.x;	// Convert x coordinate of point to x component of vector
	vector.y = point.y;	// Convert y coordinate of point to y component of vector

	return vector;
	// Return point cast as vector
}

/********************************************************************
*	CastToVector3 - Treat a point as a displacement from the origin	*
*	Input:	A three-dimensional point								*
*	Output:	An equivalent three-dimensional vector is returned		*
********************************************************************/

Vector3 CastToVector3 (Point3 point)
{
	Vector3 vector;	// Vector to be returned

	vector.tail.x = vector.tail.y = vector.tail.z = 0;	// Set vector.tail to origin

	vector.x = point.x;	// Convert x coordinate of point to x component of vector
	vector.y = point.y;	// Convert y coordinate of point to y component of vector
	vector.z = point.z;	// Convert z coordinate of point to z component of vector

	return vector;
	// Return point cast as vector
}

/********************************************************************
*	ScaleVector2 - Compute the scaled version of a vector			*
*	Input:	A two-dimensional vector								*
*	Output:	A two-dimensional vector displaced a scaled distance	*
*				from the original vector's tail						*
********************************************************************/

Vector2 ScaleVector2 (Vector2 vector, float scalar)
{
	Vector2 scaledVector;	// Vector to return as scaled vector

	scaledVector.tail = vector.tail;	// Memberwise copy from vector.tail to scaledVector.tail

	scaledVector.x = scalar * vector.x;	// Scale x component
	scaledVector.y = scalar * vector.y;	// Scale y component

	return scaledVector;
	// Return scaled vector
}

/********************************************************************
*	ScaleVector3 - Compute the scaled version of a vector			*
*	Input:	A three-dimensional vector								*
*	Output:	A three-dimensional vector displaced a scaled distance	*
*				from the original vector's tail						*
********************************************************************/

Vector3 ScaleVector3 (Vector3 vector, float scalar)
{
	Vector3 scaledVector;	// Vector to return as scaled vector

	scaledVector.tail = vector.tail;	// Memberwise copy from vector.tail to scaledVector.tail

	scaledVector.x = scalar * vector.x;	// Scale x component
	scaledVector.y = scalar * vector.y;	// Scale y component
	scaledVector.z = scalar * vector.z;	// Scale z component

	return scaledVector;
	// Return scaled vector
}

/********************************************************************
*	PointPlusVector2 - Compute the displacement from a given point	*
*	Input:	A starting point, and a displacement vector				*
*	Output:	Point displaced from the given point by the vector		*
********************************************************************/

Point2 PointPlusVector2 (Point2 point, Vector2 vector)
{
	Point2 sum;	// Point to return as sum of point and vector

	sum.x = point.x + vector.x;	// Compute x coordinate of sum
	sum.y = point.y + vector.y;	// Compute y coordinate of sum

	return sum;
	// Return point as sum of point and vector
}

/********************************************************************
*	PointPlusVector3 - Compute the displacement from a given point	*
*	Input:	A starting point, and a displacement vector				*
*	Output:	Point displaced from the given point by the vector		*
********************************************************************/

Point3 PointPlusVector3 (Point3 point, Vector3 vector)
{
	Point3 sum;	// Point to return as sum of point and vector

	sum.x = point.x + vector.x;	// Compute x coordinate of sum
	sum.y = point.y + vector.y;	// Compute y coordinate of sum
	sum.z = point.z + vector.z;	// Compute z coordinate of sum

	return sum;
	// Return point as sum of point and vector
}