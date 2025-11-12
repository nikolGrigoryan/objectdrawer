/**
 * @file LineShape.h
 * @brief Declares a line shape rendered by `QGraphicsLineItem` instances.
 * @author Nikol Grigoryan
 */
#pragma once

#include "ShapeBase.h"
#include <QGraphicsLineItem>

/**
 * @class LineShape
 * @brief Represents a line segment between two points.
 *
 * The shape stores the endpoints to provide a midpoint center used when creating
 * connections to other shapes.
 */
class LineShape : public ShapeBase
{
public:
    /**
     * @brief Creates a line shape from two endpoints.
     * @param name Unique logical name for repository lookup.
     * @param p1 Starting endpoint of the segment.
     * @param p2 Ending endpoint of the segment.
     */
    LineShape(const QString& name, const QPointF& p1, const QPointF& p2);

    /**
     * @brief Provides access to the underlying graphics item.
     * @return Pointer to the managed `QGraphicsLineItem`.
     */
    QGraphicsItem* graphicsItem() const override { return m_item; }

    /**
     * @brief Computes the midpoint between the stored endpoints.
     * @return Midpoint between `p1` and `p2`.
     */
    QPointF center() const override;

private:
    QGraphicsLineItem* m_item;
    QPointF m_p1;
    QPointF m_p2;
};
