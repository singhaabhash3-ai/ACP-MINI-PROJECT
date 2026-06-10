# 2D Graphics Editor in C

A menu-driven console-based 2D Graphics Editor built in C as a mini project for the ACP (Advanced C Programming) course. The program uses a 2D character array as the drawing canvas where shapes are rendered using ASCII characters.

## 📋 About the Project

This project implements a simple graphics editor that works entirely in the terminal. The canvas is a **30 × 60** character grid:
- Empty spaces are represented by `_` (underscore)
- Shapes are drawn using `*` (asterisk)

Users can **add**, **delete**, and **modify** graphical objects through an interactive menu system.

## ✨ Features

| Feature        | Description                                                         |
|----------------|---------------------------------------------------------------------|
| **Circle**     | Drawn using trigonometric functions (sin/cos) with aspect correction |
| **Rectangle**  | Drawn given top-left corner, height, and width                      |
| **Line**       | Drawn using Bresenham's line algorithm                              |
| **Triangle**   | Drawn by connecting three user-defined points with lines            |
| **Add**        | Add up to 50 objects to the canvas                                  |
| **Delete**     | Remove any existing object by its ID                                |
| **Modify**     | Update the parameters of any existing object                        |
| **Display**    | Print the canvas with row and column numbers for easy reference     |
| **List**       | View all active objects with their properties                       |
| **Clear All**  | Remove all objects and reset the canvas                             |

## 🛠️ How It Works

- All objects are stored in an array of `struct Object`, each holding the shape type and its parameters.
- When any object is added, deleted, or modified, the entire canvas is **redrawn** from scratch — this ensures the picture is always consistent.
- The canvas uses a coordinate system where **(row, column)** starts at **(0, 0)** in the top-left corner.

## 📂 File Structure

```
.
├── graphics_editor.c   # Main source code (single file)
└── README.md           # Project documentation
```

## 🚀 How to Compile and Run

### Prerequisites
- A C compiler (GCC, MinGW, or any standard C compiler)
- `math.h` library support (link with `-lm` on Linux/Mac)

### Compile
```bash
gcc graphics_editor.c -o graphics_editor -lm
```

### Run
```bash
./graphics_editor
```

On Windows:
```cmd
graphics_editor.exe
```

## 📖 Usage

When you run the program, you will see a menu like this:

```
========================================
     2D Graphics Editor in C
     Canvas Size: 30 x 60
========================================

--- MAIN MENU ---
1. Add Object
2. Delete Object
3. Modify Object
4. Display Picture
5. List Objects
6. Clear All
7. Exit
Enter your choice:
```

### Adding a Shape
1. Select **1. Add Object** from the main menu.
2. Choose the shape type (Circle / Rectangle / Line / Triangle).
3. Enter the required coordinates and dimensions.
4. The canvas is automatically displayed with the new shape.

### Deleting a Shape
1. Select **2. Delete Object**.
2. A list of all active objects is shown.
3. Enter the object number to delete.

### Modifying a Shape
1. Select **3. Modify Object**.
2. A list of all active objects is shown.
3. Enter the object number, then provide new values for its parameters.

## 💡 Example

Adding a circle at center (15, 30) with radius 8, followed by a rectangle at (2, 5) with height 6 and width 20:

```
   0         1         2         3         4         5
 0|____________________________________________________________|
 1|____________________________________________________________|
 2|_____***********************________________________________|
 3|_____|___________________|_____________________________________|
 ...
 7|_________________________**_________________________________|
 8|________________________*__*________________________________|
 ...
15|______________*_______________*______________________________|
 ...
```

*(Actual output will vary based on the exact rendering)*

## 🔧 Technical Details

- **Canvas Size**: 30 rows × 60 columns (configurable via `#define ROWS` and `#define COLS`)
- **Max Objects**: 50 (configurable via `#define MAX_OBJECTS`)
- **Circle Drawing**: Uses parametric equations with `sin()` and `cos()`, with a 2× horizontal stretch to compensate for characters being taller than they are wide.
- **Line Drawing**: Implements Bresenham's line algorithm for accurate line rendering.
- **Triangle Drawing**: Composed of three line segments connecting three vertices.

## 👨‍💻 Author

**Aabhash Singh**

## 📄 License

This project is created for educational purposes as part of the ACP Mini Project.
