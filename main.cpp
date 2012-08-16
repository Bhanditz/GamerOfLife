#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <time.h>
#include <vector>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>

#include "Light.hpp"
#include "Game.hpp"

#define DEBUG(str) debug(str, __LINE__);

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	700
#define SCREEN_BPP		32
#define PI				3.1415926535f

namespace fps {
    double   fps;
    double   lbb = 0;
    double   ltt = 0;

    int calc(double tt, double bb) {
        bb = bb/1000.0;
        fps::fps = (tt-ltt)/(bb-lbb);
        fps::ltt = tt;
        fps::lbb = bb;
        return fps::fps;
    }
    int calc(int tt, int bb)
        { return calc((double)tt, (double)bb); }

    int calc(Uint32 tt, Uint32 bb)
        { return calc((double)tt, (double)bb); }
}

void debug(const char* str, int line = -1) {
    if (line == -1) {
        printf("%i: DEBUG: %s\n", SDL_GetTicks(), str);
    } else {
        printf("%i: DEBUG(%i): %s\n", SDL_GetTicks(), line, str);
    }
};

void Quit(int returnCode) {
	SDL_Quit();
	exit(returnCode);
};

SDL_Surface *surface;
Uint32 tt = 0, bb = 0;
int videoFlags, done = 0;
SDL_Event event;
const SDL_VideoInfo *videoInfo;
char winTitle[256];

float usXp = 0.0f;
float usYp = 0.0f;
float usZp = 0.0f;
float usMove = 1.0f;

std::vector<Light> lights;

struct Mouse { int x, y, p; } mouse;

GLuint LoadTexture(char*,int*,int*);
double sina(double a) { return sin(a/180.0f*PI); }
double cosa(double a) { return cos(a/180.0f*PI); }

float distance(float x1, float y1, float z1, float x2, float y2, float z2)
    { return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2) + pow(z1-z2, 2)); }

float distance(float x1, float y1, float x2, float y2)
    { return distance(x1, y1, 0.0f, x2, y2, 0.0f); }

int main( int argc, char **argv ) {
    debug("STARTING", __LINE__);

    printf("\tSDL(major) v.%i\n", SDL_MAJOR_VERSION);
    printf("\tSDL(minor) v.%i\n", SDL_MINOR_VERSION);
    printf("\tSDL(patch) v.%i\n", SDL_PATCHLEVEL);
    printf("\tSDL_mixer(major) v.%i\n", SDL_MIXER_MAJOR_VERSION);
    printf("\tSDL_mixer(minor) v.%i\n", SDL_MIXER_MINOR_VERSION);
    printf("\tSDL_mixer(patch) v.%i\n", SDL_MIXER_PATCHLEVEL);
    printf("\tSDL_image(major) v.%i\n", SDL_IMAGE_MAJOR_VERSION);
    printf("\tSDL_image(minor) v.%i\n", SDL_IMAGE_MINOR_VERSION);
    printf("\tSDL_image(patch) v.%i\n", SDL_IMAGE_PATCHLEVEL);
    printf("\tOPENGL v.%i\n", GL_VERSION);

    debug("INITIALIZING VARIABLES", __LINE__);
//    Light tmplight = Light(GL_LIGHT1);
//    tmplight.setPosition(4.0f, 4.0f, 4.0f );
//    tmplight.setAmbient( 0.5f, 0.5f, 0.5f );
//    tmplight.setDiffuse( 1.0f, 1.0f, 1.0f );

//    lights.push_back(tmplight);

    Game *game = new Game();


    debug("INITIALIZING SDL", __LINE__);
    debug("Initing SDL(video)", __LINE__);
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError( ) );
		Quit(1);
	}
	if (!(videoInfo = SDL_GetVideoInfo())) {
		fprintf( stderr, "Video query failed: %s\n", SDL_GetError( ) );
		Quit(1);
	}

	videoFlags  = SDL_OPENGL;
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	videoFlags |= SDL_RESIZABLE;
    videoFlags |= (videoInfo->hw_available)?SDL_HWSURFACE:SDL_SWSURFACE;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );

    debug("Creating surface", __LINE__);
	if (!(surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags ))) {
		fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
		Quit(1);
	}

