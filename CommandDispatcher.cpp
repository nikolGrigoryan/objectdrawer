/**
 * @file CommandDispatcher.cpp
 * @brief Implements the command execution pipeline for ObjectDrawer commands.
 * @author Nikol Grigoryan
 */
#include "CommandDispatcher.h"
#include <QFile>
#include <QTextStream>
#include "LineShape.h"
#include "TriangleShape.h"
#include "RectangleShape.h"
#include "SquareShape.h"
#include "Utility.h"

/**
 * @brief Initializes the dispatcher with the graphics scene and repository.
 * @param scene Scene used to render shapes and connectors.
 * @param repo Repository that manages shape lifetimes and name lookups.
 */
CommandDispatcher::CommandDispatcher(QGraphicsScene* scene, ShapeRepository* repo)
    : m_scene(scene), m_repo(repo)
{
}

/**
 * @brief Executes a parsed command and routes it to the matching handler.
 * @param cmd Parsed command structure produced by `CommandParser`.
 * @param message Output parameter that captures user-facing feedback.
 * @return `true` when the command completes successfully.
 */
bool CommandDispatcher::execute(const Command& cmd, QString& message)
{
    // Dispatch based on command name; add more as needed
    if (cmd.name == "create_line") {
        return handleCreateLine(cmd, message);
    } else if (cmd.name == "create_triangle") {
        return handleCreateTriangle(cmd, message);
    } else if (cmd.name == "create_rectangle") {
        return handleCreateRectangle(cmd, message);
    } else if (cmd.name == "create_square") {
        return handleCreateSquare(cmd, message);
    } else if (cmd.name == "connect") {
        return handleConnect(cmd, message);
    } else if (cmd.name == "execute_file") {
        return handleExecuteFile(cmd, message);
    }

    message = QString("Unknown command '%1'.").arg(cmd.name);
    return false;
}

/**
 * @brief Validates the presence of a non-empty `-name` parameter in a command.
 * @param cmd Command to inspect.
 * @param nameOut Receives the trimmed name when available.
 * @param msg Provides details when the validation fails.
 * @return `true` when the name exists and is not empty.
 */
bool CommandDispatcher::requireName(const Command& cmd, QString& nameOut, QString& msg) const
{
    // Retrieve and validate a -name flag from command
    if (!cmd.args.contains("name")) {
        msg = "Missing -name flag.";
        return false;
    }
    nameOut = cmd.args["name"].trimmed();
    if (nameOut.isEmpty()) {
        msg = "Name cannot be empty.";
        return false;
    }
    return true;
}

/**
 * @brief Ensures the provided name is unique within the repository.
 * @param name Candidate shape name.
 * @param msg Describes why the validation failed when the method returns `false`.
 * @return `true` if the name is not already registered.
 */
bool CommandDispatcher::validateUniqueName(const QString& name, QString& msg) const
{
    // Ensure shape names are unique to simplify lookup for connect
    if (m_repo->contains(name)) {
        msg = QString("An object named '%1' already exists. Choose a unique name.").arg(name);
        return false;
    }
    return true;
}

/**
 * @brief Retrieves a coordinate value from a parsed command.
 * @param cmd Command containing coordinate arguments.
 * @param key Coordinate key without the leading dash (e.g., `coord_1`).
 * @param out Receives the coordinate if present.
 * @param msg Describes missing arguments on failure.
 * @return `true` when the coordinate exists.
 */
bool CommandDispatcher::requireCoord(const Command& cmd, const QString& key, QPointF& out, QString& msg) const
{
    // Fetch a coordinate (e.g., coord_1) from the parsed command
    if (!cmd.coords.contains(key)) {
        msg = QString("Missing -%1 coordinate.").arg(key);
        return false;
    }
    out = cmd.coords[key];
    return true;
}

/**
 * @brief Handles the `create_line` command.
 * @param cmd Parsed command providing shape name and two coordinates.
 * @param msg Output message describing success or failure.
 * @return `true` when the line is created and registered.
 */
