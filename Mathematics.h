/********************************************************************
*																	*
*							Mathematics.h							*
*																	*
*	Author:		Steven Johnson										*
*	Purpose:	Contains information relevant to mathematics		*
*																	*
********************************************************************/

#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include "Common.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Defines									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define PI 3.14159
// Ratio of circumference of circle to its diameter

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/********************************************************************
*																	*
*							Macros									*
*																	*
********************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#define DEGTORAD(degrees)	(degrees) * (PI / 180.0)
#define DEGTOGRAD(degrees)	(degrees) * (10.0 / 9.0)
// Conversions from degrees to other units of angle measurement
#define GRADTODEG(gradians)	(gradians) * 0.9)
#define GRADTORAD(gradians)	(gradians) * (PI / 200.0)
// Conversions from gradians to other units of angle measurement
#define RADTODEG(radians)	(radians) * (180.0 / PI) 
#define RADTOGRAD(radians)	(radians) * (200.0 / PI)
// Conversions from radians to other units of angle measurement

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
*			value to evaluate the curve at							*
*	Output:	Point2 that curve evaluates to at given parameter		*
********************************************************************/

Point2 Bezier2 (Curve2 curve, float t);

/********************************************************************
*	Bezier3 - Bézier curve											*
*	Input:	Start and end points, array of control points, and a	*
*			value to evaluate the curve at							*
*	Output:	Point3 that curve evaluates to at given parameter		*
********************************************************************/

Point3 Bezier3 (Curve3 curve, float t);

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

void Normalize2 (pVector2 vector);

/********************************************************************
*	Normalize3 - Normalize the given vector							*
*	Input:	Address of a three-dimensional vector					*
*	Output:	Vector is normalized									*
********************************************************************/

void Normalize3 (pVector3 vector);

/********************************************************************
*	Dot2 - Calculate the dot product of two vectors					*
*	Input:	Two 2-dimensional vectors								*
*	Output:	The dot product of the vectors							*
********************************************************************/

double Dot2 (Vector2 vector1, Vector2 vector2);

/********************************************************************
*	Dot3 - Calculate the dot product of two vectors					*
*	Input:	Two 3-dimensional vectors								*
*	Output:	The dot product of the vectors							*
********************************************************************/

double Dot3 (Vector3 vector1, Vector3 vector2);

/********************************************************************
*	Cross - Compute the cross product of two vectors				*
*	Input:	Two 3-dimensional vectors								*
*	Output:	The vector perpendicular to both the input vectors		*
********************************************************************/

Vector3 Cross (Vector3 vector1, Vector3 vector2);

/********************************************************************
*	Angle2 - Compute the angle between two vectors					*
*	Input:	Two 2-dimensional vectors, and angle mode				*
*	Output:	The angle subtended by the vectors						*
********************************************************************/

double Angle2 (Vector2 vector1, Vector2 vector2, AngleMode mode);

/********************************************************************
*	Angle3 - Compute the angle between two vectors					*
*	Input:	Two 3-dimensional vectors, and angle mode				*
*	Output:	The angle subtended by the vectors						*
********************************************************************/

double Angle3 (Vector3 vector1, Vector3 vector2, AngleMode mode);

/********************************************************************
*	Mag2 - Calculate the magnitude of a vector						*
*	Input:	A 2-dimensional vector									*
*	Output:	The magnitude of the vector								*
********************************************************************/

double Mag2 (Vector2 vector);

/********************************************************************
*	Mag3 - Calculate the magnitude of a vector						*
*	Input:	A 3-dimensional vector									*
*	Output:	The magnitude of the vector								*
********************************************************************/

double Mag3 (Vector3 vector);

/********************************************************************
*	Map2 - Maps a point and / or vector into a different space		*
*	Input:	A basis, 2-dimensional point/vector to map				*
*	Output:	Point and / or vector is mapped							*
********************************************************************/

void Map2 (Vector2 const * basis, pPoint2 point, pVector2 vector);

/********************************************************************
*	Map3 - Maps a point and / or vector into a different space		*
*	Input:	A basis, 3-dimensional point/vector to map				*
*	Output:	Point and / or vector is mapped							*
********************************************************************/

void Map3 (Vector3 const * basis, pPoint3 point, pVector3 vector);

/********************************************************************
*	Projection2 - Compute the projection of a point					*
*	Input:	A point, and a vector to project onto					*
*	Output:	Point of projection										*
********************************************************************/

Point2 Projection2 (Point2 point, Vector2 base);

/********************************************************************
*	Projection3 - Compute the projection of a point					*
*	Input:	A point, a direction vector, and a normal to a plane	*
*	Output: Point of projection										*
********************************************************************/

Point3 Projection3 (Point3 point, Vector3 direction, Vector3 normal);

/********************************************************************
*	PerspectiveProjection - Compute point's perspective projection	*
*	Input:	A point, and a normal to a plane						*
*	Output:	Point of perspective projection							*
********************************************************************/

Point3 PerspectiveProjection (Point3 point, Vector3 normal);

/********************************************************************
*	Rotate2 - Rotate a point and / or vector through alpha units	*
*	Input:	A point and / or vector to rotate, a point to rotate	*
*				about, units, angle mode							*
*	Output: Point and / or vector is rotated						*
********************************************************************/

