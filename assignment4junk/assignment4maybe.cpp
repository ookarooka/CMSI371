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

class ObjectModel {
    vector<GLfloat> _points;
    vector<GLfloat> _normals;
    vector<GLfloat> _base_colors;
    vector<GLfloat> _colors;
public:
    ObjectModel() { };
    vector<GLfloat> get_points() { return _points; };
    vector<GLfloat> get_normals() { return _normals; };
    vector<GLfloat> get_base_colors() { return _base_colors; };
    vector<GLfloat> get_colors() { return _colors; };
    void set_points(vector<GLfloat> points) { _points = points; };
    void set_normals(vector<GLfloat> normals) { _normals = normals; };
    void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
    void set_colors(vector<GLfloat> colors) { _colors = colors; };
};


//ObjectModel SCENE;

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
///////////////////////////////////////////////////////////
// Normalizes a given vector
vector<GLfloat> normalize(vector<GLfloat> a) {
    GLfloat magnitude = 0;
    
    for (int i = 0; i < a.size(); i++) {
        magnitude += pow(a[i],2);
    }
    
    magnitude = sqrt(magnitude);
    
    for (int i = 0; i < a.size(); i++) {
        a[i] = a[i] / magnitude;
    }
    
    return a;
}

// Performs the cross product between two vectors
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    
    vector<GLfloat> C;
    
    
    C.push_back(A[1]*B[2] - A[2]*B[1]); //assign x
    C.push_back(A[2]*B[0] - A[0]*B[2]); //assign y
    C.push_back(A[0]*B[1] - A[1]*B[0]); //assign z
    
    
    return normalize(C);
}

// Generates the normals to each surface (plane)
vector<GLfloat> generate_normals(vector<GLfloat> points) {
    vector<GLfloat> normals;
    
    for ( int i = 0; i < points.size(); i+=16) {
        
        vector<GLfloat> a = { points[i+0] - points[i+9],
            points[i+1] - points[i+10],
            points[i+2] - points[i+11], };
        
        vector<GLfloat> b = { points[i+6] - points[i+9],
            points[i+7] - points[i+10],
            points[i+8] - points[i+11] };
        vector<GLfloat> add = cross_product(a, b);
        normals.insert(normals.end(), add.begin(), add.end() );
    }
    
    return normals;
}


/**************************************************
 *       Shading via Illumination and Color       *
 *                                                *
 *  Generate the set of colors for each face of   *
 *  the planes that compose the objects in the    *
 *  scene. Based on the light source and surface  *
 *  normals, render the colors of the objects by  *
 *  applying the shading equation.                *
 *                                                *
 *************************************************/

// Performs the dot product between two vectors
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {
    
    return (A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]);
    
}

// Initializes the base color of a plane to a single color
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    return base_color;
}

// Initializes the base color of a plane by specifying the color of each point
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
    // This enables OpenGL to use interpolation for (Gouraud) shading the plane
    vector<GLfloat> base_color = {
        r0,   g0,   b0,
        r1,   g1,   b1,
        r2,   g2,   b2,
        r3,   g3,   b3
    };
    return base_color;
}

// Allows for ambience (a vector of 3 values), diffusion (vector of 3 values) and specular (vector of 3 values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec, GLfloat m) {
    vector<GLfloat> colors;
    
    object_model.set_points( to_cartesian_coord(object_model.get_points()) );
    vector<GLfloat> base_colors = object_model.get_base_colors();
    
    for (int i = 0; i < object_model.get_points().size() / 3; i++) {
        vector<GLfloat> light = {object_model.get_points()[i*3] - light_source[0],
            object_model.get_points()[i*3 + 1] - light_source[1],
            object_model.get_points()[i*3 + 2] - light_source[2]};
        
        light = normalize(light);
        
        vector<GLfloat> curr_normal = {object_model.get_normals()[i*3],
            object_model.get_normals()[i*3 + 1],
            object_model.get_normals()[i*3 + 2]};
        
        vector<GLfloat> h = {light[0] + camera[0],
            light[1] + camera[1],
            light[2] + camera[2]};
        
        h = normalize(h);
        
        GLfloat I_r = base_colors[0] * (amb[0] + diff[0] * dot_product(curr_normal, light) + spec[0] * pow(dot_product(curr_normal, h), m ) );
        GLfloat I_g = base_colors[i*3 + 1] * (amb[1] + diff[1] * dot_product(curr_normal, light) + spec[1] * pow(dot_product(curr_normal, h), m ) );
        GLfloat I_b = base_colors[i*3 + 2] * (amb[2] + diff[2] * dot_product(curr_normal, light) + spec[2] * pow(dot_product(curr_normal, h), m ) );
        
        colors.push_back(I_r);
        colors.push_back(I_g);
        colors.push_back(I_b);
    }
    
    object_model.set_points( to_homogeneous_coord(object_model.get_points()) );
    object_model.set_colors(colors);
    return object_model;
}
////////////////////////////////////////////////////
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