bool CommandDispatcher::handleCreateLine(const Command& cmd, QString& msg)
{
    // Expect: create_line -name NAME -coord_1 {x,y} -coord_2 {x,y}
    QString name;
    if (!requireName(cmd, name, msg) || !validateUniqueName(name, msg)) return false;

    QPointF p1, p2;
    if (!requireCoord(cmd, "coord_1", p1, msg)) return false;
    if (!requireCoord(cmd, "coord_2", p2, msg)) return false;

    // Create shape and add to scene and repo
    auto* shape = new LineShape(name, p1, p2);
    m_scene->addItem(shape->graphicsItem());
    m_repo->add(name, shape);

    msg = QString("Line '%1' created from (%2,%3) to (%4,%5).")
            .arg(name).arg(p1.x()).arg(p1.y()).arg(p2.x()).arg(p2.y());
    return true;
}

/**
 * @brief Handles the `create_triangle` command.
 * @param cmd Parsed command containing a shape name and three coordinates.
 * @param msg Informational message about the operation result.
 * @return `true` if the triangle is valid and created.
 */
bool CommandDispatcher::handleCreateTriangle(const Command& cmd, QString& msg)
{
    // Expect: create_triangle -name NAME -coord_1 -coord_2 -coord_3
    QString name;
    if (!requireName(cmd, name, msg) || !validateUniqueName(name, msg)) return false;

    QPointF p1, p2, p3;
    if (!requireCoord(cmd, "coord_1", p1, msg)) return false;
    if (!requireCoord(cmd, "coord_2", p2, msg)) return false;
    if (!requireCoord(cmd, "coord_3", p3, msg)) return false;

    // Simple non-degenerate validation
    if (Utility::areCollinear(p1, p2, p3)) {
        msg = "Triangle vertices are collinear. Provide non-collinear points.";
        return false;
    }

    auto* shape = new TriangleShape(name, p1, p2, p3);
    m_scene->addItem(shape->graphicsItem());
    m_repo->add(name, shape);

    msg = QString("Triangle '%1' created.").arg(name);
    return true;
}

/**
 * @brief Handles the `create_rectangle` command supporting diagonal or corner input.
 * @param cmd Parsed command with rectangle parameters.
 * @param msg Descriptive feedback about execution.
 * @return `true` when a rectangle is successfully added.
 */
bool CommandDispatcher::handleCreateRectangle(const Command& cmd, QString& msg)
{
    // Supports two forms:
    // 1) Diagonal: -coord_1, -coord_2 (axis-aligned rectangle)
    // 2) Four corners: -coord_1..-coord_4 (validated as rectangle)

    QString name;
    if (!requireName(cmd, name, msg) || !validateUniqueName(name, msg)) return false;

    const bool hasFour =
        cmd.coords.contains("coord_1") &&
        cmd.coords.contains("coord_2") &&
        cmd.coords.contains("coord_3") &&
        cmd.coords.contains("coord_4");

    if (hasFour) {
        QPointF p1 = cmd.coords["coord_1"];
        QPointF p2 = cmd.coords["coord_2"];
        QPointF p3 = cmd.coords["coord_3"];
        QPointF p4 = cmd.coords["coord_4"];

        if (!Utility::isRectangle(p1, p2, p3, p4)) {
            msg = "Provided corners do not form a rectangle.";
            return false;
        }

        auto* shape = new RectangleShape(name, {p1, p2, p3, p4});
        m_scene->addItem(shape->graphicsItem());
        m_repo->add(name, shape);
        msg = QString("Rectangle '%1' created from four corners.").arg(name);
        return true;
    } else {
        QPointF p1, p2;
        if (!requireCoord(cmd, "coord_1", p1, msg)) return false;
        if (!requireCoord(cmd, "coord_2", p2, msg)) return false;

        if (qFuzzyCompare(p1.x(), p2.x()) || qFuzzyCompare(p1.y(), p2.y())) {
            msg = "Diagonal points must differ in both x and y for a valid rectangle.";
            return false;
        }

        auto* shape = new RectangleShape(name, p1, p2);
        m_scene->addItem(shape->graphicsItem());
        m_repo->add(name, shape);
        msg = QString("Rectangle '%1' created from diagonal points.").arg(name);
        return true;
    }
}

/**
 * @brief Handles the `create_square` command in both diagonal and vertex modes.
 * @param cmd Parsed command with square parameters.
 * @param msg Descriptive feedback about execution.
 * @return `true` when the square is successfully added.
 */
