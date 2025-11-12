/**
 * @file ShapeRepository.cpp
 * @brief Implements the shape repository responsible for ownership and lookup operations.
 * @author Nikol Grigoryan
 */
#include "ShapeRepository.h"

/**
 * @brief Releases all shapes owned by the repository.
 */
ShapeRepository::~ShapeRepository()
{
    // Delete all owned shapes on shutdown to prevent memory leaks
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        delete it.value();
    }
}

/**
 * @brief Reports whether a shape with the given name exists.
 * @param name Logical shape name.
 * @return `true` if the name is already present; otherwise `false`.
 */
bool ShapeRepository::contains(const QString& name) const
{
    return m_items.contains(name);
}

/**
 * @brief Stores a shape under a unique name.
 * @param name New shape name. Must not be already present.
 * @param shape Pointer to the shape whose ownership transfers to the repository.
 */
void ShapeRepository::add(const QString& name, ShapeBase* shape)
{
    m_items.insert(name, shape);
}

/**
 * @brief Retrieves a shape by name.
 * @param name Logical shape name.
 * @return Stored shape pointer or `nullptr` when no shape matches.
 */
ShapeBase* ShapeRepository::get(const QString& name) const
{
    auto it = m_items.find(name);
    return it == m_items.end() ? nullptr : it.value();
}
