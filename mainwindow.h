/**
 * @file mainwindow.h
 * @brief Declares the main application window for ObjectDrawer, including command handling logic. 
 * @author Nikol Grigoryan
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QTextEdit>
#include <QSplitter>
#include <QVBoxLayout>
#include "CommandParser.h"
#include "CommandDispatcher.h"
#include "ShapeRepository.h"


class QGraphicsScene;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Hosts the primary user interface where users enter commands and visualize shapes.
 *
 * The window wires together the parser, dispatcher, and repository to turn textual commands
 * into shapes displayed on a graphics scene.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window and prepares the scene and dispatcher.
     * @param parent Optional parent widget.
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief Destroys the main window and releases UI resources.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Handles the Enter key event from the command input field.
     */
    void onCommandEntered();
    //void handleCommandResult(const CommandResult &result);


private:
    /**
     * @brief Initializes UI-related state after the generated UI is set up.
     */
    void initializeUi();
    /**
     * @brief Appends a formatted message to the log window.
     * @param message Text to display.
     * @param isError When `true`, the message is treated as an error.
     */
    void logMessage(const QString &message, bool isError = false);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;

    // UI components
    QGraphicsView* m_view;
    //QLineEdit* m_commandEdit;
    QTextEdit* m_log;

    // Collaboration components
    ShapeRepository m_repo;
    CommandParser m_parser;
    CommandDispatcher m_dispatcher;

    // Helpers
    /**
     * @brief Convenience wrapper for logging informational messages.
     */
    void logInfo(const QString& msg);
    /**
     * @brief Convenience wrapper for logging error messages.
     */
    void logError(const QString& msg);
    /**
     * @brief Sets up UI elements that are not covered by the generated form.
     */
    void setupUi();
    /**
     * @brief Connects UI signals to the appropriate slots.
     */
    void setupConnections();

};
#endif // MAINWINDOW_H
