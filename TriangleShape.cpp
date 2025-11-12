/**
 * @file TriangleShape.cpp
 * @brief Implements the triangle shape rendered with `QGraphicsPolygonItem`.
 * @author Nikol Grigoryan
 */
#include "TriangleShape.h"
#include <QPen>
#include <QBrush>

/**
 * @brief Constructs a triangle from three vertices.
 * @param name Unique logical name for the triangle.
 * @param p1 First vertex.
 * @param p2 Second vertex.
 * @param p3 Third vertex.
 */
TriangleShape::TriangleShape(const QString& name, const QPointF& p1, const QPointF& p2, const QPointF& p3)
    : ShapeBase(name), m_item(new QGraphicsPolygonItem()), m_pts{p1, p2, p3}
{
    // Configure polygon points
    QPolygonF poly;
    poly << p1 << p2 << p3;
    m_item->setPolygon(poly);
    m_item->setPen(QPen(Qt::darkGreen, 2.0));
    m_item->setBrush(QBrush(QColor(0, 180, 0, 60))); // semi-transparent fill
}

/**
 * @brief Computes the triangle centroid.
 * @return Center point of the triangle.
 */
QPointF TriangleShape::center() const
{
    // Centroid of triangle: average of vertices
    const QPointF c(
        (m_pts[0].x() + m_pts[1].x() + m_pts[2].x()) / 3.0,
        (m_pts[0].y() + m_pts[1].y() + m_pts[2].y()) / 3.0
    );
    return c;
}
