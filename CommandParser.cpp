/**
 * @file CommandParser.cpp
 * @brief Implements the command parsing logic for ObjectDrawer commands.
 * @author Nikol Grigoryan
 */
#include "CommandParser.h"
#include <QStringList>
#include <QRegularExpression>

/**
 * @brief Parses a raw command string into a structured `Command`.
 * @param raw Raw command text entered by the user.
 * @param out Destination structure that receives parsed fields on success.
 * @param errorMessage Describes why parsing failed when the method returns `false`.
 * @return `true` on success, otherwise `false`.
 */
bool CommandParser::parse(const QString& raw, Command& out, QString& errorMessage) const
{
    // Tokenize by whitespace while preserving braces in coordinate tokens.
    // We assume users won't include spaces inside braces.
    QStringList tokens = raw.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (tokens.isEmpty()) {
        errorMessage = "No tokens found in the command.";
        return false;
    }

    // First token is the command name (e.g., create_line)
    out = Command{};
    out.name = tokens.first();
    tokens.pop_front();

    // Iterate over tokens pairwise for flags and values
    for (int i = 0; i < tokens.size(); ++i) {
        const QString t = tokens[i];

        // Coordinates come in the form: -coord_X {x,y}
        if (t.startsWith("-coord_")) {
            if (i + 1 >= tokens.size()) {
                errorMessage = QString("Expected coordinate after '%1'.").arg(t);
                return false;
            }
            const QString coordToken = tokens[i + 1];
            QPointF pt;
            QString keyOut;
            if (!parseCoords(coordToken, keyOut, pt, errorMessage)) {
                return false;
            }
            // Store the coordinate under the given coord key (e.g., coord_1)
            out.coords.insert(t.mid(1), pt); // strip leading '-' for map key consistency
            ++i; // consume the value
            continue;
        }

        // Generic flags like -name value
        if (t.startsWith("-")) {
            if (i + 1 >= tokens.size()) {
                errorMessage = QString("Expected value after flag '%1'.").arg(t);
                return false;
            }
            const QString value = tokens[i + 1];
            if (!parseFlagValue(t, value, out, errorMessage)) {
                return false;
            }
            ++i; // consume the value
            continue;
        }

        // If a token is neither coordinate nor flag, it's invalid
        errorMessage = QString("Unexpected token '%1'. Flags should start with '-'.").arg(t);
        return false;
    }

    // Basic validation: command name must be present
    if (out.name.isEmpty()) {
        errorMessage = "Command name is empty.";
        return false;
    }

    return true;
}

/**
 * @brief Parses a coordinate token and extracts the numeric point.
 * @param token Token expected to be in the form `{x,y}`.
 * @param keyOut Reserved for future use; remains unchanged by the current implementation.
 * @param ptOut Receives the parsed coordinate value.
 * @param errorMessage Describes formatting issues when parsing fails.
 * @return `true` if the coordinate token is valid.
 */
bool CommandParser::parseCoords(const QString& token, QString& keyOut, QPointF& ptOut, QString& errorMessage) const
{
    // Expect token format: {x,y}
    static QRegularExpression re("^\\{\\s*(-?\\d+(?:\\.\\d+)?)\\s*,\\s*(-?\\d+(?:\\.\\d+)?)\\s*\\}$");
    QRegularExpressionMatch m = re.match(token);
    if (!m.hasMatch()) {
        errorMessage = QString("Invalid coordinate format '%1'. Expected {x,y}.").arg(token);
        return false;
    }

    bool okX = false, okY = false;
    const double x = m.captured(1).toDouble(&okX);
    const double y = m.captured(2).toDouble(&okY);
    if (!okX || !okY) {
        errorMessage = QString("Failed to parse numeric values in '%1'.").arg(token);
        return false;
    }

    ptOut = QPointF(x, y);
    return true;
}

/**
 * @brief Registers a flag-value pair within the `Command`.
 * @param flag Raw flag token including the leading dash.
 * @param value Token representing the flag's value.
 * @param out Parsed command structure to update.
 * @param errorMessage Reserved for compatibility; currently unused for failures.
 * @return `true` when the flag-value pair is accepted.
 */
bool CommandParser::parseFlagValue(const QString& flag, const QString& value, Command& out, QString& errorMessage) const
{
    // Store flags without the leading '-' to keep keys clean
    const QString key = flag.mid(1);
    // For now, we treat all non-coordinate flags as plain strings (e.g., name, file_path)
    out.args.insert(key, value);
    return true;
}