// --------- INITIALIZING OGL ---------

    debug("INITIALIZING OPENGL", __LINE__);
    glEnable( GL_TEXTURE_2D );
	glShadeModel( GL_SMOOTH );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthMask(GL_TRUE);
//	glEnable(GL_LIGHTING);
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

/*    for(Uint32 i = 0; i < lights.size(); i++) {
        glLightfv(lights[i].getId(), GL_AMBIENT,  lights[i].getAmbient());
        glLightfv(lights[i].getId(), GL_DIFFUSE,  lights[i].getDiffuse());
        glLightfv(lights[i].getId(), GL_POSITION, lights[i].getPosition());
        glEnable (lights[i].getId());
    }*/

/*    glLightfv(GL_LIGHT1, GL_AMBIENT,  { 0.5f, 0.5f, 0.5f, 1.0f });
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  { 1.0f, 1.0f, 1.0f, 1.0f });
    glLightfv(GL_LIGHT1, GL_POSITION, { 4.0f, 4.0f, 4.0f, 1.0f });
    glEnable (GL_LIGHT1);*/

	glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -(double)SCREEN_HEIGHT/(double)SCREEN_WIDTH, (double)SCREEN_HEIGHT/(double)SCREEN_WIDTH, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    debug("INITIALIZING GLEW", __LINE__);

    switch(GLenum err = glewInit()) {
        case GLEW_OK: break;
        default:
            fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    debug("LOADING", __LINE__);

    debug("MAPINFO:", __LINE__);
    printf("Map: (%i,%i)\n", game->getMap()->getWidth(), game->getMap()->getHeight());

    game->getMap()->printMap();

    debug("STARTING THE LOOP", __LINE__);

	while (!done) {
		bb = SDL_GetTicks();
		tt++;

		fps::calc(tt, bb);
		if ( fps::lbb < bb - 1000 ) {
            sprintf(winTitle, "%.2f", fps::fps);
            SDL_WM_SetCaption(winTitle, NULL);
		}

		mouse.p = SDL_GetMouseState(&mouse.x, &mouse.y);

		while ( SDL_PollEvent( &event ) ) switch( event.type ) {
		    case SDL_QUIT: done = 1; break;
		    case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE: done = 1; break;

                    case SDLK_d: usXp += usMove; break;
                    case SDLK_a: usXp -= usMove; break;
                    case SDLK_w: usYp += usMove; break;
                    case SDLK_s: usYp -= usMove; break;
                    case SDLK_e: usZp += usMove; break;
                    case SDLK_q: usZp -= usMove; break;

                    case SDLK_RIGHT: game->movePointer( 1,  0); break;
                    case SDLK_LEFT:  game->movePointer(-1,  0); break;
                    case SDLK_UP:    game->movePointer( 0,  1); break;
                    case SDLK_DOWN:  game->movePointer( 0, -1); break;

                    case SDLK_RETURN:
                        if (game->isRunning()) game->stop(); else game->run();
                        break;
                    case SDLK_SPACE:
                        game->toggleNode( game->pointer.getX(), game->pointer.getY() );
                        break;

                    case SDLK_n: game->initStage(0); break;
                    default: break;
                }
		}

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glLoadIdentity();
        glTranslated(1.0, -4.0, -20.0);
		glRotated((-1.0 + 2.0*mouse.y/SCREEN_HEIGHT) * -10.0, 1, 0, 0);
		glRotated((-1.0 + 2.0*mouse.x/SCREEN_WIDTH) * -10.0, 0, 1, 0);

        glTranslated(0.0, 4.0, 0.0);
//		glRotated(bb/100.0, 1, 1, 1);

        game->update();
        game->draw();

		SDL_GL_SwapBuffers( );
	}
	Quit(0);
	return 0;
}
