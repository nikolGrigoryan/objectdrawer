/**
 * @file TriangleShape.h
 * @brief Declares the triangle shape abstraction backed by `QGraphicsPolygonItem`.
 * @author Nikol Grigoryan
 */
#pragma once

#include "ShapeBase.h"
#include <QGraphicsPolygonItem>
#include <QVector>
#include <QPointF>

/**
 * @class TriangleShape
 * @brief Models a triangle with three vertices rendered in the graphics scene.
 *
 * The triangle stores its vertices to compute the centroid, enabling the dispatcher
 * to connect shapes using their geometric centers.
 */
class TriangleShape : public ShapeBase
{
public:
    /**
     * @brief Creates a triangle with three non-collinear vertices.
     * @param name Logical identifier assigned to the triangle.
     * @param p1 First vertex.
     * @param p2 Second vertex.
     * @param p3 Third vertex.
     */
    TriangleShape(const QString& name, const QPointF& p1, const QPointF& p2, const QPointF& p3);

    /**
     * @brief Returns the underlying polygon graphics item.
     * @return Pointer to the managed `QGraphicsPolygonItem`.
     */
    QGraphicsItem* graphicsItem() const override { return m_item; }

    /**
     * @brief Computes the centroid of the triangle.
     * @return Centroid expressed in scene coordinates.
     */
    QPointF center() const override;

private:
    QGraphicsPolygonItem* m_item;
    QVector<QPointF> m_pts;
};
