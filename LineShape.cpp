/**
 * @file LineShape.cpp
 * @brief Implements the line shape backed by `QGraphicsLineItem`.
 * @author Nikol Grigoryan
 */
#include "LineShape.h"
#include <QPen>

/**
 * @brief Constructs a line shape using two endpoints.
 * @param name Unique logical name for the shape.
 * @param p1 First endpoint of the line segment.
 * @param p2 Second endpoint of the line segment.
 */
LineShape::LineShape(const QString& name, const QPointF& p1, const QPointF& p2)
    : ShapeBase(name), m_item(new QGraphicsLineItem(QLineF(p1, p2))), m_p1(p1), m_p2(p2)
{
    // Style the line for better visibility
    m_item->setPen(QPen(Qt::blue, 2.0));
}

/**
 * @brief Computes the midpoint of the stored endpoints.
 * @return Scene coordinate of the line's midpoint.
 */
QPointF LineShape::center() const
{
    // Compute midpoint between endpoints
    return QPointF((m_p1.x() + m_p2.x()) / 2.0, (m_p1.y() + m_p2.y()) / 2.0);
}
