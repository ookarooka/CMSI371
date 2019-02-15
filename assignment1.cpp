/***
 assignment1.cpp
 Assignment-1: Cartoonify
 Name: Patterson, Joshua
 Collaborators: Tolliver, Moriah; Richardson, Alex
 Project Summary: I have used Chaikins algorithm to cartoonify a headshot of comedian Dave Chappelle.
 I used an iterative approach to handle the functions. I also used desmos.com as a tool to find proper vertex points.
 I sectioned off multiple aspects of the face to have more defined features (left eyelid, right eyelid, left nose, right nose, etc.)
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

//~~~~~~~~~~~~~~~~GENERATE CHAIKIN~~~~~~~~~~~~~~~~~~~~~~//

vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> points;
    points.push_back(control_points.at(0));
    for(int k = 0; (control_points.size()-1) > k; k++) {
        float deltax = control_points[k+1].get_x() - control_points[k].get_x();
        float deltay = control_points[k+1].get_y() - control_points[k].get_y();
        Vertex three_quarter = Vertex(control_points[k].get_x() + deltax * 0.75, control_points[k].get_y() + deltay * 0.75);
        //Vertex four_quarter = Vertex(control[k].get_x() + deltax * 1, control[k].get_y() + deltay * 1);
        Vertex quarter = Vertex(control_points[k].get_x() + deltax * 0.25, control_points[k].get_y() + deltay * 0.25);
        // points.push_back(four_quarter);
        points.push_back(quarter);
        points.push_back(three_quarter);
    }
    points.push_back(control_points.back());
    return points;
}


//~~~~~~~~~~~~~~DRAW CHAILKIN~~~~~~~~~~~~~~~~~~~~~~//

void draw_curve(vector<Vertex> control_points, int n_iter) {
    vector<Vertex> draw;
    draw = control_points;
    for(int k = 0; n_iter > k; k++) {
        draw = generate_points(draw);
    }
    glBegin(GL_LINES);
    for(int k = 0; draw.size()-1 > k; k++) {
        glVertex2f(draw.at(k).get_x(), draw.at(k).get_y());
        glVertex2f(draw.at(k+1).get_x(), draw.at(k+1).get_y());
    }
    glEnd();
}


//~~~~~~~~~~~~~~CONNECT POINTS~~~~~~~~~~~~~~~~~~~~//

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
    
    vector <Vertex> lefteyebrow = {
        Vertex(-0.095f, 0.159f),
        Vertex(-0.215f, 0.182f),
        Vertex(-0.335f, 0.1755f),
        Vertex(-0.325f, 0.204f),
        Vertex(-0.265f, 0.214f),
        Vertex(-0.095f, 0.159f),
        
    };
    
    vector <Vertex> righteyebrow = {
        Vertex(0.095f, 0.159f),
        Vertex(0.265f, 0.214f),
        Vertex(0.325f, 0.204f),
        Vertex(0.335f, 0.1755f),
        
        Vertex(0.215f, 0.182f),
        Vertex(0.095f, 0.159f),
        
    };
    
    vector <Vertex> lefteyelid = {
        Vertex(-0.119f, 0.096f),
        Vertex(-0.135f, 0.12f),
        Vertex(-0.188f, 0.148f),
        Vertex(-0.249f, 0.15f),
        Vertex(-0.312f, 0.139f),
        Vertex(-0.333f, 0.092f),
    };
    
    vector <Vertex> righteyelid = {
        Vertex(0.119f, 0.096f),
        Vertex(0.135f, 0.12f),
        Vertex(0.188f, 0.148f),
        Vertex(0.249f, 0.15f),
        Vertex(0.312f, 0.139f),
        Vertex(0.333f, 0.092f),
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
    
    vector <Vertex> lefteyeball = {
        Vertex(-0.185f, 0.084f),
        Vertex(-0.224f, 0.07f),
        Vertex(-0.275f, 0.081f),
        Vertex(-0.242f, 0.106f),
        Vertex(-0.215f, 0.12f),
        Vertex(-0.185f, 0.084f),
    };
    
    vector <Vertex> righteyeball = {
        Vertex(0.185f, 0.084f),
        Vertex(0.224f, 0.07f),
        Vertex(0.275f, 0.081f),
        Vertex(0.242f, 0.106f),
        Vertex(0.215f, 0.12f),
        Vertex(0.185f, 0.084f),
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
        Vertex(0.001f, -0.2f),
        Vertex(-0.038f, -0.185f),
        Vertex(-0.13f, -0.2f),
        Vertex(-0.18f, -0.234f),
        Vertex(-0.005f, -0.243f),
        Vertex(0.18f, -0.234f),
        Vertex(0.13f, -0.2f),
        Vertex(0.038f, -0.185f),
        Vertex(0.001f, -0.2f),
    };
    
    vector<Vertex> bottomlip =
    {
        Vertex(0.001f, -0.245f),
        Vertex(-0.038f, -0.245f),
        Vertex(-0.13f, -0.238f),
        Vertex(-0.144f, -0.296f),
        Vertex(-0.089f, -0.315f),
        Vertex(-0.008f, -0.314f),
        Vertex(0.089f, -0.315f),
        Vertex(0.144f, -0.296f),
        Vertex(0.13f, -0.238f),
        Vertex(0.038f, -0.245f),
        Vertex(0.001f, -0.245f),
    };
    
    
    vector<Vertex> leftstressmark =
    {
        Vertex(-0.086f, 0.074f),
        Vertex(-0.169f, 0.017f),
        Vertex(-0.23f, 0.007f),
    };
    
    vector<Vertex> rightstressmark =
    {
        Vertex(0.086f, 0.074f),
        Vertex(0.169f, 0.017f),
        Vertex(0.23f, 0.007f),
    };
    
    draw_curve(toprighthead, 4);
    draw_curve(toplefthead, 4);
    draw_curve(botrighthead, 4);
    draw_curve(botlefthead, 4);
    draw_curve(leftear, 4);
    draw_curve(rightear, 4);
    draw_curve(toplefteye,4);
    draw_curve(toprighteye,4);
    draw_curve(leftnose,4);
    draw_curve(rightnose, 4);
    draw_curve(lefteyebrow, 4);
    draw_curve(righteyebrow, 4);
    draw_curve(toplip, 4);
    draw_curve(bottomlip, 4);
    draw_curve(lefteyeball, 4);
    draw_curve(righteyeball, 4);
    draw_curve(lefteyelid, 4);
    draw_curve(righteyelid, 4);
    draw_curve(leftstressmark, 4);
    draw_curve(rightstressmark, 4);
    
    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,900); // Set your own window size
    glutCreateWindow("Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
