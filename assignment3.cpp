/***
 Assignment-3: Geomteric Modeling of a Scene
 
 Name: Patterson, Joshua
 
 Collaborators: Richardson, Alex
 
 Project Summary: Using methods from Rotation of cube, built multiple cubes using build_cube() and created a scene using init_scene(). Used cartesian coordinates to place unit cubes in the given scene. Scene is comprised of roughly 6 objects.
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

float EPSILON = 0.000001;
float THETA = 0.0;
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;


float deg2rad(float d) {
    return (d*M_PI) / 180.0;
}

GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        0.5,   0.5,   0.0,
        -0.5,   0.5,   0.0,
        -0.5,   -0.5,   0.0,
        0.5,   -0.5,   0.0
    };
    return vertices;
}

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

vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
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

vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    vector<GLfloat> cubePlane;
    cubePlane = to_homogeneous_coord(init_plane());
    vector<GLfloat> cubeFace;
    cubeFace = mat_mult(translation_matrix(0,0,0.5), cubePlane);
    vector<GLfloat> cubeLeftSide;
    cubeLeftSide =
    mat_mult(translation_matrix(-0.5,0,0), cubeLeftSide);
    vector<GLfloat> cubeRightSide;
    cubeRightSide =
    mat_mult(translation_matrix(0.5,0,0), cubeRightSide);
    vector<GLfloat> cubeBack;
    cubeBack =
    mat_mult(translation_matrix(0,0,-0.5), cubeBack);
    vector<GLfloat> cubeHead;
    cubeHead =
    mat_mult(translation_matrix(0,0.5,0), cubeHead);
    vector<GLfloat> cubeGround;
    cubeGround =
    mat_mult(translation_matrix(0,-0.5,0), cubeGround);
    
    cubeBack =
    mat_mult(rotation_matrix_y(180), cubePlane);
    cubeLeftSide =
    mat_mult(rotation_matrix_y((-90)), cubePlane);
    cubeRightSide =
    mat_mult(rotation_matrix_y((90)), cubePlane);
    cubeHead =
    mat_mult(rotation_matrix_x((-90)), cubePlane);
    cubeGround =
    mat_mult(rotation_matrix_x((90)), cubePlane);
    
    
    cubeFace = to_cartesian_coord(cubeFace);
    cubeLeftSide = to_cartesian_coord(cubeLeftSide);
    cubeRightSide = to_cartesian_coord(cubeRightSide);
    cubeBack = to_cartesian_coord(cubeBack);
    cubeHead = to_cartesian_coord(cubeHead);
    cubeGround = to_cartesian_coord(cubeGround);
    
    result.insert(result.end(), cubeFace.begin(), cubeFace.end());
    result.insert(result.end(), cubeLeftSide.begin(), cubeLeftSide.end());
    result.insert(result.end(), cubeRightSide.begin(), cubeRightSide.end());
    result.insert(result.end(), cubeBack.begin(), cubeBack.end());
    result.insert(result.end(), cubeHead.begin(), cubeHead.end());
    result.insert(result.end(), cubeGround.begin(), cubeGround.end());
    
    return result;
}

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
    gluPerspective(100.0, 1.0, 2.0, 30.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(4.0, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

vector<GLfloat> init_scene() {
    vector<GLfloat> scene;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> cpu;
    
    cpu = mat_mult(scaling_matrix(3.0, 2.3, 0.2), view);
    cpu = mat_mult(rotation_matrix_y(180), cpu);
    cpu = mat_mult(translation_matrix(1.0, 1.75, 2.5), cpu);
    
    vector<GLfloat> cpuNeck;
    
    cpuNeck = mat_mult(scaling_matrix(0.56, 1.5, 0.2), view);
    cpuNeck = mat_mult(rotation_matrix_z(180), cpuNeck);
    cpuNeck = mat_mult(translation_matrix(1.0,0.700,2.3), cpuNeck);
    
    vector<GLfloat> keyboard;
    
    
    keyboard = mat_mult(scaling_matrix(2.5, 0.7, 0.2), view);
    keyboard = mat_mult(rotation_matrix_y(180), keyboard);
    keyboard = mat_mult(rotation_matrix_x(100), keyboard);
    keyboard = mat_mult(translation_matrix(1.0, -0.25, 2.5), keyboard);
    
    vector<GLfloat> mouse;
    
    
    mouse = mat_mult(scaling_matrix(0.5, 1.2, 0.1), view);
    mouse = mat_mult(translation_matrix(-2.2,-2.25,2.5), mouse);
    mouse = mat_mult(rotation_matrix_x(100), mouse);
    
    mouse = mat_mult(rotation_matrix_y(180), mouse);
    
    vector<GLfloat> laptopScreen;
    
    laptopScreen = mat_mult(scaling_matrix(2.3, 1.85, 0.1), view);
    laptopScreen = mat_mult(translation_matrix(-3.3, -0.75, 1.5), laptopScreen);
    
    laptopScreen = mat_mult(rotation_matrix_x(0), laptopScreen);
    laptopScreen = mat_mult(rotation_matrix_y(40), laptopScreen);
    
    vector<GLfloat> laptopKeyboard;
    
    laptopKeyboard = mat_mult(scaling_matrix(2.3,1.6,0.1), view);
    laptopKeyboard = mat_mult(translation_matrix(-3.3,0.75,1.5), laptopKeyboard);
    
    laptopKeyboard = mat_mult(rotation_matrix_x(90), laptopKeyboard);
    laptopKeyboard = mat_mult(rotation_matrix_y(40), laptopKeyboard);
    
    vector<GLfloat> propGround;
    
    propGround = mat_mult(scaling_matrix(0.7, 1.2, 0.1), view);
    propGround = mat_mult(rotation_matrix_x(90), propGround);
    propGround = mat_mult(translation_matrix(4.0, -0.45, 2.5), propGround);
    
    vector<GLfloat> propNeck;
    
    propNeck = mat_mult(scaling_matrix(0.1, 4.0, 0.1), view);
    propNeck = mat_mult(translation_matrix(4.0, 1.4, 2.5), propNeck);
    
    vector<GLfloat> propHead;
    
    propHead = mat_mult(scaling_matrix(0.7, 0.7, 0.1), view);
    propHead = mat_mult(rotation_matrix_z(180), propHead);
    propHead = mat_mult(translation_matrix(4.0, 3.74, 2.5), propHead);
    
    
    cpu = to_cartesian_coord(cpu);
    cpuNeck = to_cartesian_coord(cpuNeck);
    keyboard = to_cartesian_coord(keyboard);
    mouse = to_cartesian_coord(mouse);
    laptopScreen = to_cartesian_coord(laptopScreen);
    laptopKeyboard = to_cartesian_coord(laptopKeyboard);
    propGround = to_cartesian_coord(propGround);
    propNeck = to_cartesian_coord(propNeck);
    propHead = to_cartesian_coord(propHead);
    
    
    scene.insert(scene.end(), cpu.begin(), cpu.end());
    scene.insert(scene.end(), keyboard.begin(), keyboard.end());
    scene.insert(scene.end(), mouse.begin(), mouse.end());
    scene.insert(scene.end(), laptopScreen.begin(), laptopScreen.end());
    scene.insert(scene.end(), laptopKeyboard.begin(), laptopKeyboard.end());
    scene.insert(scene.end(), cpuNeck.begin(), cpuNeck.end());
    scene.insert(scene.end(), propGround.begin(), propGround.end());
    scene.insert(scene.end(), propNeck.begin(), propNeck.end());
    scene.insert(scene.end(), propHead.begin(), propHead.end());
    
    return scene;
}

vector<GLfloat> init_color(vector<GLfloat> scene) {
    vector<GLfloat> colors;
    for (int i = 0; i < scene.size(); i++) {
        colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }
    return colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // World model parameters
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    long scene_size = SCENE.size()/16;
    GLfloat* pointColor = vector2array(COLOR);
    GLfloat* pointScene = vector2array(SCENE);
    
    vector<GLfloat> pointValues = to_homogeneous_coord(init_scene());
    pointValues = mat_mult(rotation_matrix_y(THETA), pointValues);
    SCENE = to_cartesian_coord(pointValues);
    
    //vector<GLfloat> xyzPoints;
    //vector<GLfloat> matValue;
    //vector<GLfloat> multResult;
    
    
    
    glVertexPointer(3,          // 3 components (x, y, z)
                    GL_FLOAT,   // Vertex type is GL_FLOAT
                    0,          // Start position in referenced memory
                    pointScene);  // Pointer to memory location to read from
    
    glColorPointer(3,           // 3 components (r, g, b)
                   GL_FLOAT,    // Vertex type is GL_FLOAT
                   0,           // Start position in referenced memory
                   pointColor);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 5 * scene_size);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    THETA = THETA + 0.03;
    display_func();
}

int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Assignment 3");
    
    setup();
    init_camera();
    SCENE = init_scene();
    COLOR = init_color(SCENE);
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    return 0;
}