void Rotate2 (pPoint2 point, pVector2 vector, Point2 center, float alpha, AngleMode mode);

/********************************************************************
*	Rotate3 - Rotate a point and / or vector through alpha units	*
*				about a given axis									*
*	Input:	A point and / or vector to rotate, units, rotation axis	*
*				and angle mode										*
*	Output:	Point and / or vector is rotated						*
********************************************************************/

void Rotate3 (pPoint3 point, pVector3 vector, float alpha, Vector3 axis, AngleMode mode);

/********************************************************************
*	xRotate3 - Rotate a sequence of points and / or vectors through	*
*				cosine and sine units about the x axis				*
*	Input:	An array of points and / or vectors to rotate, cosine,	*
*				sine, and a count of entries to rotate				*
*	Output:	Points and / or vectors are rotated						*
********************************************************************/

void xRotate3 (pPoint3 point, pVector3 vector, float cosine, float sine, int numEntries);

/********************************************************************
*	yRotate3 - Rotate a sequence of points and / or vectors through	*
*				cosine and sine units about the y axis				*
*	Input:	An array of points and / or vectors to rotate, cosine,	*
*				sine, and a count of entries to rotate				*
*	Output:	Points and / or vectors are rotated						*
********************************************************************/

void yRotate3 (pPoint3 point, pVector3 vector, float cosine, float sine, int numEntries);

/********************************************************************
*	zRotate3 - Rotate a sequence of points and / or vectors through	*
*				cosine and sine units about the z axis				*
*	Input:	An array of points and / or vectors to rotate, cosine,	*
*				sine, and a count of entries to rotate				*
*	Output:	Points and / or vectors are rotated						*
********************************************************************/

void zRotate3 (pPoint3 point, pVector3 vector, float cosine, float sine, int numEntries);

/********************************************************************
*	Barycenter2 - Compute the barycenter of an array of points		*
*	Input:	An array of points, and a count of points				*
*	Output:	Two-dimensionsal barycenter of point set is returned	*
********************************************************************/

Point2 Barycenter2 (pPoint2 point, int numPoints);

/********************************************************************
*	Barycenter3 - Compute the barycenter of an array of points		*
*	Input:	An array of points, and a count of points				*
*	Output:	Three-dimensionsal barycenter of point set is returned	*
********************************************************************/

Point3 Barycenter3 (pPoint3 point, int numPoints);

/********************************************************************
*	DiffPoints2 - Compute the vector spanning two points			*
*	Input:	Starting point and end point							*
*	Output:	Vector as a difference of two points					*
********************************************************************/

Vector2 DiffPoints2 (Point2 start, Point2 end);

/********************************************************************
*	DiffPoints3 - Compute the vector spanning two points			*
*	Input:	Starting point and end point							*
*	Output:	Vector as a difference of two points					*
********************************************************************/

Vector3 DiffPoints3 (Point3 start, Point3 end);

/********************************************************************
*	CastToPoint2 - Treat a vector as the origin plus its direction	*
*	Input:	A two-dimensional vector								*
*	Output:	An equivalent two-dimensional point is returned			*
********************************************************************/

Point2 CastToPoint2 (Vector2 vector);

/********************************************************************
*	CastToPoint3 - Treat a vector as the origin plus its direction	*
*	Input:	A three-dimensional vector								*
*	Output:	An equivalent two-dimensional point is returned			*
********************************************************************/

Point3 CastToPoint3 (Vector3 vector);

/********************************************************************
*	CastToVector2 - Treat a point as a displacement from the origin	*
*	Input:	A two-dimensional point									*
*	Output:	An equivalent two-dimensional vector is returned		*
********************************************************************/

Vector2 CastToVector2 (Point2 point);

/********************************************************************
*	CastToVector3 - Treat a point as a displacement from the origin	*
*	Input:	A three-dimensional point								*
*	Output:	An equivalent three-dimensional vector is returned		*
********************************************************************/

Vector3 CastToVector3 (Point3 point);

/********************************************************************
*	ScaleVector2 - Compute the scaled version of a vector			*
*	Input:	A two-dimensional vector								*
*	Output:	A two-dimensional vector displaced a scaled distance	*
*				from the original vector's tail						*
********************************************************************/

Vector2 ScaleVector2 (Vector2 vector, float scalar);

/********************************************************************
*	ScaleVector3 - Compute the scaled version of a vector			*
*	Input:	A three-dimensional vector								*
*	Output:	A three-dimensional vector displaced a scaled distance	*
*				from the original vector's tail						*
********************************************************************/

Vector3 ScaleVector3 (Vector3 vector, float scalar);

/********************************************************************
*	PointPlusVector2 - Compute the displacement from a given point	*
*	Input:	A starting point, and a displacement vector				*
*	Output:	Point displaced from the given point by the vector		*
********************************************************************/

Point2 PointPlusVector2 (Point2 point, Vector2 vector);

/********************************************************************
*	PointPlusVector3 - Compute the displacement from a given point	*
*	Input:	A starting point, and a displacement vector				*
*	Output:	Point displaced from the given point by the vector		*
********************************************************************/

Point3 PointPlusVector3 (Point3 point, Vector3 vector);

#endif