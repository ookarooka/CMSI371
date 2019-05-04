/***
 Assignment-4
 
 Name: Patterson, Joshua
 
 Collaborators: Richardson, Alex
 
 Project Summary: Crated a rainbow themed version of my desktop, laptop, 
 mouse and lamp using the build_cube() function to construct the scene. Added shadows through
 generating normals, defining base colors, creating a light source, and implementing a center of attention.
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
vector<GLfloat> normals;
vector<GLfloat> _points;
vector<GLfloat> _normals;
vector<GLfloat> _base_colors;
vector<GLfloat> _colors;
vector<GLfloat> amb = {0.4, 0.4, 0.4};
GLfloat m = 1.0;
vector<GLfloat> cent = {0.2, 0.4, 0.6};
vector<GLfloat> get_points() { return _points; };
vector<GLfloat> get_normals() { return _normals; };
vector<GLfloat> get_base_colors() { return _base_colors; };
vector<GLfloat> get_colors() { return _colors; };
void set_points(vector<GLfloat> points) { _points = points; };
void set_normals(vector<GLfloat> normals) { _normals = normals; };
void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
void set_colors(vector<GLfloat> colors) { _colors = colors; };
vector<GLfloat> camera = {0.0 - 4.0, 0.0 - 3.0, 0.0 - 6.0,};
vector<GLfloat> spec = {0.1, 0.1, 0.1};
vector<GLfloat> light_source = {2, 5, 10};

//////////
float deg2rad(float d) {
    return (d*M_PI) / 180.0;
}
/////////
GLfloat normShade(vector<GLfloat> x){
    GLfloat point = 0.0;
    for (int i = 0; i < x.size(); i++) {
        point += pow(x[i],2);
    }
    return sqrt(point);
}
//////////
vector<GLfloat> normVec(vector<GLfloat> x,GLfloat point) {
    for (int i = 0; i < x.size(); i++) {
        x[i] = x[i] /point;
    }
    return x;
}
////////////
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}
//////////
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        0.5,   0.5,   0.0,
        -0.5,   0.5,   0.0,
        -0.5,   -0.5,   0.0,
        0.5,   -0.5,   0.0
    };
    return vertices;
}
/////////
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
/////////
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
/////////
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
/////////
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
////////////
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
////////////
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
///////////
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
///////////
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
///////
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
    mat_mult(rotation_matrix_y((180)), cubePlane);
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

/////
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> C;
    C.push_back(A[1]*B[2] - A[2] * B[1]);
    C.push_back(-A[0]*B[2] + A[2] * B[0]);
    C.push_back(A[0]*B[1] - A[1] * B[0]);
    return C;
}

/////
vector<GLfloat> generate_normals(vector<GLfloat> points) {
    vector<GLfloat> normals;
    for(int i = 0; i < points.size(); i+=12) {
        vector<GLfloat> a;
        vector<GLfloat> b;
        vector<GLfloat> c;

        a.push_back(points[i] - points[i+9]);
        a.push_back(points[i+1] - points[i+10]);
        a.push_back(points[i+2] - points[i+11]);
        b.push_back(-points[i+6] - points[i+9]);
        b.push_back(-points[i+7] - points[i+10]);
        b.push_back(-points[i+8] - points[i+11]);
        c = cross_product(a, b);
            normals.insert(normals.end(), c.begin(), c.end());
    }
    return normals;
}

////////
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {
    return ((A[0] * B[0]) + (A[1] * B[1]) + (A[2] * B[2]));
}

///////
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    return base_color;
}
////////
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
    vector<GLfloat> base_color = {
        r0,   g0,   b0,
        r1,   g1,   b1,
        r2,   g2,   b2,
        r3,   g3,   b3
    };
    return base_color;
}


///////
vector<GLfloat> apply_shading(vector<GLfloat> points, vector<GLfloat> normals, vector<GLfloat> base_color,
                              vector<GLfloat> light_source, vector<GLfloat> camera,
                              vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec, GLfloat m) {
    vector<GLfloat> center;
    vector<GLfloat> light;
    vector<GLfloat> colors;
    points = to_cartesian_coord(points);
    
    for(int i = 0; i < points.size()/3; i++) {
        vector<GLfloat> light = {
            -1*(points[i*3] - light_source[0]),
            -1*(points[i*3 + 1] - light_source[1]),
            -1*(points[i*3 + 2] - light_source[2]),
        };
        GLfloat light_norm = normShade(light);
        light = normVec(light, light_norm);
        vector<GLfloat> center = {
            light[0] + camera[0],
            light[1] + camera[1],
            light[2] + camera[2],
        };
        GLfloat center_norm = normShade(center);
        center = normVec(center, center_norm);
        GLfloat lightNorm = dot_product({normals[i*3], normals[i*3+1], normals[i*3+2]}, light);
        GLfloat normCenter = dot_product({normals[i*3], normals[i*3+1], normals[i*3+2]}, center);
        GLfloat r = base_color[i*3 + 0] * ((amb[0] + diff[0]) * lightNorm + spec[0] * pow(normCenter,m));
        GLfloat g = base_color[i*3+1] * ((amb[1] + diff[1]) * lightNorm + spec[1] * pow(normCenter, m));
        GLfloat b = base_color[i*3+2] * ((amb[2] + diff[2]) * lightNorm + spec[2] * pow(normCenter, m));
        colors.push_back(r);
        colors.push_back(g);
        colors.push_back(b);
    }
    return colors;
}
////////
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
///////
void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    gluPerspective(120.0, 1.0, 2.0, 30.0);
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
    cpuNeck = mat_mult(translation_matrix(1.0,0.700,2.2), cpuNeck);
    
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
    
    normals = generate_normals(scene);
    return scene;
}

/////
vector<GLfloat> init_color(vector<GLfloat> points) {
    vector<GLfloat> colors;
    
    vector<GLfloat> mouse = init_base_color(.9, .90, .25);
    for(int i = 0; i < 16; i++) {
    }

    vector<GLfloat> cpuNeck = init_base_color(.9, .9, .2222);
    for(int i = 0; i < 64; i++) {
    }
    vector<GLfloat> keyboard = init_base_color(0, 0, 0);
    for(int i = 0; i < 16; i++) {
    }
    vector<GLfloat> cpuScreen = init_base_color(.5, .5, .5);
    for(int i = 0; i < 64; i++) {
    }
    vector<GLfloat> laptopScreen = init_base_color(.5, .5, .5);
    for(int i = 0; i < 64; i++) {
    }
    vector<GLfloat> laptopKeyboard = init_base_color(.9, .9, .2222);
    for(int i = 0; i < 6; i++) {
    }
    vector<GLfloat> prop = init_base_color(.9, .9, .2222);
    for(int i = 0; i < 64; i++) {
    }
    colors.insert(colors.end(), cpuScreen.begin(), cpuScreen.end());
    colors.insert(colors.end(), cpuNeck.begin(), cpuNeck.end());
    colors.insert(colors.end(), keyboard.begin(), keyboard.end());
    colors.insert(colors.end(), mouse.begin(), mouse.end());
    colors.insert(colors.end(), laptopScreen.begin(), laptopScreen.end());
    colors.insert(colors.end(), laptopKeyboard.begin(), laptopKeyboard.end());
    colors.insert(colors.end(), prop.begin(), prop.end());


    vector<GLfloat> shading = apply_shading(points, normals, colors, light_source, camera, amb, cent, spec, m);
    return shading;
}
//////
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
    glDrawArrays(GL_QUADS, 0, 5*scene_size);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
}
/////
void idle_func() {
    THETA = THETA + 0.03;
    display_func();
}
/////
int main (int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Assignment 4");
    
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
