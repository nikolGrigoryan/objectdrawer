/**
 * @file CommandDispatcher.h
 * @brief Declares the dispatcher responsible for executing parsed commands in ObjectDrawer.
 * @author Nikol Grigoryan
 */
#pragma once

#include <QString>
#include <QGraphicsScene>
#include "CommandParser.h"
#include "ShapeRepository.h"

/**
 * @class CommandDispatcher
 * @brief Routes parsed commands to specific handlers and coordinates shape creation.
 *
 * The dispatcher validates user input, instantiates shape objects, registers them with
 * the repository, and supports batch execution through command scripts.
 */
class CommandDispatcher
{
public:
    /**
     * @brief Creates a dispatcher bound to a graphics scene and repository.
     * @param scene Target scene where shapes and connections are rendered.
     * @param repo Repository that stores shape instances by name.
     */
    CommandDispatcher(QGraphicsScene* scene, ShapeRepository* repo);

    /**
     * @brief Executes a parsed command.
     * @param cmd Parsed command information.
     * @param message Receives descriptive feedback regarding success or failure.
     * @return `true` when the command is processed successfully.
     */
    bool execute(const Command& cmd, QString& message);

private:
    QGraphicsScene* m_scene;
    ShapeRepository* m_repo;

    /// @name Command Handlers
    /// @{
    bool handleCreateLine(const Command& cmd, QString& msg);
    bool handleCreateTriangle(const Command& cmd, QString& msg);
    bool handleCreateRectangle(const Command& cmd, QString& msg);
    bool handleCreateSquare(const Command& cmd, QString& msg);
    bool handleConnect(const Command& cmd, QString& msg);
    bool handleExecuteFile(const Command& cmd, QString& msg);
    /// @}

    /// @name Common Helpers
    /// @{
    /**
     * @brief Ensures the command contains a non-empty `-name` argument.
     * @param cmd Command under validation.
     * @param nameOut Receives the trimmed name.
     * @param msg Describes validation errors.
     * @return `true` when a valid name is present.
     */
    bool requireName(const Command& cmd, QString& nameOut, QString& msg) const;
    /**
     * @brief Verifies that the provided name is unique within the repository.
     * @param name Candidate shape name.
     * @param msg Describes uniqueness violations.
     * @return `true` if the name is available.
     */
    bool validateUniqueName(const QString& name, QString& msg) const;
    /**
     * @brief Retrieves a coordinate argument by key.
     * @param cmd Command under validation.
     * @param key Coordinate key without leading dash (e.g., `coord_1`).
     * @param out Receives the coordinate value.
     * @param msg Describes missing coordinates.
     * @return `true` when the coordinate exists.
     */
    bool requireCoord(const Command& cmd, const QString& key, QPointF& out, QString& msg) const;
    /// @}
};
