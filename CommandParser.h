/**
 * @file CommandParser.h
 * @brief Declares the command parsing facility for ObjectDrawer commands.
 * @author Nikol Grigoryan
 */
#pragma once

#include <QString>
#include <QMap>
#include <QVector>
#include <QPointF>

/**
 * @struct Command
 * @brief Structured representation of a parsed textual command.
 *
 * The parsing process separates flag-value pairs and coordinates that are later consumed
 * by the dispatcher to create or manipulate shapes.
 */
struct Command
{
    /**
     * @brief Name of the command (e.g., `create_line`).
     */
    QString name;
    /**
     * @brief Arbitrary flag-value pairs such as `-name`.
     */
    QMap<QString, QString> args;
    /**
     * @brief Parsed coordinate values keyed by the flag name without the leading dash.
     */
    QMap<QString, QPointF> coords;
    /**
     * @brief Optional collection of non-fatal parsing messages.
     */
    QVector<QString> errors;
};

/**
 * @class CommandParser
 * @brief Parses raw user input into structured `Command` records.
 */
class CommandParser
{
public:
    /**
     * @brief Default constructor.
     */
    CommandParser() = default;

    /**
     * @brief Parses a raw command line into a `Command` structure.
     * @param raw User-provided command text.
     * @param out Destination structure for parsed data.
     * @param errorMessage Populated with a descriptive message on failure.
     * @return `true` on success, otherwise `false`.
     */
    bool parse(const QString& raw, Command& out, QString& errorMessage) const;

private:
    /**
     * @brief Parses a coordinate token of the form `{-1.0,2.5}`.
     * @param token Raw token containing the coordinate.
     * @param keyOut Reserved for future extensions; remains unchanged.
     * @param ptOut Receives the parsed point.
     * @param errorMessage Describes parsing issues on failure.
     * @return `true` when the coordinate is valid.
     */
    bool parseCoords(const QString& token, QString& keyOut, QPointF& ptOut, QString& errorMessage) const;

    /**
     * @brief Associates a flag with its value inside the command structure.
     * @param flag Raw flag token including the leading dash (e.g., `-name`).
     * @param value Associated value token.
     * @param out Parsed command to update.
     * @param errorMessage Reserved for compatibility; unused in the current implementation.
     * @return `true` when the flag-value pair is stored successfully.
     */
    bool parseFlagValue(const QString& flag, const QString& value, Command& out, QString& errorMessage) const;
};
