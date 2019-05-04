/***
 Assignment-4: Shading via Illumination and Colors
 
 Name: Patterson, Joshua;
 
 Collaborators: Richardson, Alex;
 
 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project.
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
vector<GLfloat> amb = {0.4, 0.4, 0.4};
vector<GLfloat> diff = {0.1, 0.1, 0.1};
vector<GLfloat> spec = {0.1, 0.1, 0.1};
vector<GLfloat> light_source = {-2.0, 4, 0.0};
vector<GLfloat> camera = {0.0 - 4.0, 0.0 - 3.0, 0.0 - 6.0,};
GLfloat m = 1.0;

/**************************************************
 *              Object Model Class                *
 *                                                *
 *  Stores the points of the object as a vector   *
 *  along with the colors and normals for each    *
 *  point. Normals are computed from the points.  *
 *                                                *
 *************************************************/
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

// The model of the scene
ObjectModel SCENE;

/**************************************************
 *              Utilitie Functions                *
 *************************************************/

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
    rotate_mat_x = {
        1.0,    0.0,                    0.0,                       0.0,
        0.0,    (float)(cos(theta)),    (float)(-sin(theta)),      0.0,
        0.0,    (float)(sin(theta)),    (float)(cos(theta)),       0.0,
        0.0,    0.0,                    0.0,                       1.0
    };
    return rotate_mat_x;
}


// Definition of a rotation matrix about the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y;
    rotate_mat_y = {
        (float)cos(theta),     0.0,     (float)sin(theta),   0.0,
        0.0,                   1.0,     0.0,                  0.0,
        (float)-sin(theta),     0.0,     (float)cos(theta),    0.0,
        0.0,                   0.0,     0.0,                  1.0
    };
    return rotate_mat_y;
}


