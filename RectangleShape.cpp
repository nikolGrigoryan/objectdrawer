/**
 * @file RectangleShape.cpp
 * @brief Implements the rectangle shape logic for ObjectDrawer scenes.
 * @author Nikol Grigoryan
 */
#include "RectangleShape.h"
#include <QPen>
#include <QBrush>

/**
 * @brief Builds an axis-aligned rectangle from two diagonal points.
 * @param name Unique logical name for the rectangle.
 * @param p1 First diagonal endpoint.
 * @param p2 Opposite diagonal endpoint.
 */
RectangleShape::RectangleShape(const QString& name, const QPointF& p1, const QPointF& p2)
    : ShapeBase(name), m_item(new QGraphicsPolygonItem())
{
    // Compute axis-aligned corners from diagonal points
    const double x1 = std::min(p1.x(), p2.x());
    const double x2 = std::max(p1.x(), p2.x());
    const double y1 = std::min(p1.y(), p2.y());
    const double y2 = std::max(p1.y(), p2.y());

    m_pts = { QPointF(x1, y1), QPointF(x2, y1), QPointF(x2, y2), QPointF(x1, y2) };

    QPolygonF poly;
    for (const auto& p : m_pts) poly << p;
    m_item->setPolygon(poly);
    m_item->setPen(QPen(Qt::red, 2.0));
    m_item->setBrush(QBrush(QColor(255, 0, 0, 60)));
}

/**
 * @brief Builds a rectangle from a vetted list of corner points.
 * @param name Unique logical name for the rectangle.
 * @param points Sequence of vertices describing the rectangle.
 */
RectangleShape::RectangleShape(const QString& name, const QVector<QPointF>& points)
    : ShapeBase(name), m_item(new QGraphicsPolygonItem()), m_pts(points)
{
    double minX = points[0].x();
    double maxX = points[0].x();
    double minY = points[0].y();
    double maxY = points[0].y();
    for (const auto &pt : points) {
        minX = qMin(minX, pt.x());
        maxX = qMax(maxX, pt.x());
        minY = qMin(minY, pt.y());
        maxY = qMax(maxY, pt.y());
    }

    QPolygonF polygon;
    polygon << QPointF(minX, minY)
            << QPointF(maxX, minY)
            << QPointF(maxX, maxY)
            << QPointF(minX, maxY);

    m_item->setPolygon(polygon);
    m_item->setPen(QPen(Qt::red, 2.0));
    m_item->setBrush(QBrush(QColor(255, 0, 0, 60)));
}

/**
 * @brief Computes the center using the polygon's bounding rectangle.
 * @return Center point of the rectangle.
 */
QPointF RectangleShape::center() const
{
    // Use bounding rect center; robust for polygons
    const QRectF r = m_item->boundingRect();
    return r.center();
}
