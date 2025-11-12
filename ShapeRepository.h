/**
 * @file ShapeRepository.h
 * @brief Declares a repository for owning and retrieving shapes by name.
 * @author Nikol Grigoryan
 */
#pragma once

#include <QString>
#include <QMap>
#include "ShapeBase.h"

/**
 * @class ShapeRepository
 * @brief Owns `ShapeBase` instances and exposes name-based lookup.
 *
 * The repository guarantees uniqueness of shape names and releases the owned
 * shapes on destruction to avoid memory leaks.
 */
class ShapeRepository
{
public:
    /**
     * @brief Constructs an empty repository.
     */
    ShapeRepository() = default;

    /**
     * @brief Destroys all stored shapes.
     */
    ~ShapeRepository();

    /**
     * @brief Tests whether a shape with the given name exists.
     * @param name Logical shape name.
     * @return `true` if the repository already contains the name.
     */
    bool contains(const QString& name) const;

    /**
     * @brief Inserts a new shape instance into the repository.
     * @param name Unique logical name. Must not already exist.
     * @param shape Pointer to the caller-created shape. Ownership transfers to the repository.
     */
    void add(const QString& name, ShapeBase* shape);

    /**
     * @brief Retrieves a shape by name.
     * @param name Logical shape name.
     * @return Pointer to the stored shape, or `nullptr` when not found.
     */
    ShapeBase* get(const QString& name) const;

private:
    QMap<QString, ShapeBase*> m_items;
};
