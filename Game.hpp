#ifndef GAME_HPp
#define GAME_HPP

#include <vector>
#include <string>
#include <GL/glew.h>
#include "Pointer.hpp"
#include "Map.hpp"
#include "Stage.hpp"
#include "Player.hpp"

class Game {
 private:
    std::vector<Stage> stages;
    Uint32  lastRun;
    Uint32  currentStage;
    Uint32  runDelay;
    bool    running;
    Player  player;
 public:
    Pointer pointer;

    Game();

    int  loadStage(std::string);
    void initStage(int);

    Map *getMap(int);

    void run();
    void stop();
    bool isRunning();

    void update();

    void drawGUI();
    void draw();

    void checkNode(Uint32, Uint32);
    void movePointer(int, int);
    void toggleNode(int, int);
};

Game::Game() : player(Player()), pointer(Pointer(0, 0)) {
    loadStage("");
    loadStage("");
    loadStage("");
    running = false;
    lastRun = 0;
    runDelay = 250;
    initStage(0);
}

Map *Game::getMap(int id = -1) {
    if (id < 0) id = currentStage;

    return this->stages[id].map;
}

int Game::loadStage(std::string mapName) {
    debug(">>> loadStage()", __LINE__);
    Stage stage;
    stage.map = new Map(30, 30);

    for(Uint32 y = 0; y < stage.map->getHeight(); y++)
    for(Uint32 x = 0; x < stage.map->getWidth(); x++)
        stage.map->setNodeStatus(x, y, rand()%4 - 1);

    debug("<<< loadStage()", __LINE__);

    this->stages.push_back(stage);
    return this->stages.size()-1;
}

void Game::initStage(int stage) {
    debug(">>> initStage()", __LINE__);
    currentStage = stage;

//    this->map = stages.at(stage).map;

    this->pointer.set(this->getMap()->getWidth()/2, this->getMap()->getHeight()/2);

    lastRun = SDL_GetTicks();
    debug("<<< initStage()", __LINE__);
}

void Game::run()  { this->running = true; }
void Game::stop() { this->running = false; }
bool Game::isRunning() { return this->running; }

void Game::update() {
    if (!this->running) return;
    if (SDL_GetTicks() < this->lastRun + this->runDelay) return;

    this->getMap()->update();

    this->lastRun = SDL_GetTicks();
}

void Game::drawGUI() {
    double px = 10, py = 10;
    double s = 0.6;
    if (!this->running) {
        glColor3d(1, 0, 0);
        glBegin(GL_QUADS);
            glVertex3d(this->pointer.getX()-s, this->pointer.getY()-s,  -0.4-0.0004);
            glVertex3d(this->pointer.getX()+s, this->pointer.getY()-s,  -0.4-0.0004);
            glVertex3d(this->pointer.getX()+s, this->pointer.getY()+s,  -0.4-0.0004);
            glVertex3d(this->pointer.getX()-s, this->pointer.getY()+s,  -0.4-0.0004);
        glEnd();
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3d(-0.5,                            -0.5,                             -0.2);
        glVertex3d(-0.5,                             this->getMap()->getHeight()-0.5, -0.2);
        glVertex3d( this->getMap()->getWidth()-0.5,  this->getMap()->getHeight()-0.5, -0.2);
        glVertex3d( this->getMap()->getWidth()-0.5, -0.5,                             -0.2);
    glEnd();

    glLoadIdentity();

    glTranslated(0.0, 0.0, -1.0);

    px = 655;
    py = 45;

    if (distance(mouse.x, mouse.y, px, py) < 70) glColor3d(0.9, 0.9, 0.9);
    else glColor3d(1, 1, 1);
    if (!this->running) {
        glBegin(GL_TRIANGLES);
            glVertex3d(s2wX(630), s2wY(20),  0.0);
            glVertex3d(s2wX(630), s2wY(70),  0.0);
            glVertex3d(s2wX(680), s2wY(45),  0.0);
        glEnd();
    } else {
        glBegin(GL_QUADS);
            glVertex3d(s2wX(630), s2wY(20),  0.0);
            glVertex3d(s2wX(630), s2wY(70),  0.0);
            glVertex3d(s2wX(680), s2wY(70),  0.0);
            glVertex3d(s2wX(680), s2wY(20),  0.0);
        glEnd();
    }
    glTranslated(-3.0, -3.0, 0.0);
}
void Game::draw() {
    this->getMap()->draw();
    drawGUI();
}

void Game::checkNode(Uint32 x, Uint32 y) {
}

void Game::movePointer(int xp, int yp) {
    if (!this->running) this->pointer.move(xp, yp);
}
void Game::toggleNode(int x, int y) {
    if ( x < 0 ) x = 0;
    if ( y < 0 ) y = 0;
    if ( x >= (int)this->getMap()->getWidth()) x = this->getMap()->getWidth()-1;
    if ( y >= (int)this->getMap()->getHeight()) y = this->getMap()->getHeight()-1;

    switch(this->getMap()->getNodeStatus(x, y)) {
        case 0:
            this->getMap()->setNodeStatus(x, y, 2);
            break;
        case 2:
            this->getMap()->setNodeStatus(x, y, 0);
            break;
        default: break;
    }
}
#endif