// Definition of a rotation matrix about the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z;
    rotate_mat_z = {
        (float)cos(theta),  (float)-sin(theta), 0.0,    0.0,
        (float)sin(theta),  (float)cos(theta),  0.0,    0.0,
        0.0,                0.0,                1.0,    0.0,
        0.0,                0.0,                0.0,    1.0
    };
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;
    
    for (int b = 0; b < B.size()/4; b++) {
        for (int a = 0; a < 4; a++) {
            float element_wise_sum = 0.0;
            for (int k = 0; k < 4;  k++) {
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

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    // Primitive plane
    vector<GLfloat> a0 = to_homogeneous_coord(init_plane());
    // Construct 6 planes of the cube
    vector<GLfloat> a1 = mat_mult(translation_matrix(0.0,  0.0,  0.5), a0);
    vector<GLfloat> a2 = mat_mult(translation_matrix(0.0,  0.0, -0.5), mat_mult(rotation_matrix_y(deg2rad(180)), a0));
    vector<GLfloat> a3 = mat_mult(translation_matrix(-0.5, 0.0,  0.0), mat_mult(rotation_matrix_y(deg2rad(-90)), a0));
    vector<GLfloat> a4 = mat_mult(translation_matrix(0.5,  0.0,  0.0), mat_mult(rotation_matrix_y(deg2rad(90)), a0));
    vector<GLfloat> a5 = mat_mult(translation_matrix(0.0,  0.5,  0.0), mat_mult(rotation_matrix_x(deg2rad(-90)), a0));
    vector<GLfloat> a6 = mat_mult(translation_matrix(0.0, -0.5,  0.0), mat_mult(rotation_matrix_x(deg2rad(90)), a0));
    
    result.insert(std::end(result), std::begin(a1), std::end(a1));
    result.insert(std::end(result), std::begin(a2), std::end(a2));
    result.insert(std::end(result), std::begin(a3), std::end(a3));
    result.insert(std::end(result), std::begin(a4), std::end(a4));
    result.insert(std::end(result), std::begin(a5), std::end(a5));
    result.insert(std::end(result), std::begin(a6), std::end(a6));
    
    return result;
}


/**************************************************
 *           Generating Surface Normals           *
 *                                                *
 *  Generate the surface normals of the objects   *
 *  using the cross product between two vectors   *
 *  that lie on the surface (plane) of interest.  *
 *  Recall that the direction of the normal to a  *
 *  surface follows the Right Hand Rule.          *
 *                                                *
 *************************************************/

// Performs the cross product between two vectors
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> C;
    // AyBz - AzBy
    C.push_back(A[1]*B[2] - A[2] * B[1]);
    // - AxBz + AzBx
    C.push_back((-A[0])*B[2] + A[2] * B[0]);
    // AxBy - AyBx
    C.push_back(A[0]*B[1] - A[1] * B[0]);
    // vector of 3 elements
    return C;
}

// Generates the normals to each surface (plane)
vector<GLfloat> generate_normals(vector<GLfloat> points) {
    vector<GLfloat> normals;
    // TODO: generates the normals to each surface
    for(int i = 0; i < points.size(); i+=16) {
        vector<GLfloat> a;
        vector<GLfloat> b;
        vector<GLfloat> c;
        // a = p0 - p3;
        a.push_back(-points[i] + points[i+12]);
        a.push_back(-points[i+1] + points[i+13]);
        a.push_back(-points[i+2] + points[i+14]);
        // b = p2 - p3;
        b.push_back(-points[i+8] + points[i+12]);
        b.push_back(-points[i+9] + points[i+13]);
        b.push_back(-points[i+10] + points[i+14]);
        c = cross_product(a, b);
        for(int i = 0; i < 4; i++){
            normals.insert(normals.end(), c.begin(), c.end());
        }
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
    return ((A[0] * B[0]) + (A[1] * B[1]) + (A[2] * B[2]));
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
//////////////////////////////////////////////////?
vector<GLfloat> generate_light_vector(vector<GLfloat> light_source, vector<GLfloat> points){
    vector<GLfloat> light_vector;
    
    for(int i = 0; i < points.size(); i+=3){
        light_vector.push_back(points[i] - light_source[0]);
        light_vector.push_back(points[i+1] - light_source[1]);
        light_vector.push_back(points[i+2] - light_source[2]);
        for( int j = 0; j < 3; j++) {
            GLfloat mag;
            mag = pow((points[i] - light_vector[i]), 2) + pow((points[i+1] - light_vector[i+1]), 2) + pow((points[i+2] - light_vector[i+2]), 2);
            mag = sqrt(mag);
            light_vector[i] = light_vector[i] / mag;
            light_vector[i+1] = light_vector[i+1] / mag;
            light_vector[i+2] = light_vector[i+2] / mag;
        }
        
    }
    return light_vector;
}
GLfloat get_norm(vector<GLfloat> v){
    GLfloat norm = 0.0;
    for(int i = 0; i < 3; i++) {
        norm += pow(v[i], 2);
    }
    return sqrt(norm);
}
////////////////////////////////////////////?
vector<GLfloat> apply_norm(vector<GLfloat> v,GLfloat norm) {
    for(int i = 0; i < 3; i++) {
        v[i] = v[i]/norm;
    }
    return v;
}
///////////////////////////////////////////?


// Allows for ambience (a vector of 3 values), diffusion (vector of 3 values) and specular (vector of 3 values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec, GLfloat m) {
    vector<GLfloat> colors;
    vector<GLfloat> points = object_model.get_points();
    points = to_cartesian_coord(points);
    vector<GLfloat> normals= object_model.get_normals();
    vector<GLfloat> base_color = object_model.get_base_colors();
    vector<GLfloat> h;
    vector<GLfloat> light;
    
    // Camera Vector
    GLfloat norm = get_norm(camera);
    camera = apply_norm(camera, norm);
    
    for(int i = 0; i < points.size()/3; i++) {
        vector<GLfloat> light = {
            -1*(points[i*3] - light_source[0]),
            -1*(points[i*3 + 1] - light_source[1]),
            -1*(points[i*3 + 2] - light_source[2]),
        };
        GLfloat light_norm = get_norm(light);
        light = apply_norm(light, light_norm);
        vector<GLfloat> h = {
            light[0] + camera[0],
            light[1] + camera[1],
            light[2] + camera[2],
        };
        GLfloat h_norm = get_norm(h);
        h = apply_norm(h, h_norm);
        GLfloat n_dot_l = dot_product({normals[i*3], normals[i*3+1], normals[i*3+2]}, light);
        GLfloat n_dot_h = dot_product({normals[i*3], normals[i*3+1], normals[i*3+2]}, h);
        GLfloat I_r = base_color[i*3 + 0] * ((amb[0] + diff[0]) * n_dot_l + spec[0] * pow(n_dot_h, m));
        GLfloat I_g = base_color[i*3 + 1] * ((amb[1] + diff[1]) * n_dot_l + spec[1] * pow(n_dot_h, m));
        GLfloat I_b = base_color[i*3 + 2] * ((amb[2] + diff[2]) * n_dot_l + spec[2] * pow(n_dot_h, m));
        colors.push_back(I_r);
        colors.push_back(I_g);
        colors.push_back(I_b);
    }
    
    object_model.set_colors(colors);
    return object_model;
}


/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by setting       *
 *  the projection/viewing matrices               *
 *                                                *
 *************************************************/

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
    gluPerspective(75.0, 1.0, 1.0, 30.0);
    gluLookAt(4.0, 3.0, 6.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
}

ObjectModel build_cpu() {
    ObjectModel cpu;
    vector<GLfloat> colors;
    vector<GLfloat> cpuFace;
   
    
    cpuFace = mat_mult(scaling_matrix(3.0, 2.3, 0.2), build_cube());
    cpuFace = mat_mult(rotation_matrix_y(180), cpuFace);
    cpuFace = mat_mult(translation_matrix(1.0, 1.75, 2.5), cpuFace);
    
    
    
    //scene.insert(scene.end(), cpuFace.begin(), cpuFace.end());
 
    
    cpu.set_points(cpuFace);
    cpu.set_normals(generate_normals(cpu.get_points()));
    
    vector<GLfloat> cpu_color = init_base_color(1, 0.1, 0.4);
    for(int i = 0; i < 24; i++) {
        colors.insert(colors.end(), cpu_color.begin(), cpu_color.end());
    }
    cpu.set_base_colors(colors);
    cpu = apply_shading(cpu, light_source, camera, amb, diff, spec, m);
    cpu.set_colors(cpu.get_colors());
    
    return cpu;
}
////////////////////////////////////////
ObjectModel build_cpuNeck(){
    ObjectModel cpu_model;
    vector<GLfloat> cpuNeck;
    vector<GLfloat> colors;
    
    cpuNeck = mat_mult(scaling_matrix(0.56, 1.5, 0.2), build_cube());
    cpuNeck = mat_mult(rotation_matrix_z(180), cpuNeck);
    cpuNeck = mat_mult(translation_matrix(1.0,0.75,2.399999), cpuNeck);
    
    cpu_model.set_points(cpuNeck);
    cpu_model.set_normals(generate_normals(cpu_model.get_points()));
    
    
    vector<GLfloat> cpu_face = init_base_color(0.9, 1, 1);
    vector<GLfloat> cpuNeck_color = init_base_color(0, 0, 0);
    
    colors.insert(colors.end(), cpu_face.begin(), cpu_face.end());
    for(int i = 0; i < 5; i++) {
        colors.insert(colors.end(), cpuNeck_color.begin(), cpuNeck_color.end());
    }
    cpu_model.set_base_colors(colors);
    cpu_model = apply_shading(cpu_model, light_source, camera, amb, diff, spec, m);
    cpu_model.set_colors(cpu_model.get_colors());
    return cpu_model;
}
///////////////////////////////////////
ObjectModel build_keyboard() {
    vector<GLfloat> scene;
    ObjectModel keyboard_design;
    vector<GLfloat> keyboard;
    vector<GLfloat> colors;
    
    
    keyboard = mat_mult(scaling_matrix(2.5, 0.7, 0.2), build_cube());
    keyboard = mat_mult(rotation_matrix_y(180), keyboard);
    keyboard = mat_mult(rotation_matrix_x(100), keyboard);
    keyboard = mat_mult(translation_matrix(1.0, -0.25, 2.5), keyboard);
    
    
    scene.insert(scene.end(), keyboard.begin(), keyboard.end());
   
    
    keyboard_design.set_points(scene);
    keyboard_design.set_normals(generate_normals(keyboard_design.get_points()));
    vector<GLfloat> keyboard_color = init_base_color(0.8, 0.6, 0.2);
    
    for(int i = 0; i < 12; i++) {
        colors.insert(colors.end(), keyboard_color.begin(), keyboard_color.end());
    }
    keyboard_design.set_base_colors(colors);
    keyboard_design = apply_shading(keyboard_design, light_source, camera, amb, diff, spec, m);
    keyboard_design.set_colors(keyboard_design.get_colors());
    return keyboard_design;
}
///////////////////////////////////////////////////////////
ObjectModel build_laptop(){
    ObjectModel laptop;
    vector<GLfloat> scene;
    vector<GLfloat> laptop_screen;
    vector<GLfloat> laptopKeyboard;
    vector<GLfloat> colors;
    
    laptop_screen = mat_mult(scaling_matrix(2.3, 1.85, 0.1), build_cube());
    laptop_screen = mat_mult(translation_matrix(-3.3, -0.75, 1.5), laptop_screen);
    laptop_screen = mat_mult(rotation_matrix_x(0), laptop_screen);
    laptop_screen = mat_mult(rotation_matrix_y(40), laptop_screen);
    
    laptopKeyboard = mat_mult(scaling_matrix(2.3,1.6,0.1), build_cube());
    laptopKeyboard = mat_mult(translation_matrix(-3.3,0.75,1.5), laptopKeyboard);
    laptopKeyboard = mat_mult(rotation_matrix_x(90), laptopKeyboard);
    laptopKeyboard = mat_mult(rotation_matrix_y(40), laptopKeyboard);
    
    scene.insert(scene.end(), laptop_screen.begin(), laptop_screen.end());
    scene.insert(scene.end(), laptopKeyboard.begin(), laptopKeyboard.end());
   
    laptop.set_points(scene);
    laptop.set_normals(generate_normals(laptop.get_points()));
    
    vector<GLfloat> laptop_color = init_base_color(0, 0.9, 0.6);

    
    for(int i = 0; i < 12; i++) {
        colors.insert(colors.end(), laptop_color.begin(), laptop_color.end());
    }
   
    laptop.set_base_colors(colors);
    laptop = apply_shading(laptop, light_source, camera, amb, diff, spec, m);
    vector<GLfloat> laptop_colors = laptop.get_colors();
    for (int i = 18*4*3-1; i > 11*4*3; i--) {
        laptop_colors[i] = laptop_colors[i]+0.7;
    }
    laptop.set_colors(laptop_colors);
    
    return laptop;
}
/////////////////////////////////////////////////////////
// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene;
    vector<GLfloat> cube;
    cube  = build_cube();
    cube = mat_mult(translation_matrix(light_source[0], light_source[1], light_source[2]), cube);
    
    ObjectModel light;
    light.set_points(cube);
    vector<GLfloat> cube_colors;
    vector<GLfloat> cube_base = init_base_color(0, 1, 0);
    for (int i = 0; i < 6; i++) {
        cube_colors.insert(cube_colors.end(), cube_base.begin(), cube_base.end());
    }
    light.set_colors(cube_colors);
    
    light.set_normals(generate_normals(light.get_points()));
    ObjectModel cpu = build_cpu();
    vector<GLfloat> cpu_points = cpu.get_points();
    scene.insert(scene.end(), cpu_points.begin(), cpu_points.end());
    ObjectModel cpuNeck = build_cpuNeck();
    vector<GLfloat> cpuNeck_points = cpuNeck.get_points();
    scene.insert(scene.end(), cpuNeck_points.begin(), cpuNeck_points.end());
    ObjectModel keyboard = build_keyboard();
    vector<GLfloat> keyboard_points = keyboard.get_points();
    scene.insert(scene.end(), keyboard_points.begin(), keyboard_points.end());
    ObjectModel laptop = build_laptop();
    vector<GLfloat> laptop_points = laptop.get_points();
    scene.insert(scene.end(), laptop_points.begin(), laptop_points.end());
    
    scene.insert(scene.end(), cube.begin(), cube.end());
    return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color() {
    vector<GLfloat> colors;
    vector<GLfloat> cpu_color;
    vector<GLfloat> cpuNeck_color;
    vector<GLfloat> cpu_face;
    vector<GLfloat> keyboard_color;
    vector<GLfloat> laptop_color;
    
    ObjectModel cpu = build_cpu();
    vector<GLfloat> cpu_points = cpu.get_colors();
    colors.insert(colors.end(), cpu_points.begin(), cpu_points.end());
    
    ObjectModel cpuNeck = build_cpuNeck();
    vector<GLfloat> cpuNeck_points = cpuNeck.get_colors();
    colors.insert(colors.end(), cpuNeck_points.begin(), cpuNeck_points.end());
    
    ObjectModel keyboard = build_keyboard();
    vector<GLfloat> keyboard_points = keyboard.get_colors();
    colors.insert(colors.end(), keyboard_points.begin(), keyboard_points.end());
    
    ObjectModel laptop = build_laptop();
    vector<GLfloat> laptop_points = laptop.get_colors();
    colors.insert(colors.end(), laptop_points.begin(), laptop_points.end());
    
    return colors;
}


void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    // TODO: Apply shading to the scene
    SCENE.set_points(init_scene());
    SCENE.set_colors(init_color());
    SCENE.set_points(to_cartesian_coord(SCENE.get_points()));
    
    // TODO: Rotate the scene using the rotation matrix
    vector<GLfloat> homogenousPoints;
    homogenousPoints = to_homogeneous_coord(SCENE.get_points());
    homogenousPoints = mat_mult(rotation_matrix_y(deg2rad(THETA)), homogenousPoints);
    SCENE.set_points(to_cartesian_coord(homogenousPoints));
    
    
    GLfloat* scene_vertices = vector2array(SCENE.get_points());
    GLfloat* color_vertices = vector2array(SCENE.get_colors());
    // Pass the scene vertex pointer
    glVertexPointer(3,                // 3 components (x, y, z)
                    GL_FLOAT,         // Vertex type is GL_FLOAT
                    0,                // Start position in referenced memory
                    scene_vertices);  // Pointer to memory location to read from
    
    // Pass the color vertex pointer
    glColorPointer(3,                   // 3 components (r, g, b)
                   GL_FLOAT,            // Vertex type is GL_FLOAT
                   0,                   // Start position in referenced memory
                   color_vertices);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices with 3 dimensions
    glDrawArrays(GL_QUADS, 0, (int)SCENE.get_points().size() / 3);
    
    glFlush();            //Finish rendering
    glutSwapBuffers();
    
    // Clean up
    delete scene_vertices;
    delete color_vertices;
}

void idle_func() {
    THETA = THETA + 0.3;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Assignment 4");
    
    setup();
    init_camera();
    
    SCENE.set_points(init_scene());
    SCENE.set_base_colors(init_color());
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    return 0;
}

