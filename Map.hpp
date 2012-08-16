#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <vector>

#include "Node.hpp"

#define MAP_MAX_WIDTH 128
#define MAP_MAX_HEIGHT 128

Uint32 min(Uint32 a, Uint32 b) { return a<b?a:b; }
Uint32 max(Uint32 a, Uint32 b) { return a>b?a:b; }

class Map {
 private:
    const Uint32 width, height;
    Node nodes[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
    bool flip;
 public:
    Map(Uint32 w, Uint32 h, bool _flip = false) : width(min(w, MAP_MAX_WIDTH)), height(min(h, MAP_MAX_HEIGHT)), flip(_flip) {
        for(Uint32 y = 0; y < h; y++) {
            for(Uint32 x = 0; x < w; x++) {
                this->nodes[y][x] = Node();
            }
        }
    }

    Node getNode(int x, int y) {
        if (this->flip) {
            if (x < 0)              return getNode((int)this->width-1, y);
            if (y < 0)              return getNode(x, (int)this->height-1);

            if (x >= (int)this->width)   return getNode(0, y);
            if (y >= (int)this->height)  return getNode(x, 0);
        } else {
            if (x < 0 || y < 0 || x >= (int)this->width || y >= (int)this->height) return Node(false);
        }
        return this->nodes[y][x];
    }
    Node getNode(Uint32 x, Uint32 y) { return getNode((int)x, (int)y); }
    Node getNode(double x, double y) { return getNode((int)x, (int)y); }

    int getNodeStatus(int x,    int y)    { return getNode(x, y).getStatus(); }
    int getNodeStatus(Uint32 x, Uint32 y) { return getNodeStatus((int)x, (int)y); }
    int getNodeStatus(double x, double y) { return getNodeStatus((int)x, (int)y); }

    void setNodeStatus(Uint32 x, Uint32 y, int status) {
        this->nodes[y][x].setStatus(status);
    }

    void draw() {
        glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(-(double)this->width/2.0, -(double)this->height/2.0, 0.0);

		for(double y = 0; y < this->height; y++) {
            for(double x = 0; x < this->width; x++) {
                glTranslated( x,  y, 0);
                getNode(x, y).draw();
                glTranslated(-x, -y, 0);
            }
		}
//		glTranslatef((double)this->width/2.0, (double)this->height/2.0, 0.0);
    }

    void update() {
        int tnodes[MAP_MAX_HEIGHT][MAP_MAX_WIDTH];
        int nstatus;
        int neighborhood = 0;
        for(Uint32 y = 0; y < this->height; y++) {
            for(Uint32 x = 0; x < this->width; x++) {
                nstatus = getNodeStatus(x, y);
                if (nstatus >= 0) {
                    neighborhood = 1; // Koska itseään ei lasketa ja seuraavassa silmukassa siitä ei välitetä

                    for(int yy = -1; yy <= 1; yy++)
                    for(int xx = -1; xx <= 1; xx++)
                        switch(getNodeStatus(x+xx, y+yy)) {
                            case  1:
                            case  2:
                                neighborhood += 1;
                                break;
                            case -1:
                            case  0:
                            default: break;
                        }

                    switch(nstatus) {
                        case 1: case 2:
                            if ( neighborhood < 2 || neighborhood > 3 )
                                 nstatus = 0;
                            else nstatus = 1;
                            break;
                        case 0: default:
                            if ( neighborhood == 3 )
                                 nstatus = 1;
                            else nstatus = 0;
                            break;
                    }
                }
                tnodes[y][x] = nstatus;
            }
        }

        for(Uint32 y = 0; y < this->height; y++)
        for(Uint32 x = 0; x < this->width; x++)
            setNodeStatus(x, y, tnodes[y][x]);
    }

    const Uint32 getWidth()  { return this->width; }
    const Uint32 getHeight() { return this->height; }

    void printMap() {
        for(int y = -1; y <= (int)this->height; y++) {
            printf("[");
            for(int x = -1; x <= (int)this->width; x++) {
                printf("[%i]", getNodeStatus(x, y));
            }
            printf("]\n");
        }
    }
};

