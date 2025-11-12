/**
 * @file SquareShape.cpp
 * @brief Implements the square shape logic for ObjectDrawer scenes.
 * @author Nikol Grigoryan
 */
#include "SquareShape.h"
#include <QPen>
#include <QBrush>
#include <QtMath>

/**
 * @brief Builds a square from its diagonal endpoints.
 * @param name Unique logical name for the square.
 * @param d1 First diagonal endpoint.
 * @param d2 Opposite diagonal endpoint.
 */
SquareShape::SquareShape(const QString& name, const QPointF& d1, const QPointF& d2)
    : ShapeBase(name), m_item(new QGraphicsPolygonItem())
{
    // Compute square corners from diagonal endpoints.
    // Midpoint M and vector v = d2 - d1, perpendicular vector w.
    const QPointF v(d2.x() - d1.x(), d2.y() - d1.y());
    const QPointF M((d1.x() + d2.x()) / 2.0, (d1.y() + d2.y()) / 2.0);

    // Half-diagonal length
    const double half = qSqrt(v.x()*v.x() + v.y()*v.y()) / 2.0;

    // Unit perpendicular vectors (rotate by 90 degrees)
    const double len = qSqrt(v.x()*v.x() + v.y()*v.y());
    const double ux = (len == 0.0) ? 0.0 : v.x() / len;
    const double uy = (len == 0.0) ? 0.0 : v.y() / len;

    // Perpendicular unit vectors: ( -uy, ux ) and ( uy, -ux )
    const QPointF w1(-uy, ux);
    const QPointF w2(uy, -ux);

    // Half side length s satisfies: diagonal = s*sqrt(2) -> s = diagonal/sqrt(2)
    const double sideHalf = half / qSqrt(2.0);

    // Corners around M: M ± (v/2) ± (w * sideHalf)
    const QPointF a = QPointF(M.x() + v.x()/2.0 + w1.x()*sideHalf, M.y() + v.y()/2.0 + w1.y()*sideHalf);
    const QPointF b = QPointF(M.x() - v.x()/2.0 + w1.x()*sideHalf, M.y() - v.y()/2.0 + w1.y()*sideHalf);
    const QPointF c = QPointF(M.x() - v.x()/2.0 - w1.x()*sideHalf, M.y() - v.y()/2.0 - w1.y()*sideHalf);
    const QPointF d = QPointF(M.x() + v.x()/2.0 - w1.x()*sideHalf, M.y() + v.y()/2.0 - w1.y()*sideHalf);

    m_pts = { a, b, c, d };

    QPolygonF poly;
    for (const auto& p : m_pts) poly << p;
    m_item->setPolygon(poly);
    m_item->setPen(QPen(Qt::magenta, 2.0));
    m_item->setBrush(QBrush(QColor(255, 0, 255, 60)));
}

/**
 * @brief Builds a square using four existing vertices.
 * @param name Unique logical name for the square.
 * @param vertices Set of vertices forming a square.
 */
SquareShape::SquareShape(const QString& name, const QVector<QPointF>& vertices)
    : ShapeBase(name), m_item(new QGraphicsPolygonItem()), m_pts(vertices)
{
    QPolygonF poly;
    for (const auto& p : m_pts) poly << p;
    m_item->setPolygon(poly);
    m_item->setPen(QPen(Qt::magenta, 2.0));
    m_item->setBrush(QBrush(QColor(255, 0, 255, 60)));
}

/**
 * @brief Returns the center of the square using the polygon's bounding rectangle.
 * @return Center point of the square in scene coordinates.
 */
QPointF SquareShape::center() const
{
    const QRectF r = m_item->boundingRect();
    return r.center();
}
