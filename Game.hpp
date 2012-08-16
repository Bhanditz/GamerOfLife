#include <GL/glew.h>
#include "Map.hpp"
#include "Player.hpp"

struct Pointer {
    int x, y;
    Pointer(int x, int y) { set(x, y); }
    void move(int xp, int yp) {
        this->x += xp;
        this->y += yp;
    }
    void set(int x, int y){
        this->x = x;
        this->y = y;
    }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }

    int getX(){ return this->x; }
    int getY(){ return this->y; }
};

class Game {
 private:
    Map *map;
    Uint32 lastRun;
    Uint32 runDelay;
    bool running;
 public:
    Pointer pointer;

    Game(Uint32 w = 30, Uint32 h = 30) : pointer(Pointer(w/2, h/2)) {
        this->map = new Map(w, h, true);
        lastRun = 0;
        runDelay = 250;
        running = false;
    }

    Map *getMap() { return this->map; }

    void initStage(int stage) {

        for(Uint32 y = 0; y < this->map->getHeight(); y++)
        for(Uint32 x = 0; x < this->map->getWidth(); x++)
            this->map->setNodeStatus(x, y, 0);
//            this->tmap->setNodeStatus(x, y, rand()%4 - 1);

        this->map->setNodeStatus(10, 10, 1);
        this->map->setNodeStatus(11, 10, 2);
        this->map->setNodeStatus(12, 10, 1);
        this->map->setNodeStatus(10, 11, 2);
        this->map->setNodeStatus(11, 11, 1);
        this->map->setNodeStatus(12, 11, 2);
        this->map->setNodeStatus(10, 12, 1);
        this->map->setNodeStatus(11, 12, 2);
        this->map->setNodeStatus(12, 12, 1);

        this->map->setNodeStatus(20, 20, 1);
        this->map->setNodeStatus(21, 20, 2);
        this->map->setNodeStatus(22, 20, 1);
        this->map->setNodeStatus(20, 21, 2);
        this->map->setNodeStatus(22, 21, 2);
        this->map->setNodeStatus(20, 22, 1);
        this->map->setNodeStatus(21, 22, 2);
        this->map->setNodeStatus(22, 22, 1);

        lastRun = SDL_GetTicks();
    }

    void run()  { this->running = true; }
    void stop() { this->running = false; }
    bool isRunning() { return this->running; }

    void update() {
        if (!this->running) return;
        if (SDL_GetTicks() < this->lastRun + this->runDelay) return;

        this->map->update();

        this->lastRun = SDL_GetTicks();
    }

    void drawGUI() {
        double px = 10, py = 10;
        double s = 0.5;
        if (!this->running) {
            glColor3d(1, 0, 0);
            glBegin(GL_QUADS);
                glVertex3d(this->pointer.getX()-s, this->pointer.getY()-s,  -0.399);
                glVertex3d(this->pointer.getX()+s, this->pointer.getY()-s,  -0.399);
                glVertex3d(this->pointer.getX()+s, this->pointer.getY()+s,  -0.399);
                glVertex3d(this->pointer.getX()-s, this->pointer.getY()+s,  -0.399);
            glEnd();
        }

        glLoadIdentity();

        glTranslated(0.0, 0.0, -60.0);

        glColor3d(1, 1, 1);
        s = 0.3;
        if (this->running) {
            glBegin(GL_TRIANGLES);
                glVertex3d(px-s, py+s,  0.0);
                glVertex3d(px-s, py-s,  0.0);
                glVertex3d(px+s, py,    0.0);
            glEnd();
        } else {
            glBegin(GL_QUADS);
                glVertex3d(px-s, py-s,  0.0);
                glVertex3d(px+s, py-s,  0.0);
                glVertex3d(px+s, py+s,  0.0);
                glVertex3d(px-s, py+s,  0.0);
            glEnd();
        }
        glTranslated(-3.0, -3.0, 0.0);
    }
    void draw() {
        this->map->draw();
        drawGUI();
    }

    void checkNode(Uint32 x, Uint32 y) {
    }

    void movePointer(int xp, int yp) {
        if (!this->running) this->pointer.move(xp, yp);
    }
    void toggleNode(int x, int y) {
        if ( x < 0 ) x = 0;
        if ( y < 0 ) y = 0;
        if ( x >= (int)this->map->getWidth()) x = this->map->getWidth()-1;
        if ( y >= (int)this->map->getHeight()) y = this->map->getHeight()-1;

        switch(this->map->getNodeStatus(x, y)) {
            case 0:
                this->map->setNodeStatus(x, y, 2);
                break;
            case 2:
                this->map->setNodeStatus(x, y, 0);
                break;
            default: break;
        }
    }
};
