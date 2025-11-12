/**
 * @file Utility.cpp
 * @brief Implements geometry helper routines for ObjectDrawer validation logic.
 * @author Nikol Grigoryan
 */
#include "Utility.h"
#include <QtMath>
#include <algorithm>

namespace Utility {

/**
 * @brief Computes the vector difference `a - b`.
 */
QPointF sub(const QPointF& a, const QPointF& b) {
    return QPointF(a.x() - b.x(), a.y() - b.y());
}

/**
 * @brief Returns the dot product of vectors represented by two points.
 */
double dot(const QPointF& a, const QPointF& b) {
    return a.x()*b.x() + a.y()*b.y();
}

/**
 * @brief Calculates the squared Euclidean distance between two points.
 */
double dist2(const QPointF& a, const QPointF& b) {
    const QPointF v = sub(a,b);
    return v.x()*v.x() + v.y()*v.y();
}

/**
 * @brief Determines whether three points reside on the same line.
 */
bool areCollinear(const QPointF& a, const QPointF& b, const QPointF& c, double eps)
{
    // Area of triangle via cross product magnitude
    const QPointF ab = sub(b, a);
    const QPointF ac = sub(c, a);
    const double cross = ab.x()*ac.y() - ab.y()*ac.x();
    return qFabs(cross) < eps;
}

/**
 * @brief Internal helper that checks whether the angle at `b` is right.
 */
static bool isRightAngle(const QPointF& a, const QPointF& b, const QPointF& c, double eps)
{
    // Checks if angle at b is right: (a-b) dot (c-b) == 0
    const QPointF v1 = sub(a, b);
    const QPointF v2 = sub(c, b);
    return qFabs(dot(v1, v2)) < eps;
}

/**
 * @brief Evaluates whether four points form a rectangle.
 */
bool isRectangle(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4, double eps)
{
    // A robust approach: sort points by x,y to get a consistent order,
    // then check opposite sides equal and all angles right.

    QVector<QPointF> pts { p1, p2, p3, p4 };
    std::sort(pts.begin(), pts.end(), [](const QPointF& a, const QPointF& b){
        if (a.x() == b.x()) return a.y() < b.y();
        return a.x() < b.x();
    });

    const QPointF A = pts[0]; // bottom-left
    const QPointF B = pts[1]; // bottom-right or top-left depending on sort
    const QPointF C = pts[2];
    const QPointF D = pts[3];

    // Try to arrange them as rectangle corners by proximity
    // Simple heuristic: A (min x,y), D (max x,y). Then two others B,C.
    // Check right angles at each corner using vectors.
    const bool rightA = isRightAngle(B, A, C, eps) || isRightAngle(C, A, B, eps);
    const bool rightD = isRightAngle(B, D, C, eps) || isRightAngle(C, D, B, eps);

    // Opposite sides equal length (using squared distances)
    const double dAB = dist2(A, B);
    const double dCD = dist2(C, D);
    const double dAC = dist2(A, C);
    const double dBD = dist2(B, D);

    const bool oppEqual = qFabs(dAB - dCD) < eps && qFabs(dAC - dBD) < eps;

    return rightA && rightD && oppEqual;
}

/**
 * @brief Evaluates whether four points form a square.
 */
bool isSquare(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4, double eps)
{
    // Square: rectangle + all sides equal
    QVector<QPointF> pts { p1, p2, p3, p4 };
    std::sort(pts.begin(), pts.end(), [](const QPointF& a, const QPointF& b){
        if (a.x() == b.x()) return a.y() < b.y();
        return a.x() < b.x();
    });

    const QPointF A = pts[0];
    const QPointF B = pts[1];
    const QPointF C = pts[2];
    const QPointF D = pts[3];

    // Check rectangle property via right angles and opposite sides equality
    if (!isRectangle(A,B,C,D,eps)) return false;

    // Check all sides equal: compute nearest neighbor distances
    const double s1 = qMin(dist2(A,B), dist2(A,C));
    const double s2 = qMin(dist2(D,B), dist2(D,C));
    return qFabs(s1 - s2) < eps && s1 > eps;
}

/**
 * @brief Verifies that two points can serve as a valid diagonal of a square.
 */
bool isValidSquareDiagonal(const QPointF& d1, const QPointF& d2, double eps)
{
    // Diagonal must have non-zero length
    return dist2(d1, d2) > eps;
}

} // namespace Utility
