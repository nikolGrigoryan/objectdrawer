/**
 * @file ShapeBase.h
 * @brief Declares the abstract base class for all drawable shapes in ObjectDrawer.
 * @author Nikol Grigoryan
 */
#pragma once

#include <QString>
#include <QGraphicsItem>
#include <QPointF>

/**
 * @class ShapeBase
 * @brief Represents a polymorphic interface for drawable shapes.
 *
 * Each derived shape wraps a `QGraphicsItem` instance that can be inserted into a
 * `QGraphicsScene` and provides a consistent way to obtain the geometric center
 * used for connection operations.
 */
class ShapeBase
{
public:
    /**
     * @brief Constructs a shape with the given logical name.
     * @param name Human-readable identifier used for lookup in `ShapeRepository`.
     */
    explicit ShapeBase(const QString& name) : m_name(name) {}

    /**
     * @brief Virtual destructor to ensure derived classes clean up correctly.
     */
    virtual ~ShapeBase() = default;

    /**
     * @brief Provides access to the underlying item inserted into the scene.
     * @return Pointer to the managed `QGraphicsItem` instance.
     */
    virtual QGraphicsItem* graphicsItem() const = 0;

    /**
     * @brief Computes the geometric center of the shape.
     * @return Center point in scene coordinates.
     */
    virtual QPointF center() const = 0;

    /**
     * @brief Retrieves the logical name of the shape.
     * @return Shape name used for repository lookup.
     */
    QString name() const { return m_name; }

protected:
    QString m_name;
};