ObjectModel build_cpu(){
    vector<GLfloat> cpu;
    vector<GLfloat> view;
    vector<GLfloat> scene;
    view = to_homogeneous_coord(build_cube());
    
    cpu = mat_mult(scaling_matrix(3.0, 2.3, 0.2), view);
    cpu = mat_mult(rotation_matrix_y(180), cpu);
    cpu = mat_mult(translation_matrix(1.0, 1.75, 2.5), cpu);
    
    scene.insert(scene.end(), cpu.begin(), cpu.end());
    
    ObjectModel cpu_model;
    cpu_model.set_points(cpu);
    cpu_model.set_normals( generate_normals(cpu_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> cpu_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), cpu_colors.begin(), cpu_colors.end());
        
        cpu_model.set_base_colors(colors);
        return cpu_model;
    }
}
    
ObjectModel build_cpuNeck() {
    vector<GLfloat> cpuNeck;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    cpuNeck = mat_mult(scaling_matrix(0.56, 1.5, 0.2), view);
    cpuNeck = mat_mult(rotation_matrix_z(180), cpuNeck);
    cpuNeck = mat_mult(translation_matrix(1.0,0.700,2.3), cpuNeck);
    
    scene.insert(scene.end(), cpuNeck.begin(), cpuNeck.end());
    
    ObjectModel cpuNeck_model;
    cpuNeck_model.set_points(cpuNeck);
    cpuNeck_model.set_normals( generate_normals(cpuNeck_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> cpuNeck_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), cpuNeck_colors.begin(), cpuNeck_colors.end());
        
        cpuNeck_model.set_base_colors(colors);
        return cpuNeck_model;
    }
}

ObjectModel build_keyboard(){
    vector<GLfloat> keyboard;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    keyboard = mat_mult(scaling_matrix(2.5, 0.7, 0.2), view);
    keyboard = mat_mult(rotation_matrix_y(180), keyboard);
    keyboard = mat_mult(rotation_matrix_x(100), keyboard);
    keyboard = mat_mult(translation_matrix(1.0, -0.25, 2.5), keyboard);
    
    scene.insert(scene.end(), keyboard.begin(), keyboard.end());
    
    ObjectModel keyboard_model;
    keyboard_model.set_points(keyboard);
    keyboard_model.set_normals( generate_normals(keyboard_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> keyboard_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), keyboard_colors.begin(), keyboard_colors.end());
        
        keyboard_model.set_base_colors(colors);
        return keyboard_model;
        
    }
}

ObjectModel build_mouse(){
    vector<GLfloat> mouse;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    mouse = mat_mult(scaling_matrix(0.5, 1.2, 0.1), view);
    mouse = mat_mult(translation_matrix(-2.2,-2.25,2.5), mouse);
    mouse = mat_mult(rotation_matrix_x(100), mouse);
    mouse = mat_mult(rotation_matrix_y(180), mouse);
    
     scene.insert(scene.end(), mouse.begin(), mouse.end());
    
    ObjectModel mouse_model;
    mouse_model.set_points(mouse);
    mouse_model.set_normals( generate_normals(mouse_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> mouse_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), mouse_colors.begin(), mouse_colors.end());
        
        mouse_model.set_base_colors(colors);
        return mouse_model;
    }
}

