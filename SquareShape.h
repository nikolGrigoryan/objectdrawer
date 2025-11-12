/**
 * @file SquareShape.h
 * @brief Declares the square shape rendered as a polygon item in the scene.
 * @author Nikol Grigoryan
 */
#pragma once

#include "ShapeBase.h"
#include <QGraphicsPolygonItem>
#include <QVector>

/**
 * @class SquareShape
 * @brief Represents squares constructed either from a diagonal or explicit vertices.
 *
 * The square is modeled as a polygon item to support custom styling and reuse the
 * bounding rectangle for center computation.
 */
class SquareShape : public ShapeBase
{
public:
    /**
     * @brief Builds a square using its diagonal endpoints.
     * @param name Logical identifier registered in the repository.
     * @param d1 First diagonal endpoint.
     * @param d2 Opposite diagonal endpoint.
     */
    SquareShape(const QString& name, const QPointF& d1, const QPointF& d2);

    /**
     * @brief Builds a square using four pre-validated vertices.
     * @param name Logical identifier registered in the repository.
     * @param vertices Sequence of square vertices.
     */
    SquareShape(const QString& name, const QVector<QPointF>& vertices);

    /**
     * @brief Provides the underlying polygon item for rendering.
     * @return Pointer to the managed `QGraphicsPolygonItem`.
     */
    QGraphicsItem* graphicsItem() const override { return m_item; }

    /**
     * @brief Computes the center using the polygon's bounding rectangle.
     * @return Center point of the square in scene coordinates.
     */
    QPointF center() const override;

private:
    QGraphicsPolygonItem* m_item;
    QVector<QPointF> m_pts;
};