bool CommandDispatcher::handleCreateSquare(const Command& cmd, QString& msg)
{
    // Supports two forms:
    // 1) Diagonal: -coord_1, -coord_2 (validated equal side lengths)
    // 2) Four vertices: -coord_1..-coord_4 (validated equal sides and right angles)

    QString name;
    if (!requireName(cmd, name, msg) || !validateUniqueName(name, msg)) return false;

    const bool hasFour =
        cmd.coords.contains("coord_1") &&
        cmd.coords.contains("coord_2") &&
        cmd.coords.contains("coord_3") &&
        cmd.coords.contains("coord_4");

    if (hasFour) {
        QPointF p1 = cmd.coords["coord_1"];
        QPointF p2 = cmd.coords["coord_2"];
        QPointF p3 = cmd.coords["coord_3"];
        QPointF p4 = cmd.coords["coord_4"];

        if (!Utility::isSquare(p1, p2, p3, p4)) {
            msg = "Provided vertices do not form a square.";
            return false;
        }

        auto* shape = new SquareShape(name, {p1, p2, p3, p4});
        m_scene->addItem(shape->graphicsItem());
        m_repo->add(name, shape);
        msg = QString("Square '%1' created from four vertices.").arg(name);
        return true;
    } else {
        QPointF p1, p2;
        if (!requireCoord(cmd, "coord_1", p1, msg)) return false;
        if (!requireCoord(cmd, "coord_2", p2, msg)) return false;

        if (!Utility::isValidSquareDiagonal(p1, p2)) {
            msg = "Diagonal points do not define a valid square.";
            return false;
        }

        auto* shape = new SquareShape(name, p1, p2);
        m_scene->addItem(shape->graphicsItem());
        m_repo->add(name, shape);
        msg = QString("Square '%1' created from diagonal points.").arg(name);
        return true;
    }
}

/**
 * @brief Handles the `connect` command to link two shapes by their centers.
 * @param cmd Parsed command identifying the two shape names.
 * @param msg Message describing the result or encountered error.
 * @return `true` on successful connection.
 */
bool CommandDispatcher::handleConnect(const Command& cmd, QString& msg)
{
    // Expect: connect -object_name_1 NAME1 -object_name_2 NAME2
    if (!cmd.args.contains("object_name_1") || !cmd.args.contains("object_name_2")) {
        msg = "Missing -object_name_1 or -object_name_2.";
        return false;
    }
    const QString n1 = cmd.args["object_name_1"];
    const QString n2 = cmd.args["object_name_2"];

    auto* s1 = m_repo->get(n1);
    auto* s2 = m_repo->get(n2);
    if (!s1 || !s2) {
        msg = "One or both objects not found.";
        return false;
    }

    const QPointF c1 = s1->center();
    const QPointF c2 = s2->center();

    // Draw a simple line connecting centers; unmanaged item for simplicity
    auto* item = m_scene->addLine(QLineF(c1, c2), QPen(Qt::darkGray, 1.5, Qt::DashLine));
    Q_UNUSED(item);

    msg = QString("Connected '%1' and '%2' by their centers.").arg(n1, n2);
    return true;
}

/**
 * @brief Handles the `execute_file` command which runs commands from a script.
 * @param cmd Parsed command containing the path to the script file.
 * @param msg Aggregated result messages per processed line.
 * @return `true` if all commands in the file succeed.
 */
bool CommandDispatcher::handleExecuteFile(const Command& cmd, QString& msg)
{
    // Expect: execute_file -file_path PATH
    if (!cmd.args.contains("file_path")) {
        msg = "Missing -file_path.";
        return false;
    }
    const QString path = cmd.args["file_path"];

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        msg = QString("Failed to open script file: %1").arg(path);
        return false;
    }

    QTextStream in(&f);
    int lineNo = 0;
    int successCount = 0;
    int failureCount = 0;
    while (!in.atEnd()) {
        const QString raw = in.readLine().trimmed();
        ++lineNo;
        if (raw.isEmpty()) continue;

        Command c;
        QString parseError;
        if (!CommandParser().parse(raw, c, parseError)) {
            ++failureCount;
            // Log each parse error as a separate message
            msg += QString("\nLine %1 parse error: %2").arg(lineNo).arg(parseError);
            continue;
        }

        QString execMsg;
        if (!execute(c, execMsg)) {
            ++failureCount;
            msg += QString("\nLine %1 failed: %2").arg(lineNo).arg(execMsg);
        } else {
            ++successCount;
        }
    }

    msg = QString("Script executed: %1 successes, %2 failures.%3")
              .arg(successCount).arg(failureCount).arg(msg);
    return failureCount == 0;
}
