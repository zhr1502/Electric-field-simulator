#include<iostream>
#include<cmath>
#include<ctime>
#include "include/GL/glut.h"
#include "include/GL/freeglut.h"
using namespace std;
#define MIN_BROAD_LEFT 2.0/3.0
float zoom=1.0/0.0,user_zoom=1;
bool do_redisplay;
#define TOTAL_zoom (zoom*user_zoom)
#define abs(x) (x<0?-x:x)
#include "include/calculator.hpp"
void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0*(GLfloat)h/(GLfloat)w,
                1.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
    else
        glOrtho(-1.0*(GLfloat)w/(GLfloat)h,
                1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    float B=0;
    glColor3f(1.0,0,B);
    glLineWidth(1.0*user_zoom);
    int pos=0;
    glBegin(GL_LINE_STRIP);
    for(int i=1;i<conpoint_num;i++){
        if(i>line_que[pos]) B=0,pos++,glEnd(),glBegin(GL_LINE_STRIP);
        B=(i-line_que[pos-1])*is_lineto_neg[pos-1]/(line_que[pos]-line_que[pos-1]+1);
        glColor3f(1.0-B,0,B);
        if(i+1<=line_que[pos])
        glVertex2f(list[i].x*zoom*user_zoom,list[i].y*user_zoom*zoom),
        glVertex2f(list[i+1].x*zoom*user_zoom,list[i+1].y*zoom*user_zoom);
    }
    glEnd();
    glFlush();
    glutSwapBuffers();
    return;
}
void keyboard(unsigned char key,int x,int y){
    switch (key){
        case 27:
            exit(0);
            break;
    }
}
#ifdef linux
void mouse(int button,int state,int x,int y){
    if(button==3&&state==GLUT_DOWN){
        user_zoom*=1.2;
    }
    if(button==4&&state==GLUT_DOWN){
        user_zoom/=1.2;
    }
    calc();
    if(do_redisplay) glutPostRedisplay();
    else user_zoom=button==3?user_zoom/1.2:user_zoom*1.2;
    return;
}
#endif
#ifdef WIN32
void MouseWheel(int button,int dir,int x,int y){
    if(dir>0) user_zoom*=1.2;
    if(dir<0) user_zoom/=1.2;
    calc();
    if(do_redisplay) glutPostRedisplay();
    else user_zoom=dir>0?user_zoom/1.2:user_zoom*1.2;
    return;
}
#endif
int main(int argc,char** argv){
    init_calc();                                                       //input&calculate electricity particle
    glutInit(&argc, argv);                                             //opengl init
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(750,750);
    glutInitWindowPosition(100, 100);
    glutCreateWindow (argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    #ifdef linux
        glutMouseFunc(mouse);
    #endif
    #ifdef WIN32
        glutMouseWheelFunc(MouseWheel);
    #endif
    glutMainLoop();
}