ObjectModel build_laptopScreen(){
    vector<GLfloat> laptopScreen;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    laptopScreen = mat_mult(scaling_matrix(2.3, 1.85, 0.1), view);
    laptopScreen = mat_mult(translation_matrix(-3.3, -0.75, 1.5), laptopScreen);
    laptopScreen = mat_mult(rotation_matrix_x(0), laptopScreen);
    laptopScreen = mat_mult(rotation_matrix_y(40), laptopScreen);
    
    scene.insert(scene.end(), laptopScreen.begin(), laptopScreen.end());
    
    ObjectModel laptopScreen_model;
    laptopScreen_model.set_points(laptopScreen);
    laptopScreen_model.set_normals( generate_normals(laptopScreen_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> laptopScreen_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), laptopScreen_colors.begin(), laptopScreen_colors.end());
        
        laptopScreen_model.set_base_colors(colors);
        return laptopScreen_model;
    
    }
}

ObjectModel build_laptopKeyboard(){
    vector<GLfloat> laptopKeyboard;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    laptopKeyboard = mat_mult(scaling_matrix(2.3,1.6,0.1), view);
    laptopKeyboard = mat_mult(translation_matrix(-3.3,0.75,1.5), laptopKeyboard);
    laptopKeyboard = mat_mult(rotation_matrix_x(90), laptopKeyboard);
    laptopKeyboard = mat_mult(rotation_matrix_y(40), laptopKeyboard);
    
    scene.insert(scene.end(), laptopKeyboard.begin(), laptopKeyboard.end());
    
    ObjectModel laptopKeyboard_model;
    laptopKeyboard_model.set_points(laptopKeyboard);
    laptopKeyboard_model.set_normals( generate_normals(laptopKeyboard_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> laptopKeyboard_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), laptopKeyboard_colors.begin(), laptopKeyboard_colors.end());
        
        laptopKeyboard_model.set_base_colors(colors);
        return laptopKeyboard_model;
    
    }
}

ObjectModel build_propGround(){
    vector<GLfloat> propGround;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    propGround = mat_mult(scaling_matrix(0.7, 1.2, 0.1), view);
    propGround = mat_mult(rotation_matrix_x(90), propGround);
    propGround = mat_mult(translation_matrix(4.0, -0.45, 2.5), propGround);
    
    scene.insert(scene.end(), propGround.begin(), propGround.end());
    
    ObjectModel propGround_model;
    propGround_model.set_points(propGround);
    propGround_model.set_normals( generate_normals(propGround_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> propGround_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), propGround_colors.begin(), propGround_colors.end());
        
        propGround_model.set_base_colors(colors);
        return propGround_model;
    
    }
}

ObjectModel build_propNeck(){
    vector<GLfloat> propNeck;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    propNeck = mat_mult(scaling_matrix(0.1, 4.0, 0.1), view);
    propNeck = mat_mult(translation_matrix(4.0, 1.4, 2.5), propNeck);
    
    scene.insert(scene.end(), propNeck.begin(), propNeck.end());
    
    ObjectModel propNeck_model;
    propNeck_model.set_points(propNeck);
    propNeck_model.set_normals( generate_normals(propNeck_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> propNeck_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), propNeck_colors.begin(), propNeck_colors.end());
        
        propNeck_model.set_base_colors(colors);
        return propNeck_model;
}
    
ObjectModel build_propHead(){
    vector<GLfloat> propHead;
    vector<GLfloat> view;
    view = to_homogeneous_coord(build_cube());
    vector<GLfloat> scene;
    
    propHead = mat_mult(scaling_matrix(0.7, 0.7, 0.1), view);
    propHead = mat_mult(rotation_matrix_z(180), propHead);
    propHead = mat_mult(translation_matrix(4.0, 3.74, 2.5), propHead);
    
    scene.insert(scene.end(), propHead.begin(), propHead.end());
    
    ObjectModel propHead_model;
    propHead_model.set_points(propHead);
    propHead_model.set_normals( generate_normals(propHead_model.get_points()) );
    
    vector<GLfloat> colors;
    vector<GLfloat> propHead_colors = init_base_color(1.0, 1.0, 1.0);
    for ( int i = 0; i < 6; i++) {
        colors.insert(colors.end(), propHead_colors.begin(), propHead_colors.end());
        
        propHead_model.set_base_colors(colors);
        return propHead_model;
    }
}


    


    
    
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
