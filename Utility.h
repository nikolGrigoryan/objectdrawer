/**
 * @file Utility.h
 * @brief Declares geometry helper functions used by ObjectDrawer validation routines.
 * @author Nikol Grigoryan
 */
#pragma once

#include <QPointF>
#include <QVector>

/**
 * @namespace Utility
 * @brief Geometry helper utilities for validating and manipulating shapes.
 */
namespace Utility {

/**
 * @brief Checks whether three points are collinear using a cross-product test.
 * @param a First point.
 * @param b Second point.
 * @param c Third point.
 * @param eps Numerical tolerance applied to the cross-product.
 * @return `true` if the area of the triangle is within the tolerance.
 */
bool areCollinear(const QPointF& a, const QPointF& b, const QPointF& c, double eps = 1e-6);

/**
 * @brief Validates that four points form a rectangle.
 * @param p1 First vertex.
 * @param p2 Second vertex.
 * @param p3 Third vertex.
 * @param p4 Fourth vertex.
 * @param eps Tolerance for numeric comparisons.
 * @return `true` if the points satisfy rectangle constraints.
 */
bool isRectangle(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4, double eps = 1e-6);

/**
 * @brief Validates that four points form a square.
 * @param p1 First vertex.
 * @param p2 Second vertex.
 * @param p3 Third vertex.
 * @param p4 Fourth vertex.
 * @param eps Tolerance for numeric comparisons.
 * @return `true` if the points satisfy square constraints.
 */
bool isSquare(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4, double eps = 1e-6);

/**
 * @brief Tests whether two points may serve as a valid square diagonal.
 * @param d1 First diagonal endpoint.
 * @param d2 Second diagonal endpoint.
 * @param eps Minimum squared distance required between points.
 * @return `true` if the diagonal length exceeds the tolerance.
 */
bool isValidSquareDiagonal(const QPointF& d1, const QPointF& d2, double eps = 1e-9);

/**
 * @brief Computes the squared Euclidean distance between two points.
 */
double dist2(const QPointF& a, const QPointF& b);

/**
 * @brief Calculates the dot product of two vectors represented by points.
 */
double dot(const QPointF& a, const QPointF& b);

/**
 * @brief Produces the vector difference `a - b`.
 */
QPointF sub(const QPointF& a, const QPointF& b);

}
