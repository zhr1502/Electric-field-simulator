#include<iostream>
#include<cmath>
#include "include/GL/glut.h"
using namespace std;
#include "include/calculator.hpp"
void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
                5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
    else
        glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
                5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    float B=0;
    glColor3f(1.0,0,B);
    glPointSize(5.0);
    int pos=0;
    for(int i=1;i<=conpoint_num;i++){
        if(i>line_que[pos]) B=0,pos++;
        B=(i-line_que[pos-1])*is_lineto_neg[pos-1]/(line_que[pos]-line_que[pos-1]+1);
        glColor3f(1.0-B,0,B);
        glVertex2f(list[i].x*2,list[i].y*2);
    }
    // printf("end\n");
    glEnd();
    glFlush();
    glutSwapBuffers();
    return;
}
void keyboard(unsigned char key, int x, int y){
    switch (key){
        case 27:
            exit(0);
            break;
    }
}
int main(int argc,char** argv){
    calc();                                               //input&calculate electricity particle
    glutInit(&argc, argv);                                             //opengl init
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}