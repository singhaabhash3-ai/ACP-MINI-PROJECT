#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 30
#define COLS 60
#define MAX_OBJECTS 50

// canvas array
char canvas[ROWS][COLS];

// object types
#define CIRCLE    1
#define RECTANGLE 2
#define LINE      3
#define TRIANGLE  4

// struct to store one object
struct Object {
    int type;       // 1=circle, 2=rect, 3=line, 4=triangle
    int x1, y1;     // first point (or center for circle)
    int x2, y2;     // second point (or width/height for rect)
    int x3, y3;     // third point (only for triangle)
    int radius;     // only for circle
    int active;     // 1 = exists, 0 = deleted
};

struct Object objects[MAX_OBJECTS];
int objectCount = 0;

// ---------- helper functions ----------

// check if a point is inside the canvas
int isValid(int r, int c) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        return 1;
    return 0;
}

// put a point on canvas if valid
void putPoint(int r, int c) {
    if (isValid(r, c))
        canvas[r][c] = '*';
}

// clear the canvas with underscores
void clearCanvas() {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

// ---------- drawing functions ----------

// draw a circle using midpoint algorithm (simple version)
void drawCircle(int centerR, int centerC, int radius) {
    int angle;
    int r, c;
    for (angle = 0; angle < 360; angle++) {
        // convert angle to radians
        double rad = angle * 3.14159 / 180.0;
        r = centerR + (int)(radius * sin(rad));
        c = centerC + (int)(radius * 2 * cos(rad)); // multiply by 2 because characters are taller than wide
        putPoint(r, c);
    }
}

// draw a rectangle given top-left corner and width/height
void drawRectangle(int topR, int topC, int height, int width) {
    int i;
    // top and bottom sides
    for (i = 0; i <= width; i++) {
        putPoint(topR, topC + i);
        putPoint(topR + height, topC + i);
    }
    // left and right sides
    for (i = 0; i <= height; i++) {
        putPoint(topR + i, topC);
        putPoint(topR + i, topC + width);
    }
}

// draw a line using Bresenham's algorithm (simple version)
void drawLine(int r1, int c1, int r2, int c2) {
    int dr, dc, stepR, stepC, err, e2;

    dr = abs(r2 - r1);
    dc = abs(c2 - c1);

    if (r1 < r2) stepR = 1; else stepR = -1;
    if (c1 < c2) stepC = 1; else stepC = -1;

    err = dr - dc;

    while (1) {
        putPoint(r1, c1);
        if (r1 == r2 && c1 == c2)
            break;
        e2 = 2 * err;
        if (e2 > -dc) {
            err = err - dc;
            r1 = r1 + stepR;
        }
        if (e2 < dr) {
            err = err + dr;
            c1 = c1 + stepC;
        }
    }
}

// draw a triangle by drawing 3 lines
void drawTriangle(int r1, int c1, int r2, int c2, int r3, int c3) {
    drawLine(r1, c1, r2, c2);
    drawLine(r2, c2, r3, c3);
    drawLine(r3, c3, r1, c1);
}

// ---------- redraw all active objects ----------

void redrawAll() {
    int i;
    clearCanvas();
    for (i = 0; i < objectCount; i++) {
        if (objects[i].active == 0)
            continue;
        switch (objects[i].type) {
            case CIRCLE:
                drawCircle(objects[i].x1, objects[i].y1, objects[i].radius);
                break;
            case RECTANGLE:
                drawRectangle(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            case LINE:
                drawLine(objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            case TRIANGLE:
                drawTriangle(objects[i].x1, objects[i].y1,
                             objects[i].x2, objects[i].y2,
                             objects[i].x3, objects[i].y3);
                break;
        }
    }
}

// ---------- display function ----------

void displayCanvas() {
    int i, j;
    printf("\n");
    // print column numbers
    printf("   ");
    for (j = 0; j < COLS; j++) {
        if (j % 10 == 0)
            printf("%d", (j / 10) % 10);
        else
            printf(" ");
    }
    printf("\n");

    for (i = 0; i < ROWS; i++) {
        printf("%2d|", i);  // row number
        for (j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("|\n");
    }
    printf("\n");
}

// ---------- list all objects ----------

void listObjects() {
    int i;
    int found = 0;
    printf("\n--- List of Objects ---\n");
    for (i = 0; i < objectCount; i++) {
        if (objects[i].active == 0)
            continue;
        found = 1;
        printf("Object %d: ", i + 1);
        switch (objects[i].type) {
            case CIRCLE:
                printf("Circle - Center(%d,%d) Radius=%d\n",
                       objects[i].x1, objects[i].y1, objects[i].radius);
                break;
            case RECTANGLE:
                printf("Rectangle - TopLeft(%d,%d) Height=%d Width=%d\n",
                       objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            case LINE:
                printf("Line - From(%d,%d) To(%d,%d)\n",
                       objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
                break;
            case TRIANGLE:
                printf("Triangle - P1(%d,%d) P2(%d,%d) P3(%d,%d)\n",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2,
                       objects[i].x3, objects[i].y3);
                break;
        }
    }
    if (found == 0)
        printf("No objects added yet.\n");
    printf("-----------------------\n");
}

// ---------- add functions ----------

void addCircle() {
    int cr, cc, r;
    if (objectCount >= MAX_OBJECTS) {
        printf("Cannot add more objects! Maximum limit reached.\n");
        return;
    }
    printf("Enter center row (0-%d): ", ROWS - 1);
    scanf("%d", &cr);
    printf("Enter center column (0-%d): ", COLS - 1);
    scanf("%d", &cc);
    printf("Enter radius: ");
    scanf("%d", &r);

    objects[objectCount].type = CIRCLE;
    objects[objectCount].x1 = cr;
    objects[objectCount].y1 = cc;
    objects[objectCount].radius = r;
    objects[objectCount].active = 1;
    objectCount++;

    printf("Circle added as Object %d.\n", objectCount);
}

void addRectangle() {
    int tr, tc, h, w;
    if (objectCount >= MAX_OBJECTS) {
        printf("Cannot add more objects! Maximum limit reached.\n");
        return;
    }
    printf("Enter top-left row (0-%d): ", ROWS - 1);
    scanf("%d", &tr);
    printf("Enter top-left column (0-%d): ", COLS - 1);
    scanf("%d", &tc);
    printf("Enter height: ");
    scanf("%d", &h);
    printf("Enter width: ");
    scanf("%d", &w);

    objects[objectCount].type = RECTANGLE;
    objects[objectCount].x1 = tr;
    objects[objectCount].y1 = tc;
    objects[objectCount].x2 = h;
    objects[objectCount].y2 = w;
    objects[objectCount].active = 1;
    objectCount++;

    printf("Rectangle added as Object %d.\n", objectCount);
}

void addLine() {
    int r1, c1, r2, c2;
    if (objectCount >= MAX_OBJECTS) {
        printf("Cannot add more objects! Maximum limit reached.\n");
        return;
    }
    printf("Enter start row (0-%d): ", ROWS - 1);
    scanf("%d", &r1);
    printf("Enter start column (0-%d): ", COLS - 1);
    scanf("%d", &c1);
    printf("Enter end row (0-%d): ", ROWS - 1);
    scanf("%d", &r2);
    printf("Enter end column (0-%d): ", COLS - 1);
    scanf("%d", &c2);

    objects[objectCount].type = LINE;
    objects[objectCount].x1 = r1;
    objects[objectCount].y1 = c1;
    objects[objectCount].x2 = r2;
    objects[objectCount].y2 = c2;
    objects[objectCount].active = 1;
    objectCount++;

    printf("Line added as Object %d.\n", objectCount);
}

void addTriangle() {
    int r1, c1, r2, c2, r3, c3;
    if (objectCount >= MAX_OBJECTS) {
        printf("Cannot add more objects! Maximum limit reached.\n");
        return;
    }
    printf("Enter point 1 row: ");
    scanf("%d", &r1);
    printf("Enter point 1 column: ");
    scanf("%d", &c1);
    printf("Enter point 2 row: ");
    scanf("%d", &r2);
    printf("Enter point 2 column: ");
    scanf("%d", &c2);
    printf("Enter point 3 row: ");
    scanf("%d", &r3);
    printf("Enter point 3 column: ");
    scanf("%d", &c3);

    objects[objectCount].type = TRIANGLE;
    objects[objectCount].x1 = r1;
    objects[objectCount].y1 = c1;
    objects[objectCount].x2 = r2;
    objects[objectCount].y2 = c2;
    objects[objectCount].x3 = r3;
    objects[objectCount].y3 = c3;
    objects[objectCount].active = 1;
    objectCount++;

    printf("Triangle added as Object %d.\n", objectCount);
}

// ---------- delete function ----------

void deleteObject() {
    int id;
    listObjects();
    printf("Enter object number to delete: ");
    scanf("%d", &id);

    if (id < 1 || id > objectCount) {
        printf("Invalid object number!\n");
        return;
    }
    if (objects[id - 1].active == 0) {
        printf("Object %d is already deleted!\n", id);
        return;
    }
    objects[id - 1].active = 0;
    printf("Object %d deleted.\n", id);
}

// ---------- modify function ----------

void modifyObject() {
    int id;
    listObjects();
    printf("Enter object number to modify: ");
    scanf("%d", &id);

    if (id < 1 || id > objectCount) {
        printf("Invalid object number!\n");
        return;
    }
    if (objects[id - 1].active == 0) {
        printf("Object %d is deleted, cannot modify!\n", id);
        return;
    }

    int index = id - 1;
    printf("Re-enter the values for this object:\n");

    switch (objects[index].type) {
        case CIRCLE:
            printf("Enter new center row: ");
            scanf("%d", &objects[index].x1);
            printf("Enter new center column: ");
            scanf("%d", &objects[index].y1);
            printf("Enter new radius: ");
            scanf("%d", &objects[index].radius);
            break;
        case RECTANGLE:
            printf("Enter new top-left row: ");
            scanf("%d", &objects[index].x1);
            printf("Enter new top-left column: ");
            scanf("%d", &objects[index].y1);
            printf("Enter new height: ");
            scanf("%d", &objects[index].x2);
            printf("Enter new width: ");
            scanf("%d", &objects[index].y2);
            break;
        case LINE:
            printf("Enter new start row: ");
            scanf("%d", &objects[index].x1);
            printf("Enter new start column: ");
            scanf("%d", &objects[index].y1);
            printf("Enter new end row: ");
            scanf("%d", &objects[index].x2);
            printf("Enter new end column: ");
            scanf("%d", &objects[index].y2);
            break;
        case TRIANGLE:
            printf("Enter new point 1 row: ");
            scanf("%d", &objects[index].x1);
            printf("Enter new point 1 column: ");
            scanf("%d", &objects[index].y1);
            printf("Enter new point 2 row: ");
            scanf("%d", &objects[index].x2);
            printf("Enter new point 2 column: ");
            scanf("%d", &objects[index].y2);
            printf("Enter new point 3 row: ");
            scanf("%d", &objects[index].x3);
            printf("Enter new point 3 column: ");
            scanf("%d", &objects[index].y3);
            break;
    }
    printf("Object %d modified.\n", id);
}

// ---------- main menu ----------

int main() {
    int choice, addChoice;

    clearCanvas();

    printf("========================================\n");
    printf("     2D Graphics Editor in C\n");
    printf("     Canvas Size: %d x %d\n", ROWS, COLS);
    printf("========================================\n");

    while (1) {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. List Objects\n");
        printf("6. Clear All\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n--- Add Object ---\n");
                printf("1. Circle\n");
                printf("2. Rectangle\n");
                printf("3. Line\n");
                printf("4. Triangle\n");
                printf("Enter shape choice: ");
                scanf("%d", &addChoice);
                switch (addChoice) {
                    case 1: addCircle();    break;
                    case 2: addRectangle(); break;
                    case 3: addLine();      break;
                    case 4: addTriangle();  break;
                    default: printf("Invalid choice!\n");
                }
                // redraw and show
                redrawAll();
                displayCanvas();
                break;

            case 2:
                deleteObject();
                redrawAll();
                displayCanvas();
                break;

            case 3:
                modifyObject();
                redrawAll();
                displayCanvas();
                break;

            case 4:
                redrawAll();
                displayCanvas();
                break;

            case 5:
                listObjects();
                break;

            case 6:
                objectCount = 0;
                clearCanvas();
                printf("All objects cleared.\n");
                break;

            case 7:
                printf("Exiting... Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
