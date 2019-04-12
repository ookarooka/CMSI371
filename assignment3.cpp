/***
 Assignment-3: Rotating a Cube in 3-Dimensional Space
 
 Name: Patterson, Joshua
 
 Collaborators: Richardson, Alex
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section
 
 Project Summary: Rotation of a given cude in a 3d space, using 6 main ideas for functions: homogenous coordinates, cartesian coordinates, matrix rotations of x,y and z, and matrix multiplication. Conversions among these functions are required in order for the cube to rotate. Global variables such as theta is also used  to deinfe the degress of rotation.
 ***/

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#pragma GCC diagnostic pop

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
using namespace std;

// If a float is < EPSILON or > -EPILSON then it should be 0
float EPSILON = 0.000001;
// theta is the angle to rotate the scene
float THETA = 0.0;
// Vector placeholders for the scene and color array
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI) / 180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogeneous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < cartesian_coords.size(); i++) {
        result.push_back(cartesian_coords[i]);
        if ((i+1) % 3 == 0) {
            result.push_back(1.0);
        }
    }
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogeneous_coords.size(); i++) {
        if ((i+1) % 4 == 0) {
            continue;
        } else {
            result.push_back(homogeneous_coords[i]);
        }
    }
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat;
    translate_mat = {
        1, 0, 0, dx,
        0, 1, 0, dy,
        0, 0, 1, dz,
        0, 0, 0, 1
    };
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat;
    scale_mat = {
        sx, 0, 0, 0,
        0, sy, 0, 0,
        0, 0, sz, 0,
        0, 0, 0, 1
    };
    return scale_mat;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x;
    theta = deg2rad(theta);
    rotate_mat_x = {
        1, 0, 0, 0,
        0, cos(theta), -sin(theta), 0,
        0, sin(theta), cos(theta),  0,
        0, 0, 0, 1
    };
    return rotate_mat_x;
}

// Definition of a rotation matrix about the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y;
    theta = deg2rad(theta);
    rotate_mat_y = {
        cos(theta), 0, sin(theta), 0,
        0, 1, 0, 0,
        -sin(theta), 0, cos(theta),  0,
        0, 0, 0, 1
    };
    return rotate_mat_y;
}

// Definition of a rotation matrix about the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z;
    theta = deg2rad(theta);
    rotate_mat_z = {
        cos(theta), -sin(theta), 0, 0,
        sin(theta), cos(theta), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    // TODO: Compute matrix multiplication of A B
    for (int b = 0; b < B.size()/4; b++) {
        for (int a = 0; a < 4; a++) {
            float element_wise_sum = 0.0;
            for (int k = 0; k < 4; k++) {
                float element_wise = A[a*4+k]*B[b*4+k];
                if (element_wise < EPSILON && element_wise > -1.0*EPSILON) {
                    element_wise = 0.0;
                }
                element_wise_sum += element_wise;
            }
            result.push_back(element_wise_sum);
        }
    }
    return result;
}
////////////////////////////////////////////////////////////
// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    vector<GLfloat> cubePlane;
    cubePlane = to_homogeneous_coord(init_plane());
    
    vector<GLfloat> cubeFace;
    vector<GLfloat> cubeBack;
    vector<GLfloat> cubeLeftSide;
    vector<GLfloat> cubeRightSide;
    vector<GLfloat> cubeHead;
    vector<GLfloat> cubeGround;
    
    vector<GLfloat> insertFace;
    vector<GLfloat> insertBack;
    vector<GLfloat> insertLeftSide;
    vector<GLfloat> insertRightSide;
    vector<GLfloat> insertHead;
    vector<GLfloat> insertGround;
    
    result.insert(result.end(), cubeFace.begin(), cubeFace.end());
    result.insert(result.end(), cubeLeftSide.begin(), cubeLeftSide.end());
    result.insert(result.end(), cubeRightSide.begin(), cubeRightSide.end());
    result.insert(result.end(), cubeBack.begin(), cubeBack.end());
    result.insert(result.end(), cubeHead.begin(), cubeHead.end());
        result.insert(result.end(), cubeGround.begin(), cubeGround.end());
    
    
    cubeFace = to_cartesian_coord(cubeFace);
    cubeBack = to_cartesian_coord(cubeBack);
    cubeLeftSide = to_cartesian_coord(cubeLeftSide);
    cubeRightSide = to_cartesian_coord(cubeRightSide);
    cubeHead = to_cartesian_coord(cubeHead);
    cubeGround = to_cartesian_coord(cubeGround);
    
    
//translations
    cubeFace = mat_mult(translation_matrix(0,0,0.5), cubePlane);
    cubeBack =
        mat_mult(translation_matrix(0,0,0.5), cubeBack);
    cubeLeftSide =
    mat_mult(translation_matrix(-0.5,0,0), cubeLeftSide);
    cubeRightSide =
    mat_mult(translation_matrix(0.5,0,0), cubeRightSide);
    cubeHead =
    mat_mult(translation_matrix(0,0.5,0), cubeRightSide);
    cubeGround =
    mat_mult(translation_matrix(0,-0.5,0), cubeGround);
    
    //rotations
    cubeBack =
    mat_mult(rotation_matrix_y(180), cubePlane);
    cubeLeftSide =
    mat_mult(rotation_matrix_y(-90), cubePlane);
    cubeRightSide =
    mat_mult(rotation_matrix_y(90), cubePlane);
    cubeHead =
    mat_mult(rotation_matrix_y(-90), cubePlane);
    cubeGround =
    mat_mult(rotation_matrix_y(90), cubePlane);
   
    
    
    
    return result;
}

////////////////////////////////////////////////////////////


void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    gluPerspective(65.0, 1.0, 2.0, 30.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(4.0, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

////////////////////////////////////////////////////////////
// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene;
  
    return scene;
}

////////////////////////////////////////////////////////////
// Construct the color mapping of the scene
vector<GLfloat> init_color(vector<GLfloat> scene) {
    vector<GLfloat> colors;
    for (int i = 0; i < scene.size(); i++) {
        colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }
    return colors;
}
////////////////////////////////////////////////////////////

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    int scene_size = SCENE.size()/16;
    //ROTATIONS
    vector<GLfloat> pointValues = to_homogeneous_coord(init_scene());
    pointValues = mat_mult(rotation_matrix_y(THETA), pointValues);
    SCENE = to_cartesian_coord(pointValues);
    
    //vector<GLfloat> xyzPoints;
    //vector<GLfloat> matValue;
    //vector<GLfloat> multResult;
    
    GLfloat* pointColor = vector2array(COLOR);
    GLfloat* pointScene = vector2array(SCENE);
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    pointScene);  // Pointer to memory location to read from
    
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   pointColor);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4 * scene_size);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    THETA = THETA + 0.03;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 3");
    
    setup();
    init_camera();
    // Setting global variables SCENE and COLOR with actual values
    SCENE = init_scene();
    COLOR = init_color(SCENE);
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}
