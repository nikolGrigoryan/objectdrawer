/**
 * @file RectangleShape.h
 * @brief Declares the polygon-based rectangle shape abstraction for ObjectDrawer scenes.
 * @author Nikol Grigoryan
 */
#pragma once

#include "ShapeBase.h"
#include <QGraphicsPolygonItem>
#include <QVector>

/**
 * @class RectangleShape
 * @brief Draws axis-aligned rectangles constructed from either diagonals or explicit vertices.
 *
 * The rectangle is stored as a `QGraphicsPolygonItem` which allows consistent styling and
 * straightforward center computation via the bounding rectangle.
 */
class RectangleShape : public ShapeBase
{
public:
    /**
     * @brief Constructs an axis-aligned rectangle by its diagonal points.
     * @param name Unique logical name used in the shape repository.
     * @param p1 First diagonal endpoint.
     * @param p2 Opposite diagonal endpoint.
     */
    RectangleShape(const QString& name, const QPointF& p1, const QPointF& p2);

    /**
     * @brief Constructs a rectangle with the given corner sequence.
     * @param name Unique logical name used in the shape repository.
     * @param corners Collection of four vertices forming a valid rectangle.
     */
    RectangleShape(const QString& name, const QVector<QPointF>& corners);

    /**
     * @brief Provides access to the polygon item inserted into the scene.
     * @return Pointer to the managed `QGraphicsPolygonItem`.
     */
    QGraphicsItem* graphicsItem() const override { return m_item; }

    /**
     * @brief Computes the center using the polygon's bounding rectangle.
     * @return Center point of the rendered polygon.
     */
    QPointF center() const override;

private:
    QGraphicsPolygonItem* m_item;
    QVector<QPointF> m_pts;
};
