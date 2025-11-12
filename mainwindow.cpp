/**
 * @file mainwindow.cpp
 * @brief Implements the main application window for ObjectDrawer, including command routing and logging. 
 * @author Nikol Grigoryan
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsScene>
#include <QLineEdit>
#include <QScrollBar>

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>
#include <QFileInfo>


/**
 * @brief Constructs the main window, initializes UI components, and sets up command handling.
 * @param parent Optional parent widget.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_view(new QGraphicsView(this)),
      m_scene(new QGraphicsScene(this)),
      //m_commandEdit(new QLineEdit(this)),
      //m_log(new QTextEdit(this))
      m_repo(),
      m_parser(),
      m_dispatcher(m_scene, &m_repo)
{
    ui->setupUi(this);
    initializeUi();
    setupConnections();

    // Associate the scene with the view so drawings appear on screen
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
}

/**
 * @brief Destructor that releases the generated UI.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Binds UI signals to the appropriate slots.
 */
void MainWindow::setupConnections()
{
    // When user presses Enter in the command line, handle the command
    connect(ui->commandEdit, &QLineEdit::returnPressed,
            this, &MainWindow::onCommandEntered);
}

/**
 * @brief Reads the text from the command input, parses it, and executes it.
 */
void MainWindow::onCommandEntered()
{
    // Read the raw text from the command input field
    const QString raw = ui->commandEdit->text().trimmed();
    if (raw.isEmpty()) {
        // No command entered; gently notify user
        logError("Empty command. Please enter a valid command.");
        return;
    }

    // Parse the command into a structured representation
    Command cmd;
    QString parseError;
    if (!m_parser.parse(raw, cmd, parseError)) {
        // Parsing failed; show descriptive reason to user
        logError(QString("Parse error: %1").arg(parseError));
        return;
    }

    // Dispatch the command to perform the requested action
    QString execMsg;
    if (m_dispatcher.execute(cmd, execMsg)) {
        // Success path: log positive feedback
        logInfo(execMsg);
        ui->commandEdit->clear();
    } else {
        // Failure path: log meaningful error
        logError(execMsg);
    }
}

/**
 * @brief Initializes UI state and connects menu actions.
 */
void MainWindow::initializeUi()
{
   m_log = ui->logWindow;

    if (ui->actionExit) {
        connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    }
 }

/**
 * @brief Logs an error message in the log window.
 * @param msg Error text to append.
 */
void MainWindow::logError(const QString& msg)
{
    // Append error message to the log window with a simple prefix
    m_log->append(QString("[ERROR] %1").arg(msg));
}

/**
 * @brief Logs an informational message in the log window.
 * @param msg Text to append with an informational prefix.
 */
void MainWindow::logInfo(const QString& msg)
{
    // Append informational message to the log window with a simple prefix
    m_log->append(QString("[INFO] %1").arg(msg));
}
