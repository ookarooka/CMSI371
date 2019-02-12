/***
 assignment1.cpp
 Assignment-1: Cartoonify

 Name: Patterson, Joshua

 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project: e.g. did you use the Chaikin's or Bezier's algorithm?
 Did you take an iterative or recursive approach?
 ***/


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <vector>
#include <iostream>
#define GL_SILENCE_DEPRECATION
using namespace std;
class Vertex {
    GLfloat x, y;
public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};
Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y;
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> points;
    points.push_back(control_points.at(0));
    for(int i = 0; i < (control_points.size()-1); i++) {
        float delta_x = control_points[i+1].get_x() - control_points[i].get_x();
        float delta_y = control_points[i+1].get_y() - control_points[i].get_y();
        Vertex quarter_point = Vertex(control_points[i].get_x() + delta_x * 0.25, control_points[i].get_y() + delta_y * 0.25);
        Vertex three_quarter_point = Vertex(control_points[i].get_x() + delta_x * 0.75, control_points[i].get_y() + delta_y * 0.75);
        points.push_back(quarter_point);
        points.push_back(three_quarter_point);
    }
    points.push_back(control_points.back());
    return points;
}

void draw_curve(vector<Vertex> control_points, int n_iter) {
    vector<Vertex> draw_points;
    draw_points = control_points;
    for(int i = 0; i < n_iter; i++) {
        draw_points = generate_points(draw_points);
    }
    glBegin(GL_LINES);
    for(int i = 0; i < draw_points.size()-1; i++) {
        glVertex2f(draw_points.at(i).get_x(), draw_points.at(i).get_y());
        glVertex2f(draw_points.at(i+1).get_x(), draw_points.at(i+1).get_y());
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);
    
    vector<Vertex> toprighthead =
    { Vertex(0.0f, 0.55f),
      Vertex(0.1f, 0.54f),
      Vertex(0.239f, 0.492f),
        Vertex(0.368f, 0.407f),
        Vertex(0.415f, 0.332f),
        Vertex(0.454f, 0.221f),
        Vertex(0.463f, 0.177f),
        Vertex(0.466f, 0.175f),
        Vertex(0.462f, 0.153f),
        Vertex(0.459f, 0.139f)};
    
   vector<Vertex> toplefthead =
    { Vertex(0.0f, 0.55f),
        Vertex(-0.1f, 0.54f),
        Vertex(-0.239f, 0.492f),
        Vertex(-0.368f, 0.407f),
        Vertex(-0.415f, 0.332f),
        Vertex(-0.454f, 0.221f),
        Vertex(-0.463f, 0.177f),
        Vertex(-0.466f, 0.175f),
        Vertex(-0.462f, 0.153f),
        Vertex(-0.459f, 0.139f)};
    
    vector<Vertex> botrighthead = {
        Vertex(0.0f, -0.55f),
        Vertex(0.1f, -0.54f),
        Vertex(0.239f, -0.492f),
        Vertex(0.368f, -0.407f),
        Vertex(0.415f, -0.332f),
        Vertex(0.454f, -0.221f),
        Vertex(0.463f, -0.177f),
        Vertex(0.466f, -0.175f),
        Vertex(0.458f, -0.14f),
        Vertex(0.452f, -0.106f),
      };
    vector<Vertex> botlefthead = {
        Vertex(0.0f, -0.55f),
        Vertex(-0.1f, -0.54f),
        Vertex(-0.239f, -0.492f),
        Vertex(-0.368f, -0.407f),
        Vertex(-0.415f, -0.332f),
        Vertex(-0.454f, -0.221f),
        Vertex(-0.463f, -0.177f),
        Vertex(-0.466f, -0.175f),
        Vertex(-0.458f, -0.14f),
        Vertex(-0.452f, -0.106f),
    };
    vector<Vertex> leftear = {
       Vertex(-0.459f, 0.133f),
        Vertex(-0.477f, 0.151f),
        Vertex(-0.508f, 0.15f),
        Vertex(-0.497f, 0.085f),
        Vertex(-0.481f, 0.041f),
        Vertex(-0.478f, 0.001f),
        Vertex(-0.476f, -0.04f),
        Vertex(-0.473f, -0.084f),
        Vertex(-0.451f, -0.116f),
    };
    vector<Vertex> rightear = {
        Vertex(0.459f, 0.133f),
        Vertex(0.477f, 0.151f),
        Vertex(0.508f, 0.15f),
        Vertex(0.497f, 0.085f),
        Vertex(0.481f, 0.041f),
        Vertex(0.478f, 0.001f),
        Vertex(0.476f, -0.04f),
        Vertex(0.473f, -0.084f),
        Vertex(0.451f, -0.116f),
    };
    vector <Vertex> toplefteye = {
        Vertex(-0.119f, 0.096f),
        Vertex(-0.172f, 0.119f),
        Vertex(-0.2225f, 0.121f),
        Vertex(-0.318f, 0.093f),
        Vertex(-0.333f, 0.092f),
        Vertex(-0.306f, 0.069f),
        Vertex(-0.263f, 0.058f),
        Vertex(-0.228f, 0.055f),
        Vertex(-0.191f, 0.058f),
        Vertex(-0.119f, 0.096f),
    };
    
    
    
    vector <Vertex> lefteyebrow = {
        Vertex(-0.095f, 0.159f),
        Vertex(-0.215f, 0.182f),
        Vertex(-0.335f, 0.1755f),
  
    };
    
    vector <Vertex> righteyebrow = {
        Vertex(0.095f, 0.159f),
        Vertex(0.215f, 0.182f),
        Vertex(0.335f, 0.1755f),
        
    };

    vector <Vertex> toprighteye = {
        Vertex(0.119f, 0.096f),
        Vertex(0.172f, 0.119f),
        Vertex(0.2225f, 0.121f),
        Vertex(0.318f, 0.093f),
        Vertex(0.333f, 0.092f),
        Vertex(0.306f, 0.069f),
        Vertex(0.263f, 0.058f),
        Vertex(0.228f, 0.055f),
        Vertex(0.191f, 0.058f),
        Vertex(0.119f, 0.096f),
    };
    
    vector<Vertex> leftnose =
    {
        Vertex(-0.089f, -0.018f),
        Vertex(-0.142f, -0.026f),
        Vertex(-0.176f, -0.074f),
        Vertex(-0.114f, -0.113f),
        Vertex(-0.089f, -0.091f),
        Vertex(-0.064f, -0.085f),
        Vertex(-0.023f, -0.106f),
        Vertex(0.004f, -0.109f),
        
        
    };
    
    
    vector<Vertex> rightnose =
    {
        Vertex(0.089f, -0.018f),
        Vertex(0.142f, -0.026f),
        Vertex(0.176f, -0.074f),
        Vertex(0.114f, -0.113f),
        Vertex(0.089f, -0.091f),
        Vertex(0.064f, -0.085f),
        Vertex(0.023f, -0.106f),
        Vertex(0.004f, -0.109f),
        
        
    };
    
    
    vector<Vertex> toplip =
    {
        Vertex(0.001, -0.2),
        Vertex(-0.038f, -0.185f),
        Vertex(-0.13f, -0.2f),
        Vertex(-0.18f, -0.234f),
        Vertex(-0.005f, -0.243f),
        Vertex(0.18f, -0.234f),
        Vertex(0.13f, -0.2f),
        Vertex(0.038f, -0.185f),
        Vertex(0.001, -0.2),

        
        
        
    };
    
    vector<Vertex> bottomlip =
    {
        Vertex(0.001, -0.245),
        Vertex(-0.038f, -0.245f),
        Vertex(-0.13f, -0.238f),
        Vertex(-0.144f, -0.296f),
        Vertex(-0.089f, -0.315f),
        Vertex(-0.008f, -0.314f),
        Vertex(0.089f, -0.315f),
        Vertex(0.144f, -0.296f),
        Vertex(0.13f, -0.238f),
        Vertex(0.038f, -0.245f),
        Vertex(0.001, -0.245),
        
        
        
        
        
        
    };
    
    
    
    
    
    draw_curve(toprighthead, 10);
    draw_curve(toplefthead, 10);
    draw_curve(botrighthead, 10);
    draw_curve(botlefthead, 10);
    draw_curve(leftear, 10);
    draw_curve(rightear, 10);
    draw_curve(toplefteye,10);
    draw_curve(toprighteye,10);
    draw_curve(leftnose,10);
    draw_curve(rightnose, 10);
    draw_curve(lefteyebrow, 10);
    draw_curve(righteyebrow, 10);
    draw_curve(toplip, 10);
    draw_curve(bottomlip, 10);
    glutSwapBuffers();
}



int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,800); // Set your own window size
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
