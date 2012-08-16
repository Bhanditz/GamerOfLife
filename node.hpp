#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <vector>

#include "functions.hpp"

class node {
 private:
    /* -1 = Seinä
     *  0 = Kuollut
     *  1 = Elävä (orig.)
     *  2 = Elävä (asettu)
     */
    int status;
 public:
    node(int _status = 1) : status(_status) {}
    int getStatus()
        { return this->status; }
    void setStatus(int _status)
        { this->status = _status; }
    void draw() {
        switch(this->status) {
            case -1:
                glColor3f(0.3f, 0.3f, 0.3f);
                drawCube(0.8f, 0.8f, 0.8f);
                break;
            case 0:
                return; break;
            case 1:
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
            case 2:
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
        }
        glBegin(GL_QUADS);
            glVertex3d(-0.4, -0.4, -0.4);
            glVertex3d(-0.4,  0.4, -0.4);
            glVertex3d( 0.4,  0.4, -0.4);
            glVertex3d( 0.4, -0.4, -0.4);
        glEnd();
    }
};
