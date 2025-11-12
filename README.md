# ObjectDrawer

ObjectDrawer is a Qt Widgets desktop application that renders geometric shapes on a 2D canvas from user-entered commands. It is designed to illustrate how command parsing, scene graph updates, and domain-specific validation can be layered on top of Qt's graphics framework.

## Features

- Command-driven creation of lines, triangles, rectangles, and squares using typed coordinates.
- Real-time rendering on a `QGraphicsView`/`QGraphicsScene` canvas backed by reusable shape objects.
- Console-style command entry with an integrated log window for success and error feedback.
- Ability to connect previously created shapes by drawing a dashed line between their centers.
- Batch execution of command scripts via `execute_file`, including per-line success and error reporting.

## Build & Run

1. Install Qt (Qt 6 recommended, Qt 5 Widgets also works).
2. Configure and build with CMake:

```bash
# From the project root
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/qt6
cmake --build build
```

Replace `/path/to/qt6` with the Qt installation prefix (for example `/usr/lib/x86_64-linux-gnu/cmake/Qt6` on many Linux distributions).

3. Launch the application:

```bash
./build/ObjectDrawer
```

## Usage

1. Start the application; the main window shows the drawing canvas, a log pane, and a command line.
2. Type a command into the `Command Console` field and press `Enter`.
3. Check the log pane for `[INFO]` success messages or `[ERROR]` feedback.
4. Shapes that are successfully created appear on the canvas immediately.

### Supported Commands

- `create_line -name line1 -coord_1 {0,0} -coord_2 {5,2}`
- `create_triangle -name tri1 -coord_1 {0,0} -coord_2 {3,0} -coord_3 {0,4}`
- `create_rectangle -name rect1 -coord_1 {0,0} -coord_2 {3,4}`
- `create_rectangle -name rect2 -coord_1 {0,0} -coord_2 {3,0} -coord_3 {3,4} -coord_4 {0,4}`
- `create_square -name sq1 -coord_1 {0,0} -coord_2 {3,3}`
- `create_square -name sq2 -coord_1 {0,0} -coord_2 {3,3} -coord_3 {3,0} -coord_4 {0,3}`
- `connect -object_name_1 tri1 -object_name_2 rect1`
- `execute_file -file_path /absolute/path/to/script.txt`

Command arguments must be separated by whitespace. Coordinates must use `{x,y}` without embedded spaces. When executing a script file, blank lines and lines starting with `#` are ignored; each remaining line is parsed and executed as if typed in the console.

## Architecture Overview

- **MainWindow (`mainwindow.cpp`)** wires up the UI created in `mainwindow.ui`, captures console input, and routes parsed commands to the dispatcher while logging feedback.
- **CommandParser (`CommandParser.cpp`)** tokenizes user input, validates flags, and produces a `Command` structure that exposes argument values and typed coordinates.
- **CommandDispatcher (`CommandDispatcher.cpp`)** performs command-specific validation, creates shape objects, adds them to the `QGraphicsScene`, and stores them in the repository.
- **ShapeRepository (`ShapeRepository.cpp`)** owns all `ShapeBase` instances so they can be retrieved by name for operations such as `connect`.
- **Shape hierarchy (`ShapeBase`, `LineShape`, `TriangleShape`, `RectangleShape`, `SquareShape`)** encapsulates the `QGraphicsItem` used for rendering and provides geometric centers used when connecting shapes.
- **Utility (`Utility.cpp`)** provides geometric helpers (collinearity checks, rectangle/square validation, diagonal validation) invoked by the dispatcher before shapes are created.

This separation keeps parsing, validation, rendering, and state management loosely coupled and easier to test in isolation.

## Known Issues & Limitations

- Shape removal, undo, or editing is not implemented; restart the application to clear the scene.
- Coordinates with spaces inside the braces (for example `{0, 0}`) are rejected by the parser; enter them as `{0,0}`.
- `execute_file` expects a readable path and does not support quoted filenames or relative paths that depend on the current working directory of the GUI process.
- Connections drawn with `connect` are not stored in the repository, so they cannot be removed or referenced later.
- There is no persistence; shapes exist only for the lifetime of the running application.

## Documentation

API and architecture docs are generated with [Doxygen](https://www.doxygen.nl/).

1. **Install prerequisites**
   - Install Doxygen (e.g., `sudo apt install doxygen` on Debian/Ubuntu).
   - Optionally install Graphviz (`dot`) if you want class and call graphs.
2. **Generate the docs**
   ```bash
   cd objectdrawer
   doxygen Doxyfile
   ```
   The documentation is emitted under `docs/html/`.
3. **View the docs**
   - Open `docs/html/index.html` directly in a browser, or
   - Serve the directory locally:
     ```bash
     cd docs/html
     python3 -m http.server 9000
     ```
     and browse to `http://localhost:9000`.
4. **Regenerating**
   - Re-run `doxygen Doxyfile` whenever you update comments.
   - Remove the `docs/` directory if you want a clean rebuild.
